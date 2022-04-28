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

#include "opl1000.h"
#include "cmsis_os.h"
#include "opl1000_it.h"

#include "hal_dbg_uart.h"
#include "hal_uart.h"
#include "hal_vic.h"
#include "hal_system.h"
#include "hal_tmr.h"
#include "hal_wdt.h"
#include "hal_i2c.h"

#include "ipc.h"
#include "diag_task.h"
#include "ps.h"


uint8_t IS_M0_READY = 1;

typedef void (*T_InterruptHandler)(void);
typedef void (*T_ExceptionDumpStack)(uint32_t u32RegPsp, uint32_t u32RegMsp, uint32_t u32RegLr);

RET_DATA T_InterruptHandler IPC0_IRQHandler_Entry;
RET_DATA T_InterruptHandler IPC1_IRQHandler_Entry;
RET_DATA T_InterruptHandler IPC2_IRQHandler_Entry;
RET_DATA T_InterruptHandler IPC3_IRQHandler_Entry;
RET_DATA T_InterruptHandler GPIO_IRQHandler_Entry;
RET_DATA T_InterruptHandler SCRT_IRQHandler_Entry;
RET_DATA T_InterruptHandler MSQ_IRQHandler_Entry;
RET_DATA T_InterruptHandler UARTDBG_IRQHandler_Entry;
RET_DATA T_InterruptHandler UART0_IRQHandler_Entry;
RET_DATA T_InterruptHandler UART1_IRQHandler_Entry;
RET_DATA T_InterruptHandler I2C_IRQHandler_Entry;
RET_DATA T_InterruptHandler SPI0_IRQHandler_Entry;
RET_DATA T_InterruptHandler SPI1_IRQHandler_Entry;
RET_DATA T_InterruptHandler SPI2_IRQHandler_Entry;
RET_DATA T_InterruptHandler TIM0_IRQHandler_Entry;
RET_DATA T_InterruptHandler TIM1_IRQHandler_Entry;
RET_DATA T_InterruptHandler WDT_IRQHandler_Entry;
RET_DATA T_InterruptHandler JTAG_IRQHandler_Entry;
RET_DATA T_InterruptHandler DMA_IRQHandler_Entry;
RET_DATA T_ExceptionDumpStack ExceptionDumpStack;

uint8_t is_M0_ready(void){
	return IS_M0_READY;
}
void ExceptionDumpStack_impl(uint32_t u32RegPsp, uint32_t u32RegMsp, uint32_t u32RegLr)
{   
    tracer_drct_printf("LR=0x%08X\n", u32RegLr);
    tracer_drct_printf("PSP MSP\n");
    tracer_drct_printf("00: 0x%08X 0x%08X\n", *(uint32_t*)(u32RegPsp+0x0), *(uint32_t*)(u32RegMsp+0x0));
    tracer_drct_printf("04: 0x%08X 0x%08X\n", *(uint32_t*)(u32RegPsp+0x4), *(uint32_t*)(u32RegMsp+0x4));
    tracer_drct_printf("08: 0x%08X 0x%08X\n", *(uint32_t*)(u32RegPsp+0x8), *(uint32_t*)(u32RegMsp+0x8));
    tracer_drct_printf("0C: 0x%08X 0x%08X\n", *(uint32_t*)(u32RegPsp+0xC), *(uint32_t*)(u32RegMsp+0xC));
    tracer_drct_printf("10: 0x%08X 0x%08X\n", *(uint32_t*)(u32RegPsp+0x10), *(uint32_t*)(u32RegMsp+0x10));
    tracer_drct_printf("14: 0x%08X 0x%08X\n", *(uint32_t*)(u32RegPsp+0x14), *(uint32_t*)(u32RegMsp+0x14));
    tracer_drct_printf("18: 0x%08X 0x%08X\n", *(uint32_t*)(u32RegPsp+0x18), *(uint32_t*)(u32RegMsp+0x18));
    tracer_drct_printf("1C: 0x%08X 0x%08X\n", *(uint32_t*)(u32RegPsp+0x1C), *(uint32_t*)(u32RegMsp+0x1C));

    tracer_drct_printf("Current task: %s\n\n\n", pcTaskGetName(osThreadGetId()));
}

void HardFault_Handler(void)
{
    uint32_t u32RegPsp, u32RegMsp, u32RegLr;

    u32RegLr = __get_LR();
    u32RegPsp = __get_PSP();
    u32RegMsp = __get_MSP();
    
    /* Stop WDT first, avoid WDT reset when clockswitch is from 1p2g */
    Hal_Wdt_Stop();
    
    tracer_drct_printf("Hard fault\nE000ED28: %08X %08X", reg_read(0xE000ED28), reg_read(0xE000ED2C));
    tracer_drct_printf(" %08X %08X %08X %08X\n\n", reg_read(0xE000ED30), reg_read(0xE000ED34), reg_read(0xE000ED38), reg_read(0xE000ED3C));
    
    ExceptionDumpStack(u32RegPsp, u32RegMsp, u32RegLr);
    Hal_Sys_SwResetAll();
}



