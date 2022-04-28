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

/******************************************************************************
*  Filename:
*  ---------
*  sys_init.c
*
*  Project:
*  --------
*  OPL1000 Project - the system initialize implement file
*
*  Description:
*  ------------
*  This implement file is include the system initialize function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "sys_init.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_gpio.h"
#include "hal_dbg_uart.h"
#include "hal_uart.h"
#include "hal_spi.h"
#include "hal_flash.h"
#include "hal_pwm.h"
#include "hal_auxadc.h"
#include "hal_wdt.h"
#include "mw_fim.h"
#include "at_cmd_common.h"
#include "ipc.h"
#include "msg.h"
#include "ps.h"
#include "boot_sequence.h"
#include "mw_fim_default_group01.h"
#include "controller_wifi_com.h"
#include "agent.h"
#include "mw_ota.h"
#include "network_config.h"
#include "sys_cfg.h"

#define __SVN_REVISION__
#define __DIAG_TASK__
//#define __AT_CMD_TASK__
//#define __WIFI_MAC_TASK__
//#define __LWIP_TASK__
#define __HW_CRYPTO_ENGINE__
//#define __WPA_SUPPLICANT__
#define __CONTROLLER_TASK__
//#define __BLE__

#if defined(__SVN_REVISION__)
#include "svn_rev.h"
#endif

#if defined(__DIAG_TASK__)
#include "diag_task.h"
#endif

#if defined(__AT_CMD_TASK__)
#include "at_cmd_task.h"
#endif

#if defined(__WIFI_MAC_TASK__)
#include "wifi_mac_task.h"
#endif

#if defined(__LWIP_TASK__)
extern void lwip_task_create(void);
#endif

#if defined(__HW_CRYPTO_ENGINE__)
#include "scrt.h"
#endif

#if defined(__WPA_SUPPLICANT__)
#include "wpa_supplicant_i.h"
#include "supplicant_task.h"
#endif

#if defined(__CONTROLLER_TASK__)
#include "controller_task.h"
#endif

#if defined(__BLE__)
#include "le_rtos_task.h"
#endif

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define BOOT_MODE_ICE       0x2
#define BOOT_MODE_JTAG      0x3
#define BOOT_MODE_NORMAL    0xA

#define WDT_TIMEOUT_SECS    10

#define DEV_32K_SRC_SEL_32K_XTAL    0   /* Default */
#define DEV_32K_SRC_SEL_32K_RC      IPC_SPARE0_SEQ_32K_SRC_SEL
/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
RET_DATA T_Sys_PowerSetup_fp Sys_PowerSetup;
RET_DATA T_Sys_ClockSetup_fp Sys_ClockSetup;
RET_DATA T_Sys_UartInit_fp Sys_UartInit;
RET_DATA T_Sys_MiscModulesInit_fp Sys_MiscModulesInit;
RET_DATA T_Sys_DriverInit_fp Sys_DriverInit;
RET_DATA T_Sys_WaitforMsqReady_fp Sys_WaitforMsqReady;
RET_DATA T_Sys_NotifyReadyToMsq_fp Sys_NotifyReadyToMsq;
RET_DATA T_Sys_SetUnsuedSramEndBound_fp Sys_SetUnsuedSramEndBound;
RET_DATA T_Sys_SwitchOffUnusedSram_fp Sys_SwitchOffUnusedSram;
RET_DATA T_Sys_SwitchTo32kRC_fp Sys_SwitchTo32kRC;
RET_DATA T_Sys_ServiceInit_fp Sys_ServiceInit;
RET_DATA T_Sys_AppInit_fp Sys_AppInit;
RET_DATA T_Sys_PostInit_fp Sys_PostInit;
RET_DATA T_Sys_StackOverflowHook_fp Sys_StackOverflowHook;
RET_DATA T_Sys_IdleHook_fp Sys_IdleHook;


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
uint32_t sysSramEndBound;


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   Sys_PowerSetup
*
* DESCRIPTION:
*   the initial for power driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_PowerSetup_impl(void)
{
    Hal_Sys_PowerDefaultSettings();
}

