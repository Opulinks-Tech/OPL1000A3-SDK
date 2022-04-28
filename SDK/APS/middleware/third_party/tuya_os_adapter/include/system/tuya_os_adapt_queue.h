/**
 * @file tuya_os_adapt_queue.h
 * @brief 队列相关接口封装
 *
 * @copyright copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */

#ifndef _TUYA_OS_ADAPT_QUEUE_H
#define _TUYA_OS_ADAPT_QUEUE_H

#include "tuya_os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TUYA_OS_ADAPT_QUEUE_FOREVER 0xffffffff
typedef void* QUEUE_HANDLE;
/**
 * @brief create queue
 *
 * @param[out]     queue      queue to be create
 * @param[in]      size       the deep of the queue
 * @return  OPRT_OS_ADAPTER_OK: SUCCESS other:fail
 */
int tuya_os_adapt_queue_create_init(QUEUE_HANDLE *queue, int size);
#define tuya_hal_queue_create_init tuya_os_adapt_queue_create_init

/**
 * @brief free queue
 *
 * @param[in]     queue      queue to be free
 * @return  void
 */
void tuya_os_adapt_queue_free(QUEUE_HANDLE queue);
#define tuya_hal_queue_free tuya_os_adapt_queue_free

/**
 * @brief fetch msg from queue in timeout ms
 *
 * @param[in]      queue      queue to post
 * @param[in]      msg        msg to post
 * @param[in]      timeout    max time to wait for msg(ms), TUYA_OS_ADAPT_QUEUE_FOREVER means forever wait
 * @return  int OPRT_OS_ADAPTER_OK:success    other:fail
 */
int tuya_os_adapt_queue_post(QUEUE_HANDLE queue, void *data, unsigned int timeout);
#define tuya_hal_queue_post tuya_os_adapt_queue_post
/**
 * @brief fetch msg from queue in timeout ms
 *
 * @param[in]      queue      queue to post
 * @param[out]     msg        msg to fetch
 * @param[in]      timeout    max time to wait for msg(ms), TUYA_OS_ADAPT_QUEUE_FOREVER means forever wait
 * @return  int OPRT_OS_ADAPTER_OK:success    other:fail
 */
int tuya_os_adapt_queue_fetch(QUEUE_HANDLE queue, void **msg, unsigned int timeout);
#define tuya_hal_queue_fetch tuya_os_adapt_queue_fetch

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
