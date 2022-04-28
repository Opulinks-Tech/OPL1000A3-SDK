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

#ifndef __OPL_OS_ADAPT_RTC_H__
#define __OPL_OS_ADAPT_RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <time.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef int (*t_opl_rtc_os_adapt_init_fp)(void);
typedef int (*t_opl_rtc_os_adapt_time_get_fp)(time_t *timestamp);
typedef int (*t_opl_rtc_os_adapt_time_set_fp)(time_t  timestamp);
typedef int (*t_opl_rtc_os_adapt_control_fp)(uint8_t cmd, void *arg);
typedef int (*t_opl_rtc_os_adapt_deinit_fp)(void);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_rtc_os_adapt_init_fp opl_rtc_os_adapt_init;
extern t_opl_rtc_os_adapt_time_get_fp opl_rtc_os_adapt_time_get;
extern t_opl_rtc_os_adapt_time_set_fp opl_rtc_os_adapt_time_set;
extern t_opl_rtc_os_adapt_control_fp opl_rtc_os_adapt_control;
extern t_opl_rtc_os_adapt_deinit_fp opl_rtc_os_adapt_deinit;

// Sec 5: declaration of global function prototype
void opl_os_adapt_rtc_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_RTC_H__
