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
*  sys_reg.h
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

#ifndef __SYS_REG_H__
#define __SYS_REG_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "opl1000.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define SYS_REG         ((S_Sys_Reg_t *) SYS_BASE)

// 0x00C
#define SYS_SRAM_BYPASS_APS          (0x1<<2)
#define SYS_SRAM_BYPASS_SHARE        (0x1<<3)

// 0x010
#define SYS_SW_RESET_ALL             (0x1<<0)

// 0x014
#define SYS_SW_DBG_EN                (0x1<<2)

// 0x11C

// 0x504
#define SYS_ECO_VER_SHIFT            20
#define SYS_ECO_VER_MASK             (0xFF<<SYS_ECO_VER_SHIFT)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef struct
{
    volatile uint32_t reserve_r0[3];         // 0x000 ~ 0x008, reserved
    volatile uint32_t R_SRAM_BYPASS;         // 0x00C
    volatile uint32_t R_SW_RESET_EN;         // 0x010
    volatile uint32_t R_SW_DBG_EN;           // 0x014
    volatile uint32_t R_BOOT_STATUS;         // 0x018
    volatile uint32_t R_CHIP_ID;             // 0x01C
    volatile uint32_t reserve_r1[12];        // 0x020 ~ 0x04C, reserved
    volatile uint32_t R_CM3_I_PATCH[128];    // 0x050 ~ 0x24C
    volatile uint32_t R_CM3_I_PATCH_ST;      // 0x250
    volatile uint32_t R_CM3_I_PATCH_EN[4];   // 0x254 ~ 0x260
    volatile uint32_t R_CM3_D_P_ADDR[4];     // 0x268 ~ 0x270
    volatile uint32_t R_CM3_D_PATCH_EN;      // 0x274
    volatile uint32_t R_CM3_D_PATCH_DATA[4]; // 0x278 ~ 0x284
    volatile uint32_t reserve_r2[6];         // 0x288 ~ 0x29C, reserved
    volatile uint32_t R_CM0_I_PATCH[128];    // 0x2A0 ~ 0x49C
    volatile uint32_t R_CM0_I_PATCH_ST;      // 0x4A0
    volatile uint32_t R_CM0_I_PATCH_EN[4];   // 0x4A4 ~ 0x4B0
    volatile uint32_t R_CM3_ORIG_ADD[4];     // 0x4B4 ~ 0x4C0
    volatile uint32_t R_CM3_TAG_ADD[4];      // 0x4C4 ~ 0x4D0
    volatile uint32_t R_CM3_RMP_MASK[4];     // 0x4D4 ~ 0x4E0
    volatile uint32_t R_CM0_ORIG_ADD[3];     // 0x4E4 ~ 0x4EC
    volatile uint32_t R_CM0_TAG_ADD[3];      // 0x4F0 ~ 0x4F8
    volatile uint32_t R_CM0_RMP_MASK[3];     // 0x4FC ~ 0x504
} S_Sys_Reg_t;

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
