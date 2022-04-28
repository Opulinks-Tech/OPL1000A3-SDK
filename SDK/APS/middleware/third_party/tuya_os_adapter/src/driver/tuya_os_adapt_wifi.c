/**
 * @file tuya_os_adapt_wifi.c
 * @brief wifi操作接口
 *
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */

#define _WIFI_HWL_GLOBAL
#include "tuya_os_adapter_error_code.h"
#include "tuya_os_adapt_wifi.h"
#include "tuya_os_adapt_network.h"
#include "tuya_os_adapt_memory.h"
#include "cmsis_os.h"
#include "event_loop.h"
#include "wifi_api.h"
#include "wifi_types.h"
#include "sys_common_api.h"
#include "sys_common_types.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/netif.h"
#include "lwip_helper.h"
#include "network_config.h"
#include "tuya_os_adapt_system.h"
#include "ps_public.h"
#include "opl_os_adapt_wifi.h"
/***********************************************************
*************************micro define***********************
***********************************************************/
#define NETWORK_ADDR_LEN    16

#if 0
typedef enum {
    TCPIP_ADAPTER_IF_STA = 0,     /**< station interface */
    TCPIP_ADAPTER_IF_AP,          /**< soft-AP interface */
    TCPIP_ADAPTER_IF_MAX
} net_adapter_if_t;

typedef struct {
    ip4_addr_t ip;
    ip4_addr_t netmask;
    ip4_addr_t gw;
} net_adapter_ip_info_t;
#endif

#define OPL_ERR_OK    0
#define OPL_ERR_FAIL -1
#define WIFI_MAC_LEN  6

#define WIFI_ASSERT(msg) do { \
    printf(msg); \
    while(1) {}; \
} while(0)

typedef struct {
    uint8_t country_code;
    char country_str[3];
    uint8_t ch;
} tuya_wifi_country_t;

/***********************************************************
*************************variable define********************
***********************************************************/
static ty_wifi_event_cb tuya_event_cb = NULL;

static WF_STATION_STAT_E tuya_wifi_st = WSS_IDLE;
static wifi_config_t tuya_wifi_cfg = {0};
static osSemaphoreId wifi_wait_sem = NULL;
static char s_country_num[8] = "CN";
static tuya_wifi_country_t tuya_wifi_country_tb[] = {
    {COUNTRY_CODE_CN, "CN", 13}, /* CN, channel max = 13 */
    {COUNTRY_CODE_US, "US", 11}, /* US, channel max = 11 */
    {COUNTRY_CODE_JP, "JP", 14}, /* JP, channel max = 14 */
    {COUNTRY_CODE_EU, "EU", 13}, /* EU, channel max = 13 */ 
};

RET_DATA t_opl_net_adapter_get_ip_info_fp opl_net_adapter_get_ip_info;
RET_DATA t_opl_wifi_event_handler_cb_fp opl_wifi_event_handler_cb;
RET_DATA t_opl_os_adapt_wifi_init_fp opl_os_adapt_wifi_init;
RET_DATA t_opl_os_adapt_wifi_ap_scan_fp opl_os_adapt_wifi_ap_scan;
RET_DATA t_opl_os_adapt_wifi_release_ap_fp opl_os_adapt_wifi_release_ap;
RET_DATA t_opl_os_adapt_wifi_get_ip_fp opl_os_adapt_wifi_get_ip;
RET_DATA t_opl_os_adapt_wifi_set_mac_fp opl_os_adapt_wifi_set_mac;
RET_DATA t_opl_os_adapt_wifi_get_mac_fp opl_os_adapt_wifi_get_mac;
RET_DATA t_opl_os_adapt_wifi_get_connected_ap_info_fp opl_os_adapt_wifi_get_connected_ap_info;
RET_DATA t_opl_wifi_station_connect_fast_thread_fp opl_wifi_station_connect_fast_thread;
RET_DATA t_opl_os_adapt_wifi_station_connect_fp opl_os_adapt_wifi_station_connect;
RET_DATA t_opl_os_adapt_wifi_station_fast_connect_fp opl_os_adapt_wifi_station_fast_connect;
RET_DATA t_opl_os_adapt_wifi_station_disconnect_fp opl_os_adapt_wifi_station_disconnect;
RET_DATA t_opl_os_adapt_wifi_station_get_conn_ap_rssi_fp opl_os_adapt_wifi_station_get_conn_ap_rssi;
RET_DATA t_opl_os_adapt_wifi_get_bssid_fp opl_os_adapt_wifi_get_bssid;
RET_DATA t_opl_os_adapt_wifi_station_get_status_fp opl_os_adapt_wifi_station_get_status;
RET_DATA t_opl_os_adapt_wifi_set_country_code_fp opl_os_adapt_wifi_set_country_code;
RET_DATA t_opl_os_adapt_set_wifi_lp_mode_fp opl_os_adapt_set_wifi_lp_mode;
/***********************************************************
*************************function define********************
***********************************************************/
static int net_adapter_get_ip_info(net_adapter_if_t tcpip_if, net_adapter_ip_info_t *ip_info)
{
    return opl_net_adapter_get_ip_info(tcpip_if, ip_info);
}

