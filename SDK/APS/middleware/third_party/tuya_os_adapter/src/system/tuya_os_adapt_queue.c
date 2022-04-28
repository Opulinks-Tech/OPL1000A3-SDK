/**
 * @file tuya_os_adapt_queue.c
 * @brief 队列操作接口
 *
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 *
 */
#include "tuya_os_adapt_system.h"
#include "tuya_os_adapt_queue.h"
#include "tuya_os_adapt_memory.h"
#include "cmsis_os.h"
#include "sys_init.h"
#include "sys_init_patch.h"
#include "hal_system.h"
#include <stdio.h>
#include <stdint.h>
#include "opl_os_adapt_queue.h"
/***********************************************************
*************************micro define***********************
***********************************************************/
typedef struct {
    QueueHandle_t queue;
} QUEUE_MANAGE, *P_QUEUE_MANAGE;

/***********************************************************
*************************variable define********************
***********************************************************/
RET_DATA t_opl_os_adapt_queue_create_init_fp opl_os_adapt_queue_create_init;
RET_DATA t_opl_os_adapt_queue_free_fp opl_os_adapt_queue_free;
RET_DATA t_opl_os_adapt_queue_post_fp opl_os_adapt_queue_post;
RET_DATA t_opl_os_adapt_queue_fetch_fp opl_os_adapt_queue_fetch;

/***********************************************************
*************************function define********************
***********************************************************/
int tuya_os_adapt_queue_create_init(QUEUE_HANDLE *queue, int size)
{
    return opl_os_adapt_queue_create_init(queue, size);
}

void tuya_os_adapt_queue_free(QUEUE_HANDLE queue)
{
    opl_os_adapt_queue_free(queue);
}

int tuya_os_adapt_queue_post(QUEUE_HANDLE queue, void *msg, unsigned int timeout)
{
    return opl_os_adapt_queue_post(queue, msg, timeout);
}

int tuya_os_adapt_queue_fetch(QUEUE_HANDLE queue, void **msg, unsigned int timeout)
{
    return opl_os_adapt_queue_fetch(queue, msg, timeout);
}

/**
 * @brief create queue
 *
 * @param[out]     queue      queue to be create
 * @param[in]      size       the deep of the queue
 * @return  OPRT_OS_ADAPTER_OK: SUCCESS other:fail
 */
int tuya_os_adapt_queue_create_init_impl(QUEUE_HANDLE *queue, int size)
{

    P_QUEUE_MANAGE pQueueManage;

    if (!queue) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    *queue = NULL;

    pQueueManage = (P_QUEUE_MANAGE)tuya_os_adapt_system_malloc(sizeof(QUEUE_MANAGE));
    if (pQueueManage == NULL) {
        return OPRT_OS_ADAPTER_MALLOC_FAILED;
    }

    pQueueManage->queue = xQueueCreate(size, sizeof(void *));
    if (pQueueManage->queue == NULL) {
        tuya_os_adapt_system_free(pQueueManage);
        return OPRT_OS_ADAPTER_QUEUE_CREAT_FAILED;
    } else {
        *queue = (QUEUE_HANDLE)pQueueManage;
    }

    //printf("tuya_os_adapt_queue_create_init  %x   %x\n", pQueueManage, pQueueManage->queue);

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief free queue
 *
 * @param[in]     queue      queue to be free
 * @return  void
 */
void tuya_os_adapt_queue_free_impl(QUEUE_HANDLE queue)
{
    P_QUEUE_MANAGE pQueueManage;
    pQueueManage = (P_QUEUE_MANAGE)queue;

    if (!queue) {
        return; //返回类型错误
    }

    //printf("tuya_os_adapt_queue_free  %x   %x\n", pQueueManage, pQueueManage->queue);

    if (uxQueueMessagesWaiting(pQueueManage->queue)) {
        /* Line for breakpoint.  Should never break here! */
        portNOP();
        // TODO notify the user of failure.
    }

    vQueueDelete(pQueueManage->queue);
    tuya_os_adapt_system_free(pQueueManage);

}

/**
 * @brief fetch msg from queue in timeout ms
 *
 * @param[in]      queue      queue to post
 * @param[in]      msg        msg to post
 * @param[in]      timeout    max time to wait for msg(ms), TUYA_OS_ADAPT_QUEUE_FOREVER means forever wait
 * @return  int OPRT_OS_ADAPTER_OK:success    other:fail
 */
int tuya_os_adapt_queue_post_impl(QUEUE_HANDLE queue, void *msg, unsigned int timeout)
{
    
 int ret = pdPASS;
    P_QUEUE_MANAGE pQueueManage;
    pQueueManage = (P_QUEUE_MANAGE)queue;

    if (!queue) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    //printf("tuya_os_adapt_queue_post  %x   %x\n", pQueueManage, pQueueManage->queue);
    if(false == tuya_os_adapt_system_isrstatus()) {
        ret = xQueueSend( pQueueManage->queue, &msg, (timeout==TUYA_OS_ADAPT_QUEUE_FOREVER)?portMAX_DELAY:(timeout/ tuya_os_adapt_get_tickratems()));
    } else {
        signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        ret = xQueueSendFromISR( pQueueManage->queue, &msg, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    
    if (pdPASS == ret) {
        return OPRT_OS_ADAPTER_OK;
    } else {
        // could not post, queue must be full
        return OPRT_OS_ADAPTER_QUEUE_SEND_FAIL;
    }
}

/**
 * @brief fetch msg from queue in timeout ms
 *
 * @param[in]      queue      queue to post
 * @param[out]     msg        msg to fetch
 * @param[in]      timeout    max time to wait for msg(ms), TUYA_OS_ADAPT_QUEUE_FOREVER means forever wait
 * @return  int OPRT_OS_ADAPTER_OK:success    other:fail
 */
int tuya_os_adapt_queue_fetch_impl(QUEUE_HANDLE queue, void **msg, unsigned int timeout)
{
    void *dummyptr;
    P_QUEUE_MANAGE pQueueManage;
    pQueueManage = (P_QUEUE_MANAGE)queue;

    if (!queue) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    if (msg == NULL) {
        msg = &dummyptr;
    }

    //printf("tuya_os_adapt_queue_fetch  %x   %x\n", pQueueManage, pQueueManage->queue);

    if (pdTRUE == xQueueReceive(pQueueManage->queue, &(*msg), (timeout == TUYA_OS_ADAPT_QUEUE_FOREVER) ? portMAX_DELAY : (timeout / tuya_os_adapt_get_tickratems()))) {
        return OPRT_OS_ADAPTER_OK;
    } else { // timed out blocking for message
        return OPRT_OS_ADAPTER_QUEUE_RECV_FAIL;
    }
}

void opl_os_adapt_queue_pre_init(void)
{
    opl_os_adapt_queue_create_init = tuya_os_adapt_queue_create_init_impl;
    opl_os_adapt_queue_free = tuya_os_adapt_queue_free_impl;
    opl_os_adapt_queue_post = tuya_os_adapt_queue_post_impl;
    opl_os_adapt_queue_fetch = tuya_os_adapt_queue_fetch_impl;
    return;
}

