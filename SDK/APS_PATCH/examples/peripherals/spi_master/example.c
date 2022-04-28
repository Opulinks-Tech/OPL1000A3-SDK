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
#include "hal_spi.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define TEST_SPI    SPI_IDX_2

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
osThreadId g_tAppThread_1;


// Sec 5: declaration of global function prototype
void Main_AppThread(void *argu);

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
    osThreadDef_t tThreadDef;

    // create the thread for Main-app thread
    tThreadDef.name = "MainApp";
    tThreadDef.pthread = Main_AppThread;
    tThreadDef.tpriority = OS_TASK_PRIORITY_APP;        // osPriorityNormal
    tThreadDef.instances = 0;                           // reserved, it is no used
    tThreadDef.stacksize = OS_TASK_STACK_SIZE_APP;      // (512), unit: 4-byte, the size is 512*4 bytes
    g_tAppThread_1 = osThreadCreate(&tThreadDef, NULL);
    if (g_tAppThread_1 == NULL)
    {
        printf("To create the thread for Main_AppThread is fail.\n");
    }
}

void Main_AppThread(void *argu)
{
    uint32_t u32aTxData[4] = {0x9F, 0, 0, 0};
    uint32_t u32aRxData[4];

    // Init SPIx
    if (Hal_Spi_Init(TEST_SPI, SystemCoreClockGet()/2, SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1) !=0)
    {
        printf("SPI_%d init fail\n", TEST_SPI);
        return;
    }

    printf("SPI_%d Running\n", TEST_SPI);

    while(1)
    {
        u32aRxData[0] = 0;
        u32aRxData[1] = 0;
        u32aRxData[2] = 0;
        u32aRxData[3] = 0;

        Hal_Spi_Data_Send(TEST_SPI, TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | u32aTxData[0]);
        Hal_Spi_Data_Send(TEST_SPI, TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | u32aTxData[1]);
        Hal_Spi_Data_Send(TEST_SPI, TAG_DFS_08 | TAG_CS_CONT | TAG_1_BIT | TAG_WRITE | u32aTxData[2]);
        Hal_Spi_Data_Send(TEST_SPI, TAG_DFS_08 | TAG_CS_COMP | TAG_1_BIT | TAG_WRITE | u32aTxData[3]);

        Hal_Spi_Data_Recv(TEST_SPI, &u32aRxData[0]);
        Hal_Spi_Data_Recv(TEST_SPI, &u32aRxData[1]);
        Hal_Spi_Data_Recv(TEST_SPI, &u32aRxData[2]);
        Hal_Spi_Data_Recv(TEST_SPI, &u32aRxData[3]);
        
        printf("Got flash ID 0x%02X 0x%02X 0x%02X\n\n", u32aRxData[1], u32aRxData[2], u32aRxData[3]);
        osDelay( 1000 );
    }
}
