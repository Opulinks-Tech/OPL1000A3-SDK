 /*============================================================================
 *                                                                            *
 * Copyright (C) by Tuya Inc                                                  *
 * All rights reserved                                                        *
 *                                                                            *
 *                                                                            *
 =============================================================================*/

/*============================ INCLUDES ======================================*/
#include "tuya_i2c.h"
#include "tuya_os_adapter_error_code.h"
#include "opl_os_adapt_i2c.h"


/*============================ MACROS ========================================*/

/*============================ PROTOTYPES ====================================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ GLOBAL VARIABLES ==============================*/
RET_DATA t_opl_i2c_os_adapt_init_fp opl_i2c_os_adapt_init;
RET_DATA t_opl_i2c_os_adapt_xfer_fp opl_i2c_os_adapt_xfer;
RET_DATA t_opl_i2c_devos_adapt_control_fp opl_i2c_devos_adapt_control;
RET_DATA t_opl_i2c_os_adapt_deinit_fp opl_i2c_os_adapt_deinit;
/*============================ IMPLEMENTATION ================================*/
int i2c_os_adapt_init(tuya_i2c_t *i2c, tuya_i2c_cfg_t *cfg)
{
    return opl_i2c_os_adapt_init(i2c, cfg);
}

int i2c_os_adapt_xfer(tuya_i2c_t *i2c, tuya_i2c_msg_t *msgs, uint8_t num)
{
    return opl_i2c_os_adapt_xfer(i2c, msgs, num);
}

int i2c_devos_adapt_control(tuya_i2c_t *i2c, uint8_t cmd, void *arg)
{
    return opl_i2c_devos_adapt_control(i2c, cmd, arg);
}

int i2c_os_adapt_deinit(tuya_i2c_t *i2c)
{
    return opl_i2c_os_adapt_deinit(i2c);
}

int i2c_os_adapt_init_impl(tuya_i2c_t *i2c, tuya_i2c_cfg_t *cfg)
{
    //to do

    return OPRT_OS_ADAPTER_OK;
}


static int i2c_os_adapt_xfer_impl(tuya_i2c_t *i2c, tuya_i2c_msg_t *msgs, uint8_t num)
{
    //to do
    return OPRT_OS_ADAPTER_OK;
}

int i2c_devos_adapt_control_impl(tuya_i2c_t *i2c, uint8_t cmd, void *arg)
{
    // to do
    return OPRT_OS_ADAPTER_OK;
}

int i2c_os_adapt_deinit_impl(tuya_i2c_t *i2c)
{
     // to do
    return OPRT_OS_ADAPTER_OK;
}

void opl_os_adapt_i2c_pre_init(void)
{
    opl_i2c_os_adapt_init = i2c_os_adapt_init_impl;
    opl_i2c_os_adapt_xfer = i2c_os_adapt_xfer_impl;
    opl_i2c_devos_adapt_control = i2c_devos_adapt_control_impl;
    opl_i2c_os_adapt_deinit = i2c_os_adapt_deinit_impl;
    return;
}

