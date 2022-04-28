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
 *  @file controller_wifi_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _CONTROLLER_WIFI_PATCH_H_
#define _CONTROLLER_WIFI_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "controller_wifi.h"
#include "controller_wifi_com_patch.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define WIFI_PHY_BYP_KEEP_ADDR    0x40002010
#define WIFI_PHY_ENABLE_EQU_VAL   0x00100000
#define WIFI_PHY_DISABLE_EQU_VAL  0x00100020

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */



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
void wifi_ctrl_init_patch(void);
int CtrlWifi_PsStateForceInternal(uint8_t mode, uint8_t force_debug);
int ctrl_wifi_reg_write(uint32_t addr, uint32_t val);

#ifdef __cplusplus
}
#endif
#endif  /* _CONTROLLER_WIFI_PATCH_H_ */
