/******************************************************************************
*  Copyright 2017, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2017
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "httpclient.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/errno.h"

#ifdef HTTPCLIENT_SSL_ENABLE
#include "mbedtls/debug.h"
#endif


#if 0
#define ERR(fmt,arg...)   printf(("[HTTPClient]: "fmt"\r\n"), ##arg)
#define WARN(fmt,arg...)  printf(("[HTTPClient]: "fmt"\r\n"), ##arg)
#define DBG(fmt,arg...)   printf(("[HTTPClient]: "fmt"\r\n"), ##arg)
#else
#define DBG(x, ...)
#define WARN(x, ...)
#define ERR(x, ...)
#endif

#define HTTPC_INF(...)    \
	{\
	printf(__VA_ARGS__); \
	printf("\n"); \
	}

#define HTTPC_WARN(...)   \
	{ \
	printf("WARN:  %s L#%d ", __func__, __LINE__);  \
	printf(__VA_ARGS__); \
	printf("\n"); \
	}

#define HTTPC_ERR(...)  \
	{ \
	printf(__VA_ARGS__); \
	printf("\n"); \
	}

#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))

#define HTTPCLIENT_AUTHB_SIZE     128

#define HTTPCLIENT_CHUNK_SIZE     512
#define HTTPCLIENT_SEND_BUF_SIZE  512

#define HTTPCLIENT_MAX_HOST_LEN   64
#define HTTPCLIENT_MAX_URL_LEN    256

//#define HTTPCLIENT_TIME_DEBUG  1

#if defined(MBEDTLS_DEBUG_C)
#define DEBUG_LEVEL 2
#endif

#ifdef HTTPCLIENT_SSL_ENABLE
static int httpclient_ssl_conn_patch(httpclient_t *client, char *host);
#endif

// Import ROM private API
extern int httpclient_parse_url(const char *url, char *scheme, size_t max_scheme_len, char *host, size_t maxhost_len, int *port, char *path, size_t max_path_len);
extern int httpclient_conn(httpclient_t *client, char *host);


static void httpclient_debug( void *ctx, int level, const char *file, int line, const char *str )
{
    DBG("%s", str);
}

// Replace ROM public API
HTTPCLIENT_RESULT httpclient_connect(httpclient_t *client, char *url)
{
    int ret = HTTPCLIENT_ERROR_CONN;
    char host[HTTPCLIENT_MAX_HOST_LEN] = {0};
    char scheme[8] = {0};
    char path[HTTPCLIENT_MAX_URL_LEN] = {0};

#ifdef HTTPCLIENT_TIME_DEBUG
    int start_time = 0;
    int end_time = 0;
#endif

    /* First we need to parse the url (http[s]://host[:port][/[path]]) */
    int res = httpclient_parse_url(url, scheme, sizeof(scheme), host, sizeof(host), &(client->remote_port), path, sizeof(path));
    if (res != HTTPCLIENT_OK) {
        printf("httpclient_parse_url returned %d\n", res);
        return (HTTPCLIENT_RESULT)res;
    }

    // http or https
    if (strcmp(scheme, "https") == 0)
        client->is_http = false;
    else if (strcmp(scheme, "http") == 0)
        client->is_http = true;

    // default http 80 port, https 443 port
    if (client->remote_port == 0) {
        if (client->is_http) {
            client->remote_port = HTTP_PORT;
        } else {
            client->remote_port = HTTPS_PORT;
        }
    }

    DBG("http?:%d, port:%d, host:%s", client->is_http, client->remote_port, host);

#ifdef HTTPCLIENT_TIME_DEBUG
    start_time = sys_now();
#endif
    client->socket = -1;
    if (client->is_http)
        ret = httpclient_conn(client, host);
#ifdef HTTPCLIENT_SSL_ENABLE
    else {
        ret = httpclient_ssl_conn_patch(client, host);
        if (0 == ret) {
            httpclient_ssl_t *ssl = (httpclient_ssl_t *)client->ssl;
            client->socket = ssl->net_ctx.fd;
        }
    }
#endif

    if (ret == HTTPCLIENT_OK)
    {
       client->net_handle = 1;
    }

#ifdef HTTPCLIENT_TIME_DEBUG
    end_time = sys_now();
    printf("client connect time =%d\r\n", end_time - start_time);
#endif

    HTTPC_INF("httpclient_connect() result:%d, client:%p", ret, client);
    return (HTTPCLIENT_RESULT)ret;
}