// --------------------------------- ISR Entry---------------------------------
void IPC0_IRQHandler_Entry_impl(void)
{
    // Clear interrupt
    Hal_Vic_IpcIntClear(IPC_IDX_0);
	
    //IPC0 bit0
    #ifdef IPC_SUT
    ipc_peer_ready(1, 1);
    #endif
}

void IPC1_IRQHandler_Entry_impl(void)
{
    // Clear interrupt
    Hal_Vic_IpcIntClear(IPC_IDX_1);
	
    //IPC1 bit1
    #ifdef IPC_SUT
    ipc_peer_ready(1, 0);
    #endif
}

void IPC2_IRQHandler_Entry_impl(void)
{
    // Clear interrupt
    Hal_Vic_IpcIntClear(IPC_IDX_2);

    //IPC1 bit2
    ipc_proc();
}

void IPC3_IRQHandler_Entry_impl(void)
{
    // Clear interrupt
    Hal_Vic_IpcIntClear(IPC_IDX_3);
	
    //IPC1 bit3
    #ifdef IPC_SUT
        ipc_sut_proc();
    #endif

    // NOTE: used for IO protection of sleep
    ps_lock_gpio_apply();
}

void GPIO_IRQHandler_Entry_impl(void)
{
    E_GpioIdx_t eGpioIdx = GPIO_IDX_00;
    uint32_t u32Status = 0;

    // NOTE: used for IO protection of sleep
    ps_update_io_time();

    // Get status
    u32Status = Hal_Vic_GpioIntStatRead();

    for(eGpioIdx = GPIO_IDX_00; eGpioIdx<GPIO_IDX_MAX; eGpioIdx++)
    {
        if( u32Status & (1<<eGpioIdx) )
        {
            // Hook here...
            if(g_taHalVicGpioCallBack[eGpioIdx] != 0)
                g_taHalVicGpioCallBack[eGpioIdx](eGpioIdx);
            
            // Clear module interrupt
            Hal_Vic_GpioIntClear(eGpioIdx);
        }
    }

    // Clear VIC interrupt
    Hal_Vic_IntClear(GPIO_IRQn);
}

void SCRT_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void MSQ_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void UARTDBG_IRQHandler_Entry_impl(void)
{
    // Note: Level-sensitive interrupt
    uint32_t u32Temp = 0;
    uint32_t u32Status = 0; 

    // Get status
    u32Status = Hal_DbgUart_IntStatusRead();

    // Clear all module interrupts
    Hal_DbgUart_IntClear(u32Status);

    // Clear VIC interrupt
    Hal_Vic_IntClear(UARTDBG_IRQn);

    // Check status and handle interrupt
    if(u32Status & DBG_UART_INT_RX)
    {
        // ---- Rx interrupt case ----
        // Get data
        if(Hal_DbgUart_DataRecvTimeOut(&u32Temp,0) == 0)
        {
            // if get data, execute hook function
            if( DbgUart_RxCallBack != 0 )
                DbgUart_RxCallBack(u32Temp);
        }
    }
}

void UART0_IRQHandler_Entry_impl(void)
{
    // Note: Level-sensitive interrupt
    uint32_t u32Temp = 0;
    uint32_t u32InterruptId = 0;

    // Get status
    u32InterruptId = Hal_Uart_IntIdRead(UART_IDX_0);

    // Clear module interrupt 
    Hal_Uart_IntClear(UART_IDX_0, (E_UartIntId_t)u32InterruptId, &u32Temp);

    // Clear VIC interrupt
    Hal_Vic_IntClear(UART0_IRQn);

    if((u32InterruptId == UART_INT_RX_AVALIBLE) || (u32InterruptId == UART_INT_CHAR_TIMEOUT))
    {
        // ---- Rx interrupt case ----
        // Hook here ...
        if( Uart_0_RxCallBack != 0 )
            Uart_0_RxCallBack(u32Temp);
    }
}

void UART1_IRQHandler_Entry_impl(void)
{
    // Note: Level-sensitive interrupt
    uint32_t u32Temp = 0;
    uint32_t u32InterruptId = 0;

    // Get status
    u32InterruptId = Hal_Uart_IntIdRead(UART_IDX_1);

    // Clear module interrupt 
    Hal_Uart_IntClear(UART_IDX_1, (E_UartIntId_t)u32InterruptId, &u32Temp);

    // Clear VIC interrupt
    Hal_Vic_IntClear(UART1_IRQn);

    if((u32InterruptId == UART_INT_RX_AVALIBLE) || (u32InterruptId == UART_INT_CHAR_TIMEOUT))
    {
        // ---- Rx interrupt case ----
        // Hook here ...
        if( Uart_1_RxCallBack != 0 )
            Uart_1_RxCallBack(u32Temp);
    }
}

void I2C_IRQHandler_Entry_impl(void)
{
    // handle the I2C interrupt
    Hal_I2c_IntHandler();
    
    // Clear VIC interrupt
    Hal_Vic_IntClear(I2C_IRQn);
}

void SPI0_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void SPI1_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void SPI2_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void TIM0_IRQHandler_Entry_impl(void)
{
    // Clear module interrupt
    Hal_Tmr_IntClear(0);
    
    // Clear VIC part
    Hal_Vic_IntClear(TIM0_IRQn);
    
    // handle the timer 0 interrupt
    if(Tmr_CallBack[0] != NULL)
        Tmr_CallBack[0](0);
}

void TIM1_IRQHandler_Entry_impl(void)
{
    // Clear module interrupt
    Hal_Tmr_IntClear(1);
    
    // Clear VIC part
    Hal_Vic_IntClear(TIM1_IRQn);
    
    // handle the timer 1 interrupt
    if(Tmr_CallBack[1] != NULL)
        Tmr_CallBack[1](1);
}

void WDT_IRQHandler_Entry_impl(void)
{
    uint32_t u32RegPsp, u32RegMsp, u32RegLr;

    u32RegPsp = __get_PSP();
    u32RegMsp = __get_MSP();
    u32RegLr = __get_LR();
    
    Hal_Vic_IntClear(WDT_IRQn);
    tracer_drct_printf("WDT reset\n");
    ExceptionDumpStack(u32RegPsp, u32RegMsp, u32RegLr);
    Hal_Sys_SwResetAll();
}

void JTAG_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void DMA_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

// --------------------------------- ISR ---------------------------------
void IPC0_IRQHandler(void)
{
    IPC0_IRQHandler_Entry();
}

void IPC1_IRQHandler(void)
{
    IPC1_IRQHandler_Entry();
}

void IPC2_IRQHandler(void)
{
    IPC2_IRQHandler_Entry();
}

void IPC3_IRQHandler(void)
{
    IPC3_IRQHandler_Entry();
}

void GPIO_IRQHandler(void)
{
    GPIO_IRQHandler_Entry();
}  

void SCRT_IRQHandler(void)
{
    SCRT_IRQHandler_Entry();
}  

void MSQ_IRQHandler(void)
{
    MSQ_IRQHandler_Entry();
}

void UARTDBG_IRQHandler(void)
{
    UARTDBG_IRQHandler_Entry();
}

void UART0_IRQHandler(void)
{
    UART0_IRQHandler_Entry();
}

void UART1_IRQHandler(void)
{
    UART1_IRQHandler_Entry();
}

void I2C_IRQHandler(void)
{
    I2C_IRQHandler_Entry();
}

void SPI0_IRQHandler(void)
{
    SPI0_IRQHandler_Entry();
}

void SPI1_IRQHandler(void)
{
    SPI1_IRQHandler_Entry();
}

void SPI2_IRQHandler(void)
{
    SPI2_IRQHandler_Entry();
}

void TIM0_IRQHandler(void)
{
    TIM0_IRQHandler_Entry();
}

void TIM1_IRQHandler(void)
{
    TIM1_IRQHandler_Entry();
}

void WDT_IRQHandler(void)
{
    WDT_IRQHandler_Entry();
}

void JTAG_IRQHandler(void)
{
    JTAG_IRQHandler_Entry();
}

void DMA_IRQHandler(void)
{
    DMA_IRQHandler_Entry();
}

void ISR_Pre_Init(void)
{
    IPC0_IRQHandler_Entry    = IPC0_IRQHandler_Entry_impl;
    IPC1_IRQHandler_Entry    = IPC1_IRQHandler_Entry_impl;
    IPC2_IRQHandler_Entry    = IPC2_IRQHandler_Entry_impl;
    IPC3_IRQHandler_Entry    = IPC3_IRQHandler_Entry_impl;
    GPIO_IRQHandler_Entry    = GPIO_IRQHandler_Entry_impl;
    SCRT_IRQHandler_Entry    = SCRT_IRQHandler_Entry_impl;
    MSQ_IRQHandler_Entry     = MSQ_IRQHandler_Entry_impl;
    UARTDBG_IRQHandler_Entry = UARTDBG_IRQHandler_Entry_impl;
    UART0_IRQHandler_Entry   = UART0_IRQHandler_Entry_impl;
    UART1_IRQHandler_Entry   = UART1_IRQHandler_Entry_impl;
    I2C_IRQHandler_Entry     = I2C_IRQHandler_Entry_impl;
    SPI0_IRQHandler_Entry    = SPI0_IRQHandler_Entry_impl;
    SPI1_IRQHandler_Entry    = SPI1_IRQHandler_Entry_impl;
    SPI2_IRQHandler_Entry    = SPI2_IRQHandler_Entry_impl;
    TIM0_IRQHandler_Entry    = TIM0_IRQHandler_Entry_impl;
    TIM1_IRQHandler_Entry    = TIM1_IRQHandler_Entry_impl;
    WDT_IRQHandler_Entry     = WDT_IRQHandler_Entry_impl;
    JTAG_IRQHandler_Entry    = JTAG_IRQHandler_Entry_impl;
    DMA_IRQHandler_Entry     = DMA_IRQHandler_Entry_impl;
    
    ExceptionDumpStack       = ExceptionDumpStack_impl;
}

