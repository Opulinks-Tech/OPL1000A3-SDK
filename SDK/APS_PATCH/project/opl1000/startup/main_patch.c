/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include <stdio.h>
#include "sys_init_patch.h"
#include "hal_patch.h"
#include "hal_vic.h"
#include "hal_dbg_uart.h"
#include "hal_pin.h"
#include "hal_pin_config_patch.h"
#include "hal_system.h"
#include "at_cmd_common.h"
#include "boot_sequence.h"


static void Main_AppInit_patch(void);
static void Main_PinMuxUpdate(void);
static void Sys_MiscModulesInit_patch(void);
static void Main_ApsUartRxDectecConfig(void);
static void Main_ApsUartRxDectecCb(E_GpioIdx_t tGpioIdx);

void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT")));
void __Patch_EntryPoint(void)
{
    /* Don't remove SysInit_EntryPoint */
    SysInit_EntryPoint();
    
    /* Uncomment this function when the device is without 32k XTAL */
    //Sys_SwitchTo32kRC();
    
    Hal_SysPinMuxAppInit = Main_PinMuxUpdate;
    Sys_MiscModulesInit = Sys_MiscModulesInit_patch;
        
    /* Application initialization */
    Sys_AppInit = Main_AppInit_patch;
}



/*************************************************************************
* FUNCTION:
*   Main_AppInit_patch
*
* DESCRIPTION:
*   the initial of application
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_AppInit_patch(void)
{
    // add the application initialization from here
    printf("AppInit\n");
}


static void Main_PinMuxUpdate(void)
{
    Hal_Pin_ConfigSet(0, HAL_PIN_TYPE_PATCH_IO_0, HAL_PIN_DRIVING_PATCH_IO_0);
    Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_PATCH_IO_1, HAL_PIN_DRIVING_PATCH_IO_1);
    Hal_Pin_ConfigSet(2, HAL_PIN_TYPE_PATCH_IO_2, HAL_PIN_DRIVING_PATCH_IO_2);
    Hal_Pin_ConfigSet(3, HAL_PIN_TYPE_PATCH_IO_3, HAL_PIN_DRIVING_PATCH_IO_3);
    Hal_Pin_ConfigSet(4, HAL_PIN_TYPE_PATCH_IO_4, HAL_PIN_DRIVING_PATCH_IO_4);
    Hal_Pin_ConfigSet(5, HAL_PIN_TYPE_PATCH_IO_5, HAL_PIN_DRIVING_PATCH_IO_5);
    Hal_Pin_ConfigSet(6, HAL_PIN_TYPE_PATCH_IO_6, HAL_PIN_DRIVING_PATCH_IO_6);
    Hal_Pin_ConfigSet(7, HAL_PIN_TYPE_PATCH_IO_7, HAL_PIN_DRIVING_PATCH_IO_7);
    Hal_Pin_ConfigSet(8, HAL_PIN_TYPE_PATCH_IO_8, HAL_PIN_DRIVING_PATCH_IO_8);
    Hal_Pin_ConfigSet(9, HAL_PIN_TYPE_PATCH_IO_9, HAL_PIN_DRIVING_PATCH_IO_9);
    Hal_Pin_ConfigSet(10, HAL_PIN_TYPE_PATCH_IO_10, HAL_PIN_DRIVING_PATCH_IO_10);
    Hal_Pin_ConfigSet(11, HAL_PIN_TYPE_PATCH_IO_11, HAL_PIN_DRIVING_PATCH_IO_11);
    Hal_Pin_ConfigSet(12, HAL_PIN_TYPE_PATCH_IO_12, HAL_PIN_DRIVING_PATCH_IO_12);
    Hal_Pin_ConfigSet(13, HAL_PIN_TYPE_PATCH_IO_13, HAL_PIN_DRIVING_PATCH_IO_13);
    Hal_Pin_ConfigSet(14, HAL_PIN_TYPE_PATCH_IO_14, HAL_PIN_DRIVING_PATCH_IO_14);
    Hal_Pin_ConfigSet(15, HAL_PIN_TYPE_PATCH_IO_15, HAL_PIN_DRIVING_PATCH_IO_15);
    Hal_Pin_ConfigSet(16, HAL_PIN_TYPE_PATCH_IO_16, HAL_PIN_DRIVING_PATCH_IO_16);
    Hal_Pin_ConfigSet(17, HAL_PIN_TYPE_PATCH_IO_17, HAL_PIN_DRIVING_PATCH_IO_17);
    Hal_Pin_ConfigSet(18, HAL_PIN_TYPE_PATCH_IO_18, HAL_PIN_DRIVING_PATCH_IO_18);
    Hal_Pin_ConfigSet(19, HAL_PIN_TYPE_PATCH_IO_19, HAL_PIN_DRIVING_PATCH_IO_19);
    Hal_Pin_ConfigSet(20, HAL_PIN_TYPE_PATCH_IO_20, HAL_PIN_DRIVING_PATCH_IO_20);
    Hal_Pin_ConfigSet(21, HAL_PIN_TYPE_PATCH_IO_21, HAL_PIN_DRIVING_PATCH_IO_21);
    Hal_Pin_ConfigSet(22, HAL_PIN_TYPE_PATCH_IO_22, HAL_PIN_DRIVING_PATCH_IO_22);
    Hal_Pin_ConfigSet(23, HAL_PIN_TYPE_PATCH_IO_23, HAL_PIN_DRIVING_PATCH_IO_23);
    
    at_io01_uart_mode_set(HAL_PIN_0_1_UART_MODE_PATCH);
}
/*************************************************************************
* FUNCTION:
*   Main_MiscDriverConfigSetup
*
* DESCRIPTION:
*   the initial of driver part for cold and warm boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Sys_MiscModulesInit_patch(void)
{
    //Hal_Wdt_Stop();   //disable watchdog here.

    
    // IO 1 : detect the GPIO high level if APS UART Rx is connected to another UART Tx port.
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        if (HAL_PIN_TYPE_PATCH_IO_1 == PIN_TYPE_UART_APS_RX)
        {
            Hal_DbgUart_RxIntEn(0);
            Main_ApsUartRxDectecConfig();
        }
        else
        {
            Hal_DbgUart_RxIntEn(1);
        }
    }
}



/*************************************************************************
* FUNCTION:
*   Main_ApsUartRxDectecConfig
*
* DESCRIPTION:
*   detect the GPIO high level if APS UART Rx is connected to another UART Tx port.
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_ApsUartRxDectecConfig(void)
{
    E_GpioLevel_t eGpioLevel;

    Hal_Pin_ConfigSet(1, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_LOW);
    eGpioLevel = Hal_Vic_GpioInput(GPIO_IDX_01);
    if (GPIO_LEVEL_HIGH == eGpioLevel)
    {
        // it is connected
        Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_PATCH_IO_1, HAL_PIN_DRIVING_PATCH_IO_1);
        Hal_DbgUart_RxIntEn(1);
    }
    else //if (GPIO_LEVEL_LOW == eGpioLevel)
    {
        // it is not conncected, set the high level to trigger the GPIO interrupt
        Hal_Vic_GpioCallBackFuncSet(GPIO_IDX_01, Main_ApsUartRxDectecCb);
        //Hal_Vic_GpioDirection(GPIO_IDX_01, GPIO_INPUT);
        Hal_Vic_GpioIntEn(GPIO_IDX_01, 1);
        Hal_Vic_GpioIntTypeSel(GPIO_IDX_01, INT_TYPE_LEVEL);
        Hal_Vic_GpioIntInv(GPIO_IDX_01, 0);
        Hal_Vic_GpioIntMask(GPIO_IDX_01, 0);
    }
}

/*************************************************************************
* FUNCTION:
*   Main_ApsUartRxDectecCb
*
* DESCRIPTION:
*   detect the GPIO high level if APS UART Rx is connected to another UART Tx port.
*
* PARAMETERS
*   1. tGpioIdx : Index of call-back GPIO
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_ApsUartRxDectecCb(E_GpioIdx_t tGpioIdx)
{
    // disable the GPIO interrupt
    Hal_Vic_GpioIntEn(GPIO_IDX_01, 0);

    // it it connected
    Hal_Pin_ConfigSet(1, HAL_PIN_TYPE_PATCH_IO_1, HAL_PIN_DRIVING_PATCH_IO_1);
    Hal_DbgUart_RxIntEn(1);
}
