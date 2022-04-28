/**
 * @file tuya_os_adapt_wifi.h
 * @brief WIFI设备操作接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */

#ifndef __TUYA_OS_ADAPT_WIFI_H__
#define __TUYA_OS_ADAPT_WIFI_H__


#include <stdbool.h>
#include "tuya_os_adapter_error_code.h"
#include "tuya_os_adapt_network.h"


#ifdef __cplusplus
extern "C" {
#endif


/* tuya sdk definition of wifi ap info */
#define WIFI_SSID_LEN 32    // tuya sdk definition WIFI SSID MAX LEN
#define WIFI_PASSWD_LEN 64  // tuya sdk definition WIFI PASSWD MAX LEN
#define  DTIM_SKIP_COUNT    30
#define  POWER_SAVE_EN      1
typedef struct
{
    unsigned char channel;                 ///< AP channel
    signed char rssi;                      ///< AP rssi
    unsigned char bssid[6];                ///< AP bssid
    unsigned char ssid[WIFI_SSID_LEN+1];   ///< AP ssid array
    unsigned char s_len;                   ///< AP ssid len
    unsigned char resv1;
}AP_IF_S;

/* for fast connect*/
typedef struct {
    unsigned int len;                                                    ///< data len
    unsigned char data[0];                                                ///< data buff
}FAST_WF_CONNECTED_AP_INFO_V2_S;

typedef enum {
    COUNTRY_CODE_CN,
    COUNTRY_CODE_US,
    COUNTRY_CODE_JP,
    COUNTRY_CODE_EU
} COUNTRY_CODE_E;

/* tuya sdk definition of wifi function type */
typedef enum
{
    WF_STATION = 0,     ///< station type
    WF_AP,              ///< ap type
}WF_IF_E;

/* tuya sdk definition of wifi station work status */
typedef enum {
    WSS_IDLE = 0,                       ///< not connected
    WSS_CONNECTING,                     ///< connecting wifi
    WSS_PASSWD_WRONG,                   ///< passwd not match
    WSS_NO_AP_FOUND,                    ///< ap is not found
    WSS_CONN_FAIL,                      ///< connect fail
    WSS_CONN_SUCCESS,                   ///< connect wifi success
    WSS_GOT_IP,                         ///< get ip success
}WF_STATION_STAT_E;


typedef enum {
    TY_WIFI_CONNECTED,
    TY_WIFI_CONNECT_FAILED,
    TY_WIFI_DISCONNECTED,
    TY_WIFI_SCAN_DONE,
} TY_WF_EVENT;

typedef void (*ty_wifi_event_cb)(TY_WF_EVENT event, void *arg);
/**
 * @brief init wifi
 * 
 * @param[in]       cb          wifi event callback
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_init(ty_wifi_event_cb cb);

/**
 * @brief scan current environment and obtain all the ap
 *        infos in current environment
 * 
 * @param[in]       ssid        ap ssid(null--scan all,else only scan specified ssid)
 * @param[out]      ap_ary      current ap info array
 * @param[out]      num         the num of ar_ary
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_ap_scan(char *ssid, AP_IF_S **ap_ary, unsigned int *num);

/**
 * @brief release the memory malloced in <tuya_os_adapt_wifi_all_ap_scan>
 *        and <tuya_os_adapt_wifi_assign_ap_scan> if needed. tuya-sdk
 *        will call this function when the ap info is no use.
 * 
 * @param[in]       ap          the ap info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_release_ap(AP_IF_S *ap);

/**
 * @brief get wifi ip info.when wifi works in
 *        ap+station mode, wifi has two ips.
 * 
 * @param[in]       wf          wifi function type
 * @param[out]      ip          the ip addr info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_get_ip(const WF_IF_E wf, NW_IP_S *ip);

/**
 * @brief set wifi mac info.when wifi works in
 *        ap+station mode, wifi has two macs.
 * 
 * @param[in]       wf          wifi function type
 * @param[in]       mac         the mac info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_set_mac(const WF_IF_E wf, const NW_MAC_S *mac);

/**
 * @brief get wifi mac info.when wifi works in
 *        ap+station mode, wifi has two macs.
 * 
 * @param[in]       wf          wifi function type
 * @param[out]      mac         the mac info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_get_mac(const WF_IF_E wf, NW_MAC_S *mac);

/**
 * @brief connect wifi with ssid and passwd
 * 
 * @param[in]       ssid
 * @param[in]       passwd
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_station_connect(const signed char *ssid, const signed char *passwd);

/**
 * @brief : fast connect
 * @param[in]      fast_ap_info
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_station_fast_connect(const FAST_WF_CONNECTED_AP_INFO_V2_S *fast_ap_info);

/**
 * @brief disconnect wifi from connect ap
 * 
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_station_disconnect(void);

/**
 * @brief get wifi connect rssi
 * 
 * @param[out]      rssi        the return rssi
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_station_get_conn_ap_rssi(signed char *rssi);

/**
 * @brief get wifi bssid
 * 
 * @param[out]      mac         uplink mac
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_get_bssid(unsigned char *mac);

/**
 * @brief get wifi station work status
 * 
 * @param[out]      stat        the wifi station work status
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_station_get_status(WF_STATION_STAT_E *stat);

/**
 * @brief set wifi country code
 * 
 * @param[in]       ccode  country code
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_wifi_set_country_code(const COUNTRY_CODE_E ccode);

/**
 * @brief set wifi lowpower mode
 * 
 * @param[in]       en    enable or disable
 * @param[in]      dtim   
 * @return  OPRT_OS_ADAPTER_OK: success  Other: fail
 */
int tuya_os_adapt_set_wifi_lp_mode(const bool en, const unsigned int dtim);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif



