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

#ifndef __OPL_OS_ADAPT_SEMAPHORE_H__
#define __OPL_OS_ADAPT_SEMAPHORE_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_semaphore.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef SEM_HANDLE (*T_OplCreateSemaphoreFp)(void);
typedef int (*T_OplInitSemaphoreFp)(const SEM_HANDLE semHandle, const unsigned int semCnt,\
                         const unsigned int sem_max);
typedef int (*t_opl_os_adapt_semaphore_create_init_fp)(SEM_HANDLE *pHandle, const unsigned int semCnt, const unsigned int sem_max);
typedef int (*t_opl_os_adapt_semaphore_wait_fp)(const SEM_HANDLE semHandle);
typedef int (*t_opl_os_adapt_semaphore_waittimeout_fp)(IN const SEM_HANDLE semHandle, unsigned int timeout);
typedef int (*t_opl_os_adapt_semaphore_post_fp)(const SEM_HANDLE semHandle);
typedef int (*t_opl_os_adapt_semaphore_release_fp)(const SEM_HANDLE semHandle);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern T_OplCreateSemaphoreFp OplCreateSemaphore;
extern T_OplInitSemaphoreFp OplInitSemaphore;
extern t_opl_os_adapt_semaphore_create_init_fp opl_os_adapt_semaphore_create_init;
extern t_opl_os_adapt_semaphore_wait_fp opl_os_adapt_semaphore_wait;
extern t_opl_os_adapt_semaphore_waittimeout_fp opl_os_adapt_semaphore_waittimeout;
extern t_opl_os_adapt_semaphore_post_fp opl_os_adapt_semaphore_post;
extern t_opl_os_adapt_semaphore_release_fp opl_os_adapt_semaphore_release;

// Sec 5: declaration of global function prototype
void opl_os_adapt_semaphore_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_SEMAPHORE_H__
