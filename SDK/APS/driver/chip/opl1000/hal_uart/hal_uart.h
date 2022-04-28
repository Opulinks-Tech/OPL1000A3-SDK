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

/******************************************************************************
*  Filename:
*  ---------
*  hal_uart.h
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of uart .
*
*  Author:
*  -------
*  Chung-chun Wang
******************************************************************************/

#ifndef __HAL_UART_H__
#define __HAL_UART_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define UART_0              ((S_Uart_Reg_t *) UART0_BASE)
#define UART_1              ((S_Uart_Reg_t *) UART1_BASE)
#define UART_SRC_CLK            XTAL
#define DW_UART_DEFAULT_BAUD    115200

/*
 * baud rate:115200 -> 100 us per char, 
 * Max core 176Mhz -> 1ms = 176*1000 tick = 0x2AF80 => 0x30000
 */
#define UART_TIMEOUT_COUNT_MAX  ( 0x60000 )
#define UART_TIMEOUT_MS_MAX     ( Hal_Tick_MilliSecMax() )

#define UART_BAUD_DIV_MIN    0x1
#define UART_BAUD_DIV_MAX    0xFFFF
#define UART_BAUD_DIV_L_MASK 0xFF
#define UART_BAUD_DIV_H_MASK 0xFF

#define UART_IER_PTIME_EN (1<<7)
#define UART_IER_EDSSI_EN (1<<3)
#define UART_IER_ELSI_EN  (1<<2)
#define UART_IER_ETBEI_EN (1<<1)
#define UART_IER_ERBFI_EN (1<<0)

#define UART_FCR_REVC_TRIG_ANY      (0<<6)
#define UART_FCR_REVC_TRIG_QUARTER  (1<<6)
#define UART_FCR_REVC_TRIG_HALF     (2<<6)
#define UART_FCR_REVC_TRIG_TWO_AVA  (3<<6)
#define UART_FCR_REVC_TRIG_MASK     (0x3<<6)
#define UART_FCR_EMPTY_TRIG_NONE    (0<<4)
#define UART_FCR_EMPTY_TRIG_TWO     (1<<4)
#define UART_FCR_EMPTY_TRIG_QUARTER (2<<4)
#define UART_FCR_EMPTY_TRIG_HALF    (3<<4)
#define UART_FCR_EMPTY_TRIG_MASK    (0x3<<4)
#define UART_FCR_XMIT_FIFO_RST      (1<<2)
#define UART_FCR_RECV_FIFO_RST      (1<<1)
#define UART_FCR_XMIT_FIFO_EN       (1<<0)

#define UART_IIR_INT_ID_MASK         0xF

#define UART_LCR_BAUD_DIV_LATCH  (1<<7)
#define UART_LCR_EVEN_PARITY     (1<<4)
#define UART_LCR_PARITY_EN       (1<<3)
#define UART_LCR_STOP_BIT        (1<<2)
#define UART_LCR_DATA_BITS_5     0
#define UART_LCR_DATA_BITS_6     1
#define UART_LCR_DATA_BITS_7     2
#define UART_LCR_DATA_BITS_8     3
#define UART_LCR_DATA_BITS_MASK  0x3

#define UART_LSR_XMIT_HOLD_EMPTY (1<<5)
#define UART_LSR_DATA_READY      (1<<0)

#define UART_MCR_RTS           (1<<1)
#define UART_MCR_AUTO_FLOW_CTL (1<<5)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list…
typedef enum 
{
    PARITY_NONE = 0,
    PARITY_EVEN,
    PARITY_ODD
} E_UartParity_t;

typedef enum 
{
    STOP_BIT_1 = 0,
    STOP_BIT_1P5,
    STOP_BIT_2
} E_UartStopBit_t;

typedef enum 
{
    DATA_BIT_5 = 0,
    DATA_BIT_6,
    DATA_BIT_7,
    DATA_BIT_8
} E_UartDataBit_t;

typedef enum 
{
    UART_IDX_0 = 0,
    UART_IDX_1,
    
    UART_IDX_MAX
} E_UartIdx_t;

typedef enum 
{
    UART_INT_MODEM_STAT   = 0x0,
    UART_INT_NONE         = 0x1,
    UART_INT_TX_EMPTY     = 0x2,
    UART_INT_RX_AVALIBLE  = 0x4,
    UART_INT_RECV_LINE    = 0x6,
    UART_INT_BUSY         = 0x7,
    UART_INT_CHAR_TIMEOUT = 0xC
} E_UartIntId_t;

