/**
 * @file tuya_os_adapt_semaphore.c
 * @brief semaphore相关接口封装
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */

#define _UNI_SEMAPHORE_GLOBAL
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tuya_os_adapt_semaphore.h"
#include "tuya_os_adapter_error_code.h"
#include "tuya_os_adapt_system.h"
#include "tuya_os_adapt_memory.h"
#include "opl_os_adapt_semaphore.h"
/***********************************************************
*************************micro define***********************
***********************************************************/
typedef struct
{
    SemaphoreHandle_t sem;
}SEM_MANAGE, *P_SEM_MANAGE;

/***********************************************************
*************************variable define********************
***********************************************************/
RET_DATA T_OplCreateSemaphoreFp OplCreateSemaphore;
RET_DATA T_OplInitSemaphoreFp OplInitSemaphore;
RET_DATA t_opl_os_adapt_semaphore_create_init_fp opl_os_adapt_semaphore_create_init;
RET_DATA t_opl_os_adapt_semaphore_wait_fp opl_os_adapt_semaphore_wait;
RET_DATA t_opl_os_adapt_semaphore_waittimeout_fp opl_os_adapt_semaphore_waittimeout;
RET_DATA t_opl_os_adapt_semaphore_post_fp opl_os_adapt_semaphore_post;
RET_DATA t_opl_os_adapt_semaphore_release_fp opl_os_adapt_semaphore_release;
/***********************************************************
*************************function define********************
***********************************************************/
static SEM_HANDLE CreateSemaphore(void)
{
    return OplCreateSemaphore();
}

static int InitSemaphore(const SEM_HANDLE semHandle, const unsigned int semCnt,\
                         const unsigned int sem_max)
{
    return OplInitSemaphore(semHandle, semCnt, sem_max);
}

int tuya_os_adapt_semaphore_create_init(SEM_HANDLE *pHandle, const unsigned int semCnt, const unsigned int sem_max)
{
    return opl_os_adapt_semaphore_create_init(pHandle, semCnt, sem_max);
}

int tuya_os_adapt_semaphore_wait(const SEM_HANDLE semHandle)
{
    return opl_os_adapt_semaphore_wait(semHandle);
}

int tuya_os_adapt_semaphore_waittimeout(IN const SEM_HANDLE semHandle, unsigned int timeout)
{
    return opl_os_adapt_semaphore_waittimeout(semHandle, timeout);
}

int tuya_os_adapt_semaphore_post(const SEM_HANDLE semHandle)
{
    return opl_os_adapt_semaphore_post(semHandle);
}

int tuya_os_adapt_semaphore_release(const SEM_HANDLE semHandle)
{
    return opl_os_adapt_semaphore_release(semHandle);
}

static SEM_HANDLE CreateSemaphore_impl(void)
{
    P_SEM_MANAGE pSemManage;
    
    pSemManage = (P_SEM_MANAGE)tuya_os_adapt_system_malloc(sizeof(SEM_MANAGE));

    return (SEM_HANDLE)pSemManage;
}