/*************************************************************************
* FUNCTION:
*   Sys_ClockSetup
*
* DESCRIPTION:
*   the initial for clock driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_ClockSetup_impl(void)
{
    if (!Boot_CheckWarmBoot())
    {
        Hal_Sys_DisableClock();
    }
}

/*************************************************************************
* FUNCTION:
*   Sys_UartInit
*
* DESCRIPTION:
*   1. get the config of UART from FIM
*   2. init the UART modules
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_UartInit_impl(void)
{
    T_HalUartConfig tUartConfig;

#if 0    
    // Init UART0
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP01_UART_CFG, 0, MW_FIM_UART_CFG_SIZE, (uint8_t*)&tUartConfig))
        {
            // if fail, get the default value
            memcpy(&tUartConfig, &g_tMwFimDefaultUartConfig, MW_FIM_UART_CFG_SIZE);
        }
    }
    // warm boot
    else
    {
        if (0 != Hal_Uart_ConfigGet(UART_IDX_0, &tUartConfig))
        {
            // if fail, get the default value
            memcpy(&tUartConfig, &g_tMwFimDefaultUartConfig, MW_FIM_UART_CFG_SIZE);
        }
    }
    Hal_Uart_Init(UART_IDX_0,
                  tUartConfig.ulBuadrate,
                  (E_UartDataBit_t)(tUartConfig.ubDataBit),
                  (E_UartParity_t)(tUartConfig.ubParity),
                  (E_UartStopBit_t)(tUartConfig.ubStopBit),
                  tUartConfig.ubFlowCtrl);
#endif
    
    // Init UART1
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP01_UART_CFG, 1, MW_FIM_UART_CFG_SIZE, (uint8_t*)&tUartConfig))
        {
            // if fail, get the default value
            memcpy(&tUartConfig, &g_tMwFimDefaultUartConfig, MW_FIM_UART_CFG_SIZE);
        }
    }
    // warm boot
    else
    {
        if (0 != Hal_Uart_ConfigGet(UART_IDX_1, &tUartConfig))
        {
            // if fail, get the default value
            memcpy(&tUartConfig, &g_tMwFimDefaultUartConfig, MW_FIM_UART_CFG_SIZE);
        }
    }
    Hal_Uart_Init(UART_IDX_1,
                  tUartConfig.ulBuadrate,
                  (E_UartDataBit_t)(tUartConfig.ubDataBit),
                  (E_UartParity_t)(tUartConfig.ubParity),
                  (E_UartStopBit_t)(tUartConfig.ubStopBit),
                  tUartConfig.ubFlowCtrl);
}

/*************************************************************************
* FUNCTION:
*   Sys_MiscModulesInit
*
* DESCRIPTION:
*   the initial for misc driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_MiscModulesInit_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

/*************************************************************************
* FUNCTION:
*   Sys_DriverInit
*
* DESCRIPTION:
*   the initial for driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_DriverInit_impl(void)
{
    // Wait for M0 initialization to be completed
    Sys_WaitforMsqReady(SYS_SPARE_MSQ_CLOCK_READY);

    // First restore APS clock after MSQ ready
	if (Boot_CheckWarmBoot())
	{
		Hal_Sys_ApsClkResume();
        Sys_NotifyReadyToMsq(SYS_SPARE_APS_CLOCK_READY);
    }

    // Set power
    Sys_PowerSetup();

    // Set system clock
    Sys_ClockSetup();

    // cold boot
    if (!Boot_CheckWarmBoot())
    {
        /* GPIO initial, needs to setup before pinmux init */
        Hal_Gpio_Init();
        
        // Set pin-mux
        Hal_SysPinMuxAppInit();
        
        // Init VIC
        Hal_Vic_Init();

	    // Init GPIO
	    Hal_Vic_GpioInit();
    }
    // Fetch GPIO interrupt status at warm-boot
    else
    {
        ps_update_boot_gpio_int_status(Hal_Vic_GpioIntStatRead());
    }

	// Init IPC
    Hal_Vic_IpcIntEn(IPC_IDX_0, 1);
    Hal_Vic_IpcIntEn(IPC_IDX_1, 1);
    Hal_Vic_IpcIntEn(IPC_IDX_2, 1);
    Hal_Vic_IpcIntEn(IPC_IDX_3, 1);

    // Init DBG_UART
    Hal_DbgUart_Init(115200);
    printf("\n");

    // Init SPI 0
    Hal_Spi_Init(SPI_IDX_0, SystemCoreClockGet()/2,
        SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1);

    // Init flash on SPI 0
    Hal_Flash_Init(SPI_IDX_0);

    // FIM
    MwFim_Init();

    // Init UART1
    Sys_UartInit();
    
    //-------------------------------------------------------------------------------------
    // Other driver config need by Task-level (sleep strategy)

    // Diag task
    Hal_DbgUart_RxCallBackFuncSet(uartdbg_rx_int_handler);
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        // ICE or JTag
        if ((BOOT_MODE_ICE == Hal_Sys_StrapModeRead()) || (BOOT_MODE_JTAG == Hal_Sys_StrapModeRead()))
        {
            // the default is on
            Hal_DbgUart_RxIntEn(1);
        }
        // others
        else
        {
            // the default is off
            Hal_DbgUart_RxIntEn(0);
        }
    }
    // warm boot
    else
    {
        Hal_DbgUart_RxIntEn(Hal_DbgUart_RxIntEnStatusGet());
    }

    // HCI and AT command
    uart1_mode_set_default();

	// power-saving module init
	ps_init();

	if (Boot_CheckWarmBoot())
	{
		// TODO: Revision will be provided by Ophelia after peripheral restore mechanism completed
		uart1_mode_set_default();
		uart1_mode_set_at();

        // Init GPIO
        Hal_Vic_GpioInit();
	}

    //Watch Dog
    if (Hal_Sys_StrapModeRead() == BOOT_MODE_NORMAL)
    {
        Hal_Vic_IntTypeSel(WDT_IRQn, INT_TYPE_FALLING_EDGE);
        Hal_Vic_IntInv(WDT_IRQn, 1);
        Hal_Wdt_Init(WDT_TIMEOUT_SECS * SystemCoreClockGet());
    }
    
    // Other modules' init
    Sys_MiscModulesInit();
}

