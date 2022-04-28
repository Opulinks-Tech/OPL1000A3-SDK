/******************************************************************************
*  Copyright 2017 - 2021, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2021
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  example.c
*
*  Project:
*  --------
*  OPL1000 Project - the example implement file
*
*  Description:
*  ------------
*  This implement file is include the main patch function and api.
*
*  Author:
*  -------
*  TW FW
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <string.h>
#include "example.h"
#include "sys_os_config.h"
#include "hal_i2c.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
uint8_t gu8Cnt =0;

// Sec 5: declaration of global function prototype
void MreadCmd_Cb( void );
void MwriteCmd_Cb(uint8_t u8Data);

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
void example( void )
{
    Hal_I2c_SlaveInit(I2C_07BIT, 0x51);
    Hal_I2c_SlaveIntTxEn(1);
    Hal_I2c_SlaveIntTxCallBackFuncSet( MreadCmd_Cb );
    Hal_I2c_SlaveIntRxEn(1);
    Hal_I2c_SlaveIntRxCallBackFuncSet( MwriteCmd_Cb );

    printf("I2C-Slave Running\n");
}

void MreadCmd_Cb( void )
{
    //I2C->DATA_CMD = gu8Cnt;
    Hal_I2c_SlaveTrasmit(&gu8Cnt, 1);
    gu8Cnt++;
}

void MwriteCmd_Cb(uint8_t u8Data)
{
    gu8Cnt = u8Data;
}
