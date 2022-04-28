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

#include "msg.h"
#include "le_hci_if.h"
#include "le_cm_if.h"
#include "le_smp_if.h"
#include "le_gap_if.h"
#include "le_gatt_if.h"
#include "le_util.h"
#include "tuya_os_adapt_ble_app.h"
#include "tuya_os_adapt_ble_gatt.h"
#include "tuya_os_adapt_bt.h"
#include "opl_os_adapt_ble_app.h"


static BLE_APP_DATA_T *gTheBle = 0;

RET_DATA T_OplBleAppSetAdvtisingParaFp OplBleAppSetAdvtisingPara;
RET_DATA T_OplBleAppSmMsgHandlerFp OplBleAppSmMsgHandler;
RET_DATA T_OplBleCmMsgHandlerFp OplBleCmMsgHandler;
RET_DATA T_OplBleAppHandleTuyaInitFp OplBleAppHandleTuyaInit;
RET_DATA T_OplBleAppHandleTuyaStartFp OplBleAppHandleTuyaStart;
RET_DATA T_OplBleAppHandleTuyaDeinitFp OplBleAppHandleTuyaDeinit;
RET_DATA T_OplBleAppHandleTuyaDisconnectFp OplBleAppHandleTuyaDisconnect;
RET_DATA T_OplBleAppHandleTuyaSendDataFp OplBleAppHandleTuyaSendData;
RET_DATA T_OplBleAppHandleTuyaGetRssiFp OplBleAppHandleTuyaGetRssi;
RET_DATA T_OplBleAppMsgHandlerFp OplBleAppMsgHandler;
RET_DATA T_OplBleAppTaskHandlerFp OplBleAppTaskHandler;
RET_DATA T_OplBleAppCbTaskFp OplBleAppCbTask;
RET_DATA T_OplBleAppCreateCbTaskFp OplBleAppCreateCbTask;
RET_DATA T_OplBleAppInitFp OplBleAppInit;
RET_DATA T_OplBleAppApiSemaCheckFp OplBleAppApiSemaCheck;
RET_DATA T_OplBleAppApiSemaReleaseFp OplBleAppApiSemaRelease;
RET_DATA T_OplBleAppWaitApiResultFp OplBleAppWaitApiResult;
RET_DATA T_OplBleAppRstSemaReleaseFp OplBleAppRstSemaRelease;
RET_DATA T_OplBleAppCbSemaCheckFp OplBleAppCbSemaCheck;
RET_DATA T_OplBleAppCbNotifyFp OplBleAppCbNotify;
RET_DATA T_OplBleAppStopApiEvtFp OplBleAppStopApiEvt;
RET_DATA T_OplBleAppDevFp OplBleAppDev;

static void BleAppSetAdvtisingPara(UINT8 type, UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, UINT8 filter)
{
    OplBleAppSetAdvtisingPara(type, own_addr_type, peer_addr, filter);
}

static void BleAppSmMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    OplBleAppSmMsgHandler(task, id, message);
}

static void BleCmMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    OplBleCmMsgHandler(task, id, message);
}

static void BleAppHandleTuyaInit(TUYA_BLE_INIT_T *req)
{
    OplBleAppHandleTuyaInit(req);
}

static void BleAppHandleTuyaStart(void)
{
    OplBleAppHandleTuyaStart();
}

static void BleAppHandleTuyaDeinit(void)
{
    OplBleAppHandleTuyaDeinit();
}

static void BleAppHandleTuyaDisconnect(void)
{
    OplBleAppHandleTuyaDisconnect();
}

static void BleAppHandleTuyaSendData(TUYA_BLE_SEND_T *req)
{
    OplBleAppHandleTuyaSendData(req);
}

static void BleAppHandleTuyaGetRssi(void)
{
    OplBleAppHandleTuyaGetRssi();
}

static void BleAppMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    OplBleAppMsgHandler(task, id, message);
}

static void BleAppTaskHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    OplBleAppTaskHandler(task, id, message);
}

static void BleAppCbTask(void *pvParameters)
{
    OplBleAppCbTask(pvParameters);
}

static BOOL BleAppCreateCbTask(void *ins)
{
    return OplBleAppCreateCbTask(ins);
}

