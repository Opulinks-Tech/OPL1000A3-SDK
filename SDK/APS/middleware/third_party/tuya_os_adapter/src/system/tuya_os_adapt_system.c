/**
 * @file tuya_os_adapt_system.c
 * @brief 操作系统相关接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */
#define _UNI_SYSTEM_GLOBAL
#include <stdlib.h>
#include "tuya_os_adapt_system.h"
#include "tuya_os_adapter_error_code.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "projdefs.h"
#include "portmacro.h"
#include "portable.h"
#include "task.h"
#include "hal_tick.h"
#include "hal_wdt.h"
#include "hal_system.h"
#include "system_ARMCM3.h"
#include "opl_os_adapt_system.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
#define SYS_RESET_STS_POWER_ON  0x1F
#define SYS_RESET_STS_WARMBOOT  0x08
#define SYS_RESET_STS_WATCHDOG  0x01
#define SYS_RESET_STS_SWRESET   0x04

/***********************************************************
*************************variable define********************
***********************************************************/
RET_DATA t_opl_os_adapt_get_systemtickcount_fp opl_os_adapt_get_systemtickcount;
RET_DATA t_opl_os_adapt_get_tickratems_fp opl_os_adapt_get_tickratems;
RET_DATA t_opl_os_adapt_system_sleep_fp opl_os_adapt_system_sleep;
RET_DATA t_opl_os_adapt_system_reset_fp opl_os_adapt_system_reset;
RET_DATA t_opl_os_adapt_system_isrstatus_fp opl_os_adapt_system_isrstatus;
RET_DATA t_opl_os_adapt_watchdog_init_start_fp opl_os_adapt_watchdog_init_start;
RET_DATA t_opl_os_adapt_watchdog_refresh_fp opl_os_adapt_watchdog_refresh;
RET_DATA t_opl_os_adapt_watchdog_stop_fp opl_os_adapt_watchdog_stop;
RET_DATA t_opl_os_adapt_system_getheapsize_fp opl_os_adapt_system_getheapsize;
RET_DATA t_opl_os_adapt_system_getMiniheapsize_fp opl_os_adapt_system_getMiniheapsize;
RET_DATA t_opl_os_adapt_system_get_rst_info_fp opl_os_adapt_system_get_rst_info;
RET_DATA t_opl_os_adapt_srandom_fp opl_os_adapt_srandom;
RET_DATA t_opl_os_adapt_get_random_data_fp opl_os_adapt_get_random_data;

/***********************************************************
*************************function define********************
***********************************************************/
SYS_TICK_T tuya_os_adapt_get_systemtickcount(void)
{
    return opl_os_adapt_get_systemtickcount();
}

unsigned int tuya_os_adapt_get_tickratems(void)
{
    return opl_os_adapt_get_tickratems();
}

void tuya_os_adapt_system_sleep(const unsigned long msTime)
{
    opl_os_adapt_system_sleep(msTime);
}

void tuya_os_adapt_system_reset(void)
{
    opl_os_adapt_system_reset();
}

bool tuya_os_adapt_system_isrstatus(void)
{
    return opl_os_adapt_system_isrstatus();
}

unsigned int tuya_os_adapt_watchdog_init_start(const unsigned int timeval)
{
    return opl_os_adapt_watchdog_init_start(timeval);
}

void tuya_os_adapt_watchdog_refresh(void)
{
    opl_os_adapt_watchdog_refresh();
}

void tuya_os_adapt_watchdog_stop(void)
{
    opl_os_adapt_watchdog_stop();
}

int tuya_os_adapt_system_getheapsize(void)
{
    return opl_os_adapt_system_getheapsize();
}

int tuya_os_adapt_system_getMiniheapsize(void)
{
    return opl_os_adapt_system_getMiniheapsize();
}

TY_RST_REASON_E tuya_os_adapt_system_get_rst_info(void)
{
    return opl_os_adapt_system_get_rst_info();
}

void tuya_os_adapt_srandom(void)
{
    opl_os_adapt_srandom();
}

int tuya_os_adapt_get_random_data(const unsigned int range)
{
    return opl_os_adapt_get_random_data(range);
}

/**
 * @brief 获取系统当前的tick值
 * @return 系统当前的tick值
 */
SYS_TICK_T tuya_os_adapt_get_systemtickcount_impl(void)
{
    return (SYS_TICK_T)xTaskGetTickCount();
}


/**
 * @brief 获取系统1个ticket是多少个ms
 * 
 * @return 系统1个ticket是多少个ms
 */
unsigned int tuya_os_adapt_get_tickratems_impl(void)
{
    return (unsigned int)portTICK_PERIOD_MS;
}


/**
 * @brief 线程睡眠
 * 
 * @param [in] msTime 系统睡眠时间（ms）
 */
void tuya_os_adapt_system_sleep_impl(const unsigned long msTime)
{
    vTaskDelay((msTime)/portTICK_PERIOD_MS);
}


/**
 * @brief Reset system
 * 
 */
void tuya_os_adapt_system_reset_impl(void)
{
    Hal_Sys_SwResetAll();
	while(1);
}


/**
 * @brief 检测系统是否处于上下文中断
 * 
 * @retval true : 处于上下文中断
 * @retval false : 没处于上下文中断
 */
