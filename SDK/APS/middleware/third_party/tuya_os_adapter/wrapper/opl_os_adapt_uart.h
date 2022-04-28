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

#ifndef __OPL_OS_ADAPT_UART_H__
#define __OPL_OS_ADAPT_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_uart.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef int (*t_opl_platform_uart_init_fp)(void);
typedef void (*t_opl_OPL1000_irq_handler_fp)(uint32_t udata);
typedef int (*t_opl_uart_os_adapt_init_fp)(tuya_uart_t *uart, tuya_uart_cfg_t *cfg);
typedef int (*t_opl_uart_os_adapt_control_fp)(tuya_uart_t *uart, uint8_t cmd, void *arg);
typedef int (*t_opl_uart_os_adapt_write_byte_fp)(tuya_uart_t *uart, uint8_t byte);
typedef int (*t_opl_uart_os_adapt_read_byte_fp)(tuya_uart_t *uart, uint8_t *byte);
typedef int (*t_opl_uart_os_adapt_deinit_fp)(tuya_uart_t *uart);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_platform_uart_init_fp opl_platform_uart_init;
extern t_opl_OPL1000_irq_handler_fp opl_uart_irq_handler;
extern t_opl_uart_os_adapt_init_fp opl_uart_os_adapt_init;
extern t_opl_uart_os_adapt_control_fp opl_uart_os_adapt_control;
extern t_opl_uart_os_adapt_write_byte_fp opl_uart_os_adapt_write_byte;
extern t_opl_uart_os_adapt_read_byte_fp opl_uart_os_adapt_read_byte;
extern t_opl_uart_os_adapt_deinit_fp opl_uart_os_adapt_deinit;

// Sec 5: declaration of global function prototype
void opl_os_adapt_uart_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_UART_H__
