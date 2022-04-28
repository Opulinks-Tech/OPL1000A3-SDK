/*
 * WPA Supplicant - command line interface for wpa_supplicant daemon
 * Copyright (c) 2004-2010, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cmsis_os.h"
#include "controller_wifi.h"
#include "controller_wifi_com.h"
#include "driver_netlink.h"
#include "msg.h"
#include "scrt.h"
#include "sys_common_ctrl.h" //I0060 0000710
#include "wpa.h"
#include "wpa_cli.h"
#include "wpa_debug.h"
#include "wifi_nvm.h"
#include "wpa_supplicant_i.h"

RET_DATA u8 g_bssid[6];// = {0};
RET_DATA struct wpa_scan_results res;
RET_DATA struct wpa_config conf;
RET_DATA struct wpa_config rec_conf;
RET_DATA struct wpa_ssid ssid_data;
RET_DATA struct wpa_ssid rec_ssid_data;

extern struct wpa_supplicant *wpa_s;
extern char g_passphrase[MAX_LEN_OF_PASSWD];
extern auto_connect_cfg_t g_AutoConnect;
extern u8 gAutoConnMode;
extern u8 gsta_cfg_mac[MAC_ADDR_LEN];

Boolean isMAC(char *s) {
    int i;
    for(i = 0; i < 17; i++) {
        if(i % 3 != 2 && !isxdigit(s[i]))
            return FALSE;
        if(i % 3 == 2 && s[i] != ':')
            return FALSE;
    }
    if(s[17] != '\0')
        return FALSE;
    return TRUE;
}

int wpa_cli_connect_impl(struct wpa_config * conf)
{
    if (conf == NULL) return -1;
    if (conf->ssid == NULL) return -1;
    
    scan_info_t *pInfo = NULL;
    hap_control_t *hap_temp = get_hap_control_struct();
    
    pInfo = get_target_ap_record(conf->ssid->bssid, (char*)conf->ssid->ssid);
    if (pInfo == NULL) {
        if (hap_temp->hap_final_index != 0){
            // Security check move to wifi_get_scan_record_by_ssid() from here.
            if (!wpa_driver_netlink_connect(conf)) return -1;
            return 0;
        }
        else {
            return -3;
        }
    }
    
    if (!target_ap_security_mode_chk(pInfo)) return -1;
    
    return wpa_driver_netlink_connect(conf);
}

int wpa_cli_connect_by_bssid_impl(u8 *bssid)
{
    if (bssid == NULL) return FALSE;
    wpa_driver_netlink_connect_by_bssid(bssid);
    return TRUE;
}

int wpa_cli_connect_handler_impl(int argc, char *argv[])
{
    u8 bssid[6] = {0};
    int i;
    char passwd[MAX_LEN_OF_PASSWD] = {0};
    int len_passwd = 0;
    int len_ssid = 0;
    int ret = -1;
    
    if(argc <= 1) {
        wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
        goto DONE;
    }

    memset(&conf, 0, sizeof(conf));
    memset(&ssid_data, 0, sizeof(ssid_data));
    conf.ssid = &ssid_data;

    memset(g_passphrase, 0, MAX_LEN_OF_PASSWD);
    memset(bssid, 0, sizeof(bssid)/sizeof(bssid[0]));
    if(conf.ssid == NULL){
        goto DONE;
    }

    if(isMAC(argv[1])) //wpa_connect "bssid" "passphase"
    {
        //bssid
        hwaddr_aton2(argv[1], bssid);

        for (i=0; i<ETH_ALEN; i++)
        {
            conf.ssid->bssid[i] = bssid[i];
            g_bssid[i] = bssid[i];
        }

        //passwd
        if(argc >= 3)
        {
            len_passwd = os_strlen(argv[2]);

            if ((len_passwd >= MAX_LEN_OF_PASSWD) || (len_passwd < MIN_LEN_OF_PASSWD))
            {
                tracer_cli(LOG_HIGH_LEVEL, "[CLI]WPA: invalid parameter \r\n");
                goto DONE;
            }

            memset(passwd, 0, MAX_LEN_OF_PASSWD);
            os_memcpy(passwd, argv[2], len_passwd);

            if(len_passwd > 0)
            {
                os_memcpy((void *)g_passphrase, (void *)passwd, MAX_LEN_OF_PASSWD);
            }
        }

    }
    else //wpa_connect "ssid" "passphase" "bssid of hidden AP"
    {
        //ssid
        len_ssid = os_strlen(argv[1]);
        if(len_ssid > MAX_LEN_OF_SSID)
        {
            len_ssid = MAX_LEN_OF_SSID;
        }
        conf.ssid->ssid = os_malloc(MAX_LEN_OF_SSID + 1);
        memset(conf.ssid->ssid, 0, (MAX_LEN_OF_SSID + 1));
        os_memcpy(conf.ssid->ssid, argv[1], len_ssid);

        //passwd
        if(argc >= 3)
        {
            len_passwd = os_strlen(argv[2]);

            if ((len_passwd >= MAX_LEN_OF_PASSWD) || (len_passwd < MIN_LEN_OF_PASSWD))
            {
                tracer_cli(LOG_HIGH_LEVEL, "[CLI]WPA: invalid parameter \r\n");
                goto DONE;
            }

            memset(passwd, 0, MAX_LEN_OF_PASSWD);
            os_memcpy(passwd, argv[2], len_passwd);

            if(len_passwd > 0)
            {
                os_memcpy((void *)g_passphrase, (void *)passwd, MAX_LEN_OF_PASSWD);
            }
        }

        //bssid for hidden ap connect
        if(argc >= 4)
        {
            if(isMAC(argv[3]))
            {
                hwaddr_aton2(argv[3], bssid);

                for (i=0; i<ETH_ALEN; i++)
                {
                    conf.ssid->bssid[i] = bssid[i];
                    g_bssid[i] = bssid[i];
                }
            }
        }
    }

    ret = wpa_cli_connect(&conf);
    
DONE:
    if (conf.ssid->ssid) {
        os_free(conf.ssid->ssid);
        conf.ssid->ssid = NULL;
    }
    
    return ret;
}

int wpa_cli_disconnect_impl(const u8 *bssid, u16 reason_code)
{
    return wpa_driver_netlink_disconnect(bssid, 0);
}

int wpa_cli_disconnect_handler_impl(int argc, char *argv[])
{
    return wpa_cli_disconnect(NULL, 0);
}

int wpa_cli_reconnect_impl(struct wpa_config * conf)
{
    if (conf == NULL) return FALSE;
    if (conf->ssid == NULL) return FALSE;
    wpa_driver_netlink_reconnect(conf);
    return TRUE;
}

int wpa_cli_reconnect_handler_impl(int argc, char *argv[])
{
    int i;

    memset(&rec_conf, 0, sizeof(rec_conf));
    memset(&rec_ssid_data, 0, sizeof(rec_ssid_data));

    rec_conf.ssid=&rec_ssid_data;
    if(rec_conf.ssid == NULL) return FALSE;

    for (i=0; i<ETH_ALEN; i++) {
        rec_conf.ssid->bssid[i] = g_bssid[i]; //conf.ssid->bssid => xxxxxxxxxxxx
    }
    wpa_printf(MSG_DEBUG, "[CLI]WPA: reconnect bssid=%02x:%02x:%02x:%02x:%02x:%02x \r\n",
                                rec_conf.ssid->bssid[0],
                                rec_conf.ssid->bssid[1],
                                rec_conf.ssid->bssid[2],
                                rec_conf.ssid->bssid[3],
                                rec_conf.ssid->bssid[4],
                                rec_conf.ssid->bssid[5]);
    wpa_cli_reconnect(&rec_conf);

    return 1;
}

int wpa_cli_scan_impl(int mode)
{
    return wpa_driver_netlink_scan(mode);
}

int wpa_cli_scan_handler_impl(int argc, char *argv[])
{
    int mode = SCAN_MODE_MIX;

    if(argc > 1) {
        mode = atoi(argv[1]);
        if (mode < SCAN_MODE_ACTIVE || mode > SCAN_MODE_MIX) {
            wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
            return FALSE;
        }
    }

    wpa_printf(MSG_DEBUG, "[CLI]WPA: scan mode=%d \r\n", mode);
    return wpa_cli_scan(mode);
}

int wpa_cli_scan_by_cfg_impl(void *cfg)
{
    if (cfg == NULL) return FALSE;

    S_WIFI_MLME_SCAN_CFG *scan_cfg = (S_WIFI_MLME_SCAN_CFG *)cfg;

    /* Duration */
    if (scan_cfg->u32ActiveScanDur < SCAN_MIN_DURATION_TIME || 
        scan_cfg->u32ActiveScanDur > SCAN_MAX_NUM_OF_DUR_TIME) {
        scan_cfg->u32ActiveScanDur = SCAN_ACTIVE_MIN_DUR_TIME_DEF;
    }

    if (scan_cfg->u32PassiveScanDur < SCAN_MIN_DURATION_TIME ||
        scan_cfg->u32PassiveScanDur > SCAN_MAX_NUM_OF_DUR_TIME) {
        scan_cfg->u32PassiveScanDur = SCAN_PASSIVE_MIN_DUR_TIME_DEF;
    }
    
    /* channel */
    if (scan_cfg->u8Channel > WIFI_MLME_SCAN_MAX_NUM_CHANNELS) {
        scan_cfg->u8Channel = WIFI_MLME_SCAN_ALL_CHANNELS;
    }
    
    /* re-send counter */
    if (scan_cfg->u8ResendCnt == 0) {
        scan_cfg->u8ResendCnt = SCAN_PROBE_REQ_COUNTERS_DEF;
    }
    
    /* mac address */
    if (is_broadcast_ether_addr(scan_cfg->u8aBssid))
        return FALSE;
    
    if (is_multicast_ether_addr(scan_cfg->u8aBssid))
        return FALSE;

    return wpa_driver_netlink_scan_by_cfg(scan_cfg);
}