static int wifi_event_handler_cb(wifi_event_id_t event_id, void *data, uint16_t length)
{
    return opl_wifi_event_handler_cb(event_id, data, length);
}

int tuya_os_adapt_wifi_init(ty_wifi_event_cb cb)
{
    return opl_os_adapt_wifi_init(cb);
}

int tuya_os_adapt_wifi_ap_scan(char *ssid, AP_IF_S **ap_ary, unsigned int *num)
{
    return opl_os_adapt_wifi_ap_scan(ssid, ap_ary, num);
}

int tuya_os_adapt_wifi_release_ap(AP_IF_S *ap)
{
    return opl_os_adapt_wifi_release_ap(ap);
}

int tuya_os_adapt_wifi_get_ip(const WF_IF_E wf, NW_IP_S *ip)
{
    return opl_os_adapt_wifi_get_ip(wf, ip);
}

int tuya_os_adapt_wifi_set_mac(const WF_IF_E wf, const NW_MAC_S *mac)
{
    return opl_os_adapt_wifi_set_mac(wf, mac);
}

int tuya_os_adapt_wifi_get_mac(const WF_IF_E wf, NW_MAC_S *mac)
{
    return opl_os_adapt_wifi_get_mac(wf, mac);
}

int tuya_os_adapt_wifi_get_connected_ap_info(FAST_WF_CONNECTED_AP_INFO_V2_S **fast_ap_info)
{
    return opl_os_adapt_wifi_get_connected_ap_info(fast_ap_info);
}

static void wifi_station_connect_fast_thread(void* pArg)
{
    opl_wifi_station_connect_fast_thread(pArg);
}

int tuya_os_adapt_wifi_station_connect(const signed char *ssid, const signed char *passwd)
{
    return opl_os_adapt_wifi_station_connect(ssid, passwd);
}

int tuya_os_adapt_wifi_station_fast_connect(const FAST_WF_CONNECTED_AP_INFO_V2_S *fast_ap_info)
{
    return opl_os_adapt_wifi_station_fast_connect(fast_ap_info);
}

int tuya_os_adapt_wifi_station_disconnect(void)
{
    return opl_os_adapt_wifi_station_disconnect();
}

int tuya_os_adapt_wifi_station_get_conn_ap_rssi(signed char *rssi)
{
    return opl_os_adapt_wifi_station_get_conn_ap_rssi(rssi);
}

int tuya_os_adapt_wifi_get_bssid(unsigned char *mac)
{
    return opl_os_adapt_wifi_get_bssid(mac);
}

int tuya_os_adapt_wifi_station_get_status(WF_STATION_STAT_E *stat)
{
    return opl_os_adapt_wifi_station_get_status(stat);
}

