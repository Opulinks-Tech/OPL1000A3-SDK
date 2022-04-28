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
*  mw_ota.c
*
*  Project:
*  --------
*  OPL1000 Project - the Over The Air (OTA) HTTP implement file
*
*  Description:
*  ------------
*  This implement file is include the Over The Air (OTA) HTTP function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdlib.h>
#include <string.h>
#include "mw_ota.h"
#include "mw_ota_http.h"
#include "httpclient.h"
#include "lwip/etharp.h"

#define ENABLE_OTA_DBG
//#define ENABLE_OTA_ERR

#ifdef ENABLE_OTA_DBG
#define OTA_DBG(...)    \
	{\
	printf(__VA_ARGS__); \
	}
#else
#define OTA_DBG(...)
#endif


#ifdef ENABLE_OTA_ERR
#define OTA_ERR(...)    \
	{\
	printf(__VA_ARGS__); \
	}
#else
#define OTA_ERR(...)
#endif

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
RET_DATA osThreadId   g_tMwOtaHttpTaskId;
RET_DATA osMessageQId g_tMwOtaHttpQueueId;
RET_DATA T_MwOta_Http_StatusCb_Fp g_tMwOtaHttpStatusCb;
RET_DATA T_MwOta_Http_ServerVersionCb_Fp g_tMwOtaHttpServerVersionCb;

RET_DATA httpclient_t g_tMwOtaHttpClient;


// Sec 5: declaration of global function prototype
RET_DATA T_MwOta_Http_Init_Fp MwOta_Http_Init;
RET_DATA T_MwOta_Http_MsgSend_Fp MwOta_Http_MsgSend;
RET_DATA T_MwOta_Http_RegisterStatusCb_Fp MwOta_Http_RegisterStatusCb;
RET_DATA T_MwOta_Http_RegisterServerVersionCb_Fp MwOta_Http_RegisterServerVersionCb;

// internal part
RET_DATA T_MwOta_Http_Task_Fp MwOta_Http_Task;
RET_DATA T_MwOTa_Http_Task_Evt_Handler_Fp MwOTa_Http_Task_Evt_Handler;
RET_DATA T_MwOTa_Http_retrieve_offset_Fp MwOTa_Http_retrieve_offset;
RET_DATA T_MwOTa_Http_retrieve_offset_server_version_Fp MwOTa_Http_retrieve_offset_server_version;
RET_DATA T_MwOTa_Http_retrieve_get_Fp MwOTa_Http_retrieve_get;
RET_DATA T_MwOTa_Http_retrieve_get_server_version_Fp MwOTa_Http_retrieve_get_server_version;
RET_DATA T_MwOta_Http_DownloadByUrl_Fp MwOta_Http_DownloadByUrl;
RET_DATA T_MwOta_Http_GetServerVersion_Fp MwOta_Http_GetServerVersion;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   MwOta_Http_StatusCb_impl
*
* DESCRIPTION:
*   it is the dummy function, to update the status of OTA HTTP task by application layer
*
* PARAMETERS
*   1. i32Status : [In] the current status
*
* RETURNS
*   none
*
*************************************************************************/
void MwOta_Http_StatusCb_impl(int32_t i32Status)
{
}

/*************************************************************************
* FUNCTION:
*   MwOta_Http_ServerVersionCb_impl
*
* DESCRIPTION:
*   it is the dummy function, to update the server version of OTA HTTP task by application layer
*
* PARAMETERS
*   1. i32Status    : [In] the current status
*   2. uwProjectId  : [In] the project ID
*   3. uwChipId     : [In] the chip ID
*   4. uwFirmwareId : [In] the firmware ID
*
* RETURNS
*   none
*
*************************************************************************/
void MwOta_Http_ServerVersionCb_impl(int32_t i32Status, uint16_t uwProjectId, uint16_t uwChipId, uint16_t uwFirmwareId)
{
}

