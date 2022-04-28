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

#ifndef _TUYA_OS_ADAPT_BLE_GATT_H_
#define _TUYA_OS_ADAPT_BLE_GATT_H_

#include "le_host.h"


#define KNS_SERVICE_UUID				0x1910
#define KNS_SET_PARA_CHAR_UUID			0x2B11
#define KNS_KEY_CHAR_UUID				0x2B10


enum
{
    GATT_IDX_SVC,
	GATT_IDX_SERVICE_CHANGE_CHAR,
	GATT_IDX_SERVICE_CHANGE_VAL,
	GATT_IDX_SERVICE_CHANGE_CFG,
    GATT_IDX_TOP
};

enum
{
    GAP_IDX_SVC,

	GAP_IDX_DEVICE_NAME_CHAR,
	GAP_IDX_DEVICE_NAME_VAL,

	GAP_IDX_APPEARANCE_CHAR,
	GAP_IDX_APPEARANCE_VAL,

	GAP_IDX_CONN_PARAM_CHAR,
	GAP_IDX_CONN_PARAM_VAL,

    GAP_IDX_TOP
};

enum
{
    KNS_IDX_SVC,
        
	KNS_IDX_SET_PARA_CHAR,
	KNS_IDX_SET_PARA_VAL,
        
	KNS_IDX_KEY_CHAR,
	KNS_IDX_KEY_VAL,
	KNS_IDX_KEY_CFG,

    KNS_IDX_TOP
};


void BleAppGattMsgHandler(TASK task, MESSAGEID id, MESSAGE message);

#endif
