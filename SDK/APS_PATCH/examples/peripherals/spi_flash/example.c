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
#include "hal_flash.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define TEST_SPI               SPI_IDX_2
#define EX_FLASH_START_ADDR    0x90000
#define Q_MODE_EN              0

#define COMPARE_OK             0
#define COMPARE_NG             1

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
osThreadId g_tAppThread_1;

uint8_t gu8Seed = 0;

// Sec 5: declaration of global function prototype
void Main_AppThread(void *argu);

void Ex_Page_Clear(uint8_t *pu8Data);
uint32_t Ex_Page_Compare(uint8_t *pu8Data_A, uint8_t *pu8Data_B);
void Ex_Page_Show(uint8_t *pu8Data);
     
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
    uint8_t au8WriteData[ 256 ] = {0}; // one page = 256 bytes
    uint8_t au8ReadData[ 256 ] = {0}; // one page = 256 bytes
    uint32_t u32Res = 0;
    uint32_t u32Idx = 0;

    // Init SPIx
    printf("SPI_%d init...\n", TEST_SPI);
    u32Res = Hal_Spi_Init(TEST_SPI, SystemCoreClockGet()/2, SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1);
    if( u32Res )
    {
        printf("SPI_%d init fail\n", TEST_SPI);
        return;
    }

    // Init flash on SPIx
    printf("Flash init ...\n");
    u32Res = Hal_Flash_Init(TEST_SPI);
    if( u32Res )
    {
        printf("Flash init fail\n");
        return;
    }

    printf("SPI-flash Running\n");

    while(1)
    {
        printf("Test one page: seed = 0x%02X\n", gu8Seed);
        // prepare test block data
        for(u32Idx = 0; u32Idx<256; u32Idx++)
            au8WriteData[ u32Idx ] = gu8Seed+u32Idx;
        
        // Erase flash
        u32Res = Hal_Flash_4KSectorAddrErase(TEST_SPI, EX_FLASH_START_ADDR);
        if( u32Res )
        {
            printf("Flash erase fail\n");
        }

        // Write flash
        u32Res = Hal_Flash_AddrProgram(TEST_SPI, EX_FLASH_START_ADDR, Q_MODE_EN, 256, au8WriteData);
        if( u32Res )
        {
            printf("Flash write fail\n");
            break;
        }

        // Read flash and check erased
        Ex_Page_Clear(au8ReadData);
        u32Res = Hal_Flash_AddrRead(TEST_SPI, EX_FLASH_START_ADDR, Q_MODE_EN, 256, au8ReadData);
        if( u32Res )
        {
            printf("Flash read fail\n");
        }else{
            if( Ex_Page_Compare( au8WriteData, au8ReadData) == COMPARE_OK)
            {
                printf("Flash erase/read/write OK\n");
            }else{
                printf("Flash erase/read/write fail\n");
                printf("Wrtie dara:\n");
                Ex_Page_Show( au8WriteData );
                printf("Read dara:\n");
                Ex_Page_Show( au8ReadData );
            }
        }

        gu8Seed+=0x10;

        for(u32Idx = 0; u32Idx<5; u32Idx++)
        {
            printf("*");
            osDelay( 1000 );
        }
        printf("\n");
    }
}

void Ex_Page_Clear(uint8_t *pu8Data)
{
    uint32_t u32Idx = 0;
    for(u32Idx = 0; u32Idx<256; u32Idx++)
        pu8Data[ u32Idx ] = 0;
}

uint32_t Ex_Page_Compare(uint8_t *pu8Data_A, uint8_t *pu8Data_B)
{
    uint32_t u32Idx = 0;
    for(u32Idx = 0; u32Idx<256; u32Idx++)
    {
        if( pu8Data_A[ u32Idx ] != pu8Data_B[ u32Idx ] )
            return COMPARE_NG;
    }
    return COMPARE_OK;
}

void Ex_Page_Show(uint8_t *pu8Data)
{
    uint32_t u32Idx = 0;
    for(u32Idx = 0; u32Idx<256; u32Idx+=16)
    {
        printf("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", 
            pu8Data[ u32Idx    ], pu8Data[ u32Idx+ 1 ], pu8Data[ u32Idx+ 2 ], pu8Data[ u32Idx+ 3 ],
            pu8Data[ u32Idx+ 4 ], pu8Data[ u32Idx+ 5 ], pu8Data[ u32Idx+ 6 ], pu8Data[ u32Idx+ 7 ],
            pu8Data[ u32Idx+ 8 ], pu8Data[ u32Idx+ 9 ], pu8Data[ u32Idx+10 ], pu8Data[ u32Idx+11 ],
            pu8Data[ u32Idx+12 ], pu8Data[ u32Idx+13 ], pu8Data[ u32Idx+14 ], pu8Data[ u32Idx+15 ]);
    }
}