/*************************************************************************
* FUNCTION:
*   MwOta_Http_Init_impl
*
* DESCRIPTION:
*   OTA HTTP init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwOta_Http_Init_impl(void)
{
    osMessageQDef_t queue_def;
    osThreadDef_t task_def;

    /* Create OTA HTTP message queue*/
    queue_def.item_sz = sizeof(xMwOtaHttpMessage_t);
    queue_def.queue_sz = MW_OTA_HTTP_QUEUE_SIZE;
    g_tMwOtaHttpQueueId = osMessageCreate(&queue_def, NULL);
    if(g_tMwOtaHttpQueueId == NULL)
    {
		OTA_ERR("[OTA] msg queue create fail\r\n");
    }

    /* Create OTA HTTP task */
    task_def.name = MW_OTA_HTTP_TASK_NAME;
    task_def.stacksize = MW_OTA_HTTP_TASK_STACK_SIZE;
    task_def.tpriority = MW_OTA_HTTP_TASK_PRIORITY;
    task_def.pthread = MwOta_Http_Task;

    g_tMwOtaHttpTaskId = osThreadCreate(&task_def, (void*)NULL);
    if(g_tMwOtaHttpTaskId == NULL)
    {
        OTA_ERR("[OTA] task create fail\r\n");
    }
}

/*************************************************************************
* FUNCTION:
*   MwOta_Http_MsgSend_impl
*
* DESCRIPTION:
*   send the message to OTA HTTP task
*
* PARAMETERS
*   1. msg_type : [In] the message type
*   2. data     : [In] the data buffer pointer
*   3. data_len : [In] the length of data buffer
*
* RETURNS
*   0  : success
*   -1 : fail
*
*************************************************************************/
int MwOta_Http_MsgSend_impl(uint32_t msg_type, uint8_t *data, uint32_t data_len)
{
    int iRet = -1;
    xMwOtaHttpMessage_t *pMsg = NULL;

    /* Mem allocate */
    pMsg = malloc(sizeof(xMwOtaHttpMessage_t) + data_len);
    if (pMsg == NULL)
    {
        OTA_ERR("[OTA] task msg allocate fail\r\n");
        goto done;
    }

    pMsg->event = msg_type;
    pMsg->length = data_len;
    if (data_len > 0)
    {
        memcpy(pMsg->ucaMessage, data, data_len);
    }

    if (osMessagePut(g_tMwOtaHttpQueueId, (uint32_t)pMsg, 0) != osOK)
    {
        OTA_ERR("[OTA] task msg send fail \r\n");
        goto done;
    }

    iRet = 0;

done:
    if(iRet)
    {
        if (pMsg != NULL)
        {
            free(pMsg);
        }
    }

    return iRet;
}

/*************************************************************************
* FUNCTION:
*   MwOta_Http_RegisterStatusCb_impl
*
* DESCRIPTION:
*   register the callback function for update the status of OTA HTTP task
*
* PARAMETERS
*   1. tFunc : [In] the callback function
*
* RETURNS
*   none
*
*************************************************************************/
void MwOta_Http_RegisterStatusCb_impl(T_MwOta_Http_StatusCb_Fp tFunc)
{
    if (tFunc != NULL)
        g_tMwOtaHttpStatusCb = tFunc;
}

/*************************************************************************
* FUNCTION:
*   MwOta_Http_RegisterServerVersionCb_impl
*
* DESCRIPTION:
*   register the callback function for update the server version
*
* PARAMETERS
*   1. tFunc : [In] the callback function
*
* RETURNS
*   none
*
*************************************************************************/
void MwOta_Http_RegisterServerVersionCb_impl(T_MwOta_Http_ServerVersionCb_Fp tFunc)
{
    if (tFunc != NULL)
        g_tMwOtaHttpServerVersionCb = tFunc;
}

