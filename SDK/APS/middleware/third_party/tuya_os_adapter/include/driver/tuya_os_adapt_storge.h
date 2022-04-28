/**
 * @file tuya_os_adapt_storage.h
 * @brief STORAGE设备操作接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */

#ifndef __TUYA_OS_ADAPT_STORAGE_H__
#define __TUYA_OS_ADAPT_STORAGE_H__


#include <stdint.h>
#include <string.h>
#include "tuya_os_adapter.h"

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct {
    unsigned int start_addr; // user physical flash start address 
    unsigned int flash_sz; // user flash size
    unsigned int block_sz; // flash block/sector size

    // For data backup and power down protection data recovery
    unsigned int swap_start_addr; // swap flash start address
    unsigned int swap_flash_sz; // swap flash size    

    //for restore factor of flash encryption key 
    unsigned int key_restore_addr;
}UNI_STORAGE_DESC_S;

/**
 * @brief read data from flash
 * 
 * @param[in]       addr        flash address
 * @param[out]      dst         pointer of buffer
 * @param[in]       size        size of buffer
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_flash_read(const unsigned int addr, unsigned char *dst, const unsigned int size);
#define tuya_hal_flash_read tuya_os_adapt_flash_read

/**
 * @brief write data to flash
 * 
 * @param[in]       addr        flash address
 * @param[in]       src         pointer of buffer
 * @param[in]       size        size of buffer
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_flash_write(const unsigned int addr, const unsigned char *src, const unsigned int size);
#define tuya_hal_flash_write tuya_os_adapt_flash_write

/**
 * @brief erase flash block
 * 
 * @param[in]       addr        flash block address
 * @param[in]       size        size of flash block
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_flash_erase(const unsigned int addr, const unsigned int size);
#define tuya_hal_flash_erase tuya_os_adapt_flash_erase


/**
 * @brief get description of storage
 * 
 * @return  pointer to storage description structure
 */
UNI_STORAGE_DESC_S* tuya_os_adapt_storage_get_desc(void);
#define tuya_hal_storage_get_desc tuya_os_adapt_storage_get_desc

#ifdef __cplusplus
}
#endif

#endif // __TUYA_OS_ADAPT_STORAGE_H__

