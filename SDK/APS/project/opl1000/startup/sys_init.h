/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  sys_init.h
*
*  Project:
*  --------
*  OPL1000 Project - the system initialize definition file
*
*  Description:
*  ------------
*  This include file is the system initialize definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _SYS_INIT_H_
#define _SYS_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdint.h>
#include "cmsis_os.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define SYS_SPARE_LOAD_PATCH_READY  (1 << 3)
#define SYS_SPARE_MSQ_CLOCK_READY   (1 << 4)
#define SYS_SPARE_DEEP_SLEEP_EN     (1 << 5)
#define SYS_SPARE_MSQ_FLASH_READY   (1 << 6)
#define SYS_SPARE_APS_CLOCK_READY   (1 << 7)
    

/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
typedef void (*T_Sys_PowerSetup_fp)(void);
typedef void (*T_Sys_ClockSetup_fp)(void);
typedef void (*T_Sys_UartInit_fp)(void);
typedef void (*T_Sys_MiscModulesInit_fp)(void);
typedef void (*T_Sys_DriverInit_fp)(void);
typedef void (*T_Sys_WaitforMsqReady_fp)(uint32_t indicator);
typedef void (*T_Sys_NotifyReadyToMsq_fp)(uint32_t indicator);
typedef void (*T_Sys_SetUnsuedSramEndBound_fp)(uint32_t);
typedef void (*T_Sys_SwitchOffUnusedSram_fp)(uint32_t, uint32_t);
typedef void (*T_Sys_SwitchTo32kRC_fp)(void);
typedef void (*T_Sys_RomVersion_fp)(void);
typedef void (*T_Sys_ServiceInit_fp)(void);
typedef void (*T_Sys_AppInit_fp)(void);
typedef void (*T_Sys_PostInit_fp)(void);
typedef void (*T_Sys_StackOverflowHook_fp)(TaskHandle_t xTask, char *pcTaskName);
typedef void (*T_Sys_IdleHook_fp)(void);

extern T_Sys_PowerSetup_fp Sys_PowerSetup;
extern T_Sys_ClockSetup_fp Sys_ClockSetup;
extern T_Sys_UartInit_fp Sys_UartInit;
extern T_Sys_MiscModulesInit_fp Sys_MiscModulesInit;
extern T_Sys_DriverInit_fp Sys_DriverInit;
extern T_Sys_WaitforMsqReady_fp Sys_WaitforMsqReady;
extern T_Sys_NotifyReadyToMsq_fp Sys_NotifyReadyToMsq;
extern T_Sys_SetUnsuedSramEndBound_fp Sys_SetUnsuedSramEndBound;
extern T_Sys_SwitchOffUnusedSram_fp Sys_SwitchOffUnusedSram;
extern T_Sys_SwitchTo32kRC_fp Sys_SwitchTo32kRC;
extern T_Sys_RomVersion_fp Sys_RomVersion;
extern T_Sys_ServiceInit_fp Sys_ServiceInit;
extern T_Sys_AppInit_fp Sys_AppInit;
extern T_Sys_PostInit_fp Sys_PostInit;
extern T_Sys_StackOverflowHook_fp Sys_StackOverflowHook;
extern T_Sys_IdleHook_fp Sys_IdleHook;

extern void Sys_PreInit(void);

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _SYS_INIT_H_