int wpa_cli_scan_stop_handler_impl(int argc, char *argv[])
{
    return wpa_driver_netlink_scan_stop();
}

int wpa_cli_getscanresults_impl(struct wpa_scan_results * res)
{
    if(res == NULL) return FALSE;
    return wpa_driver_netlink_get_scan_results(res);
}

void wpa_cli_showscanresults_handler_impl(int argc, char *argv[])
{
    wpa_driver_netlink_show_scan_results();
}

void wpa_cli_getbssid_impl(u8 *bssid)
{
    if(bssid == NULL) return;
    wpa_driver_netlink_get_bssid(bssid);
}

void wpa_cli_showbssid_by_param_impl(int argc, char *argv[])
{
    u8 bssid[6] = {0};
    memset(bssid, 0, sizeof(bssid));
    wpa_driver_netlink_get_bssid(bssid);
    wpa_printf(MSG_DEBUG, "[CLI]WPA: bssid=%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                              bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
}

void wpa_cli_getssid_impl(u8 *ssid)
{
    if(ssid == NULL) return;
    wpa_driver_netlink_get_ssid(ssid);
}

void wpa_cli_showssid_by_param_impl(int argc, char *argv[])
{
    u8 ssid[MAX_LEN_OF_SSID + 1] = {0};
    int i;

    memset(ssid, 0, sizeof(ssid));
    if(wpa_s->wpa_state == WPA_ASSOCIATED || wpa_s->wpa_state == WPA_COMPLETED)
    {
        memset(ssid, 0, sizeof(ssid));
        wpa_cli_getssid(ssid);
        wpa_printf(MSG_DEBUG, "[CLI]WPA: ssid=");
        for(i = 0; i < sizeof(ssid); i++)
        {
            wpa_printf(MSG_DEBUG, "%c", ssid[i]);
        }
        wpa_printf(MSG_DEBUG, "\r\n");
    }
}

void wpa_cli_getmac_impl(u8 *mac)
{
    if(mac == NULL) return;
    wpa_driver_netlink_sta_cfg(MLME_CMD_GET_PARAM, E_WIFI_PARAM_MAC_ADDRESS, mac);
}

int check_mac_addr_len_impl(const char *txt)
{
    u8 count = 0;
    const char *pos = txt;

    while (*pos != NULL) {
        while (*pos == ':' || *pos == '.' || *pos == '-')
            pos++;

        *pos++;
        *pos++;
        count++;
    }

    if (count != MAC_ADDR_LEN)
        return -1;

    return 0;
}

void wpa_cli_setmac_impl(u8 *mac)
{
    if(mac == NULL) return;

    if (is_broadcast_ether_addr(mac)) {
        tracer_cli(LOG_HIGH_LEVEL, "[CLI]WPA: Invalid mac address, all of mac if 0xFF \r\n");
        return;
    }

    if (is_multicast_ether_addr(mac)) {
        tracer_cli(LOG_HIGH_LEVEL, "[CLI]WPA: Invalid mac address, not allow multicast mac address \r\n");
        return;
    }
    
    if (is_zero_ether_addr(mac)) {
        tracer_cli(LOG_HIGH_LEVEL, "[CLI]WPA: Invalid mac address, all of mac is zero. \r\n");
        return;
    }

    memset(&gsta_cfg_mac[0], 0, MAC_ADDR_LEN);
    memcpy(&gsta_cfg_mac[0], &mac[0], MAC_ADDR_LEN);

    wifi_nvm_sta_info_write(WIFI_NVM_STA_INFO_ID_MAC_ADDR, MAC_ADDR_LEN, gsta_cfg_mac);
}

void wpa_cli_mac_by_param_impl(int argc, char *argv[])
{
    u8 mac[6] = {0};

    if(argc == 1) //show mac
    {
        memset(mac, 0, sizeof(mac)/sizeof(mac[0]));
        wpa_cli_getmac(mac);
        osEnterCritical();
        tracer_cli(LOG_HIGH_LEVEL, "[CLI]Wifi STA mac address: %02x:%02x:%02x:%02x:%02x:%02x \r\n",
                                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        osExitCritical();
    }
    else if (argc == 2) //set mac
    {
        memset(mac, 0, sizeof(mac)/sizeof(mac[0]));
        
        if (check_mac_addr_len(argv[1]) == -1) {
            tracer_cli(LOG_HIGH_LEVEL, "[CLI]Invalid mac address, wrong length of mac address \r\n");
            return;
        }
        
        if (hwaddr_aton2(argv[1], mac) == -1) {
            tracer_cli(LOG_HIGH_LEVEL, "[CLI]Invalid mac address \r\n");
            return;
        }
        
        wpa_cli_setmac(mac);
    }
}

int wpa_cli_getrssi_impl(void)
{
    int rssi = wpa_driver_netlink_get_rssi();
    wpa_printf(MSG_INFO, "[CLI]WPA: rssi=%d \r\n", rssi);

    return rssi;
}

extern void itp_scrt_test(void);
extern void aes_ccm_sample(void);
extern void aes_ecb_sample(void);
extern void hmac_sha_1_sample(void);

void wpa_cli_scrt_dbg_by_param_impl(int argc, char *argv[])
{
#if 0
    itp_scrt_test();
    aes_ccm_sample();
    aes_ecb_sample();
    hmac_sha_1_sample();
#endif
}

int wpa_cli_clean_ac_list_impl(void)
{
    // Reset Auto/Fast connect configuartion in the FIM
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_MODE, 0);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_NUM, 0);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_CFG, 0);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO, 0);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO, 1);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO, 2);

    // Reset Auto/Fast connect configuration of global variables
    reset_auto_connect_list();

    return TRUE;
}

