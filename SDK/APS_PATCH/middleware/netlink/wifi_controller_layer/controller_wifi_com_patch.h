/* *****************************************************************************
 *  Copyright 2017-2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file controller_wifi_com_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _CONTROLLER_WIFI_COM_PATCH_H_
#define _CONTROLLER_WIFI_COM_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "controller_wifi_com.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define CONNECT_AP_ERR_OUT_OF_COUNTRY 205

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef enum {
//    STA_PS_NONE,
//    STA_PS_AWAKE_MODE,
//    STA_PS_DOZE_MODE,

//    STA_PS_MODE_NUM
    STA_MODE_LIGHT_PS = STA_PS_MODE_NUM,
} WifiSta_PSForceMode_ext_t;

typedef struct
{
    /*
     * Country code (String):
     *    "CN", "US", "JP", "EU"
     */
    char cc[3];

    /*
     * Start channel 1.
     */
    uint8_t start_chan;

    /*
     * Maximum number of Channel:
     *    "CN" : 13
     *    "US" : 11
     *    "JP" : 14
     *    "EU" : 13
     */
    uint8_t max_chan;
} ctrl_wifi_country_info_t;

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
int ctrl_wifi_set_country_info(ctrl_wifi_country_info_t *info);
int ctrl_wifi_get_country_info(ctrl_wifi_country_info_t *info);

#ifdef __cplusplus
}
#endif
#endif  /* _CONTROLLER_WIFI_COM_PATCH_H_ */
