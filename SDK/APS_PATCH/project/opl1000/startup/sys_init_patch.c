/* *****************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 * @file sys_init_patch.c
 *
 * @brief Patch for Sys init patch
 *
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "sys_init_patch.h"
#include "opl1000_it_patch.h"
#include "boot_sequence.h"
#include "hal_patch.h"
#include "hal_system_patch.h"
#include "hal_auxadc_patch.h"
#include "diag_task_patch.h"
#include "le_ctrl_patch.h"
#include "ps_patch.h"
#include "at_cmd_func_patch.h"
#include "mw_fim_patch.h"
#include "lwip_jmptbl_patch.h"
#include "controller_wifi_patch.h"
#include "wpa_cli_patch.h"
#include "driver_netlink_patch.h"
#include "wifi_service_func_init_patch.h"
#include "ble_host_patch_init.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
#if defined(GCC)
    extern uint32_t __bss_start__;
    extern uint32_t __bss_end__;
    #define ZI_REGION_START     (&__bss_start__)
    #define ZI_REGION_LENGTH    ((uint32_t) &__bss_end__ - (uint32_t) &__bss_start__)
#else
    extern unsigned int Image$$RW_IRAM1$$ZI$$Length;
    extern char Image$$RW_IRAM1$$ZI$$Base[];
    #define ZI_REGION_START     (Image$$RW_IRAM1$$ZI$$Base)
    #define ZI_REGION_LENGTH    ((unsigned int)&Image$$RW_IRAM1$$ZI$$Length)
#endif


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

/**
 * @bried Init patch function when cold boot
 */
void SysInit_EntryPoint(void)
{
    ISR_SetupHardfaultPatch();
    
    // Only for cold boot
    if (Boot_CheckWarmBoot())
        return;

    // init bss section
    memset(ZI_REGION_START, 0, ZI_REGION_LENGTH);

    // 0. Tracer

    // 1. hal patch

    // 2. boot sequence

    // 3. sys init

    // 4. IPC
    ISR_PatchInit();

    // 5. Control task

    // 6. Wifi
    wifi_ctrl_init_patch();
    wifi_api_func_init_patch();

    // 7. le_ctrl
    le_ctrl_pre_patch_init();

    // 8. le_host
    LeHostPatchAssign();
    
    // 9. LwIP
    lwip_module_interface_init_patch();

    // 10. WPAS
    wpa_cli_func_init_patch();
    wpa_driver_func_init_patch();

    // 11. AT
    at_func_init_patch();

    // 12. SCRT

    // 13. HAL driver API
    Hal_Sys_PatchInit();
    
    // 14. os

    // 15. util api

    // 16. Power Saving
    ps_patch_init();

    // 17. ISR

    // 18. DIAG
    Diag_PatchInit();

    // 19. FIM
    MwFim_PreInit_patch();

    // 20. AUXADC
    Hal_Aux_Patch_Init();

    // 21. Temperature

    // 22. Main

    // 23. Agent

    // 24. OTA

    // 25. System Common

    // 26. RF config

    // 27. User App Task
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */





