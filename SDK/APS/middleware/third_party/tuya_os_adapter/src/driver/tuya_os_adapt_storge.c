/**
 * @file tuya_os_adapt_storge.c
 * @brief flash操作接口
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */

#define __UNI_STORGE_GLOBALS
#include "tuya_os_adapt_storage.h"
#include "tuya_os_adapter_error_code.h"

#include "hal_flash.h"
#include "opl_os_adapt_storage.h"

/***********************************************************
*************************micro define***********************
***********************************************************/



/***********************************************************
*************************variable define********************
***********************************************************/
#define FLASH_SECTOR_4K         0x1000 // 4K

// todo: confirm it
#define SIMPLE_FLASH_START      0x00090000 //90000
#define SIMPLE_FLASH_SIZE       0x00008000 //40K 32K   24k
#define PARTITION_SIZE          FLASH_SECTOR_4K
#define SIMPLE_FLASH_SWAP_START 0x000A0000
#define SIMPLE_FLASH_SWAP_SIZE  0x00004000 // 16K
#define SIMPLE_FLASH_KEY_ADDR   0x000B0000

static UNI_STORAGE_DESC_S tuya_storage = 
{
    SIMPLE_FLASH_START,
    SIMPLE_FLASH_SIZE,
    PARTITION_SIZE,
    SIMPLE_FLASH_SWAP_START,
    SIMPLE_FLASH_SWAP_SIZE,
    SIMPLE_FLASH_KEY_ADDR
};

RET_DATA t_opl_os_adapt_flash_read_fp opl_os_adapt_flash_read;
RET_DATA t_opl_os_adapt_flash_write_fp opl_os_adapt_flash_write;
RET_DATA t_opl_os_adapt_flash_erase_fp opl_os_adapt_flash_erase;
RET_DATA t_opl_os_adapt_storage_get_desc_fp opl_os_adapt_storage_get_desc;
/***********************************************************
*************************function define********************
***********************************************************/
int tuya_os_adapt_flash_read(const unsigned int addr, unsigned char *dst, const unsigned int size)
{
    return opl_os_adapt_flash_read(addr, dst, size);
}

int tuya_os_adapt_flash_write(const unsigned int addr, const unsigned char *src, const unsigned int size)
{
    return opl_os_adapt_flash_write(addr, src, size);
}

int tuya_os_adapt_flash_erase(const unsigned int addr, const unsigned int size)
{
    return opl_os_adapt_flash_erase(addr, size);
}

UNI_STORAGE_DESC_S *tuya_os_adapt_storage_get_desc(void)
{
    return opl_os_adapt_storage_get_desc();
}

/**
 * @brief read data from flash
 * 
 * @param[in]       addr        flash address
 * @param[out]      dst         pointer of buffer
 * @param[in]       size        size of buffer
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_flash_read_impl(const unsigned int addr, unsigned char *dst, const unsigned int size)
{
    uint32_t u32Ret;
    
    if(NULL == dst) 
    {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    u32Ret = Hal_Flash_AddrRead(SPI_IDX_0, addr, 0, size, dst);

    if (u32Ret == 0)
    {
        return OPRT_OS_ADAPTER_OK;
    }
    else
    {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }
}

/**
 * @brief write data to flash
 * 
 * @param[in]       addr        flash address
 * @param[in]       src         pointer of buffer
 * @param[in]       size        size of buffer
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_flash_write_impl(const unsigned int addr, const unsigned char *src, const unsigned int size)
{
    uint32_t u32Ret;
    
    if(NULL == src) 
    {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    u32Ret = Hal_Flash_AddrProgram(SPI_IDX_0, addr, 0, size, (uint8_t *)src);

    if (u32Ret == 0)
    {
        return OPRT_OS_ADAPTER_OK;
    }
    else
    {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }
}

/**
 * @brief erase flash block
 * 
 * @param[in]       addr        flash block address
 * @param[in]       size        size of flash block
 * @return int 0=成功，非0=失败
 */
int tuya_os_adapt_flash_erase_impl(const unsigned int addr, const unsigned int size)
{
    uint32_t u32Ret = 0;
    uint32_t u32EraseAddr = addr;
    uint32_t u32EraseSize = size;
    
    while (u32EraseSize > 0)
    {
        u32Ret = Hal_Flash_4KSectorAddrErase(SPI_IDX_0, u32EraseAddr);
        if (u32Ret != 0)
            break;
        
        u32EraseAddr += FLASH_SECTOR_4K;
        u32EraseSize -= FLASH_SECTOR_4K;
    }

    if (u32Ret == 0)
    {
        return OPRT_OS_ADAPTER_OK;
    }
    else
    {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }
}


/**
 * @brief get description of storage
 * 
 * @return  pointer to storage description structure
 */
UNI_STORAGE_DESC_S *tuya_os_adapt_storage_get_desc_impl(void)
{
    return &tuya_storage;
}

void opl_os_adapt_storage_pre_init(void)
{
    opl_os_adapt_flash_read = tuya_os_adapt_flash_read_impl;
    opl_os_adapt_flash_write = tuya_os_adapt_flash_write_impl;
    opl_os_adapt_flash_erase = tuya_os_adapt_flash_erase_impl;
    opl_os_adapt_storage_get_desc = tuya_os_adapt_storage_get_desc_impl;
    return;
}