/*************************************************************************
* FUNCTION:
*   MwOta_Http_Task_impl
*
* DESCRIPTION:
*   the OTA HTTP task
*
* PARAMETERS
*   1. args : [In] the task args
*
* RETURNS
*   none
*
*************************************************************************/
void MwOta_Http_Task_impl(void *args)
{
    osEvent rxEvent;
    xMwOtaHttpMessage_t *rxMsg;

    for(;;)
    {
        /* Wait event */
        rxEvent = osMessageGet(g_tMwOtaHttpQueueId, osWaitForever);
        if(rxEvent.status != osEventMessage)
            continue;

        rxMsg = (xMwOtaHttpMessage_t *)rxEvent.value.p;
        MwOTa_Http_Task_Evt_Handler(rxMsg->event, rxMsg->ucaMessage, rxMsg->length);

        /* Release buffer */
        if (rxMsg != NULL)
            free(rxMsg);
    }
}

/*************************************************************************
* FUNCTION:
*   MwOTa_Http_Task_Evt_Handler_impl
*
* DESCRIPTION:
*   handle the message of OTA HTTP task
*
* PARAMETERS
*   1. msg_type : [In] the message type
*   2. data     : [In] the data buffer pointer
*   3. data_len : [In] the length of data buffer
*
* RETURNS
*   none
*
*************************************************************************/
void MwOTa_Http_Task_Evt_Handler_impl(uint32_t msg_type, uint8_t *data, uint32_t data_len)
{
    uint16_t pid;
    uint16_t cid;
    uint16_t fid;

    switch (msg_type)
    {
        case MW_OTA_HTTP_MSG_TRIG:
            g_tMwOtaHttpStatusCb(MW_OTA_HTTP_RET_OTA_ON);

            lwip_one_shot_arp_enable();

            if (MwOta_Http_DownloadByUrl((char*)data) != 0)
            {
                g_tMwOtaHttpStatusCb(MW_OTA_HTTP_RET_FAILURE);
            }
            else
            {
                g_tMwOtaHttpStatusCb(MW_OTA_HTTP_RET_SUCCESS);
            }
            break;

        case MW_OTA_HTTP_MSG_SERVER_VERSION:
            if (MwOta_Http_GetServerVersion((char*)data, &pid, &cid, &fid) != 0)
            {
                g_tMwOtaHttpServerVersionCb(MW_OTA_HTTP_RET_FAILURE, 0, 0, 0);
            }
            else
            {
                g_tMwOtaHttpServerVersionCb(MW_OTA_HTTP_RET_SUCCESS, pid, cid, fid);
            }
            break;

        default:
            break;
    }
}

