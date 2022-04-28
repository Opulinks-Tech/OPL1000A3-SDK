/**
 * @file tuya_os_adapt_output.c
 * @brief 日志操作接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */
#define _UNI_OUTPUT_GLOBAL
#include "tuya_os_adapt_output.h"
#include "tuya_os_adapter_error_code.h"
#include "opl_os_adapt_output.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
//#define TUYA_OS_ADAPT_PLATFORM_VERSION_1_1_0    "1.1.0"
//#define TUYA_OS_ADAPT_PLATFORM_VERSION_1_1_1    "1.1.1"

/***********************************************************
*************************variable define********************
***********************************************************/
volatile uint8_t g_u8TuyaLogEnable = 0;

RET_DATA t_opl_os_adapt_output_log_fp opl_os_adapt_output_log;
RET_DATA t_opl_os_adapt_log_close_fp opl_os_adapt_log_close;
RET_DATA t_opl_os_adapt_log_open_fp opl_os_adapt_log_open;
/***********************************************************
*************************function define********************
***********************************************************/
void tuya_os_adapt_output_log(const signed char *str)
{
    opl_os_adapt_output_log(str);
}

int tuya_os_adapt_log_close(void)
{
    return opl_os_adapt_log_close();
}

int tuya_os_adapt_log_open(void)
{
    return opl_os_adapt_log_open();
}

/**
 * @brief tuya_os_adapt_output_log用于输出log信息
 * 
 * @param[in] str log buffer指针
 */
void tuya_os_adapt_output_log_impl(const signed char *str)
{
    if(str == ((void *)(0))) {
        return;
    }
	
    if(g_u8TuyaLogEnable)
    {
        printf((char *)str);
    }
}

/**
 * @brief 用于关闭原厂sdk默认的log口
 * 
 */
int tuya_os_adapt_log_close_impl(void)
{
    g_u8TuyaLogEnable = 0;

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief 用于恢复原厂sdk默认的log口
 * 
 */
int tuya_os_adapt_log_open_impl(void)
{
    g_u8TuyaLogEnable = 1;

    return OPRT_OS_ADAPTER_OK;
}

//char* tuya_os_adapt_get_ver(void)
//{
//    return TUYA_OS_ADAPT_PLATFORM_VERSION_1_1_0;
//}

void opl_os_adapt_output_pre_init(void)
{
    opl_os_adapt_output_log = tuya_os_adapt_output_log_impl;
    opl_os_adapt_log_close = tuya_os_adapt_log_close_impl;
    opl_os_adapt_log_open = tuya_os_adapt_log_open_impl;
    return;
}