static HTTPCLIENT_RESULT httpclient_common_patch(httpclient_t *client, char *url, int method, httpclient_data_t *client_data)
{
    HTTPCLIENT_RESULT ret = HTTPCLIENT_ERROR_CONN;
#ifdef HTTPCLIENT_TIME_DEBUG
    int start_time, end_time;
#endif

    ret = httpclient_connect(client, url);

    if (!ret) {
        ret = httpclient_send_request(client, url, method, client_data);

        if (!ret) {
#ifdef HTTPCLIENT_TIME_DEBUG
            start_time = sys_now();
#endif
            ret = httpclient_recv_response(client, client_data);
#ifdef HTTPCLIENT_TIME_DEBUG
            end_time = sys_now();
            printf("recv_response time =%d\r\n", end_time - start_time);
#endif
        }
    }

    httpclient_close(client);
    return ret;
}

HTTPCLIENT_RESULT httpclient_get(httpclient_t *client, char *url, httpclient_data_t *client_data)
{
    return httpclient_common_patch(client, url, HTTPCLIENT_GET, client_data);
}

HTTPCLIENT_RESULT httpclient_post(httpclient_t *client, char *url, httpclient_data_t *client_data)
{
    return httpclient_common_patch(client, url, HTTPCLIENT_POST, client_data);
}

HTTPCLIENT_RESULT httpclient_put(httpclient_t *client, char *url, httpclient_data_t *client_data)
{
    return httpclient_common_patch(client, url, HTTPCLIENT_PUT, client_data);
}

HTTPCLIENT_RESULT httpclient_delete(httpclient_t *client, char *url, httpclient_data_t *client_data)
{
    return httpclient_common_patch(client, url, HTTPCLIENT_DELETE, client_data);
}


#ifdef HTTPCLIENT_SSL_ENABLE

static const int force_ciphersuite[] =
{
#if 0
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CCM,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CCM_8,

    /* All AES-128 ephemeral suites */
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8,

    /* All AES-256 suites */
    MBEDTLS_TLS_RSA_WITH_AES_256_CCM,
    MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA256,
    MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_256_CBC_SHA,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA,
    MBEDTLS_TLS_RSA_WITH_AES_256_CCM_8,

    /* All AES-128 suites */
    MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_RSA_WITH_AES_128_CCM,
    MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA,
    MBEDTLS_TLS_RSA_WITH_AES_128_CCM_8,
#endif
    MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA256,
    MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA,
    MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA,
    0
};

