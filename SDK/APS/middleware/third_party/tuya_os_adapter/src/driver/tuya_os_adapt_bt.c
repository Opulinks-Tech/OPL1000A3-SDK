#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tuya_os_adapt_bt.h"
#include "tuya_os_adapter_error_code.h"
#include "tuya_os_adapt_ble_app.h"
#include "opl_os_adapt_bt.h"

RET_DATA t_opl_adapt_ble_init_fp opl_adapt_ble_init;
RET_DATA t_opl_adapt_ble_start_fp opl_adapt_ble_start;
RET_DATA t_opl_adapt_ble_deinit_fp opl_adapt_ble_deinit;
RET_DATA t_opl_adapt_ble_send_fp opl_adapt_ble_send;
RET_DATA t_opl_os_adapt_bt_gap_disconnect_fp opl_os_adapt_bt_gap_disconnect;
RET_DATA t_opl_os_adapt_bt_get_rssi_fp opl_os_adapt_bt_get_rssi;

/***********************************************************
*************************function define********************
***********************************************************/
int tuya_adapt_ble_init(tuya_ble_data_buf_t *adv, tuya_ble_data_buf_t *res, TUYA_BLE_EVENT_CB cb)
{
    return opl_adapt_ble_init(adv, res, cb);
}

int tuya_adapt_ble_start(void)
{
    return opl_adapt_ble_start();
}

int tuya_adapt_ble_deinit(void)
{
    return opl_adapt_ble_deinit();
}

int tuya_adapt_ble_send(tuya_ble_data_buf_t *buf)
{
    return opl_adapt_ble_send(buf);
}

int tuya_os_adapt_bt_gap_disconnect(void)
{
    return opl_os_adapt_bt_gap_disconnect();
}

int tuya_os_adapt_bt_get_rssi(SCHAR_T *rssi)
{
    return opl_os_adapt_bt_get_rssi(rssi);
}

/**
 * @brief �Ω��l���Ť�
 *
 * @param[in]       adv res cb    ��l�ưѼ�
 * @return int 0=���\�A�D0=����
 */
int tuya_adapt_ble_init_impl(tuya_ble_data_buf_t *adv, tuya_ble_data_buf_t *res, TUYA_BLE_EVENT_CB cb)
{
	if (BleAppInit())
	{
		BLE_APP_DATA_T *app = BleAppDev();
		TUYA_BLE_INIT_T *req;

		BleAppApiSemaCheck();

		if (app->tuya_state > TUYA_BLE_STATE_IDLE)
		{
			BleAppApiSemaRelease();
	        return OPRT_OS_ADAPTER_BT_INIT_FAILED;
		}

		req = Malloc(sizeof(TUYA_BLE_INIT_T));

		if (!req) 
		{
			BleAppApiSemaRelease();
			return OPRT_OS_ADAPTER_MALLOC_FAILED;
		}

		req->adv_len = 0;
		req->scan_len = 0;
		req->cb = cb;

		if (adv && adv->len)
		{
			req->adv_len = adv->len;
	    
			if (req->adv_len > 31) req->adv_len = 31;

		    MemCopy(req->adv_data, adv->data, req->adv_len);
		}

		if (res && res->len)
		{
			req->scan_len = res->len;
	    
			if (req->scan_len > 31) req->scan_len = 31;
	    
		    MemCopy(req->scan_data, res->data, req->scan_len);
		}

		LeSendMessage(&app->task, BLE_APP_MSG_TUYA_INIT, req);
		BleAppWaitApiResult();

		return app->api_rst;
    }

	return OPRT_OS_ADAPTER_MALLOC_FAILED;
}

/**
 * @brief �Ұ��Ť��s��
 *
 * @return int 0=���\�A�D0=����
 */
int tuya_adapt_ble_start_impl(void)
{
	if (BleAppInit())
    {
		// printf("BLE_START_OK\r\n");
		BLE_APP_DATA_T *app = BleAppDev();

		BleAppApiSemaCheck();

		if (app->tuya_state == TUYA_BLE_STATE_START)
		{
			BleAppApiSemaRelease();
	        return OPRT_OS_ADAPTER_OK;
		}

		if (app->tuya_state != TUYA_BLE_STATE_IDLE)
		{
			BleAppApiSemaRelease();
	        return OPRT_OS_ADAPTER_BT_ADV_START_FAILED;
		}

		LeSendMessage(&app->task, BLE_APP_MSG_TUYA_START, 0);
		BleAppWaitApiResult();

		return app->api_rst;
    }

	return OPRT_OS_ADAPTER_MALLOC_FAILED;
}

/**
 * @brief �����Ť��\��
 *
 * @return int 0=���\�A�D0=����
 */
