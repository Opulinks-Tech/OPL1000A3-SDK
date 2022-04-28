/**
 * @file tuya_os_adapt_ota.c
 * @brief ota底层操作接口
 * 
 * @copyright Copyright (c) {2018-2020} 涂鸦科技 www.tuya.com
 * 
 */
#include <string.h>
#include "tuya_os_adapt_ota.h"
#include "tuya_os_adapter_error_code.h"
#include "tuya_os_adapt_system.h"

#include "cmsis_os.h"
#include "mw_ota.h"
#include "hal_system.h"
#include "opl_os_adapt_ota.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
#define OPU_OTA_ALLOC(size)     pvPortMalloc(size)
#define OPU_OTA_FREE(size)      pvPortFree(size)

/***********************************************************
*************************variable define********************
***********************************************************/
#define OPU_OTA_SKIP1_LEN       (1024 * 12)
#define OPU_OTA_HEADER_LEN      24
#define OPU_OTA_SKIP2_LEN       (1024 * 8 - OPU_OTA_HEADER_LEN)
#define OPU_OTA_BUF_LEN         256

#if 0
typedef enum
{
    OPU_OTA_STA_SKIP1,
    OPU_OTA_STA_HEADER,
    OPU_OTA_STA_SKIP2,
    OPU_OTA_STA_BODY

} OPU_OTA_STA;

typedef enum
{
    OPU_OTA_ERR_MEMORY_ALLOCATE_FAIL = 1,

    OPU_OTA_ERR_FLASH_PREPARE_ERR,
    OPU_OTA_ERR_FLASH_WRITE_ERR,
    OPU_OTA_ERR_FLASH_FINISH_ERR,

    OPU_OTA_ERR_OVER_SIZE,
    OPU_OTA_ERR_INCORRECT_CHECKSUM,
    OPU_OTA_ERR_OK,
    OPU_OTA_OK_COMPLETED,

} OPU_OTA_ERR;

typedef struct
{
    OPU_OTA_STA sta;        // OTA state

    int cnt;                // counter of buffer
    uint8_t *buf;           // buffer: size 256 bytes

    uint32_t img_size;      // total expected image size
    uint32_t cur_size;      // current received size
    uint32_t img_sum;       // final image checksum
    uint32_t cur_sum;       // current computed checksum
	
} opu_ota_data_t;
#endif

static opu_ota_data_t ota;

uint32_t gu32FlashWriteAddr = 0;


static OPU_OTA_ERR Opu_OtaDataIn(uint8_t *data, int len);
static OPU_OTA_ERR Opu_OtaDataInSkip1(opu_ota_data_t *ota, uint8_t *data, int len);
static OPU_OTA_ERR Opu_OtaDataInHeader(opu_ota_data_t *ota, uint8_t *data, int len);
static OPU_OTA_ERR Opu_OtaDataInSkip2(opu_ota_data_t *ota, uint8_t *data, int len);
static OPU_OTA_ERR Opu_OtaDataInBody(opu_ota_data_t *ota, uint8_t *data, int len);
static OPU_OTA_ERR Opu_OtaProcessHeader(uint8_t *data, uint32_t *size, uint32_t *checksum);
static OPU_OTA_ERR Opu_OtaProcessBody(uint8_t *data, int len);
static void Opu_OtaComputeSum(uint8_t *data, int len, uint32_t *checksum);

RET_DATA T_OplOtaDataInFp OplOtaDataIn;
RET_DATA T_OplOtaDataInSkip1Fp OplOtaDataInSkip1;
RET_DATA T_OplOtaDataInHeaderFp OplOtaDataInHeader;
RET_DATA T_OplOtaDataInSkip2Fp OplOtaDataInSkip2;
RET_DATA T_OplOtaDataInBodyFp OplOtaDataInBody;
RET_DATA T_OplOtaProcessHeaderFp OplOtaProcessHeader;
RET_DATA T_OplOtaProcessBodyFp OplOtaProcessBody;
RET_DATA T_OplOtaComputeSumFp OplOtaComputeSum;
RET_DATA t_opl_os_adapt_ota_start_inform_fp opl_os_adapt_ota_start_inform;
RET_DATA t_opl_os_adapt_ota_data_process_fp opl_os_adapt_ota_data_process;
RET_DATA t_opl_os_adapt_ota_end_inform_fp opl_os_adapt_ota_end_inform;

