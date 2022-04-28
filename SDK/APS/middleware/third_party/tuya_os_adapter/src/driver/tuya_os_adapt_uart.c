 /*============================================================================
 *                                                                            *
 * Copyright (C) by Tuya Inc                                                  *
 * All rights reserved                                                        *
 *                                                                            *
 *                                                                            *
 =============================================================================*/

/*============================ INCLUDES ======================================*/
#include "tuya_uart.h"
#include "tuya_os_adapter_error_code.h"
#include "hal_dbg_uart.h"
#include "hal_uart.h"
#include "hal_system.h"
#include "opl_os_adapt_uart.h"

/*============================ MACROS ========================================*/
#define UART_DEV_NUM            2
/*============================ TYPES =========================================*/
typedef struct {
    tuya_uart_t         dev;
    tuya_uart_isr_cb    isr_cb;
} uart_dev_t;
/*============================ PROTOTYPES ====================================*/

static int uart_os_adapt_init        (tuya_uart_t *uart, tuya_uart_cfg_t *cfg);
static int uart_os_adapt_write_byte  (tuya_uart_t *uart, uint8_t byte);
static int uart_os_adapt_read_byte   (tuya_uart_t *uart, uint8_t *byte);
static int uart_os_adapt_control     (tuya_uart_t *uart, uint8_t cmd, void *arg);
static int uart_os_adapt_deinit      (tuya_uart_t *uart);
/*============================ LOCAL VARIABLES ===============================*/
static uart_dev_t s_uart_dev[UART_DEV_NUM];

static const tuya_uart_ops_t  uart_dev_ops = {
    .init       = uart_os_adapt_init,
    .read_byte  = uart_os_adapt_read_byte,
    .write_byte = uart_os_adapt_write_byte,
    .control    = uart_os_adapt_control,
    .deinit     = uart_os_adapt_deinit,
};
static uint8_t g_udata;
/*============================ GLOBAL VARIABLES ==============================*/
RET_DATA t_opl_platform_uart_init_fp opl_platform_uart_init;
RET_DATA t_opl_OPL1000_irq_handler_fp opl_uart_irq_handler;
RET_DATA t_opl_uart_os_adapt_init_fp opl_uart_os_adapt_init;
RET_DATA t_opl_uart_os_adapt_control_fp opl_uart_os_adapt_control;
RET_DATA t_opl_uart_os_adapt_write_byte_fp opl_uart_os_adapt_write_byte;
RET_DATA t_opl_uart_os_adapt_read_byte_fp opl_uart_os_adapt_read_byte;
RET_DATA t_opl_uart_os_adapt_deinit_fp opl_uart_os_adapt_deinit;
/*============================ IMPLEMENTATION ================================*/
int platform_uart_init(void)
{
    return opl_platform_uart_init();
}

static void OPL1000_irq_handler(uint32_t udata)
{
    opl_uart_irq_handler(udata);
}

static int uart_os_adapt_init(tuya_uart_t *uart, tuya_uart_cfg_t *cfg)
{
    return opl_uart_os_adapt_init(uart, cfg);
}

static int uart_os_adapt_control(tuya_uart_t *uart, uint8_t cmd, void *arg)
{
    return opl_uart_os_adapt_control(uart, cmd, arg);
}

static int uart_os_adapt_write_byte(tuya_uart_t *uart, uint8_t byte)
{
    return opl_uart_os_adapt_write_byte(uart, byte);
}

static int uart_os_adapt_read_byte(tuya_uart_t *uart, uint8_t *byte)
{
    return opl_uart_os_adapt_read_byte(uart, byte);
}

static int uart_os_adapt_deinit(tuya_uart_t *uart)
{
    return opl_uart_os_adapt_deinit(uart);
}

