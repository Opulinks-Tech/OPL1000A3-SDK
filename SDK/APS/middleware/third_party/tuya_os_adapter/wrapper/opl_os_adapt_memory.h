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

#ifndef __OPL_OS_ADAPT_MEMORY_H__
#define __OPL_OS_ADAPT_MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef void *(*t_opl_os_adapt_system_malloc_fp)(const size_t size);
typedef void (*t_opl_os_adapt_system_free_fp)(void* ptr);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_os_adapt_system_malloc_fp opl_os_adapt_system_malloc;
extern t_opl_os_adapt_system_free_fp opl_os_adapt_system_free;

// Sec 5: declaration of global function prototype
void opl_os_adapt_memory_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_MEMORY_H__
