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

#ifndef __OPL_OS_ADAPT_THREAD_H__
#define __OPL_OS_ADAPT_THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_thread.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef int (*t_opl_os_adapt_thread_create_fp)(THREAD_HANDLE* thread,
                           const char* name,
                           const unsigned int stack_size,
                           const unsigned int priority,
                           const THREAD_FUNC_T func,
                           void *const arg);
typedef int (*t_opl_os_adapt_thread_release_fp)(THREAD_HANDLE thread);
typedef void (*t_opl_os_adapt_thread_enter_critical_fp)(void);
typedef void (*t_opl_os_adapt_thread_exit_critical_fp)(void);
typedef void (*t_opl_os_adapt_thread_priority_set_fp)(THREAD_HANDLE thread, int prio_thread);
typedef int (*t_opl_os_adapt_thread_priority_get_fp)(THREAD_HANDLE thread);
typedef int (*t_opl_os_adapt_thread_is_self_fp)(THREAD_HANDLE thread, BOOL_T* is_self);
typedef int (*t_opl_os_adapt_thread_get_watermark_fp)(THREAD_HANDLE thread, UINT_T* watermark);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_os_adapt_thread_create_fp opl_os_adapt_thread_create;
extern t_opl_os_adapt_thread_release_fp opl_os_adapt_thread_release;
extern t_opl_os_adapt_thread_enter_critical_fp opl_os_adapt_thread_enter_critical;
extern t_opl_os_adapt_thread_exit_critical_fp opl_os_adapt_thread_exit_critical;
extern t_opl_os_adapt_thread_priority_set_fp opl_os_adapt_thread_priority_set;
extern t_opl_os_adapt_thread_priority_get_fp opl_os_adapt_thread_priority_get;
extern t_opl_os_adapt_thread_is_self_fp opl_os_adapt_thread_is_self;
extern t_opl_os_adapt_thread_get_watermark_fp opl_os_adapt_thread_get_watermark;

// Sec 5: declaration of global function prototype
void opl_os_adapt_thread_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_THREAD_H__
