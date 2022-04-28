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

#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

/******************************************************************************
*  Filename:
*  ---------
*  hal_i2c.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of I2C .
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

#define I2C       ((S_I2C_Reg_t *) I2C_BASE)

#define I2C_CLK_STD              100000
#define I2C_CLK_FAST             400000
#define I2C_CLK_HIGH             3400000
#define I2C_CLK_SUPPORT_MAX      I2C_CLK_FAST


// According to Section 3.9 page 55
#define I2C_FS_SPIKE_MAX         50   /* UNIT: ns */
#define I2C_HS_SPIKE_MAX         10   /* UNIT: ns */
// According to Section 3.10.2.3 
#define I2C_STD_SCL_LOW_MIN      4700 /* UNIT: ns */
#define I2C_STD_SCL_HIGH_MIN     4000 /* UNIT: ns */
#define I2C_FS_SCL_LOW_MIN       1300 /* UNIT: ns */
#define I2C_FS_SCL_HIGH_MIN      600  /* UNIT: ns */
#define I2C_HS_SCL_LOW_MIN       120  /* UNIT: ns */
#define I2C_HS_SCL_HIGH_MIN      60   /* UNIT: ns */

#define I2C_CON_SLAVE_DISABLE    (1<<6)
#define I2C_CON_RESTART_EN       (1<<5)
#define I2C_CON_MASTER_07BIT     (0<<4)
#define I2C_CON_MASTER_10BIT     (1<<4)
#define I2C_CON_SLAVE_10BIT      (1<<3)
#define I2C_CON_SPEED_STD        (1<<1)
#define I2C_CON_SPEED_FAST       (2<<1)
#define I2C_CON_SPEED_HIGH       (3<<1)
#define I2C_CON_SPEED_MASK       (0x3<<1)
#define I2C_CON_MASTER_MODE      (1<<0)

#define I2C_TAR_TARGET_ADDR_MASK (0x3FF)

#define I2C_SAR_SLAVE_ADDR_MASK  (0x3FF)

#define I2C_DATA_RESTART_BIT     (1<<10)
#define I2C_DATA_STOP_BIT        (1<<9)
#define I2C_DATA_CMD_WRITE       (0<<8)
#define I2C_DATA_CMD_READ        (1<<8)
#define I2C_DATA_CMD_DATA_MASK   (0xFF)

#define I2C_STATUS_SLV_ACTIVITY  (1<<6)
#define I2C_STATUS_MST_ACTIVITY  (1<<5)
#define I2C_STATUS_RX_FULL       (1<<4)
#define I2C_STATUS_RX_NOT_EMPTY  (1<<3)
#define I2C_STATUS_TX_EMPTY      (1<<2)
#define I2C_STATUS_TX_NOT_FULL   (1<<1)

#define I2C_INT_TX_ABRT          (1<<6)
#define I2C_INT_RD_REQ           (1<<5)
#define I2C_INT_RX_FULL          (1<<2)

#define I2C_ENABLE_EN            1
#define I2C_ENABLE_STATUS_EN     1


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list...
typedef struct
{
    volatile uint32_t CON;               // 0x00
    volatile uint32_t TAR;               // 0x04
    volatile uint32_t SAR;               // 0x08
    volatile uint32_t HS_MADDR;          // 0x0C
    volatile uint32_t DATA_CMD;          // 0x10
    volatile uint32_t SS_SCL_HCNT;       // 0x14
    volatile uint32_t SS_SCL_LCNT;       // 0x18
    volatile uint32_t FS_SCL_HCNT;       // 0x1C
    volatile uint32_t FS_SCL_LCNT;       // 0x20
    volatile uint32_t HS_SCL_HCNT;       // 0x24
    volatile uint32_t HS_SCL_LCNT;       // 0x28
    volatile uint32_t INTR_STAT;         // 0x2C
    volatile uint32_t INTR_MASK;         // 0x30
    volatile uint32_t RAW_INTR_MASK;     // 0x34
    volatile uint32_t RX_TL;             // 0x38
    volatile uint32_t TX_TL;             // 0x3C
    volatile uint32_t CIR_INTR;          // 0x40
    volatile uint32_t CLR_RX_UNDER;      // 0x44
    volatile uint32_t CLR_RX_OVER;       // 0x48
    volatile uint32_t CLR_TX_OVER;       // 0x4C
    volatile uint32_t CLR_RD_REQ;        // 0x50
    volatile uint32_t CLR_TX_ABRT;       // 0x54
    volatile uint32_t CLR_RX_DONE;       // 0x58
    volatile uint32_t CLR_ACTIVITY;      // 0x5C
    volatile uint32_t CLR_STOP_DET;      // 0x60
    volatile uint32_t CLR_START_DET;     // 0x64
    volatile uint32_t CLR_GEN_CALL;      // 0x68
    volatile uint32_t ENABLE;            // 0x6C
    volatile uint32_t STATUS;            // 0x70
    volatile uint32_t TXFLR;             // 0x74
    volatile uint32_t RXFLR;             // 0x78
    volatile uint32_t SDA_HOLD;          // 0x7C
    volatile uint32_t TX_ABRT_SOURCE;    // 0x80
    volatile uint32_t SLV_DATA_NACK_ONLY;// 0x84
    volatile uint32_t DMA_CR;            // 0x88
    volatile uint32_t DMA_TDLR;          // 0x8C
    volatile uint32_t DMA_RDLR;          // 0x90
    volatile uint32_t SDA_SETUP;         // 0x94
    volatile uint32_t ACK_GENERAL_CALL;  // 0x98
    volatile uint32_t ENABLE_STATUS;     // 0x9C
    volatile uint32_t FS_SPKLEN;         // 0xA0
    volatile uint32_t HS_SPKLEN;         // 0xA4
} S_I2C_Reg_t;

