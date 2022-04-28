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
#include "hal_uart_patch.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
void Uart_RxCallBack(uint32_t u32Temp);

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
    Hal_Uart_Init(UART_IDX_0, 115200, DATA_BIT_8, PARITY_NONE, STOP_BIT_1, 0);
    Hal_Uart_RxIntEn(UART_IDX_0, 1);
    Hal_Uart_RxCallBackFuncSet( UART_IDX_0, Uart_RxCallBack);
    printf("UART_0 Running\n");
}

void Uart_RxCallBack(uint32_t u32Temp)
{
    Hal_Uart_DataSend( UART_IDX_0, '*');
    Hal_Uart_DataSend( UART_IDX_0, u32Temp);
}