static int httpclient_ssl_conn_patch(httpclient_t *client, char *host)
{
    int authmode = MBEDTLS_SSL_VERIFY_NONE;
    const char *pers = "https";
    int value, ret = 0;
    uint32_t flags;
    char port[10] = {0};
    httpclient_ssl_t *ssl;

    client->ssl = malloc(sizeof(httpclient_ssl_t));
    if (!client->ssl) {
        HTTPC_ERR("malloc error.");
        ret = -1;
        goto exit;
    }
    ssl = (httpclient_ssl_t *)client->ssl;

    if (client->server_cert)
        authmode = MBEDTLS_SSL_VERIFY_REQUIRED;

    mbedtls_ssl_config_max_content_len(1024*6);

    /*
     * Initialize the RNG and the session data
     */
#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif
    mbedtls_net_init(&ssl->net_ctx);
    mbedtls_ssl_init(&ssl->ssl_ctx);
    mbedtls_ssl_config_init(&ssl->ssl_conf);
    mbedtls_x509_crt_init(&ssl->cacert);
    mbedtls_x509_crt_init(&ssl->clicert);
    mbedtls_pk_init(&ssl->pkey);
    mbedtls_ctr_drbg_init(&ssl->ctr_drbg);
    mbedtls_entropy_init(&ssl->entropy);
    if ((value = mbedtls_ctr_drbg_seed(&ssl->ctr_drbg,
                               mbedtls_entropy_func,
                               &ssl->entropy,
                               (const unsigned char*)pers,
                               strlen(pers))) != 0) {
        DBG("mbedtls_ctr_drbg_seed() failed, value:-0x%x.", -value);
        ret = -1;
        goto exit;
    }

    /*
    * Load the Client certificate
    */
    if (client->client_cert && client->client_pk) {
        ret = mbedtls_x509_crt_parse(&ssl->clicert, (const unsigned char *)client->client_cert, client->client_cert_len);
        if (ret < 0) {
            HTTPC_ERR("Loading cli_cert failed! mbedtls_x509_crt_parse returned -0x%x.", -ret);
            goto exit;
        }

        ret = mbedtls_pk_parse_key(&ssl->pkey, (const unsigned char *)client->client_pk, client->client_pk_len, NULL, 0);
        if (ret != 0) {
            HTTPC_ERR("failed! mbedtls_pk_parse_key returned -0x%x.", -ret);
            goto exit;
        }
    }

    /*
    * Load the trusted CA
    */
    /* cert_len passed in is gotten from sizeof not strlen */
    if (client->server_cert && ((value = mbedtls_x509_crt_parse(&ssl->cacert,
                                        (const unsigned char *)client->server_cert,
                                        client->server_cert_len)) < 0)) {
        HTTPC_ERR("mbedtls_x509_crt_parse() failed, value:-0x%x.", -value);
        ret = -1;
        goto exit;
    }

    /*
     * Start the connection
     */
    snprintf(port, sizeof(port), "%d", client->remote_port) ;
    if ((ret = mbedtls_net_connect(&ssl->net_ctx, host, port, MBEDTLS_NET_PROTO_TCP)) != 0) {
        HTTPC_ERR("failed! mbedtls_net_connect returned %d, port:%s.", ret, port);
        goto exit;
    }

    /*
     * Setup stuff
     */
    if ((value = mbedtls_ssl_config_defaults(&ssl->ssl_conf,
                                           MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        DBG("mbedtls_ssl_config_defaults() failed, value:-0x%x.", -value);
        ret = -1;
        goto exit;
    }

    /* restrict ciphers */
    mbedtls_ssl_conf_ciphersuites( &ssl->ssl_conf, force_ciphersuite );

    //mbedtls_ssl_set_in_content_len(1024*6);
    //mbedtls_ssl_set_out_content_len(1024*2);

    // TODO: add customerization encryption algorithm
    memcpy(&ssl->profile, ssl->ssl_conf.cert_profile, sizeof(mbedtls_x509_crt_profile));
    ssl->profile.allowed_mds = ssl->profile.allowed_mds | MBEDTLS_X509_ID_FLAG(MBEDTLS_MD_MD5);
    mbedtls_ssl_conf_cert_profile(&ssl->ssl_conf, &ssl->profile);

    mbedtls_ssl_conf_authmode(&ssl->ssl_conf, authmode);
    mbedtls_ssl_conf_ca_chain(&ssl->ssl_conf, &ssl->cacert, NULL);

    if (client->client_cert && (ret = mbedtls_ssl_conf_own_cert(&ssl->ssl_conf, &ssl->clicert, &ssl->pkey)) != 0) {
        DBG(" failed! mbedtls_ssl_conf_own_cert returned %d.", ret );
        goto exit;
    }

    mbedtls_ssl_conf_rng(&ssl->ssl_conf, mbedtls_ctr_drbg_random, &ssl->ctr_drbg);
    mbedtls_ssl_conf_dbg(&ssl->ssl_conf, httpclient_debug, NULL);

    if ((value = mbedtls_ssl_setup(&ssl->ssl_ctx, &ssl->ssl_conf)) != 0) {
        HTTPC_ERR("mbedtls_ssl_setup() failed, value:-0x%x.", -value);
        ret = -1;
        goto exit;
    }

    mbedtls_ssl_set_bio(&ssl->ssl_ctx, &ssl->net_ctx, mbedtls_net_send, mbedtls_net_recv, NULL);

    /*
    * Handshake
    */
    while ((ret = mbedtls_ssl_handshake(&ssl->ssl_ctx)) != 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            HTTPC_ERR("mbedtls_ssl_handshake() failed, ret:-0x%x.", -ret);
            ret = -1;
            goto exit;
        }
    }

    /*
     * Verify the server certificate
     */
    /* In real life, we would have used MBEDTLS_SSL_VERIFY_REQUIRED so that the
        * handshake would not succeed if the peer's cert is bad.  Even if we used
        * MBEDTLS_SSL_VERIFY_OPTIONAL, we would bail out here if ret != 0 */
    if ((flags = mbedtls_ssl_get_verify_result(&ssl->ssl_ctx)) != 0) {
        char vrfy_buf[512];
        HTTPC_INF("svr_cert varification failed.");
        mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", flags);
        HTTPC_INF("%s", vrfy_buf);
    }
    else
        HTTPC_INF("svr_cert varification ok.");

    HTTPC_INF("Cipher suite is %s\r\n", mbedtls_ssl_get_ciphersuite(&ssl->ssl_ctx));

exit:
    HTTPC_INF("ret=%d.", ret);
    return ret;
}

#endif

