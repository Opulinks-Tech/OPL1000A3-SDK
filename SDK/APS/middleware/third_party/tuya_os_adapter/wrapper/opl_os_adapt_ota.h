/******************************************************************************
*  Copyright 2017 - 2020, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2020
******************************************************************************/

#ifndef __OPL_OS_ADAPT_OTA_H__
#define __OPL_OS_ADAPT_OTA_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
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

typedef OPU_OTA_ERR (*T_OplOtaDataInFp)(uint8_t *data, int len);
typedef OPU_OTA_ERR (*T_OplOtaDataInSkip1Fp)(opu_ota_data_t *ota, uint8_t *data, int len);
typedef OPU_OTA_ERR (*T_OplOtaDataInHeaderFp)(opu_ota_data_t *ota, uint8_t *data, int len);
typedef OPU_OTA_ERR (*T_OplOtaDataInSkip2Fp)(opu_ota_data_t *ota, uint8_t *data, int len);
typedef OPU_OTA_ERR (*T_OplOtaDataInBodyFp)(opu_ota_data_t *ota, uint8_t *data, int len);
typedef OPU_OTA_ERR (*T_OplOtaProcessHeaderFp)(uint8_t *data, uint32_t *size, uint32_t *checksum);
typedef OPU_OTA_ERR (*T_OplOtaProcessBodyFp)(uint8_t *data, int len);
typedef void (*T_OplOtaComputeSumFp)(uint8_t *data, int len, uint32_t *checksum);
typedef int (*t_opl_os_adapt_ota_start_inform_fp)(unsigned int file_size);
typedef int (*t_opl_os_adapt_ota_data_process_fp)(const unsigned int total_len, const unsigned int offset,
                              const unsigned char* data, const unsigned int len, unsigned int* remain_len, void* pri_data);
typedef int (*t_opl_os_adapt_ota_end_inform_fp)(bool reset);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern T_OplOtaDataInFp OplOtaDataIn;
extern T_OplOtaDataInSkip1Fp OplOtaDataInSkip1;
extern T_OplOtaDataInHeaderFp OplOtaDataInHeader;
extern T_OplOtaDataInSkip2Fp OplOtaDataInSkip2;
extern T_OplOtaDataInBodyFp OplOtaDataInBody;
extern T_OplOtaProcessHeaderFp OplOtaProcessHeader;
extern T_OplOtaProcessBodyFp OplOtaProcessBody;
extern T_OplOtaComputeSumFp OplOtaComputeSum;
extern t_opl_os_adapt_ota_start_inform_fp opl_os_adapt_ota_start_inform;
extern t_opl_os_adapt_ota_data_process_fp opl_os_adapt_ota_data_process;
extern t_opl_os_adapt_ota_end_inform_fp opl_os_adapt_ota_end_inform;

// Sec 5: declaration of global function prototype
void opl_os_adapt_ota_pre_init(void);


#ifdef __cplusplus
}
#endif

#endif // __OPL_OS_ADAPT_OTA_H__