int tuya_adapt_ble_deinit_impl(void)
{
	if (BleAppInit())
    {
		BLE_APP_DATA_T *app = BleAppDev();

		BleAppApiSemaCheck();

		if (app->tuya_state <= TUYA_BLE_STATE_DEINIT)
		{
			BleAppApiSemaRelease();
	        return OPRT_OS_ADAPTER_OK;
		}

		LeSendMessage(&app->task, BLE_APP_MSG_TUYA_DEINIT, 0);
		BleAppWaitApiResult();

		return app->api_rst;
	}

	return OPRT_OS_ADAPTER_MALLOC_FAILED;
}

/**
 * @brief �o�e�ƾ�
 *
 * @param[in]       buf    �o�ebuf
 * @return int 0=���\�A�D0=����
 */
int tuya_adapt_ble_send_impl(tuya_ble_data_buf_t *buf)
{
	if (BleAppInit())
    {
		BLE_APP_DATA_T *app = BleAppDev();
		TUYA_BLE_SEND_T *req;

		if (!buf || !buf->len) return OPRT_OS_ADAPTER_INVALID_PARM;

		BleAppApiSemaCheck();

		if (app->tuya_state != TUYA_BLE_STATE_CONNECTED)
		{
			BleAppApiSemaRelease();
	        return OPRT_OS_ADAPTER_BT_SEND_FAILED;
		}

		req = Malloc(sizeof(TUYA_BLE_INIT_T));

		if (!req) 
		{
			BleAppApiSemaRelease();
			return OPRT_OS_ADAPTER_MALLOC_FAILED;
		}

		req->len = buf->len;
		req->data = Malloc(req->len);

		if (!req->data) 
		{
			FreeMem(req);
			BleAppApiSemaRelease();
			return OPRT_OS_ADAPTER_MALLOC_FAILED;
		}

		MemCopy(req->data, buf->data, req->len);
		LeSendMessage(&app->task, BLE_APP_MSG_TUYA_SEND_DATA, req);
		BleAppWaitApiResult();

		return app->api_rst;
    }

	return OPRT_OS_ADAPTER_MALLOC_FAILED;
}

/**
 * @brief GAP disconnect
 * @return int 0=���\�A�D0=����
 */
int tuya_os_adapt_bt_gap_disconnect_impl(void)
{
	if (BleAppInit())
    {
		BLE_APP_DATA_T *app = BleAppDev();

		BleAppApiSemaCheck();

		if (app->tuya_state != TUYA_BLE_STATE_CONNECTED)
		{
			BleAppApiSemaRelease();
	        return OPRT_OS_ADAPTER_BT_DISCONN_FAILED;
		}

		LeSendMessage(&app->task, BLE_APP_MSG_TUYA_GAP_DISCONNECT, 0);
		BleAppWaitApiResult();

		return app->api_rst;
    }

	return OPRT_OS_ADAPTER_MALLOC_FAILED;
}

/**
 * @brief Get Connection RSSI
 * @return int 0=���\�A�D0=����
 */
int tuya_os_adapt_bt_get_rssi_impl(SCHAR_T *rssi)
{
	if (BleAppInit())
    {
		BLE_APP_DATA_T *app = BleAppDev();

		BleAppApiSemaCheck();

		if (app->tuya_state < TUYA_BLE_STATE_CONNECTED)
		{
			BleAppApiSemaRelease();
	        return OPRT_OS_ADAPTER_BT_RSSI_GET_FAILED;
		}

		LeSendMessage(&app->task, BLE_APP_MSG_TUYA_GET_RSSI, 0);
		BleAppWaitApiResult();

		if (rssi)
		{
			if (app->api_rst == OPRT_OS_ADAPTER_OK)
				*rssi = app->conn_rssi;
            else
                *rssi = 0;
        }

		return app->api_rst;
    }

	return OPRT_OS_ADAPTER_MALLOC_FAILED;
}

/**
 * @brief �����Ť��A�����ϥ�
 *
 * @param[out]       info
 * @return int 0=���\�A�D0=����
 */
// int tuya_os_adapt_bt_assign_scan(ty_bt_scan_info_t *info)
// {

//     //todo
// }

void opl_os_adapt_bt_pre_init(void)
{
    opl_adapt_ble_init = tuya_adapt_ble_init_impl;
    opl_adapt_ble_start = tuya_adapt_ble_start_impl;
    opl_adapt_ble_deinit = tuya_adapt_ble_deinit_impl;
    opl_adapt_ble_send = tuya_adapt_ble_send_impl;
    opl_os_adapt_bt_gap_disconnect = tuya_os_adapt_bt_gap_disconnect_impl;
    opl_os_adapt_bt_get_rssi = tuya_os_adapt_bt_get_rssi_impl;
    return;
}

