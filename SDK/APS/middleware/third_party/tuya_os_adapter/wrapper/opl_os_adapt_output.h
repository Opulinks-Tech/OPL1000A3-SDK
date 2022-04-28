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

#ifndef __OPL_OS_ADAPT_OUTPUT_H__
#define __OPL_OS_ADAPT_OUTPUT_H__

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
typedef void (*t_opl_os_adapt_output_log_fp)(const signed char *str);
typedef int (*t_opl_os_adapt_log_close_fp)(void);
typedef int (*t_opl_os_adapt_log_open_fp)(void);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_os_adapt_output_log_fp opl_os_adapt_output_log;
extern t_opl_os_adapt_log_close_fp opl_os_adapt_log_close;
extern t_opl_os_adapt_log_open_fp opl_os_adapt_log_open;

// Sec 5: declaration of global function prototype
void opl_os_adapt_output_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_OUTPUT_H__