int tuya_os_adapt_wifi_set_country_code(const COUNTRY_CODE_E ccode)
{
    return opl_os_adapt_wifi_set_country_code(ccode);
}

int tuya_os_adapt_set_wifi_lp_mode(const bool en, const unsigned int dtim)
{
    return opl_os_adapt_set_wifi_lp_mode(en, dtim);
}

static int net_adapter_get_ip_info_impl(net_adapter_if_t tcpip_if, net_adapter_ip_info_t *ip_info)
{
    struct netif *p_netif = NULL;
    lwip_tcpip_config_t tcpip_config = {{0}, {0}, {0}};

    if (tcpip_if >= TCPIP_ADAPTER_IF_MAX || ip_info == NULL) {
        return -1;
    }

    p_netif = netif_find("st1");

    if (p_netif != NULL && netif_is_up(p_netif)) {
        ip4_addr_set(&ip_info->ip, ip_2_ip4(&p_netif->ip_addr));
        ip4_addr_set(&ip_info->netmask, ip_2_ip4(&p_netif->netmask));
        ip4_addr_set(&ip_info->gw, ip_2_ip4(&p_netif->gw));

        return 0;
    }

    if (0 != tcpip_config_init(&tcpip_config)) {
        printf("hal_os tcpip config init fail");
        return -1;
    }

    ip4_addr_copy(ip_info->ip, tcpip_config.sta_ip);
    ip4_addr_copy(ip_info->gw, tcpip_config.sta_gw);
    ip4_addr_copy(ip_info->netmask, tcpip_config.sta_mask);

    return 0;
}

static int wifi_event_handler_cb_impl(wifi_event_id_t event_id, void *data, uint16_t length)
{
		
    switch(event_id) {
    case WIFI_EVENT_STA_START:
				
        lwip_network_init(WIFI_MODE_STA);
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("\r\n*******WIFI_EVENT_STA_CONNECTED******\r\n");
        lwip_net_start(WIFI_MODE_STA);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        tuya_wifi_st = WSS_IDLE;
        tuya_event_cb(TY_WIFI_DISCONNECTED, data);
        break;
    case WIFI_EVENT_SCAN_COMPLETE:
        osSemaphoreRelease(wifi_wait_sem);
        printf("\r\n*******WIFI_EVENT_SCAN_COMPLETE******\r\n");
        tuya_event_cb(TY_WIFI_SCAN_DONE, 0);
        break;
    case WIFI_EVENT_STA_GOT_IP:
        // Wi-Fi connected = GOT IP in Tuya SDK.
        tuya_wifi_st = WSS_CONN_SUCCESS;
        tuya_event_cb(TY_WIFI_CONNECTED, data);
        break;
    case WIFI_EVENT_STA_CONNECTION_FAILED:
        printf("\r\n*******WIFI_EVENT_STA_CONNECTION_FAILED*****\r\n");
        tuya_wifi_st = WSS_CONN_FAIL;
        tuya_event_cb(TY_WIFI_CONNECT_FAILED, data);
        break;
    default:
        break;
    }
    return 0;
}

