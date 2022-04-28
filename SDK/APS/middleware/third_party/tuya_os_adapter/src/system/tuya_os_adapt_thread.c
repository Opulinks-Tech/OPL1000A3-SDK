/**
 * @file tuya_os_adapt_thread.c
 * @brief 线程操作接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */

#include "tuya_os_adapt_thread.h"
#include "tuya_os_adapt_system.h"
#include "cmsis_os.h"
#include "sys_init.h"
#include "sys_init_patch.h"
#include "hal_system.h"
#include <stdio.h>
#include <stdint.h>
#include "opl_os_adapt_thread.h"
/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/
RET_DATA t_opl_os_adapt_thread_create_fp opl_os_adapt_thread_create;
RET_DATA t_opl_os_adapt_thread_release_fp opl_os_adapt_thread_release;
RET_DATA t_opl_os_adapt_thread_enter_critical_fp opl_os_adapt_thread_enter_critical;
RET_DATA t_opl_os_adapt_thread_exit_critical_fp opl_os_adapt_thread_exit_critical;
RET_DATA t_opl_os_adapt_thread_priority_set_fp opl_os_adapt_thread_priority_set;
RET_DATA t_opl_os_adapt_thread_priority_get_fp opl_os_adapt_thread_priority_get;
RET_DATA t_opl_os_adapt_thread_is_self_fp opl_os_adapt_thread_is_self;
RET_DATA t_opl_os_adapt_thread_get_watermark_fp opl_os_adapt_thread_get_watermark;
/***********************************************************
*************************function define********************
***********************************************************/
int tuya_os_adapt_thread_create(THREAD_HANDLE* thread,
                           const char* name,
                           const unsigned int stack_size,
                           const unsigned int priority,
                           const THREAD_FUNC_T func,
                           void *const arg)
{
    return opl_os_adapt_thread_create(thread, name, stack_size, priority, func, arg);
}

int tuya_os_adapt_thread_release(THREAD_HANDLE thread)
{
    return opl_os_adapt_thread_release(thread);
}

void tuya_os_adapt_thread_enter_critical(void)
{
    opl_os_adapt_thread_enter_critical();
}

void tuya_os_adapt_thread_exit_critical(void)
{
    opl_os_adapt_thread_exit_critical();
}

void tuya_os_adapt_thread_priority_set(THREAD_HANDLE thread, int prio_thread)
{
    opl_os_adapt_thread_priority_set(thread, prio_thread);
}

int tuya_os_adapt_thread_priority_get(THREAD_HANDLE thread)
{
    return opl_os_adapt_thread_priority_get(thread);
}

int tuya_os_adapt_thread_is_self(THREAD_HANDLE thread, BOOL_T* is_self)
{
    return opl_os_adapt_thread_is_self(thread, is_self);
}

int tuya_os_adapt_thread_get_watermark(THREAD_HANDLE thread, UINT_T* watermark)
{
    return opl_os_adapt_thread_get_watermark(thread, watermark);
}

/**
 * @brief create and start a tuya sdk thread
 * 
 * @param[out] thread       the thread handle
 * @param[in] name          the thread name
 * @param[in] stack_size    stack size of thread
 * @param[in] priority      priority of thread
 * @param[in] func          the main thread process function
 * @param[in] arg           the args of the func, can be null
 * @retval OPRT_OS_ADAPTER_OK          success
 * @retval Other            fail
 */
int tuya_os_adapt_thread_create_impl(THREAD_HANDLE* thread,
                           const char* name,
                           const unsigned int stack_size,
                           const unsigned int priority,
                           const THREAD_FUNC_T func,
                           void *const arg)
{
    BaseType_t ret = 0;
    ret = xTaskCreate(func, name, stack_size/sizeof(portSTACK_TYPE), arg, priority, thread);
    if (ret != pdPASS) {
        return OPRT_OS_ADAPTER_THRD_CREAT_FAILED;
    }
    return OPRT_OS_ADAPTER_OK;
}

/**
* @brief terminal thread and release thread resources
* 
* @param[in] thread    the input thread handle
* @retval OPRT_OS_ADAPTER_OK      success
* @retval Other        fail
*/
int tuya_os_adapt_thread_release_impl(THREAD_HANDLE thread)
{
    vTaskDelete(thread);

    return OPRT_OS_ADAPTER_OK;
}


/**
 * @brief current thread enter critical
 *
 * @param  void
 * @retval void
 */
void tuya_os_adapt_thread_enter_critical_impl(void)
{
    vPortEnterCritical();
}

/**
 * @brief current thread exit critical
 *
 * @param  void
 * @retval void
 */
void tuya_os_adapt_thread_exit_critical_impl(void)
{
    vPortExitCritical();
}

/**
* @brief set thread priority
*
* @param[in] thread       the input thread handle
* @param[in] prio_thread  the priority thread send to
* @retval void
*/
void tuya_os_adapt_thread_priority_set_impl(THREAD_HANDLE thread, int prio_thread)
{
    vTaskPrioritySet(thread, prio_thread);
}

/**
* @brief get thread priority
*
* @param[in]  thread     the input thread handle
* @param[out] priority   the priority of thread
* @retval void
*/
int tuya_os_adapt_thread_priority_get_impl(THREAD_HANDLE thread)
{
    return OPRT_OS_ADAPTER_OK;
}

int tuya_os_adapt_thread_is_self_impl(THREAD_HANDLE thread, BOOL_T* is_self)
{
    // to do

    return OPRT_OS_ADAPTER_OK;
}

int tuya_os_adapt_thread_get_watermark_impl(THREAD_HANDLE thread, UINT_T* watermark)
{
    // to do

    return OPRT_OS_ADAPTER_OK;
}

void opl_os_adapt_thread_pre_init(void)
{
    opl_os_adapt_thread_create = tuya_os_adapt_thread_create_impl;
    opl_os_adapt_thread_release = tuya_os_adapt_thread_release_impl;
    opl_os_adapt_thread_enter_critical = tuya_os_adapt_thread_enter_critical_impl;
    opl_os_adapt_thread_exit_critical = tuya_os_adapt_thread_exit_critical_impl;
    opl_os_adapt_thread_priority_set = tuya_os_adapt_thread_priority_set_impl;
    opl_os_adapt_thread_priority_get = tuya_os_adapt_thread_priority_get_impl;
    opl_os_adapt_thread_is_self = tuya_os_adapt_thread_is_self_impl;
    opl_os_adapt_thread_get_watermark = tuya_os_adapt_thread_get_watermark_impl;
    return;
}

