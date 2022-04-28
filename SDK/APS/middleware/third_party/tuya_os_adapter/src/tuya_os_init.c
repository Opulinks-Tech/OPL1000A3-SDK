/**
 * @file tuya_os_init.c
 * @author sunkz@tuya.com
 * @brief 
 * @version 0.1
 * @date 2020-05-15
 * 
 * @copyright Copyright (c) tuya.inc 2019
 * 
 */

#include "tuya_os_init.h"
#include "opl_os_adapt_init.h"

RET_DATA t_opl_os_init_fp opl_os_init;
RET_DATA t_opl_os_adapt_get_ver_fp opl_os_adapt_get_ver;

extern int platform_uart_init(void);
extern int platform_pin_init(void);
extern int platform_i2c_init(void);
extern int platform_rtc_init(void);

void tuya_os_init(void)
{
    opl_os_init();
}

char* tuya_os_adapt_get_ver(void)
{
    return opl_os_adapt_get_ver();
}

void tuya_os_init_impl(void)
{
    platform_uart_init();
    //platform_pin_init();
    //platform_i2c_init();
    //platform_rtc_init();  
}

char* tuya_os_adapt_get_ver_impl(void)
{
    return TUYA_OS_ADAPT_PLATFORM_VERSION;
}

void opl_os_adapt_init_pre_init(void)
{
    opl_os_init = tuya_os_init_impl;
    opl_os_adapt_get_ver = tuya_os_adapt_get_ver_impl;
    return;
}