int wpa_cli_fast_connect_set_impl(int argc, char *argv[])
{
    u8 mode, ap_index;
    
    if(argc != 3) {
        wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
        return FALSE;
    }

    mode = atoi(argv[1]);
    ap_index = atoi(argv[2]);

    if (mode > AUTO_CONNECT_ENABLE || ap_index >= MAX_NUM_OF_AUTO_CONNECT) {
        wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
        return FALSE;
    }

    wpa_driver_netlink_fast_connect_set(mode, ap_index);
    
    return TRUE;
}

void debug_cli_mac_addr_src_impl(void)
{
    u8 type;

    tracer_cli(LOG_HIGH_LEVEL, "Mac Address Source:\r\n");
    tracer_cli(LOG_HIGH_LEVEL, "   0:OTP, 1:Flash\r\n");
    base_mac_addr_src_get_cfg(BASE_NVM_MAC_SRC_IFACE_ID_STA, &type);
    tracer_cli(LOG_HIGH_LEVEL, "   WIFI STA : %d", type);
    base_mac_addr_src_get_cfg(BASE_NVM_MAC_SRC_IFACE_ID_SOFTAP, &type);
    tracer_cli(LOG_HIGH_LEVEL, "   SoftAP: %d", type);
    base_mac_addr_src_get_cfg(BASE_NVM_MAC_SRC_IFACE_ID_BLE, &type);
    tracer_cli(LOG_HIGH_LEVEL, "   BLE : %d\r\n", type);
}

void debug_auto_connect_impl(void)
{
    int i,j;
    MwFimAutoConnectCFG_t cfg;
    auto_conn_info_t info;
    
    tracer_cli(LOG_HIGH_LEVEL, "AP mode = %d\r\n", get_auto_connect_mode());
    tracer_cli(LOG_HIGH_LEVEL, "AP num  = %d\r\n", get_auto_connect_ap_num());
    
    get_auto_connect_ap_cfg(&cfg);
    tracer_cli(LOG_HIGH_LEVEL, "AP cfg flag = %d, front = %d, rear = %d\r\n", cfg.flag, cfg.front, cfg.rear);
    tracer_cli(LOG_HIGH_LEVEL, "AP cfg max_save_num = %d\r\n", cfg.max_save_num);

    for (i=0; i<MAX_NUM_OF_AUTO_CONNECT; i++) {
        get_auto_connect_info(i, &info);
        tracer_cli(LOG_HIGH_LEVEL, "AP[%d]  channel   = %d\r\n", i, info.ap_channel);
        tracer_cli(LOG_HIGH_LEVEL, "       fast mode = %d\r\n", info.fast_connect);
        tracer_cli(LOG_HIGH_LEVEL, "       bssid     = %02x %02x %02x %02x %02x %02x\r\n",
                    info.bssid[0], info.bssid[1], info.bssid[2], info.bssid[3], info.bssid[4],
                    info.bssid[5]);
        tracer_cli(LOG_HIGH_LEVEL, "       ssid = %s\r\n", info.ssid);
        tracer_cli(LOG_HIGH_LEVEL, "       psk  = ");
        for (j=0; j<32; j++) {
            tracer_cli(LOG_HIGH_LEVEL, "%02x", info.psk[j]);
        }
        tracer_cli(LOG_HIGH_LEVEL, "\r\n");
        tracer_cli(LOG_HIGH_LEVEL, "       DTIM period = %d \r\n", info.dtim_prod);
    }
}