BOOL BleAppInit(void)
{
    return OplBleAppInit();
}

void BleAppApiSemaCheck(void)
{
    OplBleAppApiSemaCheck();
}

void BleAppApiSemaRelease(void)
{
    OplBleAppApiSemaRelease();
}

void BleAppWaitApiResult(void)
{
    OplBleAppWaitApiResult();
}

void BleAppRstSemaRelease(void)
{
    OplBleAppRstSemaRelease();
}

void BleAppCbSemaCheck(BOOL wait)
{
    OplBleAppCbSemaCheck(wait);
}

void BleAppCbNotify(UINT32 evt, UINT16 len, void *data)
{
    OplBleAppCbNotify(evt, len, data);
}

void BleAppStopApiEvt(INT32 rst)
{
    OplBleAppStopApiEvt(rst);
}

BLE_APP_DATA_T* BleAppDev(void)
{
    return OplBleAppDev();
}

static void BleAppSetAdvtisingPara_impl(UINT8 type, UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, UINT8 filter)
{
	LE_GAP_ADVERTISING_PARAM_T para;

	para.interval_min = 160;
	para.interval_max = 160;
	para.type = type;
	para.own_addr_type = own_addr_type;

	if (peer_addr)
    {
	    para.peer_addr_type = peer_addr->type;
        MemCopy(para.peer_addr, peer_addr->addr, 6);
    }
    else
    {
	    para.peer_addr_type = LE_HCI_ADV_PEER_ADDR_PUBLIC;
		MemSet(para.peer_addr, 0, 6);
    }

	para.channel_map = 0x7;
    para.filter_policy = filter;

	LeGapSetAdvParameter(&para);
}

static void BleAppSmMsgHandler_impl(TASK task, MESSAGEID id, MESSAGE message)
{
	BLE_APP_DATA_T *app = BleAppDev();

	switch (id)
	{
		case LE_SMP_MSG_PAIRING_ACTION_IND:
		{
			LE_SMP_MSG_PAIRING_ACTION_IND_T *ind = (LE_SMP_MSG_PAIRING_ACTION_IND_T *)message;
			BLE_APP_PRINT("LE_SMP_MSG_PAIRING_ACTION_IND hdl = %x sc = %d action = %d\r\n", ind->conn_hdl, ind->sc, ind->action);
            
			LeSmpSecurityRsp(ind->conn_hdl, TRUE);
		}
		break;

		case LE_SMP_MSG_ENCRYPTION_CHANGE_IND:
        {
			LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T *ind = (LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T *)message;

            BLE_APP_PRINT("LE_SMP_MSG_ENCRYPTION_CHANGE_IND enable = %d\r\n", ind->enable);
			app->encrypted = ind->enable;
        }
        break;
        
		case LE_SMP_MSG_ENCRYPTION_REFRESH_IND:
		{
#ifdef ENABLE_APP_DEBUG
			LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T *ind = (LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T *)message;

            BLE_APP_PRINT("LE_SMP_MSG_ENCRYPTION_REFRESH_IND status = %x\r\n", ind->status);
#endif
        }
        break;
        
		case LE_SMP_MSG_PAIRING_COMPLETE_IND:
		{
			LE_SMP_MSG_PAIRING_COMPLETE_IND_T *ind = (LE_SMP_MSG_PAIRING_COMPLETE_IND_T *)message;

			BLE_APP_PRINT("LE_SMP_MSG_PAIRING_COMPLETE_IND status = %x\r\n", ind->status);

			if (ind->status == SYS_ERR_SUCCESS)
			{
				app->paired = TRUE;
            }
        }
		break;

		default:
		break;
    }
}

