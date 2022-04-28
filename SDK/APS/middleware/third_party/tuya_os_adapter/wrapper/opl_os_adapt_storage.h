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

#ifndef __OPL_OS_ADAPT_STORAGE_H__
#define __OPL_OS_ADAPT_STORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_os_adapt_storage.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef int (*t_opl_os_adapt_flash_read_fp)(const unsigned int addr, unsigned char *dst, const unsigned int size);
typedef int (*t_opl_os_adapt_flash_write_fp)(const unsigned int addr, const unsigned char *src, const unsigned int size);
typedef int (*t_opl_os_adapt_flash_erase_fp)(const unsigned int addr, const unsigned int size);
typedef UNI_STORAGE_DESC_S *(*t_opl_os_adapt_storage_get_desc_fp)(void);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_os_adapt_flash_read_fp opl_os_adapt_flash_read;
extern t_opl_os_adapt_flash_write_fp opl_os_adapt_flash_write;
extern t_opl_os_adapt_flash_erase_fp opl_os_adapt_flash_erase;
extern t_opl_os_adapt_storage_get_desc_fp opl_os_adapt_storage_get_desc;

// Sec 5: declaration of global function prototype
void opl_os_adapt_storage_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_STORAGE_H__