/* debug use */
int wpa_cli_dbg_impl(int argc, char *argv[])
{
    if (argc == 1) {
        osEnterCritical();
        debug_auto_connect();
        debug_cli_mac_addr_src();
        tracer_cli(LOG_HIGH_LEVEL, "state:%d\r\n", wifi_ctrl_state_get());
        osExitCritical();
        return TRUE;
    }
    
    if (argc > 1) {
        if (!strcmp(argv[1], "clean_ac")) {
            wpa_cli_clean_ac_list();
        }
        else if (!strcmp(argv[1], "set_addr_from")) {
            if (argc > 3) {
                u8 iface, type;
                iface = atoi(argv[2]);
                type = atoi(argv[3]);
                base_mac_addr_src_set_cfg(iface, type);
            }
        }
        else if (!strcmp(argv[1], "rf")) {
            if (argc > 2) {
                int on_off = atoi(argv[2]);
                if (on_off == 1) {
                    CtrlWifi_PsStateForce(STA_PS_AWAKE_MODE, 0);
                }
                else if (on_off == 0) {
                    CtrlWifi_PsStateForce(STA_PS_NONE, 0);
                }
            }
        }
        else if (!strcmp(argv[1], "auto_conn_mode")) {
            if (argc > 2) {
                int mode = atoi(argv[2]);
                wifi_nvm_auto_connect_write(WIFI_NVM_ID_AUTO_CONNECT_MODE, 1, NULL, &mode);
                gAutoConnMode = mode;
                g_AutoConnect.retryCount = 0; //restart fasct connect
                g_AutoConnect.targetIdx = 0;
            }
        }
        else if (!strcmp(argv[1], "auto_conn_start")) {
            wifi_auto_connect_req();
        }
    }
    
    tracer_cli(LOG_HIGH_LEVEL, "ok\r\n");
    return TRUE;
}