static void BleCmMsgHandler_impl(TASK task, MESSAGEID id, MESSAGE message)
{
	BLE_APP_DATA_T *app = BleAppDev();

	switch (id)
	{
		case LE_CM_MSG_INIT_COMPLETE_CFM:
        {
			BLE_APP_PRINT("LE_CM_MSG_INIT_COMPLETE_CFM\r\n");
            LeGattInit(&app->task);
            LeSmpInit(&app->task);
		    LeGapSetAdvData(app->adv_len, app->adv_data);
		    LeSetScanRspData(app->scan_len, app->scan_data);
            BleAppSetAdvtisingPara(LE_HCI_ADV_TYPE_ADV_IND, LE_HCI_OWN_ADDR_PUBLIC, 0, LE_HCI_ADV_FILT_NONE);
        }
		break;
        
		case LE_CM_MSG_SET_ADVERTISING_DATA_CFM:
        {
			BLE_APP_PRINT("LE_CM_MSG_SET_ADVERTISING_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_DATA_CFM_T *)message)->status);
        }
		break;

		case LE_CM_MSG_SET_SCAN_RSP_DATA_CFM:
        {
			BLE_APP_PRINT("LE_CM_MSG_SET_SCAN_RSP_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_SCAN_RSP_DATA_CFM_T *)message)->status);
            
            if (app->curr_tuya_evt == BLE_APP_MSG_TUYA_INIT)
            {
            	if ((app->tuya_state == TUYA_BLE_STATE_DEINIT) || (app->tuya_state == TUYA_BLE_STATE_IDLE))
                {
					app->tuya_state = TUYA_BLE_STATE_IDLE;
					BleAppStopApiEvt(OPRT_OS_ADAPTER_OK);
                }
            }
        }
		break;

		case LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM:
        {
			BLE_APP_PRINT("LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM_T *)message)->status);

            LeSmpSetDefaultConfig(LE_SM_IO_CAP_NO_IO, FALSE, FALSE, TRUE);
            app->state = APP_STATE_IDLE;

            if (app->curr_tuya_evt == BLE_APP_MSG_TUYA_INIT)
            {
				app->tuya_state = TUYA_BLE_STATE_IDLE;
                BleAppStopApiEvt(OPRT_OS_ADAPTER_OK);
            }
        }
        break;

		case LE_CM_MSG_ENTER_ADVERTISING_CFM:
		{
			LE_CM_MSG_ENTER_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_ENTER_ADVERTISING_CFM_T *)message;
			BLE_APP_PRINT("LE_CM_MSG_ENTER_ADVERTISING_CFM Status = %x\r\n", cfm->status);
            
			if (cfm->status == SYS_ERR_SUCCESS)
			{
				app->state = APP_STATE_ADVERTISING;
            }

			if (app->curr_tuya_evt == BLE_APP_MSG_TUYA_START)
			{
            	INT32 rc = OPRT_OS_ADAPTER_BT_ADV_START_FAILED;

				if (cfm->status == SYS_ERR_SUCCESS)
				{
					app->tuya_state = TUYA_BLE_STATE_START;
                    rc = OPRT_OS_ADAPTER_OK;
	            }

                BleAppStopApiEvt(rc);
            }
        }
		break;

		case LE_CM_MSG_EXIT_ADVERTISING_CFM:
		{
			LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_EXIT_ADVERTISING_CFM Status = %x\r\n", cfm->status);

			if (cfm->status == SYS_ERR_SUCCESS)
			{
				app->state = APP_STATE_IDLE;
            }

            if (app->curr_tuya_evt == BLE_APP_MSG_TUYA_DEINIT)
            {
				if (cfm->status == SYS_ERR_SUCCESS)
				{
					app->tuya_state = TUYA_BLE_STATE_DEINIT;
                    BleAppStopApiEvt(OPRT_OS_ADAPTER_OK);
	            }
	            else
	            {
					if (app->state == APP_STATE_CONNECTED)
                	{
						// set disconnect
						LE_ERR_STATE rc = LeGapDisconnectReq(app->conn_hdl);

						if (rc) 
						{
							BLE_APP_PRINT("EXIT_ADVERTISING_CFM disconnect fail rc = %x\r\n", rc);
					        BleAppStopApiEvt(OPRT_OS_ADAPTER_BT_DEINIT_FAILED);
					    }
					    else
					    {
					    	app->state = APP_STATE_DISCONNECTING;
			                app->tuya_state = TUYA_BLE_STATE_DISCONNECTING;
					    }
                    }
                    else
                        BleAppStopApiEvt(OPRT_OS_ADAPTER_BT_DEINIT_FAILED);
	            }
            }
        }
		break;
        
        case LE_CM_CONNECTION_COMPLETE_IND:
        {
			LE_CM_CONNECTION_COMPLETE_IND_T *ind = (LE_CM_CONNECTION_COMPLETE_IND_T *)message;

			BLE_APP_PRINT("LE_CM_CONNECTION_COMPLETE_IND status = %x\r\n", ind->status);

			if (ind->status == SYS_ERR_SUCCESS) 
			{
				app->state = APP_STATE_CONNECTED;
                app->tuya_state = TUYA_BLE_STATE_CONNECTED;

				app->conn_hdl = ind->conn_hdl;
	            app->bt_addr.type = ind->peer_addr_type;
	            MemCopy(app->bt_addr.addr, ind->peer_addr, 6);

				app->max_itvl = ind->conn_interval;
				app->latency = ind->conn_latency;
				app->sv_tmo = ind->supervison_timeout;

				BleAppCbNotify(TY_BLE_CONNECTED, 0, 0);
            }
            else
            {
				app->state = APP_STATE_IDLE;
            }
        }
        break;

        case LE_CM_MSG_SIGNAL_UPDATE_REQ:
		{
			LE_CM_MSG_SIGNAL_UPDATE_REQ_T *req = (LE_CM_MSG_SIGNAL_UPDATE_REQ_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_SIGNAL_UPDATE_REQ identifier = %d\r\n", req->identifier);
			BLE_APP_PRINT("    min = %x max = %x latency = %x timeout = %x\r\n", req->interval_min, req->interval_max, req->slave_latency, req->timeout_multiplier);
            LeGapConnUpdateResponse(req->conn_hdl, req->identifier, TRUE);
        }
        break;

		case LE_CM_MSG_CONN_PARA_REQ:
		{
            LE_CM_MSG_CONN_PARA_REQ_T *req = (LE_CM_MSG_CONN_PARA_REQ_T *)message;
			BLE_APP_PRINT("LE_CM_MSG_CONN_PARA_REQ min = %x max = %x latency = %x timeout = %x\r\n", req->itv_min, req->itv_max, req->latency, req->sv_tmo);
            LeGapConnParaRequestRsp(req->conn_hdl, TRUE);
        }
        break;

		case LE_CM_MSG_CONN_UPDATE_COMPLETE_IND:
		{
			LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T *ind = (LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T *)message;
			BLE_APP_PRINT("LE_CM_MSG_CONN_UPDATE_COMPLETE_IND status = %x, itv = %x, latency = %x svt = %x\r\n", ind->status, ind->interval, ind->latency, ind->supervision_timeout);

			if (ind->status == SYS_ERR_SUCCESS)
			{
				app->max_itvl = ind->interval;
				app->latency = ind->latency;
				app->sv_tmo = ind->supervision_timeout;
            }
			else
            {
		    	app->state = APP_STATE_DISCONNECTING;
                app->tuya_state = TUYA_BLE_STATE_DISCONNECTING;
				LeGapDisconnectReq(ind->conn_hdl);
            }
        }
		break;

        case LE_CM_MSG_SET_DISCONNECT_CFM:
        {
#ifdef ENABLE_APP_DEBUG
			LE_CM_MSG_SET_DISCONNECT_CFM_T *cfm = (LE_CM_MSG_SET_DISCONNECT_CFM_T *)message;
			BLE_APP_PRINT("LE_CM_MSG_SET_DISCONNECT_CFM conn_hdl = %x status = %x\r\n", cfm->handle, cfm->status);
#endif
        }
        break;
        
		case LE_CM_MSG_DISCONNECT_COMPLETE_IND:
		{
#ifdef ENABLE_APP_DEBUG
			LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *ind = (LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *)message;

			BLE_APP_PRINT("LE_CM_MSG_DISCONNECT_COMPLETE_IND conn_hdl = %x status = %x reason = %x \r\n", ind->conn_hdl, ind->status, ind->reason);
#endif
			app->state = APP_STATE_IDLE;

			if (app->curr_tuya_evt == BLE_APP_MSG_TUYA_DEINIT)
			{
				app->tuya_state = TUYA_BLE_STATE_DEINIT;
				BleAppStopApiEvt(OPRT_OS_ADAPTER_OK);
            }
            else
            {
				app->tuya_state = TUYA_BLE_STATE_IDLE;

                if (app->curr_tuya_evt == BLE_APP_MSG_TUYA_GAP_DISCONNECT)
                {
					BleAppStopApiEvt(OPRT_OS_ADAPTER_OK);
                }
                else if (app->curr_tuya_evt == BLE_APP_MSG_TUYA_SEND_DATA)
                {
					if (app->send_idx == app->send_len)
		                app->api_rst = OPRT_OS_ADAPTER_OK;
                    else
		                app->api_rst = OPRT_OS_ADAPTER_BT_SEND_FAILED;

                    FreeMem(app->send_data);
                    app->send_data = 0;
					app->send_idx = 0;
                    app->send_len = 0;
					BleAppStopApiEvt(app->api_rst);
                }
            }

			BleAppCbNotify(TY_BLE_DISCONNECTED, 0, 0);

            *app->send_cfg = 0;
            app->curr_mtu = 23;
        }
        break;

		default:
        break;
    }
}

static void BleAppHandleTuyaInit_impl(TUYA_BLE_INIT_T *req)
{
	BLE_APP_DATA_T *app = BleAppDev();

	app->tuya_cb = req->cb;
	app->adv_len = req->adv_len;
	app->scan_len = req->scan_len;

	if (app->adv_len) MemCopy(app->adv_data, req->adv_data, req->adv_len);
	if (app->scan_len) MemCopy(app->scan_data, req->scan_data, req->scan_len);

	if (app->state == APP_STATE_INIT) 
	{
		LeCmInit(&app->task);
    }
	else
	{
	    LeGapSetAdvData(app->adv_len, app->adv_data);
	    LeSetScanRspData(app->scan_len, app->scan_data);
    }
    
    app->curr_tuya_evt = BLE_APP_MSG_TUYA_INIT;
}

static void BleAppHandleTuyaStart_impl(void)
{
	BLE_APP_DATA_T *app = BleAppDev();
	LE_ERR_STATE rc = SYS_ERR_FAIL;
    
	if (app->state == APP_STATE_IDLE) rc = LeGapAdvertisingEnable(TRUE);

	if (rc) 
	{
		BLE_APP_PRINT("TuyaStart adv fail rc = %x\r\n", rc);
        BleAppStopApiEvt(OPRT_OS_ADAPTER_BT_ADV_START_FAILED);
    }
	else
	    app->curr_tuya_evt = BLE_APP_MSG_TUYA_START;
}

static void BleAppHandleTuyaDeinit_impl(void)
{
	BLE_APP_DATA_T *app = BleAppDev();

	if (app->tuya_state == TUYA_BLE_STATE_IDLE)
	{
		BleAppStopApiEvt(OPRT_OS_ADAPTER_OK);
    }
	else if (app->tuya_state == TUYA_BLE_STATE_START)
	{
	    LE_ERR_STATE rc = LeGapAdvertisingEnable(FALSE);
    
		if (rc != SYS_ERR_SUCCESS) 
		{
			BLE_APP_PRINT("TuyaDeinit stop adv fail rc = %x\r\n", rc);
            BleAppStopApiEvt(OPRT_OS_ADAPTER_BT_DEINIT_FAILED);
	    }
	    else
	    {
			app->curr_tuya_evt = BLE_APP_MSG_TUYA_DEINIT;
	    }
    }
	else if (app->tuya_state == TUYA_BLE_STATE_CONNECTED)
	{
		LE_ERR_STATE rc = LeGapDisconnectReq(app->conn_hdl);

		if (rc != SYS_ERR_SUCCESS) 
		{
			BLE_APP_PRINT("TuyaDeinit disconnect fail rc = %x\r\n", rc);
            BleAppStopApiEvt(OPRT_OS_ADAPTER_BT_DEINIT_FAILED);
	    }
	    else
	    {
	    	app->state = APP_STATE_DISCONNECTING;
	        app->tuya_state = TUYA_BLE_STATE_DISCONNECTING;
			app->curr_tuya_evt = BLE_APP_MSG_TUYA_DEINIT;
	    }
    }
	else if (app->tuya_state == TUYA_BLE_STATE_DISCONNECTING)
	{
		app->curr_tuya_evt = BLE_APP_MSG_TUYA_DEINIT;
    }
}

static void BleAppHandleTuyaDisconnect_impl(void)
{
	BLE_APP_DATA_T *app = BleAppDev();

	if (app->state == APP_STATE_CONNECTED)
	{
		LE_ERR_STATE rc = LeGapDisconnectReq(app->conn_hdl);

		if (rc != SYS_ERR_SUCCESS) 
		{
			BLE_APP_PRINT("TuyaDisconnect disconnect fail rc = %x\r\n", rc);
            BleAppStopApiEvt(OPRT_OS_ADAPTER_BT_DISCONN_FAILED);
	    }
	    else
	    {
	    	app->state = APP_STATE_DISCONNECTING;
	        app->tuya_state = TUYA_BLE_STATE_DISCONNECTING;
			app->curr_tuya_evt = BLE_APP_MSG_TUYA_GAP_DISCONNECT;
	    }
    }
	else if (app->state == APP_STATE_DISCONNECTING)
	{
		app->curr_tuya_evt = BLE_APP_MSG_TUYA_GAP_DISCONNECT;
    }
    else
    {
        BleAppStopApiEvt(OPRT_OS_ADAPTER_BT_DISCONN_FAILED);
    }
}

static void BleAppHandleTuyaSendData_impl(TUYA_BLE_SEND_T *req)
{
	BLE_APP_DATA_T *app = BleAppDev();

	if (app->state == APP_STATE_CONNECTED)
	{
		LE_ERR_STATE rc;
        UINT16 sendLen;
        
		if (app->send_data) FreeMem(app->send_data);

		app->send_data = req->data;
		app->send_len = req->len;
        app->send_idx = 0;

		sendLen = app->send_len;

		if (sendLen > app->curr_mtu - 3) sendLen = app->curr_mtu - 3;

		rc = LeGattCharValNotify(app->conn_hdl, app->send_hdl, sendLen, app->send_data);

		if (rc)
		{
			FreeMem(app->send_data);
            app->send_data = 0;
            BleAppStopApiEvt(OPRT_OS_ADAPTER_BT_SEND_FAILED);
        }
		else
        {
			app->curr_tuya_evt = BLE_APP_MSG_TUYA_SEND_DATA;
			app->send_idx += sendLen;
        }
    }
    else
    {
    	BleAppStopApiEvt(OPRT_OS_ADAPTER_BT_SEND_FAILED);
    }
}

static void BleAppHandleTuyaGetRssi_impl(void)
{
	BLE_APP_DATA_T *app = BleAppDev();
	INT32 rc = OPRT_OS_ADAPTER_BT_RSSI_GET_FAILED;
    
	if (app->state >= APP_STATE_CONNECTED)
	{
		app->conn_rssi = ((INT8)reg_read(0x20601298));
        rc = OPRT_OS_ADAPTER_OK;
    }

    BleAppStopApiEvt(rc);
}

static void BleAppMsgHandler_impl(TASK task, MESSAGEID id, MESSAGE message)
{
	BLE_APP_DATA_T *app = BleAppDev();

	BLE_APP_PRINT("BleAppMsgHandler id = %x\r\n", id);
	switch (id)
	{
		case BLE_APP_MSG_TUYA_INIT:
			BleAppHandleTuyaInit((TUYA_BLE_INIT_T *)message);
		break;

		case BLE_APP_MSG_TUYA_START:
			BleAppHandleTuyaStart();
		break;

		case BLE_APP_MSG_TUYA_DEINIT:
			BleAppHandleTuyaDeinit();
		break;

		case BLE_APP_MSG_TUYA_GAP_DISCONNECT:
			BleAppHandleTuyaDisconnect();
		break;

		case BLE_APP_MSG_TUYA_SEND_DATA:
			BleAppHandleTuyaSendData((TUYA_BLE_SEND_T *)message);
		break;

		case BLE_APP_MSG_TUYA_GET_RSSI:
			BleAppHandleTuyaGetRssi();
        break;

		default:
		break;
    }
}

static void BleAppTaskHandler_impl(TASK task, MESSAGEID id, MESSAGE message)
{
	if ((id >= LE_GATT_MSG_BASE) && (id < LE_GATT_MSG_TOP))
	{
		BleAppGattMsgHandler(task, id, message);
    }
    else if ((id >= BLE_APP_MSG_TUYA_INIT) && (id < BLE_APP_MSG_TOP))
    {
		BleAppMsgHandler(task, id, message);
    }
	else if ((id >= LE_SMP_MSG_BASE) && (id < LE_SMP_MSG_TOP))
	{
		BleAppSmMsgHandler(task, id, message);
    }
	else if ((id >= LE_CM_MSG_BASE) && (id < LE_CM_MSG_TOP))
	{
		BleCmMsgHandler(task, id, message);
    }
}


static void BleAppCbTask_impl(void *pvParameters)
{
	BLE_APP_DATA_T *app = BleAppDev();
	LE_QLIST_T *q = &app->cb_evt_q;
    
	BLE_APP_PRINT("BleAppCbTask Run\r\n");

	while (1)
	{
		if (!app->tuya_cb) LeQueueBroken(q);
    
		if (LeQueueCount(q))
        {
			TUYA_CB_MSG_T *m = LeQueuePop(q);

			if (m)
        	{
				TUYA_BLE_EVENT_CB cb = (TUYA_BLE_EVENT_CB)app->tuya_cb;
        		tuya_ble_data_buf_t data;

    			BLE_APP_PRINT("cb evt = %d count = %d\r\n", m->evt, LeQueueCount(&app->cb_evt_q));

				data.len = m->len;
				data.data = m->data;
				cb((ble_event_e)m->evt, &data);
                FreeMem(m);
            }
        }
		else
			BleAppCbSemaCheck(TRUE);
    }
}

static BOOL BleAppCreateCbTask_impl(void *ins)
{
	BLE_APP_DATA_T *app = ins;
    osThreadDef_t extApp;

    //create task
    extApp.name = "ble_cb_app";
    extApp.stacksize = 512;
    extApp.tpriority = osPriorityNormal;
    extApp.pthread = BleAppCbTask;
    
    app->cb_task_hdl = osThreadCreate(&extApp, (void *)app->cb_task_hdl);
    
    if(app->cb_task_hdl == NULL)
    {
        BLE_APP_PRINT("BleAppCreateCbTask create fail \r\n");
        return FALSE;
    }

	app->cb_sema_id = osSemaphoreCreate(0, 2);

	if (!app->cb_sema_id) 
	{
		BLE_APP_PRINT("BleAppCreateCbTask alloc cb_sema_id fail\r\n");
	    osThreadTerminate(app->cb_task_hdl);
        app->cb_task_hdl = 0;
	    return FALSE;
    }

	return TRUE;
}

BOOL BleAppInit_impl(void)
{
	BLE_APP_DATA_T *app = BleAppDev();

	if (!app)
	{
	    app = Malloc(sizeof(BLE_APP_DATA_T));

		if (!app) return FALSE;

		MemSet(app, 0, sizeof(BLE_APP_DATA_T));

//#ifdef ENABLE_APP_DEBUG
//		tracer_log_mode_set(2);
//	    tracer_log_level_set(255, 7);
//#else
//		tracer_log_level_set(255, 0);
//#endif
		app->state = APP_STATE_INIT;
		app->curr_mtu = 23;
	    
		app->min_itvl = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
		app->max_itvl = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
		app->latency  = DEFAULT_DESIRED_SLAVE_LATENCY;
		app->sv_tmo   = DEFAULT_DESIRED_SUPERVERSION_TIMEOUT;
	    
		LeHostCreateTask(&app->task, BleAppTaskHandler);

		app->api_sema_id = osSemaphoreCreate(0, 1);

		if (!app->api_sema_id) 
		{
			BLE_APP_PRINT("alloc api_sema_id fail\r\n");
			goto done;
	    }

		app->rst_sema_id = osSemaphoreCreate(0, 1);

		if (!app->rst_sema_id) 
		{
			BLE_APP_PRINT("alloc rst_sema_id fail\r\n");
            goto done;
	    }

		osSemaphoreWait(app->rst_sema_id, osWaitForever);

		if (!BleAppCreateCbTask(app)) goto done;
       
        gTheBle = app;
    }

    return TRUE;

done:
	if (app->api_sema_id) osSemaphoreDelete(app->api_sema_id);
	if (app->rst_sema_id) osSemaphoreDelete(app->rst_sema_id);
	FreeMem(app);
    
    return FALSE;
}

void BleAppApiSemaCheck_impl(void)
{
	BLE_APP_DATA_T *app = BleAppDev();

	if (app->api_sema_id) osSemaphoreWait(app->api_sema_id, osWaitForever);
}

void BleAppApiSemaRelease_impl(void)
{
	BLE_APP_DATA_T *app = BleAppDev();

	if (app->api_sema_id) osSemaphoreRelease(app->api_sema_id);
}

void BleAppWaitApiResult_impl(void)
{
	BLE_APP_DATA_T *app = BleAppDev();

	if (app->rst_sema_id) osSemaphoreWait(app->rst_sema_id, osWaitForever);
}

void BleAppRstSemaRelease_impl(void)
{
	BLE_APP_DATA_T *app = BleAppDev();

	if (app->rst_sema_id) osSemaphoreRelease(app->rst_sema_id);
}

void BleAppCbSemaCheck_impl(BOOL wait)
{
	BLE_APP_DATA_T *app = BleAppDev();

	if (app->cb_sema_id)
	{
		if (wait)
		    osSemaphoreWait(app->cb_sema_id, osWaitForever);
		else
            osSemaphoreRelease(app->cb_sema_id);
    }
}

void BleAppCbNotify_impl(UINT32 evt, UINT16 len, void *data)
{
	BLE_APP_DATA_T *app = BleAppDev();

    if (app->tuya_cb)
    {
		TUYA_CB_MSG_T *m = Malloc(sizeof(TUYA_CB_MSG_T) + len);

		if (!m)
		{
			BLE_APP_PRINT("BleAppCbNotify fail\r\n");
		    return;
	    }

		m->evt = evt;
		m->len = len;
		m->data = 0;
	    
		if (len)
		{
			m->data = (UINT8 *)m + sizeof(TUYA_CB_MSG_T);
	        MemCopy(m->data, data, len);
	    }

	    LeQueuePush(&app->cb_evt_q, m);
	    BleAppCbSemaCheck(FALSE);
    }
}

void BleAppStopApiEvt_impl(INT32 rst)
{
	BLE_APP_DATA_T *app = BleAppDev();

	app->curr_tuya_evt = 0;
	app->api_rst = rst;
	BleAppRstSemaRelease();
	BleAppApiSemaRelease();
}

BLE_APP_DATA_T* BleAppDev_impl(void)
{
	return gTheBle;
}

void opl_os_adapt_ble_app_pre_init(void)
{
    OplBleAppSetAdvtisingPara = BleAppSetAdvtisingPara_impl;
    OplBleAppSmMsgHandler = BleAppSmMsgHandler_impl;
    OplBleCmMsgHandler = BleCmMsgHandler_impl;
    OplBleAppHandleTuyaInit = BleAppHandleTuyaInit_impl;
    OplBleAppHandleTuyaStart = BleAppHandleTuyaStart_impl;
    OplBleAppHandleTuyaDeinit = BleAppHandleTuyaDeinit_impl;
    OplBleAppHandleTuyaDisconnect = BleAppHandleTuyaDisconnect_impl;
    OplBleAppHandleTuyaSendData = BleAppHandleTuyaSendData_impl;
    OplBleAppHandleTuyaGetRssi = BleAppHandleTuyaGetRssi_impl;
    OplBleAppMsgHandler = BleAppMsgHandler_impl;
    OplBleAppTaskHandler = BleAppTaskHandler_impl;
    OplBleAppCbTask = BleAppCbTask_impl;
    OplBleAppCreateCbTask = BleAppCreateCbTask_impl;
    OplBleAppInit = BleAppInit_impl;
    OplBleAppApiSemaCheck = BleAppApiSemaCheck_impl;
    OplBleAppApiSemaRelease = BleAppApiSemaRelease_impl;
    OplBleAppWaitApiResult = BleAppWaitApiResult_impl;
    OplBleAppRstSemaRelease = BleAppRstSemaRelease_impl;
    OplBleAppCbSemaCheck = BleAppCbSemaCheck_impl;
    OplBleAppCbNotify = BleAppCbNotify_impl;
    OplBleAppStopApiEvt = BleAppStopApiEvt_impl;
    OplBleAppDev = BleAppDev_impl;
    return;
}