/***********************************************************
*************************function define********************
***********************************************************/
static OPU_OTA_ERR Opu_OtaDataIn(uint8_t *data, int len)
{
    return OplOtaDataIn(data, len);
}

static OPU_OTA_ERR Opu_OtaDataInSkip1(opu_ota_data_t *ota, uint8_t *data, int len)
{
    return OplOtaDataInSkip1(ota, data, len);
}

static OPU_OTA_ERR Opu_OtaDataInHeader(opu_ota_data_t *ota, uint8_t *data, int len)
{
    return OplOtaDataInHeader(ota, data, len);
}

static OPU_OTA_ERR Opu_OtaDataInSkip2(opu_ota_data_t *ota, uint8_t *data, int len)
{
    return OplOtaDataInSkip2(ota, data, len);
}

static OPU_OTA_ERR Opu_OtaDataInBody(opu_ota_data_t *ota, uint8_t *data, int len)
{
    return OplOtaDataInBody(ota, data, len);
}

static OPU_OTA_ERR Opu_OtaProcessHeader(uint8_t *data, uint32_t *size, uint32_t *checksum)
{
    return OplOtaProcessHeader(data, size, checksum);
}

static OPU_OTA_ERR Opu_OtaProcessBody(uint8_t *data, int len)
{
    return OplOtaProcessBody(data, len);
}

static void Opu_OtaComputeSum(uint8_t *data, int len, uint32_t *checksum)
{
    OplOtaComputeSum(data, len, checksum);
}

int tuya_os_adapt_ota_start_inform(unsigned int file_size)
{
    return opl_os_adapt_ota_start_inform(file_size);
}

int tuya_os_adapt_ota_data_process(const unsigned int total_len, const unsigned int offset,
                              const unsigned char* data, const unsigned int len, unsigned int* remain_len, void* pri_data)
{
    return opl_os_adapt_ota_data_process(total_len, offset, data, len, remain_len, pri_data);
}

int tuya_os_adapt_ota_end_inform(bool reset)
{
    return opl_os_adapt_ota_end_inform(reset);
}
/**
 * Opulinks OTA firmware layout
 * 
 * 0x0000_0000 +----------------------------+
 *             | Boot Agent Patch       n B |
 *             +----------------------------+
 *             | Padding (All 0xFF)         |
 *             |                    12k-b B |
 * 0x0000_3000 +----------------------------+
 *             | OTA header            64 B |
 *             +----------------------------+
 *             | Padding (All 0xFF) 4k-64 B |
 * 0x0000_4000 +----------------------------+
 *             | Padding (All 0xFF)         |
 *             |                            |
 *             |                       4 kB |
 * 0x0000_5000 +----------------------------+
 *             | OTA image                  |
 *             |                            |
 *             |                       n kB |
 *             +----------------------------+
 */
static OPU_OTA_ERR Opu_OtaDataIn_impl(uint8_t *data, int len)
{
    OPU_OTA_ERR err = OPU_OTA_ERR_OK;

    switch (ota.sta)
    {
        case OPU_OTA_STA_SKIP1:
        {
            err = Opu_OtaDataInSkip1(&ota, data, len);
            break;
        }
        case OPU_OTA_STA_HEADER:
        {
            err = Opu_OtaDataInHeader(&ota, data, len);
            break;
        }
        case OPU_OTA_STA_SKIP2:
        {
            err = Opu_OtaDataInSkip2(&ota, data, len);
            break;
        }
        case OPU_OTA_STA_BODY:
        {
            err = Opu_OtaDataInBody(&ota, data, len);
            break;
        }
    }
    return err;
}

