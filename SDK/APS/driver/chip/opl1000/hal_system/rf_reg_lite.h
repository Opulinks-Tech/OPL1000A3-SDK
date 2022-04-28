/******************************************************************************
*  Copyright 2017 - 2020, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2020
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  rf_reg_lite.h
*
*  Project:
*  --------
*  NL1000_Ax series
*
*  Description:
*  ------------
*  This include file defines the registers of AOS .
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

#ifndef __RF_REG_LITE_H__
#define __RF_REG_LITE_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "opl1000.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define RF_BASE         (MSQ_PERIPH_BASE + 0x9000)
#define RF              ((S_Rf_Reg_t *) RF_BASE)


// RF 0xC0
#define RC_CAL_RST             (0x1 << 0)
#define RC_CAL_EN              (0x1 << 1)

// RF 0xC4
#define RC_CAL_XTAL_CNT_POS    0
#define RC_CAL_XTAL_CNT_MSK    (0x3FFF << RC_CAL_XTAL_CNT_POS)
#define RC_CAL_RC_CNT_POS      16
#define RC_CAL_RC_CNT_MSK      (0x3FFF << RC_CAL_RC_CNT_POS)


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef struct
{
    volatile uint32_t reserve_000[2];     // 0x000 ~ 0x004, reserved
    volatile uint32_t RG_SPARE;           // 0x008
    volatile uint32_t reserve_00C[15];    // 0x00C ~ 0x044, reserved
    volatile uint32_t RG_CK_GATE_CTRL;    // 0x048
    volatile uint32_t reserve_04C[4];     // 0x04C ~ 0x058, reserved
    volatile uint32_t AUXADC_CTRL0;       // 0x05C
    volatile uint32_t reserve_060[1];     // 0x060 ~ 0x060, reserved
    volatile uint32_t RG_AUX_IN_SEL;      // 0x064
    volatile uint32_t reserve_068[10];    // 0x068 ~ 0x08C, reserved
    volatile uint32_t PU_VAL;             // 0x090
    volatile uint32_t reserve_094[11];    // 0x094 ~ 0x0BC, reserved
    volatile uint32_t RC_OSC_CAL_CTL;     // 0x0C0
    volatile uint32_t RC_OSC_CAL_OUTPUT;  // 0x0C4
    volatile uint32_t reserve_0C8;        // 0x0C8
    volatile uint32_t AUX_ADC_CK_GEN_CTL; // 0x0CC
    volatile uint32_t RG_AUX_ADC_ECL_OUT; // 0x0D0
} S_Rf_Reg_t;

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

#endif
