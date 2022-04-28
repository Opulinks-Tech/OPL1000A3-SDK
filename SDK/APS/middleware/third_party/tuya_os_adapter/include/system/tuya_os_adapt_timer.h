/**
 * @file tuya_os_adapt_thread.h
 * @brief 线程操作接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */

#ifndef __TUYA_OS_ADAPT_TIMER_H__
#define __TUYA_OS_ADAPT_TIMER_H__


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief handle of thread
 * 
 */
typedef void* TIMER_ID;
typedef void* TIMER_HANDLE;

/**
 * @brief thread function type
 * 
 */
typedef void (*TIMER_FUNC_T)(void*);


int32_t tuya_os_adapt_timer_create(TIMER_HANDLE* xTimer,
                           const char * const pcTimerName,
                           const uint32_t xTimerPeriod,
                           const uint32_t uxAutoReload,
                           void * const pvTimerID,
                           TIMER_FUNC_T pxCallbackFunction);

int32_t tuya_os_adapt_timer_start(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime);  

int32_t tuya_os_adapt_timer_stop(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime);

int32_t tuya_os_adapt_timer_delete(TIMER_HANDLE xTimer,
                            uint32_t xBlockTime);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif // __TUYA_OS_ADAPT_TIMER_H__