int platform_uart_init_impl(void)
{
    s_uart_dev[TUYA_UART1].dev.ops = (tuya_uart_ops_t *)&uart_dev_ops;
    TUYA_UART_8N1_CFG(&s_uart_dev[TUYA_UART1].dev, TUYA_UART_BAUDRATE_9600, 256, 0);
#if 1
    // tuya_driver_register: to be implemented
    return -1;
#else
    tuya_driver_register(&s_uart_dev[TUYA_UART1].dev.node, TUYA_DRV_UART, TUYA_UART1);
    return 0;
#endif
}

static void OPL1000_irq_handler_impl(uint32_t udata)
{
    #if 0
    uint32_t u32Temp = 0;
    if(event == INT_RX_EVENT) { // to do      
        u32Temp = Hal_Uart_RxIntEn( eUartIdx, 1);
        Hal_Uart_IntClear( eUartIdx, UART_INT_RX_AVALIBLE, &u32Temp);
        uart_dev_t *uart = &s_uart_dev[eUartIdx]; 
        if(uart->isr_cb)
        uart->isr_cb(&uart->dev, TUYA_UART_INT_RX_EVENT);
    }
    #endif
    g_udata = udata;
    uart_dev_t *uart = &s_uart_dev[1]; 
    if(uart->isr_cb)
    uart->isr_cb(&uart->dev, TUYA_UART_INT_RX_EVENT);
}



static int uart_os_adapt_init_impl(tuya_uart_t *uart, tuya_uart_cfg_t *cfg)
{
    E_UartIdx_t     eUartIdx    = UART_IDX_0;
    uint32_t        u32Baud     = 115200; 
    E_UartDataBit_t u8Bits      = DATA_BIT_8;
    E_UartParity_t  u8Parity    = PARITY_NONE;
    E_UartStopBit_t u8StopBit   = STOP_BIT_1;
    uint8_t         u8EnFlowCtl = 0;

	if( uart == NULL)
        return OPRT_OS_ADAPTER_UART_INIT_FAILED;

    if( uart->node.port > UART_IDX_MAX )
        return OPRT_OS_ADAPTER_UART_INIT_FAILED;
    else
        eUartIdx = (E_UartIdx_t)uart->node.port;
    switch(uart->cfg.baudrate )
    {
        case TUYA_UART_BAUDRATE_300:
        case TUYA_UART_BAUDRATE_600:
        case TUYA_UART_BAUDRATE_1200:
        case TUYA_UART_BAUDRATE_2400:
        case TUYA_UART_BAUDRATE_4800:
        case TUYA_UART_BAUDRATE_9600:
        case TUYA_UART_BAUDRATE_19200:
        case TUYA_UART_BAUDRATE_38400:
        case TUYA_UART_BAUDRATE_57600:
        case TUYA_UART_BAUDRATE_74880:
        case TUYA_UART_BAUDRATE_115200:
        case TUYA_UART_BAUDRATE_230400:
        case TUYA_UART_BAUDRATE_460800:
            u32Baud = uart->cfg.baudrate;
        break;

        case TUYA_UART_BAUDRATE_921600:
        case TUYA_UART_BAUDRATE_1500000:
        case TUYA_UART_BAUDRATE_1843200:
        case TUYA_UART_BAUDRATE_3686400:
        default:
            return OPRT_OS_ADAPTER_UART_INIT_FAILED;
    }

    switch( uart->cfg.databits )
    {
        case TUYA_UART_DATA_BIT5:
            u8Bits = DATA_BIT_5;
        break;
        case TUYA_UART_DATA_BIT6:
            u8Bits = DATA_BIT_6;
        break;
        case TUYA_UART_DATA_BIT7:
            u8Bits = DATA_BIT_7;
        break;
        case TUYA_UART_DATA_BIT8:
            u8Bits = DATA_BIT_8;
        break;

        default:
            return OPRT_OS_ADAPTER_UART_INIT_FAILED;
    }

    switch( uart->cfg.stopbits )
    {
        case TUYA_UART_STOP_BIT1:
            u8StopBit = STOP_BIT_1;
        break;
        case TUYA_UART_STOP_BIT1_5:
            u8StopBit = STOP_BIT_1P5;
        break;
        case TUYA_UART_STOP_BIT2:
            u8StopBit = STOP_BIT_2;
        break;

        default:
            return OPRT_OS_ADAPTER_UART_INIT_FAILED;
    }

    switch(uart->cfg.parity)
    {
        case TUYA_UART_PARITY_NONE:
            u8Parity = PARITY_NONE;
        break;
        case TUYA_UART_PARITY_ODD:
            u8Parity = PARITY_ODD;
        break;
        case TUYA_UART_PARITY_EVEN:
            u8Parity = PARITY_EVEN;
        break;

        default:
            return OPRT_OS_ADAPTER_UART_INIT_FAILED;
    } 

    if( 0 == Hal_Uart_Init( eUartIdx, u32Baud, u8Bits, u8Parity, u8StopBit, u8EnFlowCtl) )
        return OPRT_OS_ADAPTER_OK;
    else
        return OPRT_OS_ADAPTER_UART_INIT_FAILED;
}

static int uart_os_adapt_control_impl(tuya_uart_t *uart, uint8_t cmd, void *arg)
{
    int32_t i32Result = OPRT_OS_ADAPTER_UART_CONTROL_FAILED;
    uint32_t u32Temp = 0;
    E_UartIdx_t eUartIdx = UART_IDX_0;
    uart_dev_t *uart_dev = (uart_dev_t *)uart;
    if( uart->node.port > UART_IDX_MAX )
        return OPRT_OS_ADAPTER_UART_CONTROL_FAILED;
    else
        eUartIdx = (E_UartIdx_t)uart->node.port;

    switch(cmd)
    {
        // From tuya_drv_flag_t
        case TUYA_DRV_INT_RX_FLAG:
        {
            u32Temp = Hal_Uart_IntIdRead( eUartIdx );
            * (uint32_t *)arg = u32Temp;
            i32Result = OPRT_OS_ADAPTER_OK;
        }
        break;
        case TUYA_DRV_INT_TX_FLAG:
        {
            i32Result = OPRT_OS_ADAPTER_UART_CONTROL_FAILED;
        }
        break;

        // From tuya_drv_cmd_t
        case TUYA_DRV_SET_INT_CMD:
        {
            u32Temp = Hal_Uart_RxIntEn( eUartIdx, 1);
            if( u32Temp )
                i32Result = OPRT_OS_ADAPTER_UART_CONTROL_FAILED;
            else
                i32Result = OPRT_OS_ADAPTER_OK;
        }
        break;
        case TUYA_DRV_CLR_INT_CMD:
        {
            u32Temp = Hal_Uart_RxIntEn( eUartIdx, 0);
            if( u32Temp )
            {
                i32Result = OPRT_OS_ADAPTER_UART_CONTROL_FAILED;
            }else{
                i32Result = OPRT_OS_ADAPTER_OK;
                Hal_Uart_IntClear( eUartIdx, UART_INT_RX_AVALIBLE, &u32Temp);
            }
        }
        break;
        case TUYA_DRV_SET_ISR_CMD:
        {
            uart_dev->isr_cb = (tuya_uart_isr_cb)arg;
            Hal_Uart_RxCallBackFuncSet((E_UartIdx_t)uart_dev->dev.node.port, OPL1000_irq_handler);
            i32Result = OPRT_OS_ADAPTER_OK;
        }
        break;

        default:
            i32Result = OPRT_OS_ADAPTER_UART_CONTROL_FAILED;
    }
    return i32Result;
}

/**
 * @brief ?¨ä??‘é€ä?ä¸ªå???
 * 
 * @param[in]  uart     ä¸²å£?¥æ?
 * @param[in]  byte     è¦å??ç?å­—è?
 */

static int uart_os_adapt_write_byte_impl(tuya_uart_t *uart, uint8_t byte)
{
    uint32_t u32Temp = 0;
    E_UartIdx_t eUartIdx = UART_IDX_0;

    if( uart->node.port > UART_IDX_MAX )
        return OPRT_OS_ADAPTER_UART_SEND_FAILED;
    else
        eUartIdx = (E_UartIdx_t)uart->node.port;

    u32Temp = byte;
    if( 0 == Hal_Uart_DataSend( eUartIdx, u32Temp) )
        return OPRT_OS_ADAPTER_OK;
    else
        return OPRT_OS_ADAPTER_UART_SEND_FAILED;
}

/**
 * @brief ?¨ä?è¯»å?ä¸€ä¸ªå???
 * 
 * @param[in]   uart     ä¸²å£?¥æ?
 * @param[out]  ch       è¦è¯»?–ç?å­—è??‡é?
 */
static int uart_os_adapt_read_byte_impl(tuya_uart_t *uart, uint8_t *byte)
{
#if 0
    uint32_t u32Temp = 0;
    E_UartIdx_t eUartIdx = UART_IDX_1;
#endif
    *byte = g_udata;
    return OPRT_OS_ADAPTER_OK;
#if 0
    if( uart->node.port > UART_IDX_MAX )
        return OPRT_OS_ADAPTER_UART_READ_FAILED;
    else
        eUartIdx = (E_UartIdx_t)uart->node.port;

    if( 0 == Hal_Uart_DataRecv( eUartIdx, &u32Temp) )
    {
        *byte = u32Temp;
        return OPRT_OS_ADAPTER_OK;
    }
    else
        return OPRT_OS_ADAPTER_UART_READ_FAILED;
#endif
}

/**
 * @brief ?¨ä??Šæ”¾ä¸²å£
 * 
 * @param[in]  port     ä¸²å£?¥æ?
 */
static int uart_os_adapt_deinit_impl(tuya_uart_t *uart)
{
    int ret = OPRT_OS_ADAPTER_UART_DEINIT_FAILED;
    if( uart->node.port == TUYA_UART0 )
    {
        Hal_Sys_ApsClkEn(0, APS_CLK_UART_0);
        Hal_Sys_ApsModuleRst(APS_RST_UART_0);
        ret = OPRT_OS_ADAPTER_OK;
    }
    else if( uart->node.port == TUYA_UART1 )
    {
        Hal_Sys_ApsClkEn(0, APS_CLK_UART_1);
        Hal_Sys_ApsModuleRst(APS_RST_UART_1);
        ret = OPRT_OS_ADAPTER_OK;
    }

    return ret;
}

void opl_os_adapt_uart_pre_init(void)
{
    opl_platform_uart_init = platform_uart_init_impl;
    opl_uart_irq_handler = OPL1000_irq_handler_impl;
    opl_uart_os_adapt_init = uart_os_adapt_init_impl;
    opl_uart_os_adapt_control = uart_os_adapt_control_impl;
    opl_uart_os_adapt_write_byte = uart_os_adapt_write_byte_impl;
    opl_uart_os_adapt_read_byte = uart_os_adapt_read_byte_impl;
    opl_uart_os_adapt_deinit = uart_os_adapt_deinit_impl;
    return;
}
// --------------------------------------------------------------------------
// For debug test
#if TUYA_DEDBG

#include <stdlib.h>
#include <string.h>

tuya_uart_t     g_uart;
tuya_uart_cfg_t g_uart_cfg;

void Tuya_Uart_RxCallBack(uint32_t u32Buff)
{
    printf("Got %c(= 0x%X)\n\r", u32Buff, u32Buff);
}

