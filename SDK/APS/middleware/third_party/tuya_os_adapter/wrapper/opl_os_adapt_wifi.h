/******************************************************************************
*  Copyright 2017 - 2020, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2020
******************************************************************************/

#ifndef __OPL_OS_ADAPT_WIFI_H__
#define __OPL_OS_ADAPT_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_wifi.h"
#include "event_loop.h"

    #ifdef ENOMEM
    #undef ENOMEM
#endif

#ifdef EINVAL
    #undef EINVAL
#endif

#ifdef EDOM
    #undef EDOM
#endif

#ifdef ERANGE
    #undef ERANGE
#endif

#ifdef EILSEQ
    #undef EILSEQ
#endif

#include "lwip/sockets.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
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

typedef int (*t_opl_net_adapter_get_ip_info_fp)(net_adapter_if_t tcpip_if, net_adapter_ip_info_t *ip_info);
typedef int (*t_opl_wifi_event_handler_cb_fp)(wifi_event_id_t event_id, void *data, uint16_t length);
typedef int (*t_opl_os_adapt_wifi_init_fp)(ty_wifi_event_cb cb);
typedef int (*t_opl_os_adapt_wifi_ap_scan_fp)(char *ssid, AP_IF_S **ap_ary, unsigned int *num);
typedef int (*t_opl_os_adapt_wifi_release_ap_fp)(AP_IF_S *ap);
typedef int (*t_opl_os_adapt_wifi_get_ip_fp)(const WF_IF_E wf, NW_IP_S *ip);
typedef int (*t_opl_os_adapt_wifi_set_mac_fp)(const WF_IF_E wf, const NW_MAC_S *mac);
typedef int (*t_opl_os_adapt_wifi_get_mac_fp)(const WF_IF_E wf, NW_MAC_S *mac);
typedef int (*t_opl_os_adapt_wifi_get_connected_ap_info_fp)(FAST_WF_CONNECTED_AP_INFO_V2_S **fast_ap_info);
typedef void (*t_opl_wifi_station_connect_fast_thread_fp)(void* pArg);
typedef int (*t_opl_os_adapt_wifi_station_connect_fp)(const signed char *ssid, const signed char *passwd);
typedef int (*t_opl_os_adapt_wifi_station_fast_connect_fp)(const FAST_WF_CONNECTED_AP_INFO_V2_S *fast_ap_info);
typedef int (*t_opl_os_adapt_wifi_station_disconnect_fp)(void);
typedef int (*t_opl_os_adapt_wifi_station_get_conn_ap_rssi_fp)(signed char *rssi);
typedef int (*t_opl_os_adapt_wifi_get_bssid_fp)(unsigned char *mac);
typedef int (*t_opl_os_adapt_wifi_station_get_status_fp)(WF_STATION_STAT_E *stat);
typedef int (*t_opl_os_adapt_wifi_set_country_code_fp)(const COUNTRY_CODE_E ccode);
typedef int (*t_opl_os_adapt_set_wifi_lp_mode_fp)(const bool en, const unsigned int dtim);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_net_adapter_get_ip_info_fp opl_net_adapter_get_ip_info;
extern t_opl_wifi_event_handler_cb_fp opl_wifi_event_handler_cb;
extern t_opl_os_adapt_wifi_init_fp opl_os_adapt_wifi_init;
extern t_opl_os_adapt_wifi_ap_scan_fp opl_os_adapt_wifi_ap_scan;
extern t_opl_os_adapt_wifi_release_ap_fp opl_os_adapt_wifi_release_ap;
extern t_opl_os_adapt_wifi_get_ip_fp opl_os_adapt_wifi_get_ip;
extern t_opl_os_adapt_wifi_set_mac_fp opl_os_adapt_wifi_set_mac;
extern t_opl_os_adapt_wifi_get_mac_fp opl_os_adapt_wifi_get_mac;
extern t_opl_os_adapt_wifi_get_connected_ap_info_fp opl_os_adapt_wifi_get_connected_ap_info;
extern t_opl_wifi_station_connect_fast_thread_fp opl_wifi_station_connect_fast_thread;
extern t_opl_os_adapt_wifi_station_connect_fp opl_os_adapt_wifi_station_connect;
extern t_opl_os_adapt_wifi_station_fast_connect_fp opl_os_adapt_wifi_station_fast_connect;
extern t_opl_os_adapt_wifi_station_disconnect_fp opl_os_adapt_wifi_station_disconnect;
extern t_opl_os_adapt_wifi_station_get_conn_ap_rssi_fp opl_os_adapt_wifi_station_get_conn_ap_rssi;
extern t_opl_os_adapt_wifi_get_bssid_fp opl_os_adapt_wifi_get_bssid;
extern t_opl_os_adapt_wifi_station_get_status_fp opl_os_adapt_wifi_station_get_status;
extern t_opl_os_adapt_wifi_set_country_code_fp opl_os_adapt_wifi_set_country_code;
extern t_opl_os_adapt_set_wifi_lp_mode_fp opl_os_adapt_set_wifi_lp_mode;

// Sec 5: declaration of global function prototype
void opl_os_adapt_wifi_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_WIFI_H__
