/******************************************************************************
*  Copyright 2017 - 2020, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2020
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File

// init
#include "opl_os_adapt_init.h"

// platform
#include "opl_os_adapt_platform.h"

// driver
#include "opl_os_adapt_ble_app.h"
#include "opl_os_adapt_ble_gatt.h"
#include "opl_os_adapt_bt.h"
#include "opl_os_adapt_i2c.h"
#include "opl_os_adapt_ota.h"
#include "opl_os_adapt_pin.h"
#include "opl_os_adapt_rtc.h"
#include "opl_os_adapt_storage.h"
#include "opl_os_adapt_uart.h"
#include "opl_os_adapt_wifi.h"

// system
#include "opl_os_adapt_memory.h"
#include "opl_os_adapt_mutex.h"
#include "opl_os_adapt_network.h"
#include "opl_os_adapt_output.h"
#include "opl_os_adapt_queue.h"
#include "opl_os_adapt_semaphore.h"
#include "opl_os_adapt_system.h"
#include "opl_os_adapt_thread.h"
#include "opl_os_adapt_timer.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable

// Sec 7: declaration of static function prototype


void opl_os_adapt_pre_init(void)
{
    // init
    opl_os_adapt_init_pre_init();

    // platform
    opl_os_adapt_platform_pre_init();

    // driver
    opl_os_adapt_ble_app_pre_init();
    opl_os_adapt_ble_gatt_pre_init();
    opl_os_adapt_bt_pre_init();
    opl_os_adapt_i2c_pre_init();
    opl_os_adapt_ota_pre_init();
    opl_os_adapt_pin_pre_init();
    opl_os_adapt_rtc_pre_init();
    opl_os_adapt_storage_pre_init();
    opl_os_adapt_uart_pre_init();
    opl_os_adapt_wifi_pre_init();

    // system
    opl_os_adapt_memory_pre_init();
    opl_os_adapt_mutex_pre_init();
    opl_os_adapt_network_pre_init();
    opl_os_adapt_output_pre_init();
    opl_os_adapt_queue_pre_init();
    opl_os_adapt_semaphore_pre_init();
    opl_os_adapt_system_pre_init();
    opl_os_adapt_thread_pre_init();
    opl_os_adapt_timer_pre_init();

    return;
}

