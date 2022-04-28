/**
 * @file tuya_os_adapt_mutex.c
 * @brief 互斥锁操作接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */
#define _UNI_MUTEX_GLOBAL
#include "tuya_os_adapt_mutex.h"
#include "cmsis_os.h"
#include "tuya_os_adapt_memory.h"
#include "opl_os_adapt_mutex.h"
/***********************************************************
*************************micro define***********************
***********************************************************/
typedef SemaphoreHandle_t THRD_MUTEX;

typedef struct
{
    THRD_MUTEX mutex;
}MUTEX_MANAGE,*P_MUTEX_MANAGE;


/***********************************************************
*************************variable define********************
***********************************************************/
RET_DATA t_opl_os_adapt_mutex_create_init_fp opl_os_adapt_mutex_create_init;
RET_DATA t_opl_os_adapt_mutex_lock_fp opl_os_adapt_mutex_lock;
RET_DATA t_opl_os_adapt_mutex_unlock_fp opl_os_adapt_mutex_unlock;
RET_DATA t_opl_os_adapt_mutex_release_fp opl_os_adapt_mutex_release;
/***********************************************************
*************************function define********************
***********************************************************/
int tuya_os_adapt_mutex_create_init(MUTEX_HANDLE *pMutexHandle)
{
    return opl_os_adapt_mutex_create_init(pMutexHandle);
}

int tuya_os_adapt_mutex_lock(const MUTEX_HANDLE mutexHandle)
{
    return opl_os_adapt_mutex_lock(mutexHandle);
}

int tuya_os_adapt_mutex_unlock(const MUTEX_HANDLE mutexHandle)
{
    return opl_os_adapt_mutex_unlock(mutexHandle);
}

int tuya_os_adapt_mutex_release(const MUTEX_HANDLE mutexHandle)
{
    return opl_os_adapt_mutex_release(mutexHandle);
}

/**
 * @brief tuya_os_adapt_mutex_create_init用于创建并初始化tuya mutex
 * 
 * @param[out] pMutexHandle 返回mutex句柄
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_mutex_create_init_impl(MUTEX_HANDLE *pMutexHandle)
{
    if(!pMutexHandle)
        return OPRT_OS_ADAPTER_INVALID_PARM;
    P_MUTEX_MANAGE pMutexManage;
    pMutexManage = (P_MUTEX_MANAGE)tuya_os_adapt_system_malloc(sizeof(MUTEX_MANAGE));
    if(!(pMutexManage))
        return OPRT_OS_ADAPTER_MALLOC_FAILED;

    pMutexManage->mutex = xSemaphoreCreateMutex();

    if(NULL == pMutexManage->mutex) {
        return OPRT_OS_ADAPTER_MUTEX_CREAT_FAILED;
    }

    *pMutexHandle = (MUTEX_HANDLE)pMutexManage;
    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief tuya_os_adapt_mutex_lock用于lock tuya mutex
 * 
 * @param[in] mutexHandle tuya mutex句柄
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_mutex_lock_impl(const MUTEX_HANDLE mutexHandle)
{
    if(!mutexHandle)
        return OPRT_OS_ADAPTER_INVALID_PARM;


    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief tuya_os_adapt_mutex_unlock用于unlock tuya mutex
 * 
 * @param[in] mutexHandle tuya mutex句柄
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_mutex_unlock_impl(const MUTEX_HANDLE mutexHandle)
{
    if(!mutexHandle)
        return OPRT_OS_ADAPTER_INVALID_PARM;  


    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief tuya_os_adapt_mutex_release用于释放tuya mutex
 * 
 * @param[in] mutexHandle MUTEX_HANDLE tuya mutex句柄
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_mutex_release_impl(const MUTEX_HANDLE mutexHandle)
{
    if(!mutexHandle)
        return OPRT_OS_ADAPTER_INVALID_PARM;

    return OPRT_OS_ADAPTER_OK;
}

void opl_os_adapt_mutex_pre_init(void)
{
    opl_os_adapt_mutex_create_init = tuya_os_adapt_mutex_create_init_impl;
    opl_os_adapt_mutex_lock = tuya_os_adapt_mutex_lock_impl;
    opl_os_adapt_mutex_unlock = tuya_os_adapt_mutex_unlock_impl;
    opl_os_adapt_mutex_release = tuya_os_adapt_mutex_release_impl;
    return;
}

