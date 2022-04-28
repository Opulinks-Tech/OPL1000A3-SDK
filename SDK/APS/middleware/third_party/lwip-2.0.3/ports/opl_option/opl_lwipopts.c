#include <string.h>

#include "lwip/opt.h"
#include "lwip/def.h"


/* ---------- IP options ---------- */

/* maximum transfer unit */
LWIP_RETDATA uint16_t opl_lwip_mtu;
LWIP_RETDATA uint16_t opl_lwip_mtu_update;

/* ---------- TCP options ---------- */

/* TCP Maximum segment size. */
LWIP_RETDATA uint16_t opl_lwip_tcp_mss;

/**
 * TCP_OVERSIZE: The maximum number of bytes that tcp_write may
 * allocate ahead of time
 */
LWIP_RETDATA uint16_t opl_lwip_tcp_over_size;

/* TCP sender buffer space (bytes). */
LWIP_RETDATA uint16_t opl_lwip_tcp_snd_buf;

/* TCP sender buffer space (pbufs). This must be at least = 2 *
   TCP_SND_BUF/TCP_MSS for things to work. */
LWIP_RETDATA uint16_t opl_lwip_tcp_snd_queue_len;

/* TCP writable space (bytes). This must be less than or equal
   to TCP_SND_BUF. It is the amount of space which must be
   available in the tcp snd_buf for select to return writable */
LWIP_RETDATA uint16_t opl_lwip_tcp_snd_low_wat;
LWIP_RETDATA uint16_t opl_lwip_tcp_snd_queue_low_wat;

/* TCP receive window. */
LWIP_RETDATA uint16_t opl_lwip_tcp_receive_wnd;

/**
 * TCP_WND_UPDATE_THRESHOLD: difference in window to trigger an
 * explicit window update
 */
LWIP_RETDATA uint32_t opl_lwip_tcp_wnd_update_threshold;

/* TCP_TMR_INTERVAL: TCP timer interval */
LWIP_RETDATA uint32_t opl_lwip_tcp_tmr_interval;

/* Maximum number of retransmissions of data segments. */
LWIP_RETDATA uint16_t opl_lwip_tcp_max_retry_of_data;

/* Maximum number of retransmissions of SYN segments. */
LWIP_RETDATA uint16_t opl_lwip_tcp_max_retry_of_sync;

/* TCP retransmission mode and backoff interval*/
LWIP_RETDATA uint8_t opl_lwip_tcp_retrans_mode;
LWIP_RETDATA uint16_t opl_lwip_tcp_fixed_mode_rto_interval;


/* ---------- TCP/IP options ---------- */
/**
 * TCPIP_MBOX_SIZE: The mailbox size for the tcpip thread messages
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when tcpip_init is called.
 */
LWIP_RETDATA uint16_t opl_lwip_tcpip_mbox_size;
LWIP_RETDATA uint16_t opl_lwip_tcp_recv_mbox_size;
LWIP_RETDATA uint16_t opl_lwip_udp_recv_mbox_size;
LWIP_RETDATA uint16_t opl_lwip_raw_recv_mbox_size;
LWIP_RETDATA uint16_t opl_lwip_accept_mbox_size;

typedef struct {
    uint8_t mode;
    opl_lwip_option_t cfg;
}opl_lwip_option_internal_t;
LWIP_RETDATA opl_lwip_option_internal_t opl_lwip_option;

RET_DATA opl_lwip_option_default_mode_fp         opl_lwip_option_default_mode;
RET_DATA opl_lwip_option_low_throughput_mode_fp  opl_lwip_option_low_throughput_mode;
RET_DATA opl_lwip_option_high_throughput_mode_fp opl_lwip_option_high_throughput_mode;
RET_DATA opl_lwip_option_customer_mode_fp        opl_lwip_option_customer_mode;
RET_DATA opl_lwip_option_get_fp                  opl_lwip_option_get;
RET_DATA opl_lwip_option_set_fp                  opl_lwip_option_set;

