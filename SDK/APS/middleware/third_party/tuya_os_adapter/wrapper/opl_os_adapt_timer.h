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

#ifndef __OPL_OS_ADAPT_TIMER_H__
#define __OPL_OS_ADAPT_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_timer.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef int32_t (*t_opl_os_adapt_timer_create_fp)(TIMER_HANDLE* xTimer,
                           const char * const pcTimerName,
                           const uint32_t xTimerPeriod,
                           const uint32_t uxAutoReload,
                           void * const pvTimerID,
                           TIMER_FUNC_T pxCallbackFunction);
typedef int32_t (*t_opl_os_adapt_timer_start_fp)(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime);
typedef int32_t (*t_opl_os_adapt_timer_stop_fp)(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime);
typedef int32_t (*t_opl_os_adapt_timer_delete_fp)(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_os_adapt_timer_create_fp opl_os_adapt_timer_create;
extern t_opl_os_adapt_timer_start_fp opl_os_adapt_timer_start;
extern t_opl_os_adapt_timer_stop_fp opl_os_adapt_timer_stop;
extern t_opl_os_adapt_timer_delete_fp opl_os_adapt_timer_delete;

// Sec 5: declaration of global function prototype
void opl_os_adapt_timer_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_TIMER_H__