int wpa_cli_cmd_reset_impl(int argc, char *argv[])
{
    return wpa_driver_netlink_reset();
}

int wpa_cli_cmd_level_impl(int argc, char *argv[])
{
    if (argc == 1) {
        tracer_cli(LOG_HIGH_LEVEL, "Current level: %d \n", wpa_driver_netlink_get_log_level());
        tracer_cli(LOG_HIGH_LEVEL, "EXCESSIVE = 0, MSGDUMP = 1, DEBUG = 2, INFO = 3, WARNING = 4, ERROR = 5 \n");
    }
    else {
        u8 level = atoi(argv[1]);
        if (level > 5) {
            return FALSE;
        }
        wpa_driver_netlink_set_log_level(level);
        
        tracer_cli(LOG_HIGH_LEVEL, "Current level: %d \n", wpa_driver_netlink_get_log_level());
        tracer_cli(LOG_HIGH_LEVEL, "EXCESSIVE = 0, MSGDUMP = 1, DEBUG = 2, INFO = 3, WARNING = 4, ERROR = 5 \n");
    }
    return TRUE;
}

int wpa_cli_cmd_status_impl(int argc, char *argv[])
{
    char buf[64] = {0};
    u8 mac[MAC_ADDR_LEN] = {0};
    u8 ssid[MAX_LEN_OF_SSID + 1] = {0};
    
    if (wpa_s->wpa_state >= WPA_ASSOCIATED) {
        osEnterCritical();
        tracer_cli(LOG_HIGH_LEVEL, "bssid=" MACSTR "\n", MAC2STR(wpa_s->bssid));
        tracer_cli(LOG_HIGH_LEVEL, "freq=%u \n", wpa_s->assoc_freq);
        wpa_cli_getssid(ssid);
        tracer_cli(LOG_HIGH_LEVEL, "ssid=%s \n", ssid);
        wpa_sm_get_status(wpa_s->wpa, buf, sizeof(buf), 0);
        tracer_cli(LOG_HIGH_LEVEL, "%s", buf);
        osExitCritical();
    }
    
    wpa_cli_getmac(mac);
    
    osEnterCritical();
    tracer_cli(LOG_HIGH_LEVEL, "wpa_state=%s \n", wpa_supplicant_state_txt(wpa_s->wpa_state));
    tracer_cli(LOG_HIGH_LEVEL, "address=" MACSTR "\n", MAC2STR(mac));
    osExitCritical();

    return TRUE;
}

int wpa_cli_set_encrypt_mode_impl(int argc, char *argv[])
{
    int ret=TRUE;
    int en_value=0;
    if(argc!=2)
    {
         wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
         return FALSE;
    }    
    en_value=atoi(argv[1]);
    ret=wpa_driver_netlink_set_encrypt_mode(en_value);
    
    return ret;
}