int opl_lwip_option_default_mode_impl(void)
{
    /* For Lwip definition */
    opl_lwip_mtu                   = 576;
    opl_lwip_tcp_mss               = opl_lwip_mtu - 40;
    opl_lwip_tcp_over_size         = opl_lwip_tcp_mss;
    opl_lwip_tcp_snd_buf           = 2 * opl_lwip_tcp_mss;
    opl_lwip_tcp_snd_queue_len     = 4 * opl_lwip_tcp_snd_buf / opl_lwip_tcp_mss;
    opl_lwip_tcp_snd_low_wat       = opl_lwip_tcp_snd_buf / 2;
    opl_lwip_tcp_snd_queue_low_wat = LWIP_MAX(((opl_lwip_tcp_snd_queue_len)/2), 5);
    opl_lwip_tcp_receive_wnd       = 4 * opl_lwip_tcp_mss;
    opl_lwip_tcp_wnd_update_threshold = LWIP_MIN((opl_lwip_tcp_receive_wnd / 4), (opl_lwip_tcp_mss * 4));
    opl_lwip_tcp_tmr_interval      = 250;
    opl_lwip_tcp_max_retry_of_data = 12;
    opl_lwip_tcp_max_retry_of_sync = 4;
    opl_lwip_tcpip_mbox_size       = 16;
    opl_lwip_tcp_recv_mbox_size    = 6;
    opl_lwip_udp_recv_mbox_size    = 6;
    opl_lwip_raw_recv_mbox_size    = 6;
    opl_lwip_accept_mbox_size      = 6;
    opl_lwip_tcp_retrans_mode      = OPL_LWIP_TCP_FIXED_INTERVAL;
    opl_lwip_tcp_fixed_mode_rto_interval = 3;

    /* For User setting configuration */
    opl_lwip_option.mode                             = OPL_LWIP_DEFAULT;
    opl_lwip_option.cfg.ip.mtu                       = opl_lwip_mtu;
    opl_lwip_option.cfg.tcp.tcp_mss                  = opl_lwip_tcp_mss;
    opl_lwip_option.cfg.tcp.tcp_over_size            = opl_lwip_tcp_over_size;
    opl_lwip_option.cfg.tcp.tcp_snd_buf              = opl_lwip_tcp_snd_buf;
    opl_lwip_option.cfg.tcp.tcp_snd_queue_len        = opl_lwip_tcp_snd_queue_len;
    opl_lwip_option.cfg.tcp.tcp_snd_low_wat          = opl_lwip_tcp_snd_low_wat;
    opl_lwip_option.cfg.tcp.tcp_snd_queue_low_wat    = opl_lwip_tcp_snd_queue_low_wat;
    opl_lwip_option.cfg.tcp.tcp_receive_wnd          = opl_lwip_tcp_receive_wnd;
    opl_lwip_option.cfg.tcp.tcp_wnd_update_threshold = opl_lwip_tcp_wnd_update_threshold;
    opl_lwip_option.cfg.tcp.tcp_tmr_interval         = opl_lwip_tcp_tmr_interval;
    opl_lwip_option.cfg.tcp.tcp_max_retry_of_data    = opl_lwip_tcp_max_retry_of_data;
    opl_lwip_option.cfg.tcp.tcp_max_retry_of_sync    = opl_lwip_tcp_max_retry_of_sync;
    opl_lwip_option.cfg.tcpip.tcpip_mbox_size        = opl_lwip_tcpip_mbox_size;
    opl_lwip_option.cfg.tcpip.tcp_recv_mbox_size     = opl_lwip_tcp_recv_mbox_size;
    opl_lwip_option.cfg.tcpip.udp_recv_mbox_size     = opl_lwip_udp_recv_mbox_size;
    opl_lwip_option.cfg.tcpip.raw_recv_mbox_size     = opl_lwip_raw_recv_mbox_size;
    opl_lwip_option.cfg.tcpip.accept_mbox_size       = opl_lwip_accept_mbox_size;
    opl_lwip_option.cfg.retrans.mode                 = (opl_lwip_tcp_backoff_mode_t)opl_lwip_tcp_retrans_mode;
    opl_lwip_option.cfg.retrans.tcp_rto              = opl_lwip_tcp_fixed_mode_rto_interval;

    return 0;
}

int opl_lwip_option_low_throughput_mode_impl(void)
{
    /* For Lwip definition */
    opl_lwip_mtu                   = 576;
    opl_lwip_tcp_mss               = opl_lwip_mtu - 40;
    opl_lwip_tcp_over_size         = opl_lwip_tcp_mss;
    opl_lwip_tcp_snd_buf           = opl_lwip_tcp_mss;
    opl_lwip_tcp_snd_queue_len     = 4 * opl_lwip_tcp_snd_buf / opl_lwip_tcp_mss;
    opl_lwip_tcp_snd_low_wat       = opl_lwip_tcp_snd_buf / 2;
    opl_lwip_tcp_snd_queue_low_wat = LWIP_MAX(((opl_lwip_tcp_snd_queue_len)/2), 5);
    opl_lwip_tcp_receive_wnd       = opl_lwip_tcp_mss;
    opl_lwip_tcp_wnd_update_threshold = LWIP_MIN((opl_lwip_tcp_receive_wnd / 4), (opl_lwip_tcp_mss * 4));
    opl_lwip_tcp_tmr_interval      = 250;
    opl_lwip_tcp_max_retry_of_data = 12;
    opl_lwip_tcp_max_retry_of_sync = 4;
    opl_lwip_tcpip_mbox_size       = 8;
    opl_lwip_tcp_recv_mbox_size    = 3;
    opl_lwip_udp_recv_mbox_size    = 3;
    opl_lwip_raw_recv_mbox_size    = 3;
    opl_lwip_accept_mbox_size      = 3;
    opl_lwip_tcp_retrans_mode      = OPL_LWIP_TCP_FIXED_INTERVAL;
    opl_lwip_tcp_fixed_mode_rto_interval = 3;

    /* For User setting configuration */
    opl_lwip_option.mode                             = OPL_LWIP_LOW_THROUGHPUT;
    opl_lwip_option.cfg.ip.mtu                       = opl_lwip_mtu;
    opl_lwip_option.cfg.tcp.tcp_mss                  = opl_lwip_tcp_mss;
    opl_lwip_option.cfg.tcp.tcp_over_size            = opl_lwip_tcp_over_size;
    opl_lwip_option.cfg.tcp.tcp_snd_buf              = opl_lwip_tcp_snd_buf;
    opl_lwip_option.cfg.tcp.tcp_snd_queue_len        = opl_lwip_tcp_snd_queue_len;
    opl_lwip_option.cfg.tcp.tcp_snd_low_wat          = opl_lwip_tcp_snd_low_wat;
    opl_lwip_option.cfg.tcp.tcp_snd_queue_low_wat    = opl_lwip_tcp_snd_queue_low_wat;
    opl_lwip_option.cfg.tcp.tcp_receive_wnd          = opl_lwip_tcp_receive_wnd;
    opl_lwip_option.cfg.tcp.tcp_wnd_update_threshold = opl_lwip_tcp_wnd_update_threshold;
    opl_lwip_option.cfg.tcp.tcp_tmr_interval         = opl_lwip_tcp_tmr_interval;
    opl_lwip_option.cfg.tcp.tcp_max_retry_of_data    = opl_lwip_tcp_max_retry_of_data;
    opl_lwip_option.cfg.tcp.tcp_max_retry_of_sync    = opl_lwip_tcp_max_retry_of_sync;
    opl_lwip_option.cfg.tcpip.tcpip_mbox_size        = opl_lwip_tcpip_mbox_size;
    opl_lwip_option.cfg.tcpip.tcp_recv_mbox_size     = opl_lwip_tcp_recv_mbox_size;
    opl_lwip_option.cfg.tcpip.udp_recv_mbox_size     = opl_lwip_udp_recv_mbox_size;
    opl_lwip_option.cfg.tcpip.raw_recv_mbox_size     = opl_lwip_raw_recv_mbox_size;
    opl_lwip_option.cfg.tcpip.accept_mbox_size       = opl_lwip_accept_mbox_size;
    opl_lwip_option.cfg.retrans.mode                 = (opl_lwip_tcp_backoff_mode_t)opl_lwip_tcp_retrans_mode;
    opl_lwip_option.cfg.retrans.tcp_rto              = opl_lwip_tcp_fixed_mode_rto_interval;

    return 0;
}

int opl_lwip_option_high_throughput_mode_impl(void)
{
    /* For Lwip definition */
    opl_lwip_mtu                   = 576;
    opl_lwip_tcp_mss               = opl_lwip_mtu - 40;
    opl_lwip_tcp_over_size         = opl_lwip_tcp_mss;
    opl_lwip_tcp_snd_buf           = 50 * opl_lwip_tcp_mss;
    opl_lwip_tcp_snd_queue_len     = 4 * opl_lwip_tcp_snd_buf / opl_lwip_tcp_mss;
    opl_lwip_tcp_snd_low_wat       = LWIP_MIN(LWIP_MAX(((opl_lwip_tcp_snd_buf)/2), (2 * opl_lwip_tcp_mss) + 1), (opl_lwip_tcp_snd_buf) - 1);
    opl_lwip_tcp_snd_queue_low_wat = LWIP_MAX(((opl_lwip_tcp_snd_queue_len)/2), 5);
    opl_lwip_tcp_receive_wnd       = 50 * opl_lwip_tcp_mss;
    opl_lwip_tcp_wnd_update_threshold = LWIP_MIN((opl_lwip_tcp_receive_wnd / 4), (opl_lwip_tcp_mss * 4));
    opl_lwip_tcp_tmr_interval      = 250;
    opl_lwip_tcp_max_retry_of_data = 12;
    opl_lwip_tcp_max_retry_of_sync = 4;
    opl_lwip_tcpip_mbox_size       = 64;
    opl_lwip_tcp_recv_mbox_size    = 64;
    opl_lwip_udp_recv_mbox_size    = 64;
    opl_lwip_raw_recv_mbox_size    = 64;
    opl_lwip_accept_mbox_size      = 64;
    opl_lwip_tcp_retrans_mode      = OPL_LWIP_TCP_COARSE_GRAINED;
    opl_lwip_tcp_fixed_mode_rto_interval = 0;

    /* For User setting configuration */
    opl_lwip_option.mode                             = OPL_LWIP_HIGH_THROUGHPUT;
    opl_lwip_option.cfg.ip.mtu                       = opl_lwip_mtu;
    opl_lwip_option.cfg.tcp.tcp_mss                  = opl_lwip_tcp_mss;
    opl_lwip_option.cfg.tcp.tcp_over_size            = opl_lwip_tcp_over_size;
    opl_lwip_option.cfg.tcp.tcp_snd_buf              = opl_lwip_tcp_snd_buf;
    opl_lwip_option.cfg.tcp.tcp_snd_queue_len        = opl_lwip_tcp_snd_queue_len;
    opl_lwip_option.cfg.tcp.tcp_snd_low_wat          = opl_lwip_tcp_snd_low_wat;
    opl_lwip_option.cfg.tcp.tcp_snd_queue_low_wat    = opl_lwip_tcp_snd_queue_low_wat;
    opl_lwip_option.cfg.tcp.tcp_receive_wnd          = opl_lwip_tcp_receive_wnd;
    opl_lwip_option.cfg.tcp.tcp_wnd_update_threshold = opl_lwip_tcp_wnd_update_threshold;
    opl_lwip_option.cfg.tcp.tcp_tmr_interval         = opl_lwip_tcp_tmr_interval;
    opl_lwip_option.cfg.tcp.tcp_max_retry_of_data    = opl_lwip_tcp_max_retry_of_data;
    opl_lwip_option.cfg.tcp.tcp_max_retry_of_sync    = opl_lwip_tcp_max_retry_of_sync;
    opl_lwip_option.cfg.tcpip.tcpip_mbox_size        = opl_lwip_tcpip_mbox_size;
    opl_lwip_option.cfg.tcpip.tcp_recv_mbox_size     = opl_lwip_tcp_recv_mbox_size;
    opl_lwip_option.cfg.tcpip.udp_recv_mbox_size     = opl_lwip_udp_recv_mbox_size;
    opl_lwip_option.cfg.tcpip.raw_recv_mbox_size     = opl_lwip_raw_recv_mbox_size;
    opl_lwip_option.cfg.tcpip.accept_mbox_size       = opl_lwip_accept_mbox_size;
    opl_lwip_option.cfg.retrans.mode                 = (opl_lwip_tcp_backoff_mode_t)opl_lwip_tcp_retrans_mode;
    opl_lwip_option.cfg.retrans.tcp_rto              = opl_lwip_tcp_fixed_mode_rto_interval;

    return 0;
}