bool tuya_os_adapt_system_isrstatus_impl(void)
{
    return (__get_IPSR())?true:false;
}

/**
 * @brief 初始化并运行看门狗
 * 
 * @param [in] timeval 看门狗复位检测时间间隔
 * @note 如果timeval大于看门狗的最大可设置时间，则使用平台可设置时间的最大值，并且返回该最大值
 * @return 芯片实际设置的看门狗时间 (sec)
 */
unsigned int tuya_os_adapt_watchdog_init_start_impl(const unsigned int timeval)
{
	int tval = timeval;
	unsigned int ticks = SystemCoreClockGet();
    int max_sec = ticks / 1000000;
    
    if (tval > max_sec)
        tval = max_sec;
        
    Hal_Wdt_Init(tval * ticks);
    
    return tval;
}

/**
 * @brief 用于刷新watch dog（喂狗）
 * 
 */
void tuya_os_adapt_watchdog_refresh_impl(void)
{
    Hal_Wdt_Clear();
}

/**
 * @brief 停止watch dog
 * 
 */
void tuya_os_adapt_watchdog_stop_impl(void)
{
    Hal_Wdt_Stop();
}


/**
 * @brief 获取系统剩余内存大小
 * 
 * @return 剩余内存
 */
int tuya_os_adapt_system_getheapsize_impl(void)
{
    return (int)xPortGetFreeHeapSize();
}

/**
 * @brief 获取系统最小剩余内存大小
 * 
 * @return 最小剩余内存
 */
int tuya_os_adapt_system_getMiniheapsize_impl(void)
{
	return (int)xPortGetMinimumEverFreeHeapSize();
}

/**
 * @brief 获取系统重启原因
 * 
 * @return 重启原因 ::TY_RST_REASON_E
 * @retval TY_RST_POWER_OFF         电源重启
 * @retval TY_RST_HARDWARE_WATCHDOG 硬件看门狗复位
 * @retval TY_RST_FATAL_EXCEPTION   异代码常
 * @retval TY_RST_SOFTWARE_WATCHDOG 软件看门狗重启
 * @retval TY_RST_SOFTWARE          软件复位
 * @retval TY_RST_DEEPSLEEP         深度睡眠
 * @retval TY_RST_HARDWARE          硬件复位
 * @retval TY_RST_OTHER             其它原因
 * @retval TY_RST_UNSUPPORT         不支持
 */
TY_RST_REASON_E tuya_os_adapt_system_get_rst_info_impl(void)
{
    unsigned int boot_reason = Hal_Sys_ResetStatusRead();
    TY_RST_REASON_E rtn_value;

    switch(boot_reason) {
        case SYS_RESET_STS_POWER_ON:
            //Power on reset
            rtn_value = TY_RST_POWER_OFF;
            break;
		
        case SYS_RESET_STS_WARMBOOT:
            // Wakeup from deepsleep
            rtn_value = TY_RST_DEEPSLEEP;
            break;
        
		case SYS_RESET_STS_WATCHDOG:
            // watchdog
            rtn_value = TY_RST_HARDWARE_WATCHDOG;
            break;
		
		case SYS_RESET_STS_SWRESET:
            // Software reset
			rtn_value = TY_RST_SOFTWARE;
            break;
        
        default:
            //Other
            rtn_value = TY_RST_OTHER;
    }
	
    return rtn_value;
	
}
/**
 * @brief init random
 *
 * @param  void
 * @retval void
 */
void tuya_os_adapt_srandom_impl(void)
{
    srand(Hal_Tick_Diff(0));
}

/**
 * @brief tuya_os_adapt_get_random_data用于获取指定条件下的随机数
 * 
 * @param[in] range 
 * @return 随机数
 */
int tuya_os_adapt_get_random_data_impl(const unsigned int range)
{
    static char flag = 0;
    if(flag == 0) {
        flag = 1;
        tuya_os_adapt_srandom();
    }
    
    unsigned int trange = range;
    if (range == 0)
        trange = 0xFF;
    
    return (rand() % trange);
}

void opl_os_adapt_system_pre_init(void)
{
    opl_os_adapt_get_systemtickcount = tuya_os_adapt_get_systemtickcount_impl;
    opl_os_adapt_get_tickratems = tuya_os_adapt_get_tickratems_impl;
    opl_os_adapt_system_sleep = tuya_os_adapt_system_sleep_impl;
    opl_os_adapt_system_reset = tuya_os_adapt_system_reset_impl;
    opl_os_adapt_system_isrstatus = tuya_os_adapt_system_isrstatus_impl;
    opl_os_adapt_watchdog_init_start = tuya_os_adapt_watchdog_init_start_impl;
    opl_os_adapt_watchdog_refresh = tuya_os_adapt_watchdog_refresh_impl;
    opl_os_adapt_watchdog_stop = tuya_os_adapt_watchdog_stop_impl;
    opl_os_adapt_system_getheapsize = tuya_os_adapt_system_getheapsize_impl;
    opl_os_adapt_system_getMiniheapsize = tuya_os_adapt_system_getMiniheapsize_impl;
    opl_os_adapt_system_get_rst_info = tuya_os_adapt_system_get_rst_info_impl;
    opl_os_adapt_srandom = tuya_os_adapt_srandom_impl;
    opl_os_adapt_get_random_data = tuya_os_adapt_get_random_data_impl;
    return;
}