/*************************************************************************
* FUNCTION:
*   Sys_SetUnsuedSramEndBound
*
* DESCRIPTION:
*   set unused SRAM end bound
*
* PARAMETERS
*   endAddr: APS memory footprint from high location address, this changes according by chip version, projects
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_SetUnsuedSramEndBound_impl(uint32_t endAddr)
{
    sysSramEndBound = endAddr;
}

/*************************************************************************
* FUNCTION:
*   Sys_SwitchOffUnusedSram
*
* DESCRIPTION:
*   to reduce current consumption, switch off unused SRAM blocks
*
* PARAMETERS
*   memFootPrint: APS memory footprint, this changes according by chip version, projects
*   offMskBase: The base switch value, it depends on actual foot-print derived from SHM or others
* RETURNS
*   none
*
*************************************************************************/
void Sys_SwitchOffUnusedSram_impl(uint32_t memFootPrint, uint32_t offMskBase)
{
    typedef struct {
        uint32_t idxMsk;
        uint32_t startAddr;
        uint32_t endAddr;
    } S_SramSwitchMap;

    S_SramSwitchMap mapping[] = {
        (1 << 4), 0x00418000, 0x00420000,  // block 4 off
        (1 << 5), 0x00420000, 0x00428000,  // block 5 off
        (1 << 6), 0x00428000, 0x00430000,  // block 6 off
        (1 << 7), 0x00430000, 0x00438000,  // block 7 off
        (1 << 8), 0x00438000, 0x00440000,  // block 8 off
    };

    //uint32_t offMsk = (1 << 11);           // block 11 off (shared memory 0)
    //uint32_t offMsk = 0;                   // block 11 on (shared memory 0)
    int i;

    if (!sysSramEndBound)
        sysSramEndBound = 0x00440000;

    for (i = 0; i < sizeof(mapping) / sizeof(S_SramSwitchMap); i++)
    {
        S_SramSwitchMap entry = mapping[i];

        if (memFootPrint > entry.startAddr)
            continue;
        else if (sysSramEndBound < entry.endAddr)
            break;
        else
            offMskBase |= entry.idxMsk;
    }

    Hal_Sys_RetRamTurnOff(offMskBase);
}

/*************************************************************************
* FUNCTION:
*   Sys_SwitchTo32kRC
*
* DESCRIPTION:
*   Must called at __Patch_EntryPoint
*   Select 32k RC as RTC timer/SEQ clock source
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_SwitchTo32kRC_impl(void)
{
    uint32_t u32SpareReg;

    if (Boot_CheckWarmBoot())
        return;
    
    while (1) {
        Hal_Sys_SpareRegRead(SPARE_0, &u32SpareReg);
        
        /* Wait MSQ get load patch done finished, avoid data corruption */
        if ((u32SpareReg & IPC_SPARE0_LOAD_PATCH_READY) == 0)
            break;
    } 
    
    u32SpareReg |= DEV_32K_SRC_SEL_32K_RC;
    Hal_Sys_SpareRegWrite(SPARE_0, u32SpareReg);
}