int opl_lwip_option_customer_mode_impl(opl_lwip_option_t *pOption)
{
    if (pOption->ip.mtu != 0) {
        opl_lwip_mtu = pOption->ip.mtu;
        opl_lwip_option.cfg.ip.mtu = pOption->ip.mtu;
    }

    if (pOption->tcp.tcp_mss != 0) {
        opl_lwip_tcp_mss = pOption->tcp.tcp_mss;
        opl_lwip_option.cfg.tcp.tcp_mss = pOption->tcp.tcp_mss;
    }

    if (pOption->tcp.tcp_over_size != 0) {
        opl_lwip_tcp_over_size = pOption->tcp.tcp_over_size;
        opl_lwip_option.cfg.tcp.tcp_over_size = pOption->tcp.tcp_over_size;
    }

    if (pOption->tcp.tcp_snd_buf != 0) {
        opl_lwip_tcp_snd_buf = pOption->tcp.tcp_snd_buf;
        opl_lwip_option.cfg.tcp.tcp_snd_buf = pOption->tcp.tcp_snd_buf;
    }

    if (pOption->tcp.tcp_snd_queue_len != 0) {
        opl_lwip_tcp_snd_queue_len = pOption->tcp.tcp_snd_queue_len;
        opl_lwip_option.cfg.tcp.tcp_snd_queue_len = pOption->tcp.tcp_snd_queue_len;
    }

    if (pOption->tcp.tcp_snd_low_wat != 0) {
        opl_lwip_tcp_snd_low_wat = pOption->tcp.tcp_snd_low_wat;
        opl_lwip_option.cfg.tcp.tcp_snd_low_wat = pOption->tcp.tcp_snd_low_wat;
    }

    if (pOption->tcp.tcp_snd_queue_low_wat != 0) {
        opl_lwip_tcp_snd_queue_low_wat = pOption->tcp.tcp_snd_queue_low_wat;
        opl_lwip_option.cfg.tcp.tcp_snd_queue_low_wat = pOption->tcp.tcp_snd_queue_low_wat;
    }

    if (pOption->tcp.tcp_receive_wnd != 0) {
        opl_lwip_tcp_receive_wnd = pOption->tcp.tcp_receive_wnd;
        opl_lwip_option.cfg.tcp.tcp_receive_wnd = pOption->tcp.tcp_receive_wnd;
    }

    if (pOption->tcp.tcp_wnd_update_threshold != 0) {
        opl_lwip_tcp_wnd_update_threshold = pOption->tcp.tcp_wnd_update_threshold;
        opl_lwip_option.cfg.tcp.tcp_wnd_update_threshold = pOption->tcp.tcp_wnd_update_threshold;
    }

    if (pOption->tcp.tcp_tmr_interval != 0) {
        opl_lwip_tcp_tmr_interval = pOption->tcp.tcp_tmr_interval;
        opl_lwip_option.cfg.tcp.tcp_tmr_interval = pOption->tcp.tcp_tmr_interval;
    }

    if (pOption->tcp.tcp_max_retry_of_data != 0) {
        opl_lwip_tcp_max_retry_of_data = pOption->tcp.tcp_max_retry_of_data;
        opl_lwip_option.cfg.tcp.tcp_max_retry_of_sync = pOption->tcp.tcp_max_retry_of_data;;
    }

    if (pOption->tcp.tcp_max_retry_of_sync != 0) {
        opl_lwip_tcp_max_retry_of_sync = pOption->tcp.tcp_max_retry_of_sync;
        opl_lwip_option.cfg.tcp.tcp_max_retry_of_sync = pOption->tcp.tcp_max_retry_of_sync;
    }

    if (pOption->tcpip.tcpip_mbox_size != 0) {
        opl_lwip_tcpip_mbox_size = pOption->tcpip.tcpip_mbox_size;
        opl_lwip_option.cfg.tcpip.tcpip_mbox_size = pOption->tcpip.tcpip_mbox_size;
    }

    if (pOption->tcpip.tcp_recv_mbox_size != 0) {
        opl_lwip_tcp_recv_mbox_size = pOption->tcpip.tcp_recv_mbox_size;
        opl_lwip_option.cfg.tcpip.tcp_recv_mbox_size = pOption->tcpip.tcp_recv_mbox_size;
    }

    if (pOption->tcpip.udp_recv_mbox_size != 0) {
        opl_lwip_udp_recv_mbox_size = pOption->tcpip.udp_recv_mbox_size;
        opl_lwip_option.cfg.tcpip.udp_recv_mbox_size = pOption->tcpip.udp_recv_mbox_size;
    }

    if (pOption->tcpip.raw_recv_mbox_size != 0) {
        opl_lwip_raw_recv_mbox_size = pOption->tcpip.raw_recv_mbox_size;
        opl_lwip_option.cfg.tcpip.raw_recv_mbox_size = pOption->tcpip.raw_recv_mbox_size;
    }

    if (pOption->tcpip.accept_mbox_size != 0) {
        opl_lwip_accept_mbox_size = pOption->tcpip.accept_mbox_size;
        opl_lwip_option.cfg.tcpip.accept_mbox_size = pOption->tcpip.accept_mbox_size;
    }

    if (pOption->retrans.mode != 0) {
        opl_lwip_tcp_retrans_mode = pOption->retrans.mode;
        opl_lwip_option.cfg.retrans.mode = pOption->retrans.mode;
    }

    if (pOption->retrans.tcp_rto != 0) {
        opl_lwip_tcp_fixed_mode_rto_interval = pOption->retrans.tcp_rto;
        opl_lwip_option.cfg.retrans.tcp_rto = pOption->retrans.tcp_rto;
    }

    opl_lwip_option.mode = OPL_LWIP_CUSTOMER;


    return 0;
}

