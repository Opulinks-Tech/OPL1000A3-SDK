/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _TUYA_OS_ADAPT_BLE_APP_H_
#define _TUYA_OS_ADAPT_BLE_APP_H_

#include "cmsis_os.h"
#include "le_queue.h"
#include "le.h"
#include "le_host.h"
#include "msg.h"
#include "tuya_os_adapter_error_code.h"


#define ENABLE_APP_DEBUG

#ifdef ENABLE_APP_DEBUG
#define BLE_APP_PRINT		printf
#else
#define BLE_APP_PRINT(...)
#endif

#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     	80
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     	80
#define DEFAULT_DESIRED_SLAVE_LATENCY			0
#define DEFAULT_DESIRED_SUPERVERSION_TIMEOUT   	300


enum
{
	APP_STATE_INIT,
	APP_STATE_IDLE,
	APP_STATE_ADVERTISING,
	APP_STATE_CONNECTED,
	APP_STATE_DISCONNECTING,
};

enum
{
	TUYA_BLE_STATE_INIT,
	TUYA_BLE_STATE_DEINIT,
	TUYA_BLE_STATE_IDLE,
	TUYA_BLE_STATE_START,
	TUYA_BLE_STATE_CONNECTED,
	TUYA_BLE_STATE_DISCONNECTING,
};

enum
{
	BLE_APP_MSG_TUYA_INIT = 1,
	BLE_APP_MSG_TUYA_START,
	BLE_APP_MSG_TUYA_DEINIT,
	BLE_APP_MSG_TUYA_SEND_DATA,
	BLE_APP_MSG_TUYA_GAP_DISCONNECT,
	BLE_APP_MSG_TUYA_GET_RSSI,

	BLE_APP_MSG_TOP
};

typedef struct
{
	UINT16			adv_len;
	UINT8			adv_data[31];
	UINT16			scan_len;
	UINT8			scan_data[31];
	void			*cb;
} TUYA_BLE_INIT_T;

typedef struct
{
	UINT16			len;
	UINT8			*data;
} TUYA_BLE_SEND_T;

typedef struct
{
	UINT16			handle[4];
    UINT32			evt;
	UINT16			len;
	UINT8			*data;
} TUYA_CB_MSG_T;

typedef struct
{
	TASKPACK		task;
	UINT16			state;
	UINT16			conn_hdl;
	LE_BT_ADDR_T	bt_addr;
    
	UINT16			curr_mtu;
	BOOL			encrypted;
    BOOL			paired;

    UINT16			min_itvl;
    UINT16			max_itvl;
    UINT16			latency;
    UINT16			sv_tmo;

	osThreadId 		cb_task_hdl;
	osSemaphoreId 	cb_sema_id;
	LE_QLIST_T		cb_evt_q;
    
	osSemaphoreId 	api_sema_id;
	osSemaphoreId 	rst_sema_id;
	UINT16			tuya_state;
    UINT16			curr_tuya_evt;
    void			*tuya_cb;
	INT32			api_rst;
	INT8			conn_rssi;
    
	UINT8			adv_len;
	UINT8			scan_len;
	UINT8			adv_data[31];
	UINT8			scan_data[31];

	UINT16			*send_cfg;
	UINT16			send_hdl;
	UINT16			send_len;
	UINT16			send_idx;
	UINT8			*send_data;
    
} BLE_APP_DATA_T;


BOOL BleAppInit(void);

void BleAppApiSemaCheck(void);

void BleAppApiSemaRelease(void);

void BleAppWaitApiResult(void);

void BleAppRstSemaRelease(void);

void BleAppCbSemaCheck(BOOL wait);

void BleAppCbNotify(UINT32 evt, UINT16 len, void *data);

void BleAppStopApiEvt(INT32 rst);

BLE_APP_DATA_T* BleAppDev(void);

#endif