typedef enum 
{
    I2C_SPEED_STANDARD, /* 100K */
    I2C_SPEED_FAST      /* 400K */
    //I2C_SPEED_HIGH    /* 3.4M, not supported */
} E_I2cSpeed_t;

typedef enum 
{
    I2C_07BIT,
    I2C_10BIT
} E_I2cAddrMode_t;

typedef E_I2cSpeed_t (*T_Hal_I2c_SpeedGet)(void);
typedef uint32_t (*T_Hal_I2c_SpeedSet)(E_I2cSpeed_t eSpeed);
typedef uint32_t (*T_Hal_I2c_FsClockSet)(uint16_t u16Hcnt, uint16_t u16Lcnt);
typedef void (*T_Hal_I2c_IntHandler)(void);

/* Master mode relative */
typedef uint32_t (*T_Hal_I2c_TargetAddrSet)(uint16_t u16TargetAddr);
typedef uint32_t (*T_Hal_I2c_MasterReceive)(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit);
typedef uint32_t (*T_Hal_I2c_MasterTrasmit)(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit);
typedef uint32_t (*T_Hal_I2c_MasterTxRestartRx)(uint8_t *pu8WriteData, uint32_t u32WriteLength, uint8_t *pu8ReadData, uint32_t u32ReadLength);
typedef uint32_t (*T_Hal_I2c_MasterInit)(E_I2cAddrMode_t eAddrMode, E_I2cSpeed_t eSpeed);

/* Slave mode relative */
typedef void (*T_Hal_I2c_SlaveIntTxCallBack)(void);
typedef void (*T_Hal_I2c_SlaveIntRxCallBack)(uint8_t);
typedef uint32_t (*T_Hal_I2c_SlaveReceive)(uint8_t *pu8Data, uint32_t u32Length);
typedef uint32_t (*T_Hal_I2c_SlaveTrasmit)(uint8_t *pu8Data, uint32_t u32Length);
typedef uint32_t (*T_Hal_I2c_SlaveInit)(E_I2cAddrMode_t eAddrMode, uint16_t u16SlaveAddr);
typedef uint32_t (*T_Hal_I2c_SlaveIntTxEn)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_I2c_SlaveIntRxEn)(uint8_t u8Enable);
typedef void (*T_Hal_I2c_SlaveIntTxCallBackFuncSet)(T_Hal_I2c_SlaveIntTxCallBack tFunc);
typedef void (*T_Hal_I2c_SlaveIntRxCallBackFuncSet)(T_Hal_I2c_SlaveIntRxCallBack tFunc);


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
uint32_t Hal_I2c_MasterTxRestartRx_impl(uint8_t *pu8WriteData, uint32_t u32WriteLength, uint8_t *pu8ReadData, uint32_t u32ReadLength);

/***********
C Functions
***********/
// Sec 8: C Functions
extern T_Hal_I2c_SpeedGet   Hal_I2c_SpeedGet;
extern T_Hal_I2c_SpeedSet   Hal_I2c_SpeedSet;
extern T_Hal_I2c_IntHandler Hal_I2c_IntHandler;
extern T_Hal_I2c_FsClockSet Hal_I2c_FsClockSet;

/* Master mode relative */
extern T_Hal_I2c_TargetAddrSet      Hal_I2c_TargetAddrSet;
extern T_Hal_I2c_MasterReceive      Hal_I2c_MasterReceive;
extern T_Hal_I2c_MasterTrasmit      Hal_I2c_MasterTrasmit;
extern T_Hal_I2c_MasterTxRestartRx  Hal_I2c_MasterTxRestartRx;
extern T_Hal_I2c_MasterInit         Hal_I2c_MasterInit;

/* Slave mode relative */
extern T_Hal_I2c_SlaveReceive              Hal_I2c_SlaveReceive;
extern T_Hal_I2c_SlaveTrasmit              Hal_I2c_SlaveTrasmit;
extern T_Hal_I2c_SlaveInit                 Hal_I2c_SlaveInit;
extern T_Hal_I2c_SlaveIntTxEn              Hal_I2c_SlaveIntTxEn;
extern T_Hal_I2c_SlaveIntRxEn              Hal_I2c_SlaveIntRxEn;
extern T_Hal_I2c_SlaveIntTxCallBackFuncSet Hal_I2c_SlaveIntTxCallBackFuncSet;
extern T_Hal_I2c_SlaveIntRxCallBackFuncSet Hal_I2c_SlaveIntRxCallBackFuncSet;


void Hal_I2c_Pre_Init(void);

#endif