int opl_lwip_option_get_impl(opl_lwip_mode_t *mode, opl_lwip_option_t *pOption)
{
    if ((mode == NULL) || (pOption == NULL)) {
        return -1;
    }

    *mode = (opl_lwip_mode_t)opl_lwip_option.mode;
    memcpy(pOption, &opl_lwip_option.cfg, sizeof(opl_lwip_option_t));

    return 0;
}

int opl_lwip_option_set_impl(opl_lwip_mode_t mode, opl_lwip_option_t *pOption)
{
    switch(mode) {
        case OPL_LWIP_DEFAULT:
            opl_lwip_option_default_mode();
            break;
        case OPL_LWIP_LOW_THROUGHPUT:
            opl_lwip_option_low_throughput_mode();
            break;
        case OPL_LWIP_HIGH_THROUGHPUT:
            opl_lwip_option_high_throughput_mode();
            break;
        case OPL_LWIP_CUSTOMER:
            if (pOption == NULL) {
                return -1;
            }
            opl_lwip_option_customer_mode(pOption);
            break;
        default:
            break;
    }
    return 0;
}

void opl_lwip_load_interface_option_init(void)
{
    /* Load interface */
    opl_lwip_option_default_mode         = opl_lwip_option_default_mode_impl;
    opl_lwip_option_low_throughput_mode  = opl_lwip_option_low_throughput_mode_impl;
    opl_lwip_option_high_throughput_mode = opl_lwip_option_high_throughput_mode_impl;
    opl_lwip_option_customer_mode        = opl_lwip_option_customer_mode_impl;
    opl_lwip_option_get                  = opl_lwip_option_get_impl;
    opl_lwip_option_set                  = opl_lwip_option_set_impl;

    /* Load default configuration */
    opl_lwip_option_default_mode();

    /* set opl_lwip_mtu_update value <= opl_lwip_mtu */
    opl_lwip_mtu_update = opl_lwip_mtu;
}