/*************************************************************************
* FUNCTION:
*   Sys_WaitforMsqReady
*
* DESCRIPTION:
*   wait for M0 initialization to be completed
*
* PARAMETERS
*   indicator: wait item including
*   [4]: MSQ clock ready
*   [6]: MSQ flash access done
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_WaitforMsqReady_impl(uint32_t indicator)
{
	const uint32_t m0_ready_msk = 1 << 4;
	uint32_t reg_spare_0_val;

	do {
		Hal_Sys_SpareRegRead(SPARE_0, &reg_spare_0_val);
	} while (!(reg_spare_0_val & m0_ready_msk));

	Hal_Sys_SpareRegWrite(SPARE_0, reg_spare_0_val & ~m0_ready_msk);
}

/*************************************************************************
* FUNCTION:
*   Sys_NotifyReadyToMsq
*
* DESCRIPTION:
*   notify M0 aps completed items
*
* PARAMETERS
*   indicator: notify items including
*   [7]: APS clock ready
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_NotifyReadyToMsq_impl(uint32_t indicator)
{
	uint32_t reg_spare_0_val;

	Hal_Sys_SpareRegRead(SPARE_0, &reg_spare_0_val);
	Hal_Sys_SpareRegWrite(SPARE_0, reg_spare_0_val | indicator);
}

/*************************************************************************
* FUNCTION:
*   Sys_ServiceInit
*
* DESCRIPTION:
*   the initial for service
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_ServiceInit_impl(void)
{
    T_MwOtaLayoutInfo tLayout;
    
    Diag_ShowRevision(NULL);

#if defined(__DIAG_TASK__)
    diag_task_create();
#endif

#if defined(__AT_CMD_TASK__)
    at_task_create();
#endif

#if defined(__WIFI_MAC_TASK__)
    wifi_mac_task_create();
#endif

#if defined(__LWIP_TASK__)
    lwip_task_create();
#endif

#if defined(__HW_CRYPTO_ENGINE__)
    nl_scrt_Init();
#endif

#if defined(__WPA_SUPPLICANT__)
    do_supplicant_init();
#endif

#if defined(__CONTROLLER_TASK__)
	controller_task_create();
#endif

    ipc_init();

#if defined(__BLE__)
    LeRtosTaskCreat();
#endif

#if defined(__WIFI_AUTO_CONNECT__)
    auto_connect_init();
#endif

    // Agent
    agent_init();

    // Load param from FIM for Tracer
    tracer_load();
    
    // OTA
    tLayout.ulaHeaderAddr[0] = MW_OTA_HEADER_ADDR_1;
    tLayout.ulaHeaderAddr[1] = MW_OTA_HEADER_ADDR_2;
    tLayout.ulaImageAddr[0] = MW_OTA_IMAGE_ADDR_1;
    tLayout.ulaImageAddr[1] = MW_OTA_IMAGE_ADDR_2;
    tLayout.ulImageSize = MW_OTA_IMAGE_SIZE;
    MwOta_Init(&tLayout, 0);

    // DHCP ARP check
    tcpip_config_dhcp_arp_check_init();

    // SYS config
    sys_cfg_init();
}

/*************************************************************************
* FUNCTION:
*   Sys_AppInit
*
* DESCRIPTION:
*   the initial for application
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_AppInit_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

/*************************************************************************
* FUNCTION:
*   Sys_PostInit
*
* DESCRIPTION:
*   the post initial for sys init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_PostInit_impl(void)
{
    /* Patch Function */
    sys_cfg_rf_init(NULL);
    sys_cfg_clk_init();
    tracer_init();
}

/*************************************************************************
* FUNCTION:
*   Sys_StackOverflowHook
*
* DESCRIPTION:
*   the hook function of stack overflow
*
* PARAMETERS
*   1. xTask      : [In] the pointer of task handle
*   2. pcTaskName : [In] the task name
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_StackOverflowHook_impl(TaskHandle_t xTask, char *pcTaskName)
{
	tracer_drct_printf("stack overflow: %x %s\r\n", (unsigned int)xTask, (portCHAR *)pcTaskName);
    
    while(1) {}
}

/*************************************************************************
* FUNCTION:
*   Sys_IdleHook
*
* DESCRIPTION:
*   the hook function of idle task
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_IdleHook_impl(void)
{
    if (Hal_Sys_StrapModeRead() == BOOT_MODE_NORMAL)
    {
        //Clear watchdog interrupt & reload conter
        Hal_Wdt_Clear();
    }
    
	ps_sleep();
}

/*************************************************************************
* FUNCTION:
*   Sys_PreInit
*
* DESCRIPTION:
*   the previous initial for sys init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_PreInit(void)
{
    Sys_PowerSetup = Sys_PowerSetup_impl;
    Sys_ClockSetup = Sys_ClockSetup_impl;
    Sys_UartInit = Sys_UartInit_impl;
    Sys_MiscModulesInit = Sys_MiscModulesInit_impl;
    Sys_DriverInit = Sys_DriverInit_impl;
    Sys_WaitforMsqReady = Sys_WaitforMsqReady_impl;
    Sys_NotifyReadyToMsq = Sys_NotifyReadyToMsq_impl;
    Sys_SetUnsuedSramEndBound = Sys_SetUnsuedSramEndBound_impl;
    Sys_SwitchOffUnusedSram = Sys_SwitchOffUnusedSram_impl;
    Sys_SwitchTo32kRC = Sys_SwitchTo32kRC_impl;
    Sys_ServiceInit = Sys_ServiceInit_impl;
    Sys_AppInit = Sys_AppInit_impl;
    Sys_PostInit = Sys_PostInit_impl;
    Sys_StackOverflowHook = Sys_StackOverflowHook_impl;
    Sys_IdleHook = Sys_IdleHook_impl;
}