static OPU_OTA_ERR Opu_OtaDataInSkip1_impl(opu_ota_data_t *ota, uint8_t *data, int len)
{
    OPU_OTA_ERR err = OPU_OTA_ERR_OK;
    int lack_len = OPU_OTA_SKIP1_LEN - ota->cnt;

    // not enough len of skip
    if (len < lack_len)
    {
        ota->cnt += len;
    }
    else
    {
        // change state
        data += lack_len;
        len -= lack_len;

        ota->cnt = 0;
        ota->sta = OPU_OTA_STA_HEADER;

        if (len)
        {
            err = Opu_OtaDataInHeader(ota, data, len);
        }
    }
    return err;
}

static OPU_OTA_ERR Opu_OtaDataInHeader_impl(opu_ota_data_t *ota, uint8_t *data, int len)
{
    OPU_OTA_ERR err = OPU_OTA_ERR_OK;
    int lack_len = OPU_OTA_HEADER_LEN - ota->cnt;
    // not enough len of header
    if (len < lack_len)
    {
        memcpy(&ota->buf[ota->cnt], data, len);
        ota->cnt += len;
    }
    else
    {   
        
        memcpy(&ota->buf[ota->cnt], data, lack_len);
        //tuya_debug_hex_dump("Header",16,ota->buf,24);
        err = Opu_OtaProcessHeader(ota->buf, &ota->img_size, &ota->img_sum);

        // change state
        ota->cnt = len - lack_len;
        ota->sta = OPU_OTA_STA_SKIP2;
    }
    return err;
}

static OPU_OTA_ERR Opu_OtaDataInSkip2_impl(opu_ota_data_t *ota, uint8_t *data, int len)
{
    OPU_OTA_ERR err = OPU_OTA_ERR_OK;
    int lack_len = OPU_OTA_SKIP2_LEN - ota->cnt;

    // not enough len of skip
    if (len < lack_len)
    {
        ota->cnt += len;
    }
    else
    {
        // change state
        data += lack_len;
        len -= lack_len;

        ota->cnt = 0;
        ota->sta = OPU_OTA_STA_BODY;

        if (len)
        {
            err = Opu_OtaDataInBody(ota, data, len);
        }
    }
    return err;
}

static OPU_OTA_ERR Opu_OtaDataInBody_impl(opu_ota_data_t *ota, uint8_t *data, int len)
{
    OPU_OTA_ERR err = OPU_OTA_ERR_OK;
    int lack_len = (OPU_OTA_BUF_LEN - ota->cnt) < (ota->img_size - ota->cur_size) ? (OPU_OTA_BUF_LEN - ota->cnt) : (ota->img_size - ota->cur_size);
    //int lack_len = ota->img_size - ota->cur_size - ota->cnt;
    //unsigned int write_len = len + ota->cnt;
    //unsigned int over_bit = write_len % 4;
    // not enough len of buffer or remaing part
    if (len < lack_len)
    {
        memcpy(&ota->buf[ota->cnt], data, len);
        ota->cnt += len;
        ota->cur_size += len;
        // ota->cur_size += write_len;
        // if(over_bit != 0) {
        //     write_len = len - over_bit;
        // }               
        // Opu_OtaComputeSum(ota->buf, ota->cnt, &ota->cur_sum);
        // Opu_OtaComputeSum(data, len, &ota->cur_sum);
        // if (((err = Opu_OtaProcessBody(ota->buf, ota->cnt)) != OPU_OTA_ERR_OK)\
        //     || ((err = Opu_OtaProcessBody(data, write_len)) != OPU_OTA_ERR_OK))
        //         return err;
        // ota->cnt = 0;
        // data += write_len;
        // memcpy(&ota->buf[ota->cnt], data, over_bit);
        // ota->cnt += over_bit;           
    }
    else
    {
        //MwOta_DataGiveUp();
        //return OPU_OTA_ERR_OVER_SIZE;      
        
        #if 1
        do {
            // buffer processing
            memcpy(&ota->buf[ota->cnt], data, lack_len);
            ota->cnt += lack_len;
            ota->cur_size += lack_len;

            // store
            Opu_OtaComputeSum(ota->buf, ota->cnt, &ota->cur_sum);
            if ((err = Opu_OtaProcessBody(ota->buf, ota->cnt)) != OPU_OTA_ERR_OK)
                return err;

            // input data processing
            len -= lack_len;
            data += lack_len;

            // reset cnt, lack_len for next round
            ota->cnt = 0;
            lack_len = (OPU_OTA_BUF_LEN - ota->cnt) < (ota->img_size - ota->cur_size) ? (OPU_OTA_BUF_LEN - ota->cnt) : (ota->img_size - ota->cur_size);

        } while (len >= lack_len && ota->cur_size < ota->img_size);

        // not enough len of buffer or remaing part
        if (ota->cur_size < ota->img_size)
        {
            memcpy(ota->buf, data, len);
            ota->cnt = len;
            ota->cur_size += len;
        }
        // received size exceeded expected image size
        else if (len > lack_len)
        {
            MwOta_DataGiveUp();
            return OPU_OTA_ERR_OVER_SIZE;
        }
        // received expected size
        // else
        // {
        //     uint8_t state = MwOta_DataFinish();
        //     return (state == MW_OTA_OK) ? OPU_OTA_OK_COMPLETED : OPU_OTA_ERR_FLASH_FINISH_ERR;
        // }
        #endif
    }
	
    return err;
}

