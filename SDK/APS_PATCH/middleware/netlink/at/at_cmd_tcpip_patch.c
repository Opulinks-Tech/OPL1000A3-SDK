/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "cmsis_os.h"
#include "hal_wdt.h"
#include "at_cmd.h"
#include "at_cmd_tcpip.h"
#include "at_cmd_common.h"
#include "at_cmd_tcpip.h"
#include "at_cmd.h"
#include "at_cmd_data_process.h"
#include "at_cmd_app.h"
#include "at_cmd_nvm.h"

#include "sys_os_config.h"

#include "controller_wifi.h"
#include "wpa_cli.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/netif.h"
#include "lwip/apps/sntp.h"
#include "wlannetif.h"
#include "ping_cmd.h"
#include "wifi_api.h"
#include "network_config.h"
#include "controller_wifi_com.h"
#include "sys_common_api.h"

#include "controller_wifi_patch.h"
#include "at_cmd_tcpip_patch.h"

/******************************************************
 *                      Macros
 ******************************************************/
//#define AT_CMD_TCPIP_DBG

#ifdef AT_CMD_TCPIP_DBG
    #define AT_LOG                      printf(("[AT]: "fmt"\r\n"), ##arg)
#else
    #define AT_LOG(...)
#endif

#define AT_LOGI(fmt,arg...)             printf(("[AT]: "fmt"\r\n"), ##arg)

/******************************************************
 *                    Constants
 ******************************************************/
#define AT_CMD_TCPIP_PING_IDX           16

#define IP_STR_BUF_LEN                  16
#define IP_STR_DELIM                    "."

#define AT_CMD_PING_SERV_PORT_STR       "20" // not used
#define AT_CMD_PING_RECV_TIMEOUT        1000 // ms
#define AT_CMD_PING_PERIOD              10 // ms
#define AT_CMD_PING_NUM                 4
#define AT_CMD_PING_SIZE                32
#define AT_CMD_PING_WAIT_RSP            10 // ms
#define AT_CMD_PING_WAIT_RSP_MAX_CNT    (((AT_CMD_PING_RECV_TIMEOUT + AT_CMD_PING_PERIOD) * (AT_CMD_PING_NUM + 1)) / AT_CMD_PING_WAIT_RSP)


 /******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
 
 /******************************************************
 *                    Structures
 ******************************************************/
 
 /******************************************************
 *               Static Function Declarations
 ******************************************************/

extern uint8_t opl_wifi_ps_mode_restore;
extern ping_result_t g_tPingResult;
extern volatile uint8_t g_u8PingCmdDone;

static void at_cmd_ping_callback_patch(ping_result_t *ptResult)
{
    if(ptResult)
    {
        memcpy((void *)&g_tPingResult, ptResult, sizeof(g_tPingResult));
    }

    g_u8PingCmdDone = 1;
    
    CtrlWifi_PsStateForceInternal(opl_wifi_ps_mode_restore, false);
    return;
}

/*
 * @brief Command at+ping
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
static int at_cmd_tcpip_ping_patch(char *buf, int len, int mode)
{
    int iRet = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;
    char baIpStrBuf[IP_STR_BUF_LEN] = {0};
    char *sTarget = NULL;
    uint32_t u32Cnt = 0;

    if(mode != AT_CMD_MODE_SET)
    {
        AT_LOG("[%s %d] invalid mode[%d]\n", __func__, __LINE__, mode);
        goto done;
    }

    if(!at_cmd_is_tcpip_ready())
    {
        AT_LOG("[%s %d] at_cmd_is_tcpip_ready fail\n", __func__, __LINE__);
        goto done;
    }

    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        AT_LOG("[%s %d] at_cmd_buf_to_argc_argv fail\n", __func__, __LINE__);
        goto done;
    }

    if(argc < 2)
    {
        AT_LOG("[%s %d] no param\n", __func__, __LINE__);
        goto done;
    }

    AT_LOG("[%s %d] argc[%d] argv[1]: [%s][%d]\n", __func__, __LINE__, argc, argv[1], strlen(argv[1]));

    //sTarget = at_cmd_param_trim(argv[1]);
    sTarget = argv[1];

    if(!sTarget)
    {
        AT_LOG("[%s %d] invalid param\n", __func__, __LINE__);
        goto done;
    }

    if(at_cmd_is_valid_ip(sTarget))
    {
        strcpy(baIpStrBuf, sTarget);
    }
    else
    {
        struct addrinfo *ptAddrInfo = NULL;

        if(getaddrinfo(sTarget, AT_CMD_PING_SERV_PORT_STR, NULL, &ptAddrInfo))
        {
            AT_LOG("[%s %d] getaddrinfo fail\n", __func__, __LINE__);
            goto done;
        }

        if(ptAddrInfo)
        {
            struct sockaddr_in *ptAddrIn = (struct sockaddr_in *)ptAddrInfo->ai_addr;

            snprintf(baIpStrBuf, sizeof(baIpStrBuf), "%s", inet_ntoa(ptAddrIn->sin_addr));
            freeaddrinfo(ptAddrInfo);
        }
        else
        {
            AT_LOG("[%s %d] ptAddrInfo is NULL\n", __func__, __LINE__);
            goto done;
        }
    }

    AT_LOG("[%s %d] baIpStrBuf[%s][%d]\n", __func__, __LINE__, baIpStrBuf, strlen(baIpStrBuf));

    g_tPingResult.recv_num = 0;

    g_u8PingCmdDone = 0;

    ping_request(AT_CMD_PING_NUM, baIpStrBuf, PING_IP_ADDR_V4, AT_CMD_PING_SIZE, AT_CMD_PING_RECV_TIMEOUT, AT_CMD_PING_PERIOD, at_cmd_ping_callback_patch);
    
    CtrlWifi_PsStateForceInternal(STA_PS_AWAKE_MODE, true);
    
    while(u32Cnt < AT_CMD_PING_WAIT_RSP_MAX_CNT)
    {
        if(g_u8PingCmdDone)
        {
            if(g_tPingResult.recv_num)
            {
                iRet = 1;
            }

            break;
        }

        osDelay(AT_CMD_PING_WAIT_RSP);
        ++u32Cnt;
    }

done:
    if(iRet)
    {
        at_output("\r\n+PING:%u\r\nOK\r\n", g_tPingResult.avg_time);
    }
    else
    {
        at_output("\r\n+PING:TIMEOUT\r\nERROR\r\n");
    }
    
    return iRet;
}

/******************************************************
 *               Variable Definitions
 ******************************************************/
extern at_command_t *g_AtCmdTbl_Tcpip_Ptr;

 /******************************************************
 *               Function Definitions
 ******************************************************/

 /******************************************************
 *               Interface assignment
 ******************************************************/
void at_cmd_tcpip_api_preinit_patch(void)
{
    g_AtCmdTbl_Tcpip_Ptr[19].cmd_handle = at_cmd_tcpip_ping_patch;
}
