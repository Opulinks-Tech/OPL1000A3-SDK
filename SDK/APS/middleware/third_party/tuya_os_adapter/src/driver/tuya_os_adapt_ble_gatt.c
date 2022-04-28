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

#include "le_gap_if.h"
#include "le_gatt_if.h"
#include "le_uuid.h"
#include "tuya_os_adapt_ble_app.h"
#include "tuya_os_adapt_ble_gatt.h"
#include "tuya_os_adapt_bt.h"
#include "opl_os_adapt_ble_gatt.h"


#define BLE_DEVICE_NAME					"TY"


// This is used for GATT service
static UINT16 gGattSvcUuid = ATT_SVC_GENERIC_ATTRIBUTE;

static UINT16 gGattServiceChangeUuid      = ATT_CHAR_SERVICE_CHANGED;
static UINT8  gGattServiceChangeCharVal[] = CHAR_DECL_UUID16_ATTR_VAL(LE_GATT_CHAR_PROP_IND, ATT_CHAR_SERVICE_CHANGED);
static UINT8  gGattServiceChangeVal[4]    = {0, 0, 0, 0};
static UINT16 gGattServiceChangeClientCfg = 0;

// This is used for GAP service 
static UINT16 gGapSvcUuid = ATT_SVC_GENERIC_ACCESS;

static UINT16 gGapDeviceNameUuid          = ATT_CHAR_DEVICE_NAME;
static UINT8  gGapDeviceNameCharVal[]     = CHAR_DECL_UUID16_ATTR_VAL(LE_GATT_CHAR_PROP_RD, ATT_CHAR_DEVICE_NAME);
static UINT8  gGapDeviceNameVal[31]       = BLE_DEVICE_NAME;

static UINT16 gGapAppearanceUuid          = ATT_CHAR_APPEARANCE;
static UINT8  gGapAppearanceCharVal[]     = CHAR_DECL_UUID16_ATTR_VAL(LE_GATT_CHAR_PROP_RD, ATT_CHAR_APPEARANCE);
static UINT8  gGapAppearanceVal[2]        = {0, 0};

static UINT16 gGapConnParamUuid           = ATT_CHAR_PERIPH_PREF_CON_PARAM;
static UINT8  gGapConnParamCharVal[]      = CHAR_DECL_UUID16_ATTR_VAL(LE_GATT_CHAR_PROP_RD, ATT_CHAR_PERIPH_PREF_CON_PARAM);
static UINT16 gGapConnParamVal[4]         = {DEFAULT_DESIRED_MIN_CONN_INTERVAL, DEFAULT_DESIRED_MAX_CONN_INTERVAL, DEFAULT_DESIRED_SLAVE_LATENCY, DEFAULT_DESIRED_SUPERVERSION_TIMEOUT};

// This is used for KNS service
static UINT16 gKnsSvcUuid = KNS_SERVICE_UUID;

static UINT16 gKnsSetParaUuid      		  = KNS_SET_PARA_CHAR_UUID;
static UINT8  gKnsSetParaCharVal[] 		  = CHAR_DECL_UUID16_ATTR_VAL(LE_GATT_CHAR_PROP_WR | LE_GATT_CHAR_PROP_WR_NO_RESP, KNS_SET_PARA_CHAR_UUID);
static UINT8  gKnsSetParaVal[1]    		  = {0};

static UINT16 gKnsKeyUuid      			  = KNS_KEY_CHAR_UUID;
static UINT8  gKnsKeyCharVal[] 			  = CHAR_DECL_UUID16_ATTR_VAL(LE_GATT_CHAR_PROP_NTF, KNS_KEY_CHAR_UUID);
static UINT8  gKnsKeyVal[1]    			  = {0};
static UINT16 gKnsKeyClientCfg 			  = 0;


static LE_GATT_ATTR_T gGattSvcDb[GATT_IDX_TOP] =
{
    // GATT Service Declaration
    [GATT_IDX_SVC]                 = PRIMARY_SERVICE_DECL_UUID16(&gGattSvcUuid),
    // GATT Service Change Characteristic 
    [GATT_IDX_SERVICE_CHANGE_CHAR] = CHARACTERISTIC_DECL_UUID16(gGattServiceChangeCharVal),
    [GATT_IDX_SERVICE_CHANGE_VAL]  = CHARACTERISTIC_UUID16(&gGattServiceChangeUuid, 0, 0, sizeof(gGattServiceChangeVal), gGattServiceChangeVal),
    [GATT_IDX_SERVICE_CHANGE_CFG]  = CHAR_CLIENT_CONFIG_DESCRIPTOR(LE_GATT_PERMIT_READ | LE_GATT_PERMIT_AUTHOR_WRITE, &gGattServiceChangeClientCfg)
};

