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

#ifndef __OPL_OS_ADAPT_QUEUE_H__
#define __OPL_OS_ADAPT_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_queue.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef int (*t_opl_os_adapt_queue_create_init_fp)(QUEUE_HANDLE *queue, int size);
typedef void (*t_opl_os_adapt_queue_free_fp)(QUEUE_HANDLE queue);
typedef int (*t_opl_os_adapt_queue_post_fp)(QUEUE_HANDLE queue, void *msg, unsigned int timeout);
typedef int (*t_opl_os_adapt_queue_fetch_fp)(QUEUE_HANDLE queue, void **msg, unsigned int timeout);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_os_adapt_queue_create_init_fp opl_os_adapt_queue_create_init;
extern t_opl_os_adapt_queue_free_fp opl_os_adapt_queue_free;
extern t_opl_os_adapt_queue_post_fp opl_os_adapt_queue_post;
extern t_opl_os_adapt_queue_fetch_fp opl_os_adapt_queue_fetch;

// Sec 5: declaration of global function prototype
void opl_os_adapt_queue_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_QUEUE_H__
