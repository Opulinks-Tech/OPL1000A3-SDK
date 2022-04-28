/**
 * @file tuya_adapt_ble.h
 * @brief BLE�t���ާ@���f
 *
 * @copyright Copyright(C),2018-2020, ��~��� www.tuya.com
 *
 */

#ifndef TUYA_ADAPT_BLE_H
#define TUYA_ADAPT_BLE_H

#include <stdint.h>
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************
*************************micro define***********************
***********************************************************/
typedef struct {
    uint8_t *data;
    uint32_t len;
}tuya_ble_data_buf_t;


typedef enum {
    TY_BLE_CONNECTED,
    TY_BLE_DISCONNECTED,
    TY_BLE_RECV
}ble_event_e;

typedef void (*TUYA_BLE_EVENT_CB)(ble_event_e event, tuya_ble_data_buf_t *data);
/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/**
 * @brief �Ω��l���Ť�
 *
 * @param[in]       adv res cb    ��l�ưѼ�
 * @return int 0=���\�A�D0=����
 */
extern int tuya_adapt_ble_init(tuya_ble_data_buf_t *adv, tuya_ble_data_buf_t *res, TUYA_BLE_EVENT_CB cb);

/**
 * @brief �Ұ��Ť��s��
 *
 * @return int 0=���\�A�D0=����
 */
extern int tuya_adapt_ble_start(void);

/**
 * @brief �����Ť��\��
 *
 * @return int 0=���\�A�D0=����
 */
extern int tuya_adapt_ble_deinit(void);

/**
 * @brief �o�e�s���ƾ�
 *
 * @param[in]       buf    �o�ebuf
 * @return int 0=���\�A�D0=����
 */
extern int tuya_adapt_ble_send(tuya_ble_data_buf_t *buf);

/**
 * @brief GAP disconnect
 * @return int 0=���\�A�D0=����
 */

extern int tuya_os_adapt_bt_gap_disconnect(void);

/**
 * @brief Get Connection RSSI
 * @return int 0=���\�A�D0=����
 */
extern int tuya_os_adapt_bt_get_rssi(SCHAR_T *rssi);

/**
 * @brief �����Ť��A�����ϥ�
 *
 * @param[out]       info
 * @return int 0=���\�A�D0=����
 */
//int tuya_os_adapt_bt_assign_scan(ty_bt_scan_info_t *info);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