static LE_GATT_ATTR_T gGapSvcDb[GAP_IDX_TOP] =
{
    // GAP Service Declaration
    [GAP_IDX_SVC]                  = PRIMARY_SERVICE_DECL_UUID16(&gGapSvcUuid),
    // GAP Device Name Characteristic
    [GAP_IDX_DEVICE_NAME_CHAR]     = CHARACTERISTIC_DECL_UUID16(gGapDeviceNameCharVal),
    [GAP_IDX_DEVICE_NAME_VAL]      = CHARACTERISTIC_UUID16(&gGapDeviceNameUuid, LE_GATT_PERMIT_READ, sizeof(gGapDeviceNameVal), sizeof(BLE_DEVICE_NAME) - 1, gGapDeviceNameVal),
    // GAP Appearance Characteristic
    [GAP_IDX_APPEARANCE_CHAR]      = CHARACTERISTIC_DECL_UUID16(gGapAppearanceCharVal),
    [GAP_IDX_APPEARANCE_VAL]       = CHARACTERISTIC_UUID16(&gGapAppearanceUuid, LE_GATT_PERMIT_READ, 0, 2, gGapAppearanceVal),
    // GAP Connection Parameter Characteristic Declaration
    [GAP_IDX_CONN_PARAM_CHAR]      = CHARACTERISTIC_DECL_UUID16(gGapConnParamCharVal),
    [GAP_IDX_CONN_PARAM_VAL]       = CHARACTERISTIC_UUID16(&gGapConnParamUuid, LE_GATT_PERMIT_READ, 0, 8, gGapConnParamVal)
};

static LE_GATT_ATTR_T gKnsSvcDb[KNS_IDX_TOP] =
{
    // KNS Service Declaration
    [KNS_IDX_SVC]                  = PRIMARY_SERVICE_DECL_UUID16(&gKnsSvcUuid),
    // KNS Set Parameter Characteristic 
    [KNS_IDX_SET_PARA_CHAR] 	   = CHARACTERISTIC_DECL_UUID16(gKnsSetParaCharVal),
    [KNS_IDX_SET_PARA_VAL]  	   = CHARACTERISTIC_UUID16(&gKnsSetParaUuid, LE_GATT_PERMIT_AUTHOR_WRITE, 247, 0, gKnsSetParaVal),
    // KNS Key Characteristic 
    [KNS_IDX_KEY_CHAR] 			   = CHARACTERISTIC_DECL_UUID16(gKnsKeyCharVal),
    [KNS_IDX_KEY_VAL]  			   = CHARACTERISTIC_UUID16(&gKnsKeyUuid, 0, 247, 0, gKnsKeyVal),
    [KNS_IDX_KEY_CFG]  			   = CHAR_CLIENT_CONFIG_DESCRIPTOR(LE_GATT_PERMIT_READ | LE_GATT_PERMIT_AUTHOR_WRITE, &gKnsKeyClientCfg)
};


static LE_GATT_SERVICE_T *gGattSvc = 0;
static LE_GATT_SERVICE_T *gGapSvc = 0;
static LE_GATT_SERVICE_T *gKnsSvc = 0;

RET_DATA T_OplBleAppHandleGattServiceWriteFp OplBleAppHandleGattServiceWrite;
RET_DATA T_OplBleAppHandleKnsServiceWriteFp OplBleAppHandleKnsServiceWrite;
RET_DATA T_OplBleGattHandleAccessReadFp OplBleGattHandleAccessRead;
RET_DATA T_OplBleGattHandleAccessWriteFp OplBleGattHandleAccessWrite;
RET_DATA T_OplBleAppGattMsgHandlerFp OplBleAppGattMsgHandler;

static void BleAppHandleGattServiceWrite(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind)
{
    OplBleAppHandleGattServiceWrite(ind);
}

static void BleAppHandleKnsServiceWrite(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind)
{
    OplBleAppHandleKnsServiceWrite(ind);
}

static void BleGattHandleAccessRead(LE_GATT_MSG_ACCESS_READ_IND_T *ind)
{
    OplBleGattHandleAccessRead(ind);
}

static void BleGattHandleAccessWrite(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind)
{
    OplBleGattHandleAccessWrite(ind);
}

void BleAppGattMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    OplBleAppGattMsgHandler(task, id, message);
}

