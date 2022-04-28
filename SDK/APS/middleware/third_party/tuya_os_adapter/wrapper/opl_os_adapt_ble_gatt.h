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

#ifndef __OPL_OS_ADAPT_BLE_GATT_H__
#define __OPL_OS_ADAPT_BLE_GATT_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "le_gatt_if.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef void (*T_OplBleAppHandleGattServiceWriteFp)(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind);
typedef void (*T_OplBleAppHandleKnsServiceWriteFp)(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind);
typedef void (*T_OplBleGattHandleAccessReadFp)(LE_GATT_MSG_ACCESS_READ_IND_T *ind);
typedef void (*T_OplBleGattHandleAccessWriteFp)(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind);
typedef void (*T_OplBleAppGattMsgHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern T_OplBleAppHandleGattServiceWriteFp OplBleAppHandleGattServiceWrite;
extern T_OplBleAppHandleKnsServiceWriteFp OplBleAppHandleKnsServiceWrite;
extern T_OplBleGattHandleAccessReadFp OplBleGattHandleAccessRead;
extern T_OplBleGattHandleAccessWriteFp OplBleGattHandleAccessWrite;
extern T_OplBleAppGattMsgHandlerFp OplBleAppGattMsgHandler;

// Sec 5: declaration of global function prototype
void opl_os_adapt_ble_gatt_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_BLE_GATT_H__