static int InitSemaphore_impl(const SEM_HANDLE semHandle, const unsigned int semCnt,\
                         const unsigned int sem_max)
{
    if(!semHandle)
        return OPRT_OS_ADAPTER_INVALID_PARM;
        
    P_SEM_MANAGE pSemManage;
    pSemManage = (P_SEM_MANAGE)semHandle;

    pSemManage->sem = xSemaphoreCreateCounting(sem_max, semCnt);
    if(NULL == pSemManage->sem) {
        return OPRT_OS_ADAPTER_SEM_CREAT_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief tuya_os_adapt_semaphore_create_init用于创建并初始化semaphore
 * 
 * @param[out] *pHandle semaphore句柄
 * @param[in] semCnt 
 * @param[in] sem_max 
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_semaphore_create_init_impl(SEM_HANDLE *pHandle, const unsigned int semCnt, const unsigned int sem_max)
{
    if(NULL == pHandle) {

        return OPRT_OS_ADAPTER_INVALID_PARM;
    }
    *pHandle = CreateSemaphore();
    if(*pHandle == NULL) {
        return OPRT_OS_ADAPTER_MALLOC_FAILED;
    }

    int ret = InitSemaphore(*pHandle, semCnt, sem_max);
    if(ret != OPRT_OS_ADAPTER_OK) {
        tuya_os_adapt_system_free(*pHandle);
        *pHandle = NULL;  
			return ret;
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief tuya_os_adapt_semaphore_wait用于wait semaphore
 * 
 * @param[in] semHandle semaphore句柄
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_semaphore_wait_impl(const SEM_HANDLE semHandle)
{
    if(!semHandle) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    P_SEM_MANAGE pSemManage;
    pSemManage = (P_SEM_MANAGE)semHandle;

    BaseType_t ret;
    ret = xSemaphoreTake(pSemManage->sem, portMAX_DELAY);
    if(pdTRUE != ret) {
        return OPRT_OS_ADAPTER_SEM_WAIT_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief tuya_os_adapt_semaphore_wait used fo wait semaphore with timeout
 *
 * @param[in] semHandle semaphore句柄
 * @param[in] timeout  semaphore超时时间
 * @return int 0=成功，非0=失败
*/
int tuya_os_adapt_semaphore_waittimeout_impl(IN const SEM_HANDLE semHandle, unsigned int timeout)
{
    if(!semHandle) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    P_SEM_MANAGE pSemManage;
    pSemManage = (P_SEM_MANAGE)semHandle;

    BaseType_t ret;
    if (timeout == TUYA_OS_ADAPT_SEM_FOREVER) {
        ret = xSemaphoreTake(pSemManage->sem, portMAX_DELAY);
    } else {
        ret = xSemaphoreTake(pSemManage->sem, timeout / tuya_os_adapt_get_tickratems());
    }
    if (pdTRUE != ret) {
        return OPRT_OS_ADAPTER_SEM_WAIT_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief tuya_os_adapt_semaphore_post用于post semaphore
 * 
 * @param[in] semHandle semaphore句柄
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_semaphore_post_impl(const SEM_HANDLE semHandle)
{
    if(!semHandle) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    P_SEM_MANAGE pSemManage;
    pSemManage = (P_SEM_MANAGE)semHandle;

    BaseType_t ret;
    if(false == tuya_os_adapt_system_isrstatus()) {
        ret = xSemaphoreGive(pSemManage->sem);
    }else {
        signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        ret = xSemaphoreGiveFromISR(pSemManage->sem,
                                    &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }

    if(pdTRUE != ret) {
        return OPRT_OS_ADAPTER_SEM_POST_FAILED;
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief tuya_os_adapt_semaphore_release用于release semaphore
 * 
 * @param[in] semHandle 
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_semaphore_release_impl(const SEM_HANDLE semHandle)
{
    if(!semHandle) {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    P_SEM_MANAGE pSemManage;
    pSemManage = (P_SEM_MANAGE)semHandle;

    vSemaphoreDelete(pSemManage->sem);
    tuya_os_adapt_system_free(semHandle); // 释放信号量管理结构

    return OPRT_OS_ADAPTER_OK;
}

void opl_os_adapt_semaphore_pre_init(void)
{
    OplCreateSemaphore = CreateSemaphore_impl;
    OplInitSemaphore = InitSemaphore_impl;
    opl_os_adapt_semaphore_create_init = tuya_os_adapt_semaphore_create_init_impl;
    opl_os_adapt_semaphore_wait = tuya_os_adapt_semaphore_wait_impl;
    opl_os_adapt_semaphore_waittimeout = tuya_os_adapt_semaphore_waittimeout_impl;
    opl_os_adapt_semaphore_post = tuya_os_adapt_semaphore_post_impl;
    opl_os_adapt_semaphore_release = tuya_os_adapt_semaphore_release_impl;
    return;
}