static void BleAppHandleGattServiceWrite_impl(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind)
{
	UINT8 attErr = 0;
	UINT16 attrid = ind->handle - gGattSvc->startHdl;
    
    BLE_APP_PRINT("BleAppHandleGattServiceWrite attId = %d op = %x offset = %d\r\n", attrid, ind->flag, ind->offset);
    
	switch (attrid)
    {
		case GATT_IDX_SERVICE_CHANGE_CFG:
		{
			LeGattChangeAttrVal(gGattSvc, attrid, ind->len, ind->pVal);
        }
        break;

		default:
            attErr = LE_ATT_ERR_WRITE_NOT_PERMITTED;
        break;
    }

	LeGattAccessWriteRsp(ind->conn_hdl, ind->flag, ind->handle, attErr);
}

static void BleAppHandleKnsServiceWrite_impl(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind)
{
	UINT8 attErr = 0;
	UINT16 attrid = ind->handle - gKnsSvc->startHdl;
    
    BLE_APP_PRINT("BleAppHandleKnsServiceWrite attId = %d op = %x offset = %d\r\n", attrid, ind->flag, ind->offset);
    
	switch (attrid)
    {
		case KNS_IDX_KEY_CFG:
		{
			LeGattChangeAttrVal(gKnsSvc, attrid, ind->len, ind->pVal);
        }
        break;

		case KNS_IDX_SET_PARA_VAL:
		{
			BLE_APP_DATA_T *app = BleAppDev();
            
			if (app->tuya_cb && (app->tuya_state >= TUYA_BLE_STATE_CONNECTED))
	        {
				BleAppCbNotify(TY_BLE_RECV, ind->len, ind->pVal);
	        }
        }
        break;

		default:
            attErr = LE_ATT_ERR_WRITE_NOT_PERMITTED;
        break;
    }

	LeGattAccessWriteRsp(ind->conn_hdl, ind->flag, ind->handle, attErr);
}

static void BleGattHandleAccessRead_impl(LE_GATT_MSG_ACCESS_READ_IND_T *ind)
{
	LeGattAccessReadRsp(ind->conn_hdl, ind->handle, LE_ATT_ERR_READ_NOT_PERMITTED);
}

static void BleGattHandleAccessWrite_impl(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind)
{
	if ((ind->handle >= gGattSvc->startHdl) && (ind->handle <= gGattSvc->endHdl))
	{
		BleAppHandleGattServiceWrite(ind);
    }
	else if ((ind->handle >= gKnsSvc->startHdl) && (ind->handle <= gKnsSvc->endHdl))
	{
		BleAppHandleKnsServiceWrite(ind);
    }
	else
	{
		LeGattAccessWriteRsp(ind->conn_hdl, ind->flag, ind->handle, LE_ATT_ERR_WRITE_NOT_PERMITTED);
    }
}

