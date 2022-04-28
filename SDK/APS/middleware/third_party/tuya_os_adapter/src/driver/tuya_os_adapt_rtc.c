 /*============================================================================
 *                                                                            *
 * Copyright (C) by Tuya Inc                                                  *
 * All rights reserved                                                        *
 *                                                                            *
 *                                                                            *
 =============================================================================*/

/*============================ INCLUDES ======================================*/
#include "tuya_rtc.h"
#include "tuya_os_adapter_error_code.h"
//#include "rtc_api.h"
#include "opl_os_adapt_rtc.h"


/*============================ MACROS ========================================*/
/*============================ TYPES =========================================*/
/*============================ PROTOTYPES ====================================*/

/*============================ LOCAL VARIABLES ===============================*/

/*============================ GLOBAL VARIABLES ==============================*/
RET_DATA t_opl_rtc_os_adapt_init_fp opl_rtc_os_adapt_init;
RET_DATA t_opl_rtc_os_adapt_time_get_fp opl_rtc_os_adapt_time_get;
RET_DATA t_opl_rtc_os_adapt_time_set_fp opl_rtc_os_adapt_time_set;
RET_DATA t_opl_rtc_os_adapt_control_fp opl_rtc_os_adapt_control;
RET_DATA t_opl_rtc_os_adapt_deinit_fp opl_rtc_os_adapt_deinit;
/*============================ IMPLEMENTATION ================================*/

int rtc_os_adapt_init(void)
{
    return opl_rtc_os_adapt_init();
}

int rtc_os_adapt_time_get(time_t *timestamp)
{
    return opl_rtc_os_adapt_time_get(timestamp);
}

int rtc_os_adapt_time_set(time_t  timestamp)
{
    return opl_rtc_os_adapt_time_set(timestamp);
}

int rtc_os_adapt_control(uint8_t cmd, void *arg)
{
    return opl_rtc_os_adapt_control(cmd, arg);
}

int rtc_os_adapt_deinit(void)
{
    return opl_rtc_os_adapt_deinit();
}

static int rtc_os_adapt_init_impl(void)
{
    // to do 

    return OPRT_OS_ADAPTER_OK;
}


static int rtc_os_adapt_time_get_impl(time_t *timestamp)
{    
    // to do 
    
    return OPRT_OS_ADAPTER_OK;
}

static int rtc_os_adapt_time_set_impl(time_t  timestamp)
{
    // to do 
    return OPRT_OS_ADAPTER_OK;
}

static int rtc_os_adapt_control_impl(uint8_t cmd, void *arg)
{
    return OPRT_OS_ADAPTER_OK;

}
static int rtc_os_adapt_deinit_impl(void)
{
    return OPRT_OS_ADAPTER_OK;

}

void opl_os_adapt_rtc_pre_init(void)
{
    opl_rtc_os_adapt_init = rtc_os_adapt_init_impl;
    opl_rtc_os_adapt_time_get = rtc_os_adapt_time_get_impl;
    opl_rtc_os_adapt_time_set = rtc_os_adapt_time_set_impl;
    opl_rtc_os_adapt_control = rtc_os_adapt_control_impl;
    opl_rtc_os_adapt_deinit = rtc_os_adapt_deinit_impl;
    return;
}