static OPU_OTA_ERR Opu_OtaProcessHeader_impl(uint8_t *data, uint32_t *size, uint32_t *checksum)
{
    T_MwOtaFlashHeader *header = (T_MwOtaFlashHeader *) data;
    *size = header->ulImageSize;
    *checksum = header->ulImageSum;
    
    uint8_t state = MwOta_Prepare(header->uwProjectId, header->uwChipId, header->uwFirmwareId, header->ulImageSize, header->ulImageSum);
    if (state != MW_OTA_OK)
    {
        MwOta_DataGiveUp();
    }
    // extern T_MwOtaLayoutInfo g_tMwOtaLayoutInfo; 
    // printf("ulaHeaderAddr1: %08X\r\n ulaHeaderAddr2: %08X\r\nulaImageAddr1: %08X\r\n ulaImageAddr2: %08X\r\n", 
    //     g_tMwOtaLayoutInfo.ulaHeaderAddr[0], g_tMwOtaLayoutInfo.ulaHeaderAddr[1],g_tMwOtaLayoutInfo.ulaImageAddr[0],g_tMwOtaLayoutInfo.ulaImageAddr[1]);
	
    // extern T_MwOtaFlashHeader g_tMwOtaPrepareHeaderInfo;
    // printf("uwProjectId: %08X\r\n uwChipId: %08X\r\nuwFirmwareId: %08X\r\n ulImageSize: %08X\r\n ulImageSum: %08X\r\n", 
    //     g_tMwOtaPrepareHeaderInfo.uwProjectId,g_tMwOtaPrepareHeaderInfo.uwChipId,
    //     g_tMwOtaPrepareHeaderInfo.uwFirmwareId,g_tMwOtaPrepareHeaderInfo.ulImageSize,
    //     g_tMwOtaPrepareHeaderInfo.ulImageSum);   
    // Show OTA Header & Image Address Info
    extern uint32_t g_ulMwOtaPrepareHeaderAddr;  // the start address of header
    extern uint32_t g_ulMwOtaPrepareImageAddr;   // the start address of image
    printf("OTA hdr addr: %08X\r\nOTA img addr: %08X\r\nOTA img size: %08X\r\n", 
        g_ulMwOtaPrepareHeaderAddr, g_ulMwOtaPrepareImageAddr, header->ulImageSize);
	
	gu32FlashWriteAddr = g_ulMwOtaPrepareImageAddr;
	
    return (state == MW_OTA_OK) ? OPU_OTA_ERR_OK : OPU_OTA_ERR_FLASH_PREPARE_ERR;
}