/*************************************************************************
* FUNCTION:
*   MwOTa_Http_retrieve_offset_impl
*
* DESCRIPTION:
*   retrieve the server version
*
* PARAMETERS
*   1. client        : [In] the http client
*   2. client_data   : [In] the data of http client
*   3. offset        : [In] the offset address of data
*   4. parse_hdr     : [In] want to parse header or not
*
* RETURNS
*   HTTPCLIENT_OK : success
*   others        : fail
*
*************************************************************************/
int MwOTa_Http_retrieve_offset_impl(httpclient_t *client, httpclient_data_t *client_data, int offset, int parse_hdr)
{
    int ret = HTTPCLIENT_ERROR_CONN;
    uint8_t ota_hdr1_start = 0;
    T_MwOtaFlashHeader *ota_hdr = NULL;

    ota_hdr1_start = parse_hdr;

    do
    {
        // Receive response from server
        ret = httpclient_recv_response(client, client_data);
        if (ret < 0)
        {
            OTA_ERR("[OTA] client recv response error, ret = %d, errno = %d \r\n", ret, errno);
            return ret;
        }

        if (ota_hdr1_start == 1)
        {
            ota_hdr = (T_MwOtaFlashHeader*)client_data->response_buf;
            OTA_DBG("[OTA] proj_id=%d, chip_id=%d, fw_id=%d, checksum=%d, total_len=%d\r\n",
                  ota_hdr->uwProjectId, ota_hdr->uwChipId, ota_hdr->uwFirmwareId, ota_hdr->ulImageSum, ota_hdr->ulImageSize);

            if (MwOta_Prepare(ota_hdr->uwProjectId, ota_hdr->uwChipId, ota_hdr->uwFirmwareId, ota_hdr->ulImageSize, ota_hdr->ulImageSum) != MW_OTA_OK)
            {
                OTA_ERR("[OTA] ota_prepare fail\r\n");
                return -1;
            }
            ota_hdr1_start = 0;
        }

        if ( (client_data->response_content_len - client_data->retrieve_len) == offset )
            break;

    }
    while (ret == HTTPCLIENT_RETRIEVE_MORE_DATA);

    return HTTPCLIENT_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOTa_Http_retrieve_offset_server_version_impl
*
* DESCRIPTION:
*   retrieve the server version
*
* PARAMETERS
*   1. client        : [In] the http client
*   2. client_data   : [In] the data of http client
*   3. offset        : [In] the offset address of data
*   4. parse_hdr     : [In] want to parse header or not
*   5. puwProjectId  : [Out] the project ID
*   6. puwChipId     : [Out] the chip ID
*   7. puwFirmwareId : [Out] the firmware ID
*
* RETURNS
*   HTTPCLIENT_OK : success
*   others        : fail
*
*************************************************************************/
int MwOTa_Http_retrieve_offset_server_version_impl(httpclient_t *client, httpclient_data_t *client_data, int offset, int parse_hdr, uint16_t *puwProjectId, uint16_t *puwChipId, uint16_t *puwFirmwareId)
{
    int ret = HTTPCLIENT_ERROR_CONN;
    uint8_t ota_hdr1_start = 0;
    T_MwOtaFlashHeader *ota_hdr = NULL;

    ota_hdr1_start = parse_hdr;

    do
    {
        // Receive response from server
        ret = httpclient_recv_response(client, client_data);
        if (ret < 0)
        {
            OTA_ERR("[OTA] client recv response error, ret = %d, errno = %d \r\n", ret, errno);
            return ret;
        }

        if (ota_hdr1_start == 1)
        {
            ota_hdr = (T_MwOtaFlashHeader*)client_data->response_buf;
            OTA_DBG("[OTA] proj_id=%d, chip_id=%d, fw_id=%d, checksum=%d, total_len=%d\r\n",
                  ota_hdr->uwProjectId, ota_hdr->uwChipId, ota_hdr->uwFirmwareId, ota_hdr->ulImageSum, ota_hdr->ulImageSize);

            ota_hdr1_start = 0;

            *puwProjectId = ota_hdr->uwProjectId;
            *puwChipId = ota_hdr->uwChipId;
            *puwFirmwareId = ota_hdr->uwFirmwareId;
        }

        if ( (client_data->response_content_len - client_data->retrieve_len) == offset )
            break;

    }
    while (ret == HTTPCLIENT_RETRIEVE_MORE_DATA);

    return HTTPCLIENT_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOTa_Http_retrieve_get_impl
*
* DESCRIPTION:
*   retrieve the new data from cloud
*
* PARAMETERS
*   1. get_url : [In] the http url
*   2. buf     : [In] the data buffer
*   3. len     : [In] the size of data buffer
*
* RETURNS
*   HTTPCLIENT_OK : success
*   others        : fail
*
*************************************************************************/
int MwOTa_Http_retrieve_get_impl(char* get_url, char* buf, uint32_t len)
{
    int ret = HTTPCLIENT_ERROR_CONN;
    httpclient_data_t client_data = {0};
    uint32_t write_count = 0;
    uint32_t data_recv = 0;
    uint32_t recv_temp = 0;
    uint32_t data_len = 0;

    client_data.response_buf = buf;
    client_data.response_buf_len = len;

    // Send request to server
    ret = httpclient_send_request(&g_tMwOtaHttpClient, get_url, HTTPCLIENT_GET, &client_data);
    if (ret < 0)
    {
        OTA_ERR("[OTA] client fail to send request, errno = %d \r\n", errno);
        return ret;
    }

    // Response body start

    // skip 2nd boot agent,  0x00000000 ~ 0x00003000 : 12 KB,
    ret = MwOTa_Http_retrieve_offset(&g_tMwOtaHttpClient, &client_data, MW_OTA_HEADER_ADDR_1, 0);
    if (ret < 0)
    {
        OTA_ERR("[OTA] retrieve offset error, ret = %d, errno = %d \r\n", ret, errno);
        return ret;
    }

    // parse 1st OTA header, 0x00003000 ~ 0x00004000 : 4 KB
    ret = MwOTa_Http_retrieve_offset(&g_tMwOtaHttpClient, &client_data, MW_OTA_HEADER_ADDR_2, 1);
    if (ret < 0)
    {
        OTA_ERR("[OTA] retrieve offset error, ret = %d, errno = %d \r\n", ret, errno);
        return ret;
    }

    // skip 2st OTA header,  0x00004000 ~ 0x00005000 : 4 KB
    ret = MwOTa_Http_retrieve_offset(&g_tMwOtaHttpClient, &client_data, MW_OTA_IMAGE_ADDR_1, 0);
    if (ret < 0)
    {
        OTA_ERR("[OTA] retrieve offset error, ret = %d, errno = %d \r\n", ret, errno);
        return ret;
    }

    recv_temp = client_data.retrieve_len;
    data_recv = client_data.response_content_len - client_data.retrieve_len;
    do
    {
        // Receive response from server
        ret = httpclient_recv_response(&g_tMwOtaHttpClient, &client_data);

        if (ret < 0)
        {
            OTA_ERR("[OTA] client recv response error, ret = %d, errno = %d \r\n", ret, errno);
            return ret;
        }

        data_len = recv_temp - client_data.retrieve_len;
        if (MwOta_DataIn((uint8_t*)client_data.response_buf, data_len) != MW_OTA_OK)
        {
            OTA_ERR("[OTA] MwOta_DataIn fail\r\n");
            return -1;
        }

        write_count += data_len;
        data_recv += data_len;
        recv_temp = client_data.retrieve_len;
        OTA_DBG("[OTA] have written image length %d\r\n", write_count);
        OTA_DBG("[OTA] download progress = %u \r\n", data_recv * 100 / client_data.response_content_len);

    }
    while (ret == HTTPCLIENT_RETRIEVE_MORE_DATA);

    OTA_DBG("[OTA] total write data length : %d\r\n", write_count);
    OTA_DBG("[OTA] data received length : %d\r\n", data_recv);

    if (data_recv != client_data.response_content_len || httpclient_get_response_code(&g_tMwOtaHttpClient) != 200)
    {
        OTA_ERR("[OTA] data received not completed, or invalid error code \r\n");
        return -1;
    }
    else if (data_recv == 0)
    {
        OTA_ERR("[OTA] receive length is zero, file not found \n");
        return -2;
    }
    else
    {
        OTA_DBG("[OTA] download success \n");
        return ret;
    }
}

/*************************************************************************
* FUNCTION:
*   MwOTa_Http_retrieve_get_server_version_impl
*
* DESCRIPTION:
*   get the server version
*
* PARAMETERS
*   1. get_url       : [In] the http url
*   2. buf           : [In] the data buffer
*   3. len           : [In] the size of data buffer
*   4. puwProjectId  : [Out] the project ID
*   5. puwChipId     : [Out] the chip ID
*   6. puwFirmwareId : [Out] the firmware ID
*
* RETURNS
*   HTTPCLIENT_OK : success
*   others        : fail
*
*************************************************************************/
int MwOTa_Http_retrieve_get_server_version_impl(char* get_url, char* buf, uint32_t len, uint16_t *puwProjectId, uint16_t *puwChipId, uint16_t *puwFirmwareId)
{
    int ret = HTTPCLIENT_ERROR_CONN;
    httpclient_data_t client_data = {0};
    char *header = NULL;

    client_data.response_buf = buf;
    client_data.response_buf_len = len;

    // Requesting a specific range from a server
    // parse 1st OTA header, 0x00003000 ~ 0x00003FFF : 4 KB
    header = "Range: bytes=12288-16383\r\n";
    httpclient_set_custom_header(&g_tMwOtaHttpClient, header);

    // Send request to server
    ret = httpclient_send_request(&g_tMwOtaHttpClient, get_url, HTTPCLIENT_GET, &client_data);
    if (ret < 0)
    {
        OTA_ERR("[OTA] client fail to send request, errno = %d \r\n", errno);
        return ret;
    }

    // Response body start
    ret = MwOTa_Http_retrieve_offset_server_version(&g_tMwOtaHttpClient, &client_data, 0x1000, 1, puwProjectId, puwChipId, puwFirmwareId);
    if (ret < 0)
    {
        OTA_ERR("[OTA] retrieve offset error, ret = %d, errno = %d \r\n", ret, errno);
        return ret;
    }

    return ret;
}

/*************************************************************************
* FUNCTION:
*   MwOta_Http_DownloadByUrl_impl
*
* DESCRIPTION:
*   download the new firmware by http url
*
* PARAMETERS
*   1. url : [In] the http url
*
* RETURNS
*   0  : success
*   -1 : fail
*
*************************************************************************/
int MwOta_Http_DownloadByUrl_impl(char *url)
{
    char get_url[MW_OTA_HTTP_URL_BUF_LEN];
    int32_t ret = HTTPCLIENT_ERROR_CONN;
    uint32_t len_param = strlen(url);
    uint16_t retry_count = 0;
    uint16_t pid;
    uint16_t cid;
    uint16_t fid;

    if (len_param < 1)
    {
        return -1;
    }

    memset(get_url, 0, MW_OTA_HTTP_URL_BUF_LEN);
    OTA_DBG("[OTA] URL: %s\r\n", url);
    strcpy(get_url, url);

    char* buf = malloc(MW_OTA_HTTP_DATA_BUF_SIZE);
    if (buf == NULL)
    {
        OTA_ERR("[OTA] buf malloc failed.\r\n");
        return -1;
    }

    lwip_auto_arp_enable(1, 0);

    // Connect to server
    do
    {
        ret = httpclient_connect(&g_tMwOtaHttpClient, get_url);
        if (!ret)
        {
            OTA_DBG("[OTA] connect to http server\r\n");
            g_tMwOtaHttpClient.timeout_ms = MW_OTA_HTTP_SOCKET_TIMEOUT;
            break;
        }
        else
        {
            OTA_DBG("[OTA] connect to http server failed! retry again, errno = %d\r\n", errno);
            osDelay(1000);
            retry_count++;
            continue;
        }
    }
    while (retry_count < 3);

    if (retry_count == 3)
        goto do_exit;

    MwOta_VersionGet(&pid, &cid, &fid);

    OTA_DBG("[OTA] device version: pid=%d, cid=%d, fid=%d\r\n", pid, cid, fid);

    ret = MwOTa_Http_retrieve_get(get_url, buf, MW_OTA_HTTP_DATA_BUF_SIZE);
    if (ret < 0)
    {
        if (MwOta_DataGiveUp() != MW_OTA_OK)
        {
            OTA_DBG("[OTA] ota_abort error.\r\n");
        }
    }
    else
    {
        if (MwOta_DataFinish() != MW_OTA_OK)
        {
            OTA_DBG("[OTA] ota_data_finish error.\r\n");
        }
    }

do_exit:
    OTA_DBG("[OTA] download result = %d\r\n", (int)ret);

    // Close http connection
    httpclient_close(&g_tMwOtaHttpClient);
    free(buf);
    buf = NULL;

    if (HTTPCLIENT_OK == ret)
        return 0;
    else
        return -1;
}

/*************************************************************************
* FUNCTION:
*   MwOta_Http_GetServerVersion_impl
*
* DESCRIPTION:
*   get the server version by http url
*
* PARAMETERS
*   1. get_url       : [In] the http url
*   2. puwProjectId  : [Out] the project ID
*   3. puwChipId     : [Out] the chip ID
*   4. puwFirmwareId : [Out] the firmware ID
*
* RETURNS
*   0  : success
*   -1 : fail
*
*************************************************************************/
int MwOta_Http_GetServerVersion_impl(char *url, uint16_t *puwProjectId, uint16_t *puwChipId, uint16_t *puwFirmwareId)
{
    char get_url[MW_OTA_HTTP_URL_BUF_LEN];
    int32_t ret = HTTPCLIENT_ERROR_CONN;
    uint32_t len_param = strlen(url);
    uint16_t retry_count = 0;

    if (len_param < 1)
    {
        return -1;
    }

    memset(get_url, 0, MW_OTA_HTTP_URL_BUF_LEN);
    OTA_DBG("[OTA] URL: %s\r\n", url);
    strcpy(get_url, url);

    char* buf = malloc(MW_OTA_HTTP_DATA_BUF_SIZE);
    if (buf == NULL)
    {
        OTA_ERR("[OTA] buf malloc failed.\r\n");
        return -1;
    }

    lwip_auto_arp_enable(1, 0);

    // Connect to server
    do
    {
        ret = httpclient_connect(&g_tMwOtaHttpClient, get_url);
        if (!ret)
        {
            OTA_DBG("[OTA] connect to http server\r\n");
            g_tMwOtaHttpClient.timeout_ms = MW_OTA_HTTP_SOCKET_TIMEOUT;
            break;
        }
        else
        {
            OTA_DBG("[OTA] connect to http server failed! retry again, errno = %d\r\n", errno);
            osDelay(1000);
            retry_count++;
            continue;
        }
    }
    while (retry_count < 3);

    if (retry_count == 3)
        goto do_exit;

    ret = MwOTa_Http_retrieve_get_server_version(get_url, buf, MW_OTA_HTTP_DATA_BUF_SIZE, puwProjectId, puwChipId, puwFirmwareId);

do_exit:
    OTA_DBG("[OTA] result = %d\r\n", (int)ret);

    // Close http connection
    httpclient_close(&g_tMwOtaHttpClient);
    free(buf);
    buf = NULL;

    if (HTTPCLIENT_OK == ret)
        return 0;
    else
        return -1;
}


/*************************************************************************
* FUNCTION:
*   MwOta_HttpPreInitCold
*
* DESCRIPTION:
*   the previous initial for cold boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwOta_HttpPreInitCold(void)
{
    g_tMwOtaHttpStatusCb        = MwOta_Http_StatusCb_impl;
    g_tMwOtaHttpServerVersionCb = MwOta_Http_ServerVersionCb_impl;

    memset(&g_tMwOtaHttpClient, 0, sizeof(g_tMwOtaHttpClient));

    MwOta_Http_Init                    = MwOta_Http_Init_impl;
    MwOta_Http_MsgSend                 = MwOta_Http_MsgSend_impl;
    MwOta_Http_RegisterStatusCb        = MwOta_Http_RegisterStatusCb_impl;
    MwOta_Http_RegisterServerVersionCb = MwOta_Http_RegisterServerVersionCb_impl;

    MwOta_Http_Task             = MwOta_Http_Task_impl;
    MwOTa_Http_Task_Evt_Handler = MwOTa_Http_Task_Evt_Handler_impl;

    MwOTa_Http_retrieve_offset  = MwOTa_Http_retrieve_offset_impl;
    MwOTa_Http_retrieve_offset_server_version = MwOTa_Http_retrieve_offset_server_version_impl;
    MwOTa_Http_retrieve_get = MwOTa_Http_retrieve_get_impl;
    MwOTa_Http_retrieve_get_server_version = MwOTa_Http_retrieve_get_server_version_impl;
    MwOta_Http_DownloadByUrl    = MwOta_Http_DownloadByUrl_impl;
    MwOta_Http_GetServerVersion = MwOta_Http_GetServerVersion_impl;
}
