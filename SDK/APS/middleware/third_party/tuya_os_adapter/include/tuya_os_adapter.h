/**
 * @file tuya_os_adapter.h
 * @author sunkz@tuya.com
 * @brief 
 * @version 0.1
 * @date 2020-05-15
 * 
 * @copyright Copyright (c) tuya.inc 2019
 * 
 */
#ifndef _TUYA_OS_ADAPTER_H
#define _TUYA_OS_ADAPTER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "tuya_os_adapter_error_code.h"
#include "tuya_os_adapt_system.h"
#include "tuya_os_adapt_memory.h"
#include "tuya_cloud_types.h"
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************
*************************micro define***********************
***********************************************************/
typedef enum {
    INTF_MEMORY,
    INTF_MUTEX,
    INTF_SEMAPHORE,
    INTF_THREAD,
    INTF_FILE_SYSTEM,
    INTF_NETWORK,
    INTF_OUTPUT,
    INTF_SYSTEM,
    INTF_INTERRUPT,
    INTF_QUEUE,
    INTF_WIFI,
    INTF_BT,
    INTF_STORAGE,
    INTF_OTA,
    INTF_WIRED,
    ALL_INTF_NUMS
} INTF_TYPE_ENUM;

#define PR_DEBUG_RAW printf
#define PR_DEBUG printf
#define PR_NOTICE printf
#define PR_ERR printf
/********************************************************************************
 *********************************tuya_os_fs_intf********************************
 ********************************************************************************/
/* tuya sdk definition of 127.0.0.1 */
#define TY_IPADDR_LOOPBACK     ((uint32_t)0x7f000001UL)
/* tuya sdk definition of 0.0.0.0 */
#define TY_IPADDR_ANY          ((uint32_t)0x00000000UL)
/* tuya sdk definition of 255.255.255.255 */
#define TY_IPADDR_BROADCAST    ((uint32_t)0xffffffffUL)

/* tuya sdk definition of shutdown type */
#define UNW_SHUT_RD   0
#define UNW_SHUT_WR   1
#define UNW_SHUT_RDWR 2


/* tuya sdk definition of socket protocol */


/* tuya sdk definition of transfer type */


/* tuya sdk definition of IP addr */

/* tuya sdk definition of IP info */

//the MAC addr len

/* tuya sdk definition of MAC info */

/********************************************************************************
 *********************************tuya_os_storage_intf*****************************
 ********************************************************************************/
/**
 * @brief storage description
 * 
 */


#ifdef __cplusplus
}
#endif

#endif /* _TUYA_OS_ADAPTER_H */
