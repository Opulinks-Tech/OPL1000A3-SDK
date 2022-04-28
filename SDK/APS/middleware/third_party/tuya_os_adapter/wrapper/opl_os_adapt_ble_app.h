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

#ifndef __OPL_OS_ADAPT_BLE_APP_H__
#define __OPL_OS_ADAPT_BLE_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_ble_app.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef void (*T_OplBleAppSetAdvtisingParaFp)(UINT8 type, UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, UINT8 filter);
typedef void (*T_OplBleAppSmMsgHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_OplBleCmMsgHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_OplBleAppHandleTuyaInitFp)(TUYA_BLE_INIT_T *req);
typedef void (*T_OplBleAppHandleTuyaStartFp)(void);
typedef void (*T_OplBleAppHandleTuyaDeinitFp)(void);
typedef void (*T_OplBleAppHandleTuyaDisconnectFp)(void);
typedef void (*T_OplBleAppHandleTuyaSendDataFp)(TUYA_BLE_SEND_T *req);
typedef void (*T_OplBleAppHandleTuyaGetRssiFp)(void);
typedef void (*T_OplBleAppMsgHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_OplBleAppTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_OplBleAppCbTaskFp)(void *pvParameters);
typedef BOOL (*T_OplBleAppCreateCbTaskFp)(void *ins);
typedef BOOL (*T_OplBleAppInitFp)(void);
typedef void (*T_OplBleAppApiSemaCheckFp)(void);
typedef void (*T_OplBleAppApiSemaReleaseFp)(void);
typedef void (*T_OplBleAppWaitApiResultFp)(void);
typedef void (*T_OplBleAppRstSemaReleaseFp)(void);
typedef void (*T_OplBleAppCbSemaCheckFp)(BOOL wait);
typedef void (*T_OplBleAppCbNotifyFp)(UINT32 evt, UINT16 len, void *data);
typedef void (*T_OplBleAppStopApiEvtFp)(INT32 rst);
typedef BLE_APP_DATA_T* (*T_OplBleAppDevFp)(void);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern T_OplBleAppSetAdvtisingParaFp OplBleAppSetAdvtisingPara;
extern T_OplBleAppSmMsgHandlerFp OplBleAppSmMsgHandler;
extern T_OplBleCmMsgHandlerFp OplBleCmMsgHandler;
extern T_OplBleAppHandleTuyaInitFp OplBleAppHandleTuyaInit;
extern T_OplBleAppHandleTuyaStartFp OplBleAppHandleTuyaStart;
extern T_OplBleAppHandleTuyaDeinitFp OplBleAppHandleTuyaDeinit;
extern T_OplBleAppHandleTuyaDisconnectFp OplBleAppHandleTuyaDisconnect;
extern T_OplBleAppHandleTuyaSendDataFp OplBleAppHandleTuyaSendData;
extern T_OplBleAppHandleTuyaGetRssiFp OplBleAppHandleTuyaGetRssi;
extern T_OplBleAppMsgHandlerFp OplBleAppMsgHandler;
extern T_OplBleAppTaskHandlerFp OplBleAppTaskHandler;
extern T_OplBleAppCbTaskFp OplBleAppCbTask;
extern T_OplBleAppCreateCbTaskFp OplBleAppCreateCbTask;
extern T_OplBleAppInitFp OplBleAppInit;
extern T_OplBleAppApiSemaCheckFp OplBleAppApiSemaCheck;
extern T_OplBleAppApiSemaReleaseFp OplBleAppApiSemaRelease;
extern T_OplBleAppWaitApiResultFp OplBleAppWaitApiResult;
extern T_OplBleAppRstSemaReleaseFp OplBleAppRstSemaRelease;
extern T_OplBleAppCbSemaCheckFp OplBleAppCbSemaCheck;
extern T_OplBleAppCbNotifyFp OplBleAppCbNotify;
extern T_OplBleAppStopApiEvtFp OplBleAppStopApiEvt;
extern T_OplBleAppDevFp OplBleAppDev;

// Sec 5: declaration of global function prototype
void opl_os_adapt_ble_app_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_BLE_APP_H__