/**
 * @brief init wifi
 *
 * @param[in]       cb          wifi event callback
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_init_impl(ty_wifi_event_cb cb)
{
    if (cb == NULL) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }
    
    // Initialize the Event Loop handler
    if (wifi_event_loop_init((wifi_event_cb_t)wifi_event_handler_cb) != OPL_ERR_OK) {
        WIFI_ASSERT("[wifi adapt]init event loop fail \r\n");
//        return -1; //TBD
    }

    // Initialize wifi init complete event handler
    if (wifi_init(NULL, NULL) != OPL_ERR_OK) {
        WIFI_ASSERT("[wifi adapt]init wifi fail \r\n");
//        return -1; //TBD
    }
    
    if (wifi_start() != OPL_ERR_OK) {
        WIFI_ASSERT("[wifi adapt]init lwip fail \r\n");
//        return -1; //TBD
    }
    
    tuya_event_cb = cb;

    printf("\r\nwifi init success!!\r\n");

    return OPRT_OS_ADAPTER_OK;
}


/**
 * @brief scan current environment and obtain all the ap
 *        infos in current environment
 *
 * @param[in]       ssid        ap ssid(null--scan all,else only scan specified ssid)
 * @param[out]      ap_ary      current ap info array
 * @param[out]      num         the num of ar_ary
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_ap_scan_impl(char *ssid, AP_IF_S **ap_ary, unsigned int *num)
{
    if (*ap_ary != NULL) {
        return OPRT_OS_ADAPTER_AP_SCAN_FAILED;
    }
    
    uint16_t ssid_len = 0;
    wifi_scan_config_t scan_cfg = {0};
    scan_cfg.scan_type = WIFI_SCAN_TYPE_MIX;
    
    if (ssid != NULL) {
        ssid_len = strlen(ssid);
        if (ssid_len > (WIFI_MAX_LENGTH_OF_SSID - 1)) {
            return OPRT_OS_ADAPTER_INVALID_PARM;
        }
        scan_cfg.ssid = (uint8_t *)ssid;
    }
    
    /***** Initialize the semaphore *****/

    osSemaphoreDef_t sem_def = {0};
    if (wifi_wait_sem == NULL) {
        wifi_wait_sem = osSemaphoreCreate(&sem_def, 1);
        if (wifi_wait_sem == NULL) {
            WIFI_ASSERT("[wifi adapt]create semaphore fail \r\n");
//          return -1; //TBD
        }
    }
    osSemaphoreWait(wifi_wait_sem, osWaitForever);
    /***** Scan start *****/

    if (wifi_scan_start(&scan_cfg, 0) != OPL_ERR_OK) {
        osSemaphoreDelete(wifi_wait_sem);
        wifi_wait_sem = NULL;
        return OPRT_OS_ADAPTER_AP_SCAN_FAILED;
    }
    /***** Wait the response *****/
    osSemaphoreWait(wifi_wait_sem, 25000);
    osSemaphoreDelete(wifi_wait_sem);
    wifi_wait_sem = NULL;
    
    /***** Pack scan result *****/
    
    wifi_scan_get_ap_num((uint16_t *)num);
    if (*num == 0) {
        return OPRT_OS_ADAPTER_AP_SCAN_FAILED;
    }
    
    wifi_scan_info_t *ap_info = NULL;
    ap_info = malloc(sizeof(wifi_scan_info_t) * *num);
    if (ap_info == NULL) {
        return OPRT_OS_ADAPTER_MALLOC_FAILED;
    }
    wifi_scan_get_ap_records((uint16_t *)num, ap_info);
    
    AP_IF_S *p_ap_info = NULL;
    p_ap_info = malloc(sizeof(AP_IF_S) * *num);
    if (p_ap_info == NULL) {
        if (ap_info) {
            free(ap_info);
        }
        return OPRT_OS_ADAPTER_MALLOC_FAILED;
    }
    memset(p_ap_info, 0, sizeof(AP_IF_S) * *num);
    
    int i;
    for (i=0; i<*num; i++) {
        p_ap_info[i].channel = ap_info[i].channel;
        p_ap_info[i].rssi = ap_info[i].rssi;
        memcpy(p_ap_info[i].bssid, ap_info[i].bssid, WIFI_MAC_ADDRESS_LENGTH);
        memcpy(p_ap_info[i].ssid, ap_info[i].ssid, strlen((char *)ap_info[i].ssid));
        p_ap_info[i].s_len = strlen((char *)ap_info[i].ssid);
    }
    
    *ap_ary = p_ap_info;
    
    free(ap_info);
    
    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief release the memory malloced in <tuya_os_adapt_wifi_all_ap_scan>
 *        and <tuya_os_adapt_wifi_assign_ap_scan> if needed. tuya-sdk
 *        will call this function when the ap info is no use.
 *
 * @param[in]       ap          the ap info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_release_ap_impl(AP_IF_S *ap)
{
    if(NULL == ap) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    free(ap);

    return OPRT_OS_ADAPTER_OK;
}


