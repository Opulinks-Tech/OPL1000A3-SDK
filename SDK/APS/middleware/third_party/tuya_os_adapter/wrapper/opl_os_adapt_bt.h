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

#ifndef __OPL_OS_ADAPT_BT_H__
#define __OPL_OS_ADAPT_BT_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_bt.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef int (*t_opl_adapt_ble_init_fp)(tuya_ble_data_buf_t *adv, tuya_ble_data_buf_t *res, TUYA_BLE_EVENT_CB cb);
typedef int (*t_opl_adapt_ble_start_fp)(void);
typedef int (*t_opl_adapt_ble_deinit_fp)(void);
typedef int (*t_opl_adapt_ble_send_fp)(tuya_ble_data_buf_t *buf);
typedef int (*t_opl_os_adapt_bt_gap_disconnect_fp)(void);
typedef int (*t_opl_os_adapt_bt_get_rssi_fp)(SCHAR_T *rssi);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_adapt_ble_init_fp opl_adapt_ble_init;
extern t_opl_adapt_ble_start_fp opl_adapt_ble_start;
extern t_opl_adapt_ble_deinit_fp opl_adapt_ble_deinit;
extern t_opl_adapt_ble_send_fp opl_adapt_ble_send;
extern t_opl_os_adapt_bt_gap_disconnect_fp opl_os_adapt_bt_gap_disconnect;
extern t_opl_os_adapt_bt_get_rssi_fp opl_os_adapt_bt_get_rssi;

// Sec 5: declaration of global function prototype
void opl_os_adapt_bt_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_BT_H__
