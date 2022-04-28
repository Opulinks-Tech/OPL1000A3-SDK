 /*============================================================================
 *                                                                            *
 * Copyright (C) by Tuya Inc                                                  *
 * All rights reserved                                                        *
 *                                                                            *
 =============================================================================*/
#include "tuya_pin.h"
#include "tuya_os_adapter.h"
#include "tuya_os_adapter_error_code.h"
#include "opl_os_adapt_pin.h"

/*============================ INCLUDES ======================================*/


/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ PROTOTYPES ====================================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ GLOBAL VARIABLES ==============================*/
RET_DATA t_opl_pin_os_adapt_init_fp opl_pin_os_adapt_init;
RET_DATA t_opl_pin_os_adapt_write_fp opl_pin_os_adapt_write;
RET_DATA t_opl_pin_os_adapt_read_fp opl_pin_os_adapt_read;
RET_DATA t_opl_pin_os_adapt_toggle_fp opl_pin_os_adapt_toggle;
RET_DATA t_opl_pin_os_adapt_control_fp opl_pin_os_adapt_control;
/*============================ IMPLEMENTATION ================================*/

int pin_os_adapt_init(tuya_pin_name_t pin, tuya_pin_mode_t mode)
{
    return opl_pin_os_adapt_init(pin, mode);
}

int pin_os_adapt_write(tuya_pin_name_t pin, tuya_pin_level_t level)
{
    return opl_pin_os_adapt_write(pin, level);
}

int pin_os_adapt_read(tuya_pin_name_t pin)
{
    return opl_pin_os_adapt_read(pin);
}

int pin_os_adapt_toggle(tuya_pin_name_t pin)
{
    return opl_pin_os_adapt_toggle(pin);
}

int pin_os_adapt_control(tuya_pin_name_t pin, uint8_t cmd, void *arg)
{
    return opl_pin_os_adapt_control(pin, cmd, arg);
}

static int pin_os_adapt_init_impl(tuya_pin_name_t pin, tuya_pin_mode_t mode)
{
    // to do 
    return OPRT_OS_ADAPTER_OK;
}


static int pin_os_adapt_write_impl(tuya_pin_name_t pin, tuya_pin_level_t level)
{
    
    // to do 
    return OPRT_OS_ADAPTER_OK;
}

static int pin_os_adapt_read_impl(tuya_pin_name_t pin)
{
    // to do 

#if 1
    return OPRT_OS_ADAPTER_COM_ERROR;
#else
    return gpio_read(pinmap[pin].obj);
#endif
}

static int pin_os_adapt_toggle_impl(tuya_pin_name_t pin)
{
    // to do 
    return OPRT_OS_ADAPTER_OK;
}

static int pin_os_adapt_control_impl(tuya_pin_name_t pin, uint8_t cmd, void *arg)
{
    // to do 
    return OPRT_OS_ADAPTER_OK;
}

void opl_os_adapt_pin_pre_init(void)
{
    opl_pin_os_adapt_init = pin_os_adapt_init_impl;
    opl_pin_os_adapt_write = pin_os_adapt_write_impl;
    opl_pin_os_adapt_read = pin_os_adapt_read_impl;
    opl_pin_os_adapt_toggle = pin_os_adapt_toggle_impl;
    opl_pin_os_adapt_control = pin_os_adapt_control_impl;
    return;
}

