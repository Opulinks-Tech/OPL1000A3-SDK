/******************************************************************************
*  Copyright 2017 - 2021, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2021
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  mw_ota_http.h
*
*  Project:
*  --------
*  OPL1000 Project - the Over The Air (OTA) HTTP definition file
*
*  Description:
*  ------------
*  This include file is the Over The Air (OTA) HTTP definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _MW_OTA_HTTP_H_
#define _MW_OTA_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
//#include "mw_ota_def.h"
#include "httpclient.h"
#include "cmsis_os.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define MW_OTA_HTTP_TASK_NAME           "MW_OTA_HTTP"
#define MW_OTA_HTTP_TASK_STACK_SIZE     (1024)
#define MW_OTA_HTTP_TASK_PRIORITY       (osPriorityNormal)
#define MW_OTA_HTTP_QUEUE_SIZE          (20)

#define MW_OTA_HTTP_DATA_BUF_SIZE       (1024 + 1)
#define MW_OTA_HTTP_URL_BUF_LEN         (256)

#define MW_OTA_HTTP_SOCKET_TIMEOUT      (30000)     // 30s


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
typedef enum
{
    MW_OTA_HTTP_RET_SUCCESS = 0,
    MW_OTA_HTTP_RET_FAILURE = 1,
    MW_OTA_HTTP_RET_OTA_ON  = 2
} mw_ota_http_ret_type_e;

typedef enum
{
    MW_OTA_HTTP_MSG_TRIG           = 0,
    MW_OTA_HTTP_MSG_SERVER_VERSION = 1
} mw_ota_http_msg_type_e;

typedef struct
{
    uint32_t event;
    uint32_t length;
    uint8_t ucaMessage[];
} xMwOtaHttpMessage_t;

typedef void (*T_MwOta_Http_StatusCb_Fp)(int32_t i32Status);
typedef void (*T_MwOta_Http_ServerVersionCb_Fp)(int32_t i32Status, uint16_t uwProjectId, uint16_t uwChipId, uint16_t uwFirmwareId);

typedef void (*T_MwOta_Http_Init_Fp)(void);
typedef int (*T_MwOta_Http_MsgSend_Fp)(uint32_t msg_type, uint8_t *data, uint32_t data_len);
typedef void (*T_MwOta_Http_RegisterStatusCb_Fp)(T_MwOta_Http_StatusCb_Fp tFunc);
typedef void (*T_MwOta_Http_RegisterServerVersionCb_Fp)(T_MwOta_Http_ServerVersionCb_Fp tFunc);

typedef void (*T_MwOta_Http_Task_Fp)(void *args);
typedef void (*T_MwOTa_Http_Task_Evt_Handler_Fp)(uint32_t msg_type, uint8_t *data, uint32_t data_len);
typedef int (*T_MwOTa_Http_retrieve_offset_Fp)(httpclient_t *client, httpclient_data_t *client_data, int offset, int parse_hdr);
typedef int (*T_MwOTa_Http_retrieve_offset_server_version_Fp)(httpclient_t *client, httpclient_data_t *client_data, int offset, int parse_hdr, uint16_t *puwProjectId, uint16_t *puwChipId, uint16_t *puwFirmwareId);
typedef int (*T_MwOTa_Http_retrieve_get_Fp)(char* get_url, char* buf, uint32_t len);
typedef int (*T_MwOTa_Http_retrieve_get_server_version_Fp)(char* get_url, char* buf, uint32_t len, uint16_t *puwProjectId, uint16_t *puwChipId, uint16_t *puwFirmwareId);
typedef int (*T_MwOta_Http_DownloadByUrl_Fp)(char *url);
typedef int (*T_MwOta_Http_GetServerVersion_Fp)(char *url, uint16_t *puwProjectId, uint16_t *puwChipId, uint16_t *puwFirmwareId);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
extern T_MwOta_Http_Init_Fp MwOta_Http_Init;
extern T_MwOta_Http_MsgSend_Fp MwOta_Http_MsgSend;
extern T_MwOta_Http_RegisterStatusCb_Fp MwOta_Http_RegisterStatusCb;
extern T_MwOta_Http_RegisterServerVersionCb_Fp MwOta_Http_RegisterServerVersionCb;

// internal part
extern T_MwOta_Http_Task_Fp MwOta_Http_Task;
extern T_MwOTa_Http_Task_Evt_Handler_Fp MwOTa_Http_Task_Evt_Handler;
extern T_MwOTa_Http_retrieve_offset_Fp MwOTa_Http_retrieve_offset;
extern T_MwOTa_Http_retrieve_offset_server_version_Fp MwOTa_Http_retrieve_offset_server_version;
extern T_MwOTa_Http_retrieve_get_Fp MwOTa_Http_retrieve_get;
extern T_MwOTa_Http_retrieve_get_server_version_Fp MwOTa_Http_retrieve_get_server_version;
extern T_MwOta_Http_DownloadByUrl_Fp MwOta_Http_DownloadByUrl;
extern T_MwOta_Http_GetServerVersion_Fp MwOta_Http_GetServerVersion;

void MwOta_HttpPreInitCold(void);


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_OTA_HTTP_H_
