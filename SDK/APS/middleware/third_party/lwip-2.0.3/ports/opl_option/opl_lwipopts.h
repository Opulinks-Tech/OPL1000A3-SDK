/******************************************************************************
*  Copyright 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _OPL_LWIP_OPTION_H_
#define _OPL_LWIP_OPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "stdint.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define OPL_LWIP_OPTION_DUMP(mode, option) \
do { \
    printf("Dump the opulinks lwip option : \r\n"); \
    printf("  Mode : %d\r\n", mode); \
    printf("  IP   :\r\n"); \
    printf("    Ethernet MTU length : %d\r\n", option.ip.mtu); \
    printf("  TCP  :\r\n"); \
    printf("    Maximum segment size  : %d\r\n", option.tcp.tcp_mss); \
    printf("    Over size             : %d\r\n", option.tcp.tcp_over_size); \
    printf("    Sending buffer        : %d\r\n", option.tcp.tcp_snd_buf); \
    printf("    Sending quene length  : %d\r\n", option.tcp.tcp_snd_queue_len); \
    printf("    Sending low water mark: %d\r\n", option.tcp.tcp_snd_low_wat); \
    printf("    Sending queue low water mark: %d\r\n", option.tcp.tcp_snd_queue_low_wat); \
    printf("    Receive window              : %d\r\n", option.tcp.tcp_receive_wnd); \
    printf("    Window update threshold     : %d\r\n", option.tcp.tcp_wnd_update_threshold); \
    printf("    Timer interval        : %d\r\n", option.tcp.tcp_tmr_interval); \
    printf("    Maximum retry of data : %d\r\n", option.tcp.tcp_max_retry_of_data); \
    printf("    Maximum retry of sync : %d\r\n", option.tcp.tcp_max_retry_of_sync); \
    printf("    BackOff Mode          : %d\r\n", option.retrans.mode); \
    printf("    BackOff Interval      : %d\r\n", option.retrans.tcp_rto); \
    printf("  TCPIP :\r\n"); \
    printf("    Maximum mbox size : %d\r\n", option.tcpip.tcpip_mbox_size); \
    printf("    Maximum tcp receive mbox size : %d\r\n", option.tcpip.tcp_recv_mbox_size); \
    printf("    Maximum udp receive mbox size : %d\r\n", option.tcpip.udp_recv_mbox_size); \
    printf("    Maximum raw receive mbox size : %d\r\n", option.tcpip.raw_recv_mbox_size); \
    printf("    Maximum accept mbox size : %d\r\n", option.tcpip.accept_mbox_size); \
} while(0)

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef enum {
    OPL_LWIP_DEFAULT,
    OPL_LWIP_LOW_THROUGHPUT,
    OPL_LWIP_HIGH_THROUGHPUT,
    OPL_LWIP_CUSTOMER,
}opl_lwip_mode_t;

typedef enum {
    OPL_LWIP_TCP_BACKOFF_NONE,
    OPL_LWIP_TCP_COARSE_GRAINED, /* Refer to const u8_t tcp_backoff */
    OPL_LWIP_TCP_FIXED_INTERVAL, /* If this set, the tcp_rto also given a rto count. */
}opl_lwip_tcp_backoff_mode_t;

typedef struct {
    uint16_t mtu;
}opl_lwip_option_ip_t;

typedef struct {
    uint16_t tcp_mss;
    uint16_t tcp_over_size;
    uint16_t tcp_snd_buf;
    uint16_t tcp_snd_queue_len;
    uint16_t tcp_snd_low_wat;
    uint16_t tcp_snd_queue_low_wat;
    uint16_t tcp_receive_wnd;
    uint32_t tcp_wnd_update_threshold;
    uint32_t tcp_tmr_interval;
    uint16_t tcp_max_retry_of_data;
    uint16_t tcp_max_retry_of_sync;
}opl_lwip_option_tcp_t;

typedef struct {
    opl_lwip_tcp_backoff_mode_t mode;
    uint16_t tcp_rto;                  /* retransmission time-out */
}opl_lwip_option_tcp_retrans_t;

typedef struct {
    uint16_t tcpip_mbox_size;
    uint16_t tcp_recv_mbox_size;
    uint16_t udp_recv_mbox_size;
    uint16_t raw_recv_mbox_size;
    uint16_t accept_mbox_size;
}opl_lwip_option_tcpip_t;

typedef struct {
    opl_lwip_option_ip_t ip;
    opl_lwip_option_tcp_t tcp;
    opl_lwip_option_tcpip_t tcpip;
    opl_lwip_option_tcp_retrans_t retrans;
}opl_lwip_option_t;

typedef int (*opl_lwip_option_default_mode_fp)(void);
typedef int (*opl_lwip_option_low_throughput_mode_fp)(void);
typedef int (*opl_lwip_option_high_throughput_mode_fp)(void);
typedef int (*opl_lwip_option_customer_mode_fp)(opl_lwip_option_t *pOption);
typedef int (*opl_lwip_option_get_fp)(opl_lwip_mode_t *mode, opl_lwip_option_t *pOption);
typedef int (*opl_lwip_option_set_fp)(opl_lwip_mode_t mode, opl_lwip_option_t *pOption);

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern uint16_t opl_lwip_mtu;
extern uint16_t opl_lwip_mtu_update;
extern uint16_t opl_lwip_tcp_mss;
extern uint16_t opl_lwip_tcp_over_size;
extern uint16_t opl_lwip_tcp_snd_buf;
extern uint16_t opl_lwip_tcp_snd_queue_len;
extern uint16_t opl_lwip_tcp_snd_low_wat;
extern uint16_t opl_lwip_tcp_snd_queue_low_wat;
extern uint16_t opl_lwip_tcp_receive_wnd;
extern uint32_t opl_lwip_tcp_wnd_update_threshold;
extern uint32_t opl_lwip_tcp_tmr_interval;
extern uint16_t opl_lwip_tcp_max_retry_of_data;
extern uint16_t opl_lwip_tcp_max_retry_of_sync;
extern uint16_t opl_lwip_tcpip_mbox_size;
extern uint16_t opl_lwip_tcp_recv_mbox_size;
extern uint16_t opl_lwip_udp_recv_mbox_size;
extern uint16_t opl_lwip_raw_recv_mbox_size;
extern uint16_t opl_lwip_accept_mbox_size;
extern uint8_t opl_lwip_tcp_retrans_mode;
extern uint16_t opl_lwip_tcp_fixed_mode_rto_interval;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void opl_lwip_load_interface_option_init(void);

extern opl_lwip_option_default_mode_fp         opl_lwip_option_default_mode;
extern opl_lwip_option_low_throughput_mode_fp  opl_lwip_option_low_throughput_mode;
extern opl_lwip_option_high_throughput_mode_fp opl_lwip_option_high_throughput_mode;
extern opl_lwip_option_customer_mode_fp        opl_lwip_option_customer_mode;
extern opl_lwip_option_get_fp                  opl_lwip_option_get;
extern opl_lwip_option_set_fp                  opl_lwip_option_set;

#ifdef __cplusplus
}
#endif

#endif /* _OPL_LWIP_OPTION_H_ */
