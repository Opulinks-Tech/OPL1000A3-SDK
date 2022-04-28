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
*  hal_spi.h
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of spi .
*
*  Author:
*  -------
*  Chung-chun Wang
******************************************************************************/

#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define SPI_0     ((S_Spi_Reg_t *) SPI0_BASE)
#define SPI_1     ((S_Spi_Reg_t *) SPI1_BASE)
#define SPI_2     ((S_Spi_Reg_t *) SPI2_BASE)
#define SPI_TIMEOUT 0x5000

#define SPI_CTL_FMT_MOTO             (0<<4)
#define SPI_CTL_FMT_TI               (1<<4)
#define SPI_CTL_FMT_NSC              (2<<4)
#define SPI_CTL_FMT_MASK             (0x3<<4)
#define SPI_CTL_FMT_MASK             (0x3<<4)
#define SPI_CTL_CLK_PHASE_START      (0<<6)
#define SPI_CTL_CLK_PHASE_MIDDLE     (1<<6)
#define SPI_CTL_CLK_PHASE_MASK       (0x1<<6)
#define SPI_CTL_CLK_POLAR_H_ACT      (0<<7)
#define SPI_CTL_CLK_POLAR_L_ACT      (1<<7)
#define SPI_CTL_CLK_POLAR_MASK       (0x1<<7)
#define SPI_CTL_OPR_MODE_TX_RX       (0<<8)
#define SPI_CTL_OPR_MODE_TX_ONLY     (1<<8)
#define SPI_CTL_OPR_MODE_RX_ONLY     (2<<8)
#define SPI_CTL_OPR_MODE_EEPROM_READ (3<<8)
#define SPI_CTL_OPR_MODE_MASK        (0x3<<8)

#define SPI_SSIER_EN                 (1<<0)
#define SPI_SSIER_TAG                (1<<1)

#define SPI_SR_RX_FULL               (1<<4)
#define SPI_SR_RX_NOT_EMPTY          (1<<3)
#define SPI_SR_TX_EMPTY              (1<<2)
#define SPI_SR_TX_NOT_FULL           (1<<1)

#define SPI_BUADR_MIN 0x2
#define SPI_BUADR_MAX 0xFFFE


// Frame size, Quad mode only accept 8/16 bits
#define TAG_DFS_08      (SPI_DFS_08_bit << 20)
#define TAG_DFS_16      (SPI_DFS_16_bit << 20)

// CS signal
#define TAG_CS_CONT (0 << 19)
#define TAG_CS_COMP (1 << 19)

// single-bit / quad mode
#define TAG_1_BIT   (0 << 17)
#define TAG_4_BIT   (3 << 17)

// Write / Read
#define TAG_WRITE   (0 << 16)
#define TAG_READ    (1 << 16)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

typedef enum 
{
    SPI_IDX_0,
    SPI_IDX_1,
    SPI_IDX_2,
    SPI_IDX_MAX
} E_SpiIdx_t;

typedef enum 
{
    SPI_DFS_04_bit = 0x3,
    SPI_DFS_05_bit,
    SPI_DFS_06_bit,
    SPI_DFS_07_bit,
    SPI_DFS_08_bit = 0x7,
    SPI_DFS_09_bit,
    SPI_DFS_10_bit,
    SPI_DFS_11_bit,
    SPI_DFS_12_bit,
    SPI_DFS_13_bit,
    SPI_DFS_14_bit,
    SPI_DFS_15_bit,
    SPI_DFS_16_bit = 0xF
} E_SpiDataFrameSize_t;

typedef enum 
{
    SPI_CLK_PLOAR_HIGH_ACT,
    SPI_CLK_PLOAR_LOW_ACT
} E_SpiClkPolarity_t;

typedef enum 
{
    SPI_CLK_PHASE_START,
    SPI_CLK_PHASE_MIDDLE
} E_SpiClkPhase_t;

typedef enum 
{
    SPI_FMT_MOTOROLA,
    SPI_FMT_TEXAS_INSTR,
    SPI_FMT_NATION_SEMI,
} E_SpiFrameFormat_t;

typedef struct
{
    volatile uint32_t CTRLR0;  // 0x00
	volatile uint32_t CTRLR1;  // 0x04
	volatile uint32_t SSIENR;  // 0x08
	volatile uint32_t MWCR;    // 0x0C
	volatile uint32_t SER;     // 0x10
	volatile uint32_t BAUDR;   // 0x14
	volatile uint32_t TXFTLR;  // 0x18
	volatile uint32_t RXFTLR;  // 0x1C
	volatile uint32_t TXFLR;   // 0x20
	volatile uint32_t RXFLR;   // 0x24
	volatile uint32_t SR;      // 0x28
	volatile uint32_t IMR;     // 0x2C
	volatile uint32_t ISR;     // 0x30
	volatile uint32_t PISR;    // 0x34
	volatile uint32_t TXOICR;  // 0x38
	volatile uint32_t RXOICR;  // 0x3C
	volatile uint32_t RXUICR;  // 0x40
	volatile uint32_t MSTICR;  // 0x44
	volatile uint32_t ICR;     // 0x48
	volatile uint32_t DMACR;   // 0x4C
	volatile uint32_t DMATDLR; // 0x50
	volatile uint32_t DMARDLR; // 0x54
	volatile uint32_t IDR;     // 0x58
	volatile uint32_t SSI_VER; // 0x5C
	volatile uint32_t DR[36];  // 0x60 ~ 0xEC
} S_Spi_Reg_t;


typedef uint32_t (*T_Hal_Spi_BaudRateGet)(E_SpiIdx_t eSpiIdx);
typedef uint32_t (*T_Hal_Spi_BaudRateSet)(E_SpiIdx_t eSpiIdx, uint32_t u32Baud);
typedef uint32_t (*T_Hal_Spi_DividerUpdate)(E_SpiIdx_t eSpiIdx);
typedef uint32_t (*T_Hal_Spi_Init)(E_SpiIdx_t eSpiIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn);
typedef uint32_t (*T_Hal_Spi_Data_Send)(E_SpiIdx_t eSpiIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Spi_Data_Recv)(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
extern S_Spi_Reg_t *g_Spi_psaSpiReg[SPI_IDX_MAX];
extern uint32_t g_Spi_u32aSpiSpeed[SPI_IDX_MAX];
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
extern T_Hal_Spi_BaudRateGet Hal_Spi_BaudRateGet;
extern T_Hal_Spi_BaudRateSet Hal_Spi_BaudRateSet;
extern T_Hal_Spi_DividerUpdate Hal_Spi_DividerUpdate;
extern T_Hal_Spi_Init Hal_Spi_Init;
extern T_Hal_Spi_Data_Send Hal_Spi_Data_Send;
extern T_Hal_Spi_Data_Recv Hal_Spi_Data_Recv;

void Hal_Spi_Pre_Init(void);

#endif
