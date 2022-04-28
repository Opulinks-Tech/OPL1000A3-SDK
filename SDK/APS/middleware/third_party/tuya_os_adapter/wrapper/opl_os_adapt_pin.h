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

#ifndef __OPL_OS_ADAPT_PIN_H__
#define __OPL_OS_ADAPT_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_pin.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef int (*t_opl_pin_os_adapt_init_fp)(tuya_pin_name_t pin, tuya_pin_mode_t mode);
typedef int (*t_opl_pin_os_adapt_write_fp)(tuya_pin_name_t pin, tuya_pin_level_t level);
typedef int (*t_opl_pin_os_adapt_read_fp)(tuya_pin_name_t pin);
typedef int (*t_opl_pin_os_adapt_toggle_fp)(tuya_pin_name_t pin);
typedef int (*t_opl_pin_os_adapt_control_fp)(tuya_pin_name_t pin, uint8_t cmd, void *arg);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_pin_os_adapt_init_fp opl_pin_os_adapt_init;
extern t_opl_pin_os_adapt_write_fp opl_pin_os_adapt_write;
extern t_opl_pin_os_adapt_read_fp opl_pin_os_adapt_read;
extern t_opl_pin_os_adapt_toggle_fp opl_pin_os_adapt_toggle;
extern t_opl_pin_os_adapt_control_fp opl_pin_os_adapt_control;

// Sec 5: declaration of global function prototype
void opl_os_adapt_pin_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_PIN_H__