uint32_t wpa_cli_cmd_handler_impl(int argc, char *argv[])
{
    if (argc < 1) return FALSE;
    tracer_cli(LOG_HIGH_LEVEL, "\n");

    if (os_strncasecmp(WPA_CLI_CMD_SCAN, argv[0], os_strlen(argv[0])) == 0){
        wpa_cli_scan_handler(argc, argv);
    } else if(strncasecmp(WPA_CLI_CMD_SCAN_STOP, argv[0], os_strlen(WPA_CLI_CMD_SCAN_STOP)) == 0) {
        wpa_cli_scan_stop_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_SCAN_RESULTS, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_showscanresults_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_CONNECT, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_connect_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_RECONNECT, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_reconnect_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_DISCONNECT, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_disconnect_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_BSSID, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_showbssid_by_param(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_SSID, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_showssid_by_param(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_HELP, argv[0], os_strlen(argv[0])) == 0) {
        wpa_printf(MSG_INFO, "[CLI]WPA: not support yet \r\n");
    } else if (os_strncasecmp(WPA_CLI_CMD_STATE, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_cmd_status(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_MAC, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_mac_by_param(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_SCRT_DEBUG, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_scrt_dbg_by_param(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_RSSI, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_getrssi();
    } else if (os_strncasecmp(WPA_CLI_CMD_FAST_CONNECT, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_fast_connect_set(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_DBG, argv[0], os_strlen(WPA_CLI_CMD_DBG)) == 0) {
        wpa_cli_dbg(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_RESET, argv[0], os_strlen(WPA_CLI_CMD_RESET)) == 0) {
        wpa_cli_cmd_reset(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_LOG_LEVEL, argv[0], os_strlen(WPA_CLI_CMD_LOG_LEVEL)) == 0) {
        wpa_cli_cmd_level(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_SET_ENCRYPT_MODE, argv[0], os_strlen(WPA_CLI_CMD_SET_ENCRYPT_MODE)) == 0) {
        wpa_cli_set_encrypt_mode(argc, argv);
    } else {
        //nothing
        msg_print(LOG_HIGH_LEVEL, "\r\n");
    }

    return TRUE;
}


void wpa_cli_parse_impl(char* pszData) //pszData => wpa_xxx parm1 parm2 ...
{
    char *argv[WPA_CLI_CMD_NUM_MAX] = {0}; //argv[0]:wap_xxx argv[1]:parm1 argv[2]:parm3
    int count = 0;
    char *p = NULL;
    char *pSavedPtr = NULL;

    /* get the first word from the message, seperated by
          space character */
    p = strtok_r(pszData, " ", &pSavedPtr); //word1: wpa_xxx
    argv[count] = p; //count = 0
    count++;

    /* the following loop gets the rest of the words until the
     * end of the message */
    while ((p = strtok_r(NULL, " ", &pSavedPtr)) != NULL){ //p: parmx
        argv[count] = p;
        count++;
    }

    wpa_cli_cmd_handler(count, argv);
}

RET_DATA wpa_cli_connect_fp_t wpa_cli_connect;
RET_DATA wpa_cli_connect_by_bssid_fp_t wpa_cli_connect_by_bssid;
RET_DATA wpa_cli_connect_handler_fp_t wpa_cli_connect_handler;
RET_DATA wpa_cli_disconnect_fp_t wpa_cli_disconnect;
RET_DATA wpa_cli_disconnect_handler_fp_t wpa_cli_disconnect_handler;
RET_DATA wpa_cli_reconnect_fp_t wpa_cli_reconnect;
RET_DATA wpa_cli_reconnect_handler_fp_t wpa_cli_reconnect_handler;
RET_DATA wpa_cli_scan_fp_t wpa_cli_scan;
RET_DATA wpa_cli_scan_handler_fp_t wpa_cli_scan_handler;
RET_DATA wpa_cli_scan_stop_handler_fp_t wpa_cli_scan_stop_handler;
RET_DATA wpa_cli_getscanresults_fp_t wpa_cli_getscanresults;
RET_DATA wpa_cli_showscanresults_handler_fp_t wpa_cli_showscanresults_handler;
RET_DATA wpa_cli_getbssid_fp_t wpa_cli_getbssid;
RET_DATA wpa_cli_showbssid_by_param_fp_t wpa_cli_showbssid_by_param;
RET_DATA wpa_cli_getssid_fp_t wpa_cli_getssid;
RET_DATA wpa_cli_showssid_by_param_fp_t wpa_cli_showssid_by_param;
RET_DATA wpa_cli_getmac_fp_t wpa_cli_getmac;
RET_DATA wpa_cli_mac_by_param_fp_t wpa_cli_mac_by_param;
RET_DATA wpa_cli_setmac_fp_t wpa_cli_setmac;
RET_DATA wpa_cli_parse_fp_t wpa_cli_parse;
RET_DATA wpa_cli_scrt_dbg_by_param_fp_t wpa_cli_scrt_dbg_by_param;
RET_DATA wpa_cli_cmd_handler_fp_t wpa_cli_cmd_handler;
RET_DATA wpa_cli_getrssi_fp_t wpa_cli_getrssi;
RET_DATA debug_auto_connect_fp_t debug_auto_connect;
RET_DATA wpa_cli_clean_ac_list_fp_t wpa_cli_clean_ac_list;
RET_DATA wpa_cli_fast_connect_set_fp_t wpa_cli_fast_connect_set;
RET_DATA wpa_cli_dbg_fp_t wpa_cli_dbg;
RET_DATA wpa_cli_cmd_reset_fp_t wpa_cli_cmd_reset;
RET_DATA wpa_cli_cmd_level_fp_t wpa_cli_cmd_level;
RET_DATA wpa_cli_set_encrypt_mode_fp_t wpa_cli_set_encrypt_mode;   //I0064 0000907

RET_DATA check_mac_addr_len_fp_t check_mac_addr_len; //I0060 0000662
RET_DATA debug_cli_mac_addr_src_fp_t debug_cli_mac_addr_src; //I0060 0000710
RET_DATA wpa_cli_scan_by_cfg_fp_t wpa_cli_scan_by_cfg;
RET_DATA wpa_cli_cmd_status_fp_t wpa_cli_cmd_status;

/*
   Interface Initialization: WPA CLI
 */
void wpa_cli_func_init(void)
{
    wpa_cli_connect                 = wpa_cli_connect_impl;
    wpa_cli_connect_by_bssid        = wpa_cli_connect_by_bssid_impl;
    wpa_cli_connect_handler         = wpa_cli_connect_handler_impl;
    wpa_cli_disconnect              = wpa_cli_disconnect_impl;
    wpa_cli_disconnect_handler      = wpa_cli_disconnect_handler_impl;
    wpa_cli_reconnect               = wpa_cli_reconnect_impl;
    wpa_cli_reconnect_handler       = wpa_cli_reconnect_handler_impl;
    wpa_cli_scan                    = wpa_cli_scan_impl;
    wpa_cli_scan_handler            = wpa_cli_scan_handler_impl;
    wpa_cli_scan_stop_handler       = wpa_cli_scan_stop_handler_impl;
    wpa_cli_getscanresults          = wpa_cli_getscanresults_impl;
    wpa_cli_showscanresults_handler = wpa_cli_showscanresults_handler_impl;
    wpa_cli_getbssid                = wpa_cli_getbssid_impl;
    wpa_cli_showbssid_by_param      = wpa_cli_showbssid_by_param_impl;
    wpa_cli_getssid                 = wpa_cli_getssid_impl;
    wpa_cli_showssid_by_param       = wpa_cli_showssid_by_param_impl;
    wpa_cli_getmac                  = wpa_cli_getmac_impl;
    wpa_cli_mac_by_param            = wpa_cli_mac_by_param_impl;
    wpa_cli_setmac                  = wpa_cli_setmac_impl;
    wpa_cli_parse                   = wpa_cli_parse_impl;
    wpa_cli_scrt_dbg_by_param       = wpa_cli_scrt_dbg_by_param_impl;
    wpa_cli_cmd_handler             = wpa_cli_cmd_handler_impl;
    wpa_cli_getrssi                 = wpa_cli_getrssi_impl;
    debug_auto_connect              = debug_auto_connect_impl;
    wpa_cli_clean_ac_list           = wpa_cli_clean_ac_list_impl;
    wpa_cli_fast_connect_set        = wpa_cli_fast_connect_set_impl;
    wpa_cli_dbg                     = wpa_cli_dbg_impl;
    wpa_cli_cmd_reset               = wpa_cli_cmd_reset_impl;
    check_mac_addr_len              = check_mac_addr_len_impl; //I0060 0000662
    debug_cli_mac_addr_src          = debug_cli_mac_addr_src_impl; //I0060 0000710
    wpa_cli_scan_by_cfg             = wpa_cli_scan_by_cfg_impl;
    wpa_cli_cmd_level               = wpa_cli_cmd_level_impl;
    wpa_cli_cmd_status              = wpa_cli_cmd_status_impl;
    wpa_cli_set_encrypt_mode        = wpa_cli_set_encrypt_mode_impl; //I0064 0000907
}