E_CLI_CMD_PROC test_cmd(char *pbuf, int len)
{
    E_CLI_CMD_PROC tRet = CMD_CONTINUE;
    char *argv[16] = {0};
    uint32_t dwNum = 16;
    uint32_t argc = 0;
    uint32_t u32Mode = 0;
    int32_t i32Temp = 0;
    uint32_t u32Temp = 0;
    uint8_t u8Temp = 0;

    if(strncmp(pbuf, "uart", 4))
    {
        goto done;
    }

    printf("\n");

    argc = ParseParam(pbuf, argv, dwNum);

    tRet = CMD_FINISHED;

    if(argc > 1)
    {
        u32Mode = strtoul(argv[1], NULL, 10);
    }
    else
    {
        goto done;
    }

    if(argc > 2)
        g_uart.node.port = strtoul(argv[2], NULL, 10);
    else
        g_uart.node.port = 1;

    g_uart.cfg.baudrate = TUYA_UART_BAUDRATE_115200;
    g_uart.cfg.databits = TUYA_UART_DATA_BIT8;
    g_uart.cfg.stopbits = TUYA_UART_STOP_BIT1;
    g_uart.cfg.parity   = TUYA_UART_PARITY_NONE;

    switch( u32Mode )
    {
        case 0:
        {
            printf("tuya_uart_init\n\r");
            i32Temp = tuya_uart_init( &g_uart, &g_uart_cfg);
            if( i32Temp == OPRT_OS_ADAPTER_OK)
                printf("OK\n\r");
            else
                printf("Fail\n\r");
        }
        break;
        case 1:
        {
            printf("tuya_uart_control\n\r");
            if(argc > 3)
                u8Temp = strtoul(argv[3], NULL, 10);
            switch(u8Temp)
            {
                // From tuya_drv_flag_t
                case TUYA_DRV_INT_RX_FLAG: // 1
                {
                    printf("TUYA_DRV_INT_RX_FLAG\n\r");
                    i32Temp = uart_os_adapt_control( &g_uart, u8Temp, (void *)&u32Temp);
                    printf("Status = 0x%08X\n\r", u32Temp);
                }
                break;
                case TUYA_DRV_INT_TX_FLAG: // 2
                {
                    printf("TUYA_DRV_INT_TX_FLAG\n\r");
                    i32Temp = uart_os_adapt_control( &g_uart, u8Temp, 0);
                }
                break;

                // From tuya_drv_cmd_t
                case TUYA_DRV_SET_INT_CMD: // 3
                {
                    printf("TUYA_DRV_SET_INT_CMD\n\r");
                    i32Temp = uart_os_adapt_control( &g_uart, u8Temp, 0);
                }
                break;
                case TUYA_DRV_CLR_INT_CMD: // 4
                {
                    printf("TUYA_DRV_CLR_INT_CMD\n\r");
                    i32Temp = uart_os_adapt_control( &g_uart, u8Temp, 0);
                }
                break;
                case TUYA_DRV_SET_ISR_CMD: // 5
                {
                    printf("TUYA_DRV_SET_ISR_CMD\n\r");
                    i32Temp = uart_os_adapt_control( &g_uart, u8Temp, Tuya_Uart_RxCallBack);
                }
                break;

                default:
                    i32Temp = OPRT_OS_ADAPTER_UART_CONTROL_FAILED;
            }
            //tuya_uart_control
            if( i32Temp == OPRT_OS_ADAPTER_OK)
                printf("OK\n\r");
            else
                printf("Fail\n\r");
        }
        break;
        case 2:
        {
            printf("tuya_uart_read\n\r");
            i32Temp = tuya_uart_read(&g_uart, &u8Temp);
            if( i32Temp == OPRT_OS_ADAPTER_OK)
                printf("Got %c(= 0x%X)\n\r", u8Temp, u8Temp);
            else
                printf("Fail\n\r");
        }
        break;
        case 3:
        {
            printf("tuya_uart_write\n\r");
            if(argc > 3)
                u8Temp = strtoul(argv[3], NULL, 10);
            else
                u8Temp = '?';
            i32Temp = tuya_uart_write(&g_uart, u8Temp);
            if( i32Temp == OPRT_OS_ADAPTER_OK)
                printf("OK\n\r");
            else
                printf("Fail\n\r");
                
        }
        break;
        case 4:
        {
            printf("tuya_uart_deinit\n\r");
            i32Temp = tuya_uart_deinit(&g_uart);
            if( i32Temp == OPRT_OS_ADAPTER_OK)
                printf("OK\n\r");
            else
                printf("Fail\n\r");
        }
        break;
    }
    
done:
   return tRet;
}
#endif
