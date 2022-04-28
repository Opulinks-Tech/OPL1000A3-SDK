/**
 * @file tuya_os_adapt_timer.c
 * @brief 线程操作接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */
#include <FreeRTOS.h>
#include <task.h>
#include "timers.h"

#include "tuya_os_adapt_timer.h"
#include "tuya_os_adapter_error_code.h"
#include "opl_os_adapt_timer.h"

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/
RET_DATA t_opl_os_adapt_timer_create_fp opl_os_adapt_timer_create;
RET_DATA t_opl_os_adapt_timer_start_fp opl_os_adapt_timer_start;
RET_DATA t_opl_os_adapt_timer_stop_fp opl_os_adapt_timer_stop;
RET_DATA t_opl_os_adapt_timer_delete_fp opl_os_adapt_timer_delete;
/***********************************************************
*************************function define********************
***********************************************************/

int32_t tuya_os_adapt_timer_create(TIMER_HANDLE* xTimer,
                           const char * const pcTimerName,
                           const uint32_t xTimerPeriod,
                           const uint32_t uxAutoReload,
                           void * const pvTimerID,
                           TIMER_FUNC_T pxCallbackFunction)
{
    return opl_os_adapt_timer_create(xTimer, pcTimerName, xTimerPeriod, uxAutoReload, pvTimerID, pxCallbackFunction);
}

int32_t tuya_os_adapt_timer_start(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime)
{
    return opl_os_adapt_timer_start(xTimer, xBlockTime);
}

int32_t tuya_os_adapt_timer_stop(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime)
{
    return opl_os_adapt_timer_stop(xTimer, xBlockTime);
}

int32_t tuya_os_adapt_timer_delete(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime)
{
    return opl_os_adapt_timer_delete(xTimer, xBlockTime);
}

int32_t tuya_os_adapt_timer_create_impl(TIMER_HANDLE* xTimer,
                           const char * const pcTimerName,
                           const uint32_t xTimerPeriod,
                           const uint32_t uxAutoReload,
                           void * const pvTimerID,
                           TIMER_FUNC_T pxCallbackFunction)
{
    //BaseType_t ret = 0;
    

    *xTimer = xTimerCreate(pcTimerName, xTimerPeriod, uxAutoReload, pvTimerID, pxCallbackFunction);
    if(*xTimer == NULL){
        return OPRT_OS_ADAPTER_THRD_CREAT_FAILED;
    }
    return OPRT_OS_ADAPTER_OK;
}

int32_t tuya_os_adapt_timer_start_impl(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime)
{
    BaseType_t ret = 0;

    ret = xTimerStart( xTimer, xBlockTime );
    if (ret != pdPASS) {
        return OPRT_OS_ADAPTER_THRD_CREAT_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;    
}   

int32_t tuya_os_adapt_timer_stop_impl(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime)
{
    BaseType_t ret = 0;

    ret = xTimerStop( xTimer, xBlockTime );
    if (ret != pdPASS) {
        return OPRT_OS_ADAPTER_THRD_CREAT_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;    
}   

int32_t tuya_os_adapt_timer_delete_impl(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime)
{
    BaseType_t ret = 0;

    ret = xTimerDelete( xTimer, xBlockTime );
    if (ret != pdPASS) {
        return OPRT_OS_ADAPTER_THRD_CREAT_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;    
}  

void opl_os_adapt_timer_pre_init(void)
{
    opl_os_adapt_timer_create = tuya_os_adapt_timer_create_impl;
    opl_os_adapt_timer_start = tuya_os_adapt_timer_start_impl;
    opl_os_adapt_timer_stop = tuya_os_adapt_timer_stop_impl;
    opl_os_adapt_timer_delete = tuya_os_adapt_timer_delete_impl;
    return;
}

