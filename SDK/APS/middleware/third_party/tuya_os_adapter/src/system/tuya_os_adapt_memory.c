/**
 * @file tuya_os_adapt_memory.c
 * @brief 内存操作接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */
#include <stdlib.h>
#include "tuya_os_adapt_memory.h"
#include "opl_os_adapt_memory.h"


/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/
RET_DATA t_opl_os_adapt_system_malloc_fp opl_os_adapt_system_malloc;
RET_DATA t_opl_os_adapt_system_free_fp opl_os_adapt_system_free;

/***********************************************************
*************************function define********************
***********************************************************/
void *tuya_os_adapt_system_malloc(const size_t size)
{
    return opl_os_adapt_system_malloc(size);
}

void tuya_os_adapt_system_free(void* ptr)
{
    opl_os_adapt_system_free(ptr);
}

/**
 * @brief tuya_os_adapt_system_malloc用于分配内存
 * 
 * @param[in]       size        需要分配的内存大小
 * @return  分配得到的内存指针
 */
void *tuya_os_adapt_system_malloc_impl(const size_t size)
{
    if(size == 0) 
        return NULL;
    void *ptr = malloc(size);
#if 0
    if(ptr==NULL) 
        printf("\r\n**system_getheapsize:%d**\r\n",tuya_os_adapt_system_getheapsize());
#endif
    return ptr;
}

/**
 * @brief tuya_os_adapt_system_free用于释放内存
 * 
 * @param[in]       ptr         需要释放的内存指针
 */
void tuya_os_adapt_system_free_impl(void* ptr)
{
    if(ptr == NULL) {
        return;
    }

    free(ptr);
}

void opl_os_adapt_memory_pre_init(void)
{
    opl_os_adapt_system_malloc = tuya_os_adapt_system_malloc_impl;
    opl_os_adapt_system_free = tuya_os_adapt_system_free_impl;
    return;
}