/**
 * @brief get wifi ip info.when wifi works in
 *        ap+station mode, wifi has two ips.
 *
 * @param[in]       wf          wifi function type
 * @param[out]      ip          the ip addr info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_get_ip_impl(const WF_IF_E wf, NW_IP_S *ip)
{
    if(ip == NULL) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    if (wf == WF_AP) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }


    net_adapter_ip_info_t info;
    net_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &info);
    memcpy(ip->ip, inet_ntoa(info.ip.addr), NETWORK_ADDR_LEN);
    memcpy(ip->mask, inet_ntoa(info.netmask.addr), NETWORK_ADDR_LEN);
    memcpy(ip->gw, inet_ntoa(info.gw.addr), NETWORK_ADDR_LEN);

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief set wifi mac info.when wifi works in
 *        ap+station mode, wifi has two macs.
 *
 * @param[in]       wf          wifi function type
 * @param[in]       mac         the mac info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_set_mac_impl(const WF_IF_E wf, const NW_MAC_S *mac)
{
    if (mac == NULL) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    // Currently, OPL1000 does not support SoftAP mode.
    if (wf == WF_AP) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    // printf("\r\nset mac : %02x:%02x:%02x:%02x:%02x:%02x\r\n",mac->mac[5],mac->mac[4],mac->mac[3],mac->mac[2],mac->mac[1],mac->mac[0]);

    if (wifi_config_set_mac_address(WIFI_MODE_STA, (uint8_t *)mac->mac) != OPL_ERR_OK) {
        return OPRT_OS_ADAPTER_MAC_SET_FAILED;
    }
    
    if (mac_addr_set_config_source(MAC_IFACE_WIFI_STA, MAC_SOURCE_FROM_FLASH) != OPL_ERR_OK) {
        return OPRT_OS_ADAPTER_MAC_SET_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief get wifi mac info.when wifi works in
 *        ap+station mode, wifi has two macs.
 *
 * @param[in]       wf          wifi function type
 * @param[out]      mac         the mac info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_get_mac_impl(const WF_IF_E wf, NW_MAC_S *mac)
{
    if(mac == NULL) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    // Currently, OPL1000 does not support SoftAP mode.
    if (wf == WF_AP) {
        memset(mac->mac, 0, WIFI_MAC_ADDRESS_LENGTH);
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    if (wifi_config_get_mac_address(WIFI_MODE_STA, mac->mac) != OPL_ERR_OK) {
        printf("\r\nget mac error\r\n");
        memset(mac->mac, 0, WIFI_MAC_ADDRESS_LENGTH);
        return OPRT_OS_ADAPTER_MAC_GET_FAILED;
    }

    //printf("\r\nget mac : %02x:%02x:%02x:%02x:%02x:%02x\r\n",mac->mac[5],mac->mac[4],mac->mac[3],mac->mac[2],mac->mac[1],mac->mac[0]);


    return OPRT_OS_ADAPTER_OK;
}
/**
 * @brief : get ap info for fast connect
 * @param[out]      fast_ap_info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_get_connected_ap_info_impl(FAST_WF_CONNECTED_AP_INFO_V2_S **fast_ap_info)
{
    
    return OPRT_OS_ADAPTER_COM_ERROR;
    #if 0
	  if(NULL == fast_ap_info) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    FAST_WF_CONNECTED_AP_INFO_V2_S *ap_infor_v2_buf = NULL;
    unsigned int len = sizeof(FAST_WF_CONNECTED_AP_INFO_S);
    ap_infor_v2_buf = (FAST_WF_CONNECTED_AP_INFO_V2_S *)tuya_os_adapt_system_malloc(sizeof(FAST_WF_CONNECTED_AP_INFO_V2_S)+len);
    if(NULL == ap_infor_v2_buf) {
        return OPRT_OS_ADAPTER_MALLOC_FAILED;
    }
    memset(ap_infor_v2_buf, 0, sizeof(FAST_WF_CONNECTED_AP_INFO_V2_S)+len);

    rtw_wifi_setting_t setting;
    if(wifi_get_setting((const char*)WLAN0_NAME,&setting) || setting.mode == RTW_MODE_AP) {
        return OPRT_OS_ADAPTER_APINFO_GET_FAILED;
    }
    
    FAST_WF_CONNECTED_AP_INFO_S ap_info = {0};
    ap_info.chan = setting.channel;
    ap_info.security = setting.security_type;

    memcpy(ap_info.ssid, wifi.ssid.val, wifi.ssid.len);
    if (strlen(psk_passphrase64) == 64) {
        memcpy(ap_info.passwd, psk_passphrase64, sizeof(ap_info.passwd));
    }else if (strlen(psk_passphrase) != 0){
        memcpy(ap_info.passwd, psk_passphrase[0], sizeof(ap_info.passwd));
    }else {
        memcpy(ap_info.passwd, tuya_pwd, sizeof(tuya_pwd));
    }

    ap_infor_v2_buf->len = len;
    memcpy(ap_infor_v2_buf->data, &ap_info, len);

    *fast_ap_info = (void *)ap_infor_v2_buf;

    printf("\r\n***(get)fast ap info: ssid:%s, security:%d, pwd:%s, chan:%d\r\n",ap_info.ssid,ap_info.security,ap_info.passwd,ap_info.chan);

    return OPRT_OS_ADAPTER_OK;
		#endif
}

static void wifi_station_connect_fast_thread_impl(void* pArg)
{
    #if 0
    FAST_WF_CONNECTED_AP_INFO_S *ap_info = (FAST_WF_CONNECTED_AP_INFO_S *)pArg;
    unsigned char channel = ap_info->chan;
    unsigned char pscan_config = PSCAN_ENABLE | PSCAN_FAST_SURVEY;

    //set partial scan for entering to listen beacon quickly
    int ret = wifi_set_pscan_chan((unsigned char *)&channel, &pscan_config, 1);
    if(ret < 0) {
        tuya_wifi_event_callback( TY_WIFI_CONNECT_FAILED, NULL );
        goto exit;
    }

    int key_id = -1;
    if(RTW_SECURITY_WEP_PSK == ap_info->security) {
        key_id = 0;
    }
    
    printf("\r\n***(connect)fast ap info: ssid:%s, security:%d, pwd:%s, chan:%d\r\n",ap_info->ssid,ap_info->security,ap_info->passwd,channel);
    event_flag = 0;
    ret = wifi_connect((char*)ap_info->ssid, ap_info->security, (char*)ap_info->passwd, strlen(ap_info->ssid),strlen(ap_info->passwd), key_id, NULL);
     if ( RTW_SUCCESS != ret ) {
        if ( event_flag == 0 && NULL != tuya_wifi_event_callback ) {
            tuya_wifi_event_callback( TY_WIFI_CONNECT_FAILED, NULL );
        }
    }else {
        //get ip address after radio connected.
        ret = LwIP_DHCP(0, DHCP_START);
        if(ret == DHCP_ADDRESS_ASSIGNED) {
            if ( NULL != tuya_wifi_event_callback ) {
                tuya_wifi_event_callback( TY_WIFI_CONNECTED, NULL );
                printf("\r\n***************fast connect success!!!!\r\n");
            }
        }else {
            if ( NULL != tuya_wifi_event_callback ) {
                tuya_wifi_event_callback( TY_WIFI_CONNECT_FAILED, NULL );
            }
        }
    }

#if ONLY_SUPPORT_BT_CONFIG
    hwl_wf_btctrl_disable_for_lp();
#endif

exit:
    //release thread.
    sta_conn_thrd = NULL;
    ret = tuya_os_adapt_thread_release( NULL );
    #endif
    return;
}
/**
 * @brief connect wifi with ssid and passwd
 *
 * @param[in]       ssid
 * @param[in]       passwd
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
// only support wap/wap2
int tuya_os_adapt_wifi_station_connect_impl(const signed char *ssid, const signed char *passwd)
{ 
    if(NULL == ssid ) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }
		
    AP_IF_S *ap_ary = NULL;
    unsigned int num = 0;
    int ret = tuya_os_adapt_wifi_ap_scan((char *)ssid,&ap_ary,&num);
    if(ret != 0) {
        return ret;
    }

    printf("scan num : %d", num);
		
    uint16_t ssid_len = strlen((char *)ssid);
    uint16_t passwd_len = strlen((char *)passwd);
    
    if ((ssid_len > WIFI_MAX_LENGTH_OF_SSID - 1) ||
        (((passwd_len > WIFI_LENGTH_PASSPHRASE - 1) || (passwd_len < 8)) && (passwd_len > 0))) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }
    
    memset(&tuya_wifi_cfg, 0, sizeof(wifi_config_t));
    
    memcpy((void *)tuya_wifi_cfg.sta_config.ssid, (void *)ssid, ssid_len);
    tuya_wifi_cfg.sta_config.ssid_length = ssid_len;
    
    memcpy((void *)tuya_wifi_cfg.sta_config.password, (void *)passwd, passwd_len);
    tuya_wifi_cfg.sta_config.password_length = passwd_len;
    
    if (wifi_connection_connect(&tuya_wifi_cfg) != OPL_ERR_OK) {
        //OPRT_OS_ADAPTER_AP_NOT_FOUND : Not support return error code.
        return -1; //TBD
    }
    
    tuya_wifi_st = WSS_CONNECTING;
    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief : fast connect
 * @param[in]      fast_ap_info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_station_fast_connect_impl(const FAST_WF_CONNECTED_AP_INFO_V2_S *fast_ap_info)
{
    #if 0
    if(NULL == fast_ap_info) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    memcpy(&ap_info, fast_ap_info->data, sizeof(ap_info));
    memcpy(tuya_pwd, ap_info.passwd, sizeof(tuya_pwd));

        //init wifi(rtw_network_info) struct
    memset( &wifi, 0, sizeof( wifi ) );
    strncpy( wifi.ssid.val, ap_info.ssid, sizeof(wifi.ssid.val) );
    wifi.ssid.len = strlen( ap_info.ssid );
 

    //start connection request thread.
    int ret = tuya_os_adapt_thread_create( &sta_conn_thrd, "sta_conn_thrd", 1024+1024, 5, wifi_station_connect_fast_thread, (void*)&ap_info );
    if ( OPRT_OS_ADAPTER_OK != ret ) {
        return ret;
    }
    #endif
    (void)wifi_station_connect_fast_thread;
    return OPRT_OS_ADAPTER_COM_ERROR;
}
/**
 * @brief disconnect wifi from connect ap
 *
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_station_disconnect_impl(void)
{
    if (wifi_connection_disconnect_ap() != OPL_ERR_OK) {
        return OPRT_OS_ADAPTER_DISCONN_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief get wifi connect rssi
 *
 * @param[out]      rssi        the return rssi
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_station_get_conn_ap_rssi_impl(signed char *rssi)
{
    if(NULL == rssi) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    if (wifi_connection_get_rssi(rssi) != OPL_ERR_OK) {
        *rssi = 0;
        return OPRT_OS_ADAPTER_RSSI_GET_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief get wifi bssid
 *
 * @param[out]      mac         uplink mac
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_get_bssid_impl(unsigned char *mac)
{
    if(NULL == mac) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    if (tuya_wifi_st != WSS_CONN_SUCCESS) {
        memset(mac, 0, WIFI_MAC_ADDRESS_LENGTH);
        return OPRT_OS_ADAPTER_BSSID_GET_FAILED;
    }
    
    wifi_ap_record_t ap_info = {0};
    if (wifi_sta_get_ap_info(&ap_info) != OPL_ERR_OK) {
        memset(mac, 0, WIFI_MAC_ADDRESS_LENGTH);
        return OPRT_OS_ADAPTER_BSSID_GET_FAILED;
    }

    memcpy(mac, &ap_info.bssid[0], WIFI_MAC_ADDRESS_LENGTH);

    return OPRT_OS_ADAPTER_OK;
}


/**
 * @brief get wifi station work status
 *
 * @param[out]      stat        the wifi station work status
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_station_get_status_impl(WF_STATION_STAT_E *stat)
{
    if(NULL == stat) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    *stat = tuya_wifi_st;

    return OPRT_OS_ADAPTER_OK;
}


/**
 * @brief set wifi country code
 *
 * @param[in]       ccode  country code
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_set_country_code_impl(const COUNTRY_CODE_E ccode)
{
    if (ccode == NULL ||
        ccode > COUNTRY_CODE_EU) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    int i;
    for (i=0; i<3; i++) {
        if (tuya_wifi_country_tb[i].country_code == ccode) {
            strncpy(s_country_num, tuya_wifi_country_tb[i].country_str, 2);
            break;
        }
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief set wifi lowpower mode
 *
 * @param[in]       en    enable or disable
 * @param[in]      dtim
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_set_wifi_lp_mode_impl(const bool en, const unsigned int dtim)
{
	  if(en && ((dtim == 0))) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }
    if (en == 1) {
        if (dtim == 0)        
            wifi_config_set_skip_dtim(0, false);
        else
            wifi_config_set_skip_dtim(dtim - 1, false);
        ps_smart_sleep(POWER_SAVE_EN);
    } 
    else {
        ps_smart_sleep(0);
    }

    return OPRT_OS_ADAPTER_OK;
}

void opl_os_adapt_wifi_pre_init(void)
{
    opl_net_adapter_get_ip_info = net_adapter_get_ip_info_impl;
    opl_wifi_event_handler_cb = wifi_event_handler_cb_impl;
    opl_os_adapt_wifi_init = tuya_os_adapt_wifi_init_impl;
    opl_os_adapt_wifi_ap_scan = tuya_os_adapt_wifi_ap_scan_impl;
    opl_os_adapt_wifi_release_ap = tuya_os_adapt_wifi_release_ap_impl;
    opl_os_adapt_wifi_get_ip = tuya_os_adapt_wifi_get_ip_impl;
    opl_os_adapt_wifi_set_mac = tuya_os_adapt_wifi_set_mac_impl;
    opl_os_adapt_wifi_get_mac = tuya_os_adapt_wifi_get_mac_impl;
    opl_os_adapt_wifi_get_connected_ap_info = tuya_os_adapt_wifi_get_connected_ap_info_impl;
    opl_wifi_station_connect_fast_thread = wifi_station_connect_fast_thread_impl;
    opl_os_adapt_wifi_station_connect = tuya_os_adapt_wifi_station_connect_impl;
    opl_os_adapt_wifi_station_fast_connect = tuya_os_adapt_wifi_station_fast_connect_impl;
    opl_os_adapt_wifi_station_disconnect = tuya_os_adapt_wifi_station_disconnect_impl;
    opl_os_adapt_wifi_station_get_conn_ap_rssi = tuya_os_adapt_wifi_station_get_conn_ap_rssi_impl;
    opl_os_adapt_wifi_get_bssid = tuya_os_adapt_wifi_get_bssid_impl;
    opl_os_adapt_wifi_station_get_status = tuya_os_adapt_wifi_station_get_status_impl;
    opl_os_adapt_wifi_set_country_code = tuya_os_adapt_wifi_set_country_code_impl;
    opl_os_adapt_set_wifi_lp_mode = tuya_os_adapt_set_wifi_lp_mode_impl;
    return;
}