static OPU_OTA_ERR Opu_OtaProcessBody_impl(uint8_t *data, int len)
{
    uint8_t state = MwOta_DataIn(data, len);
    
    if (state != MW_OTA_OK)
    {
        MwOta_DataGiveUp();
    }
    
    gu32FlashWriteAddr += len;
	
    return (state == MW_OTA_OK) ? OPU_OTA_ERR_OK : OPU_OTA_ERR_FLASH_WRITE_ERR;
}

static void Opu_OtaComputeSum_impl(uint8_t *data, int len, uint32_t *checksum)
{
    int i;

    for (i = 0; i < len; i++)
    {
        *checksum += data[i];
    }
}



/**
 * @brief 升级开始通知函数
 * 
 * @param[in] file_size 升级固件大小
 * 
 * @retval  =0      成功
 * @retval  <0      错误码
 */
int tuya_os_adapt_ota_start_inform_impl(unsigned int file_size)
{
    if (file_size == 0) 
    {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }

    ota.sta = OPU_OTA_STA_SKIP1;
    ota.cnt = 0;
    ota.cur_size = 0;
    ota.cur_sum = 0;
    
    if (!ota.buf)
    {
        ota.buf = (uint8_t *) OPU_OTA_ALLOC(OPU_OTA_BUF_LEN);

        if (!ota.buf)
        {
            return OPRT_OS_ADAPTER_INVALID_PARM; // OPU_OTA_ERR_MEMORY_ALLOCATE_FAIL;
        }
    }

    return OPRT_OS_ADAPTER_OK;
}

/**
 * @brief ota数据包处理
 * 
 * @param[in] total_len ota升级包总大小
 * @param[in] offset 当前data在升级包中的偏移
 * @param[in] data ota数据buffer指针
 * @param[in] len ota数据buffer长度
 * @param[out] remain_len 内部已经下发但该函数还未处理的数据长度
 * @param[in] pri_data 保留参数
 *
 * @retval  =0      成功
 * @retval  <0      错误码
 */
int tuya_os_adapt_ota_data_process_impl(const unsigned int total_len, const unsigned int offset,
                              const unsigned char* data, const unsigned int len, unsigned int* remain_len, void* pri_data)
{
    OPU_OTA_ERR tOtaErr = Opu_OtaDataIn((uint8_t *)data, len);
    *remain_len = 0; // todo: confirm it

    if (tOtaErr != OPU_OTA_ERR_OK)
        goto error;

    return OPRT_OS_ADAPTER_OK;

error:
    return OPRT_OS_ADAPTER_INVALID_PARM;
}

/**
 * @brief 固件ota数据传输完毕通知
 *        用户可以做固件校验以及设备重启
 * param[in]        reset       是否需要重启
 * @retval  =0      成功
 * @retval  <0      错误码
 */
int tuya_os_adapt_ota_end_inform_impl(bool reset)
{
    uint8_t u8Ret;

    u8Ret = MwOta_DataFinish();
    printf("\r\n*******u8Ret: %d*********", u8Ret);
    if (u8Ret == MW_OTA_OK)
    {
        if (reset) Hal_Sys_SwResetAll();
        return OPRT_OS_ADAPTER_OK;
    }
    else
    {
        return OPRT_OS_ADAPTER_INVALID_PARM;
    }
}

void opl_os_adapt_ota_pre_init(void)
{
    OplOtaDataIn = Opu_OtaDataIn_impl;
    OplOtaDataInSkip1 = Opu_OtaDataInSkip1_impl;
    OplOtaDataInHeader = Opu_OtaDataInHeader_impl;
    OplOtaDataInSkip2 = Opu_OtaDataInSkip2_impl;
    OplOtaDataInBody = Opu_OtaDataInBody_impl;
    OplOtaProcessHeader = Opu_OtaProcessHeader_impl;
    OplOtaProcessBody = Opu_OtaProcessBody_impl;
    OplOtaComputeSum = Opu_OtaComputeSum_impl;
    opl_os_adapt_ota_start_inform = tuya_os_adapt_ota_start_inform_impl;
    opl_os_adapt_ota_data_process = tuya_os_adapt_ota_data_process_impl;
    opl_os_adapt_ota_end_inform = tuya_os_adapt_ota_end_inform_impl;
    return;
}