// the information of Uart config
typedef struct
{
    uint32_t ulBuadrate;
    uint8_t ubDataBit;
    uint8_t ubStopBit;
    uint8_t ubParity;
    uint8_t ubFlowCtrl;
} T_HalUartConfig;

typedef struct
{
    volatile uint32_t DATA;         //0x00, RBR(R) / THR(W) / DLL
    volatile uint32_t INT_EN;       //0x04, DLH / IER
    volatile uint32_t INT_STATUS;   //0x08, IIR(R) / FCR(W)
    volatile uint32_t LCR;          //0x0C, Line control
    volatile uint32_t MCR;          //0x10, Moden control
    volatile uint32_t LSR;          //0x14, Line status
    volatile uint32_t MSR;          //0x18, Moden status
    volatile uint32_t resv[24];     //0x1C ~ 0x78
    volatile uint32_t USR;          //0x7C UART status
} S_Uart_Reg_t;


typedef void (*T_Uart_RxCallBack)(uint32_t);

typedef uint32_t (*T_Hal_Uart_RxIntEn)(E_UartIdx_t eUartIdx, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Uart_IntIdRead)(E_UartIdx_t eUartIdx);
typedef uint32_t (*T_Hal_Uart_IntClear)(E_UartIdx_t eUartIdx, E_UartIntId_t eIntID, uint32_t *pu32Buff);
typedef uint32_t (*T_Hal_Uart_BaudRateSet)(E_UartIdx_t eUartIdx, uint32_t u32Baud);
typedef uint32_t (*T_Hal_Uart_Init)(E_UartIdx_t eUartIdx, uint32_t u32Baud, E_UartDataBit_t u8Bits, E_UartParity_t u8Parity, E_UartStopBit_t u8StopBit, uint8_t u8EnFlowCtl);
typedef uint32_t (*T_Hal_Uart_ConfigGet)(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig);
typedef uint32_t (*T_Hal_Uart_ConfigSet)(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig);
typedef uint32_t (*T_Hal_Uart_DataSend)(E_UartIdx_t eUartIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Uart_DataRecv)(E_UartIdx_t eUartIdx, uint32_t* pu32Data);
typedef void (*T_Hal_Uart_RxCallBackFuncSet)(E_UartIdx_t eUartIdx, T_Uart_RxCallBack tFunc);

// Those function are used for general case, time-out with default value(1 ms)
typedef uint32_t (*T_Hal_Uart_DataSend)(E_UartIdx_t eUartIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Uart_DataRecv)(E_UartIdx_t eUartIdx, uint32_t* pu32Data);

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
typedef uint32_t (*T_Hal_Uart_DataSendTimeOut)(E_UartIdx_t eUartIdx, uint32_t u32Data, uint32_t u32MilliSec);
typedef uint32_t (*T_Hal_Uart_DataRecvTimeOut)(E_UartIdx_t eUartIdx, uint32_t* pu32Data, uint32_t u32MilliSec);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable

// Sec 5: declaration of global function prototype

/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions
extern T_Uart_RxCallBack            Uart_0_RxCallBack;
extern T_Uart_RxCallBack            Uart_1_RxCallBack;

extern T_Hal_Uart_RxIntEn           Hal_Uart_RxIntEn;
extern T_Hal_Uart_IntIdRead         Hal_Uart_IntIdRead;
extern T_Hal_Uart_IntClear          Hal_Uart_IntClear;
extern T_Hal_Uart_BaudRateSet       Hal_Uart_BaudRateSet;
extern T_Hal_Uart_Init              Hal_Uart_Init;
extern T_Hal_Uart_ConfigGet         Hal_Uart_ConfigGet;
extern T_Hal_Uart_ConfigSet         Hal_Uart_ConfigSet;
extern T_Hal_Uart_RxCallBackFuncSet Hal_Uart_RxCallBackFuncSet;

// Those function are used for general case, time-out with default value(1 ms)
extern T_Hal_Uart_DataSend Hal_Uart_DataSend;
extern T_Hal_Uart_DataRecv Hal_Uart_DataRecv;

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
extern T_Hal_Uart_DataSendTimeOut Hal_Uart_DataSendTimeOut;
extern T_Hal_Uart_DataRecvTimeOut Hal_Uart_DataRecvTimeOut;

void Hal_Uart_Pre_Init(void);

#endif

