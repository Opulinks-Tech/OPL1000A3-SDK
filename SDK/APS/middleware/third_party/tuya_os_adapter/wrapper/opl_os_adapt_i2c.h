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

#ifndef __OPL_OS_ADAPT_I2C_H__
#define __OPL_OS_ADAPT_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "tuya_i2c.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef int (*t_opl_i2c_os_adapt_init_fp)(tuya_i2c_t *i2c, tuya_i2c_cfg_t *cfg);
typedef int (*t_opl_i2c_os_adapt_xfer_fp)(tuya_i2c_t *i2c, tuya_i2c_msg_t *msgs, uint8_t num);
typedef int (*t_opl_i2c_devos_adapt_control_fp)(tuya_i2c_t *i2c, uint8_t cmd, void *arg);
typedef int (*t_opl_i2c_os_adapt_deinit_fp)(tuya_i2c_t *i2c);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern t_opl_i2c_os_adapt_init_fp opl_i2c_os_adapt_init;
extern t_opl_i2c_os_adapt_xfer_fp opl_i2c_os_adapt_xfer;
extern t_opl_i2c_devos_adapt_control_fp opl_i2c_devos_adapt_control;
extern t_opl_i2c_os_adapt_deinit_fp opl_i2c_os_adapt_deinit;

// Sec 5: declaration of global function prototype
void opl_os_adapt_i2c_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_I2C_H__
