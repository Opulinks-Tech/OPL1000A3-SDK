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

#ifndef __OPL_OS_ADAPT_SYSTEM_H__
#define __OPL_OS_ADAPT_SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_system.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef SYS_TICK_T (*t_opl_os_adapt_get_systemtickcount_fp)(void);
typedef unsigned int (*t_opl_os_adapt_get_tickratems_fp)(void);
typedef void (*t_opl_os_adapt_system_sleep_fp)(const unsigned long msTime);
typedef void (*t_opl_os_adapt_system_reset_fp)(void);
typedef bool (*t_opl_os_adapt_system_isrstatus_fp)(void);
typedef unsigned int (*t_opl_os_adapt_watchdog_init_start_fp)(const unsigned int timeval);
typedef void (*t_opl_os_adapt_watchdog_refresh_fp)(void);
typedef void (*t_opl_os_adapt_watchdog_stop_fp)(void);
typedef int (*t_opl_os_adapt_system_getheapsize_fp)(void);
typedef int (*t_opl_os_adapt_system_getMiniheapsize_fp)(void);
typedef TY_RST_REASON_E (*t_opl_os_adapt_system_get_rst_info_fp)(void);
typedef void (*t_opl_os_adapt_srandom_fp)(void);
typedef int (*t_opl_os_adapt_get_random_data_fp)(const unsigned int range);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_os_adapt_get_systemtickcount_fp opl_os_adapt_get_systemtickcount;
extern t_opl_os_adapt_get_tickratems_fp opl_os_adapt_get_tickratems;
extern t_opl_os_adapt_system_sleep_fp opl_os_adapt_system_sleep;
extern t_opl_os_adapt_system_reset_fp opl_os_adapt_system_reset;
extern t_opl_os_adapt_system_isrstatus_fp opl_os_adapt_system_isrstatus;
extern t_opl_os_adapt_watchdog_init_start_fp opl_os_adapt_watchdog_init_start;
extern t_opl_os_adapt_watchdog_refresh_fp opl_os_adapt_watchdog_refresh;
extern t_opl_os_adapt_watchdog_stop_fp opl_os_adapt_watchdog_stop;
extern t_opl_os_adapt_system_getheapsize_fp opl_os_adapt_system_getheapsize;
extern t_opl_os_adapt_system_getMiniheapsize_fp opl_os_adapt_system_getMiniheapsize;
extern t_opl_os_adapt_system_get_rst_info_fp opl_os_adapt_system_get_rst_info;
extern t_opl_os_adapt_srandom_fp opl_os_adapt_srandom;
extern t_opl_os_adapt_get_random_data_fp opl_os_adapt_get_random_data;

// Sec 5: declaration of global function prototype
void opl_os_adapt_system_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_SYSTEM_H__
