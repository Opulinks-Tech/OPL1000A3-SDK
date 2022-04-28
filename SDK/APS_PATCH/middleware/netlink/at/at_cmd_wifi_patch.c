/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2019
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "at_cmd.h"
#include "at_cmd_common.h"
#include "at_cmd_data_process.h"
#include "at_cmd_nvm.h"
#include "at_cmd_msg_ext.h"
#include "controller_wifi_com.h"
#include "wpa_at_if.h"
#include "wpa_cli.h"
#include "driver_netlink.h"


#include "at_cmd_wifi_patch.h"
#include "controller_wifi_com_patch.h"

extern at_command_t *g_AtCmdTbl_Wifi_Ptr;

/*
 * @brief Command at+cwjap
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_wifi_cwjap_patch(char *buf, int len, int mode)
{
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;
    //char msg[128] = {0};
    int state;
    int rssi;
    u8 bssid[MAC_ADDR_LEN] = {0};
    u8 ssid[MAX_LEN_OF_SSID + 1] = {0};
    int freq;
    u8 ret = FALSE;

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    switch(mode)
    {
        case AT_CMD_MODE_READ:
            state = wpas_get_state();

            if(state == WPA_COMPLETED || state == WPA_ASSOCIATED)
            {
                memset(bssid, 0, MAC_ADDR_LEN);
                memset(ssid, 0, MAX_LEN_OF_SSID + 1);
                wpa_cli_getssid(ssid);
                wpa_cli_getbssid(bssid);
                freq = wpas_get_assoc_freq();
                rssi = wpa_driver_netlink_get_rssi();

                msg_print_uart1("\r\n+CWJAP:");
                msg_print_uart1("%s,", ssid);
                msg_print_uart1("%02x:%02x:%02x:%02x:%02x:%02x,", bssid[0], bssid[1], bssid[2],bssid[3], bssid[4], bssid[5]);
                msg_print_uart1("%d,", freq);
                msg_print_uart1("%d\r\n", rssi);
            }
            else
            {
                msg_print_uart1("\r\nNo AP connected\r\n");
            }
            msg_print_uart1("\r\nOK\r\n");
            break;

        case AT_CMD_MODE_EXECUTION:
            break;

        case AT_CMD_MODE_SET:
            if (argc == 3) {
                if (strlen(argv[2]) >= MAX_LEN_OF_PASSWD) {
                    msg_print_uart1("\r\n+CWJAP:%d\r\n", ERR_WIFI_CWJAP_PWD_INVALID);
                    msg_print_uart1("\r\nERROR\r\n");
                    return false;
                }
            }

            ret = wpa_cli_connect_handler(argc, argv);

            if (ret) {
                at_msg_ext_wifi_connect(AT_MSG_EXT_ESPRESSIF, ERR_WIFI_CWJAP_NO_AP);
            }
            break;

        case AT_CMD_MODE_TESTING:
            break;

        default:
            break;
    }


    return true;
}

void at_cmd_wifi_func_init_patch(void)
{
    g_AtCmdTbl_Wifi_Ptr[1].cmd_handle = at_cmd_wifi_cwjap_patch;
}