void BleAppGattMsgHandler_impl(TASK task, MESSAGEID id, MESSAGE message)
{
	BLE_APP_DATA_T *app = BleAppDev();

	switch (id)
	{
		case LE_GATT_MSG_INIT_CFM:
		{
			BLE_APP_PRINT("LE_GATT_MSG_INIT_CFM\r\n");

			gGattSvc = LeGattRegisterService(gGattSvcDb, sizeof(gGattSvcDb) / sizeof(LE_GATT_ATTR_T));

			if (gGattSvc)
			{
				BLE_APP_PRINT("LeGattRegisterService gGattSvc success\r\n");
            }

			gGapSvc = LeGattRegisterService(gGapSvcDb, sizeof(gGapSvcDb) / sizeof(LE_GATT_ATTR_T));

			if (gGapSvc)
			{
				BLE_APP_PRINT("LeGattRegisterService gGapSvc success\r\n");
            }

			gKnsSvc = LeGattRegisterService(gKnsSvcDb, sizeof(gKnsSvcDb) / sizeof(LE_GATT_ATTR_T));

			if (gKnsSvc)
			{
				BLE_APP_PRINT("LeGattRegisterService gKnsSvc success\r\n");
            }

            app->send_hdl = LeGattGetAttrHandle(gKnsSvc, KNS_IDX_KEY_VAL);
            app->send_cfg = &gKnsKeyClientCfg;
        }
		break;

		case LE_GATT_MSG_NOTIFY_CFM:
		{
			LE_GATT_MSG_NOTIFY_CFM_T *cfm = (LE_GATT_MSG_NOTIFY_CFM_T *)message;

			if ((cfm->handle == app->send_hdl) && (app->curr_tuya_evt == BLE_APP_MSG_TUYA_SEND_DATA) && app->send_data)
            {
            	INT32 rst = OPRT_OS_ADAPTER_BT_SEND_FAILED;
                
				if (cfm->status == SYS_ERR_SUCCESS)
				{
			        UINT16 sendLen;

					sendLen = app->send_len - app->send_idx;

					if (sendLen > app->curr_mtu - 3) sendLen = app->curr_mtu - 3;

					if (sendLen)
					{
						LE_ERR_STATE rc = LeGattCharValNotify(app->conn_hdl, app->send_hdl, sendLen, &app->send_data[app->send_idx]);

						if (!rc)
				        {
                            BLE_APP_PRINT("NOTIFY_CFM success sendlen = %d idx = %d\r\n", sendLen, app->send_idx);
							app->curr_tuya_evt = BLE_APP_MSG_TUYA_SEND_DATA;
							app->send_idx += sendLen;

                            break;
				        }
                    }
					else
					{
                        BLE_APP_PRINT("NOTIFY_CFM success sendlen = %d idx = %d\r\n", sendLen, app->send_idx);
						rst = OPRT_OS_ADAPTER_OK;
                    }
                }

				FreeMem(app->send_data);
	            app->send_data = 0;
                BleAppStopApiEvt(rst);
            }
        }
		break;

		case LE_GATT_MSG_ACCESS_READ_IND:
		{
            BLE_APP_PRINT("LE_GATT_MSG_ACCESS_READ_IND\r\n");
			BleGattHandleAccessRead((LE_GATT_MSG_ACCESS_READ_IND_T *)message);
        }
		break;

		case LE_GATT_MSG_ACCESS_WRITE_IND:
		{
            BLE_APP_PRINT("LE_GATT_MSG_ACCESS_WRITE_IND\r\n");
			BleGattHandleAccessWrite((LE_GATT_MSG_ACCESS_WRITE_IND_T *)message);
        }
        break;

		case LE_GATT_MSG_EXCHANGE_MTU_IND:
		{
            LE_GATT_MSG_EXCHANGE_MTU_IND_T *ind = (LE_GATT_MSG_EXCHANGE_MTU_IND_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_EXCHANGE_MTU_IND client mtu = %d\r\n", ind->client_rx_mtu);
            LeGattExchangeMtuRsp(ind->conn_hdl, LE_ATT_MAX_MTU);
		}
        break;

        case LE_GATT_MSG_EXCHANGE_MTU_CFM:
		{
			LE_GATT_MSG_EXCHANGE_MTU_CFM_T *cfm = (LE_GATT_MSG_EXCHANGE_MTU_CFM_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_EXCHANGE_MTU_CFM curr mtu = %d\r\n", cfm->current_rx_mtu);
            BleAppDev()->curr_mtu = cfm->current_rx_mtu;
        }
        break;
        
		case LE_GATT_MSG_CONFIRMATION_CFM:
		{
#ifdef ENABLE_APP_DEBUG
			LE_GATT_MSG_CONFIRMATION_CFM_T *cfm = (LE_GATT_MSG_CONFIRMATION_CFM_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_CONFIRMATION_CFM curr handle = %d\r\n", cfm->handle);
#endif
        }
        break;

        case LE_GATT_MSG_OPERATION_TIMEOUT:
		{
#ifdef ENABLE_APP_DEBUG
			LE_GATT_MSG_OPERATION_TIMEOUT_T *ind = (LE_GATT_MSG_OPERATION_TIMEOUT_T *)message;
			BLE_APP_PRINT("LE_GATT_MSG_OPERATION_TIMEOUT op = %x\r\n", ind->att_op);
#endif
			LeGapDisconnectReq(app->conn_hdl);
	    	app->state = APP_STATE_DISCONNECTING;
	        app->tuya_state = TUYA_BLE_STATE_DISCONNECTING;
        }
        break;

		default:
        break;
    }
}

void opl_os_adapt_ble_gatt_pre_init(void)
{
    OplBleAppHandleGattServiceWrite = BleAppHandleGattServiceWrite_impl;
    OplBleAppHandleKnsServiceWrite = BleAppHandleKnsServiceWrite_impl;
    OplBleGattHandleAccessRead = BleGattHandleAccessRead_impl;
    OplBleGattHandleAccessWrite = BleGattHandleAccessWrite_impl;
    OplBleAppGattMsgHandler = BleAppGattMsgHandler_impl;
    return;
}

