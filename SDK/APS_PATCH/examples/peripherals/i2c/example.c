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
#include "example.h"
#include "sys_os_config.h"
#include "hal_i2c.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define EEPROM_SIZE     0x4000      // 128Kb

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
    uint16_t uwAddr = 0;
    uint8_t ubaWriteCmd[3];
    uint8_t ubaReadCmd[3];

    printf("I2C Running\n");

    Hal_I2c_MasterInit(I2C_07BIT, I2C_SPEED_STANDARD);
    Hal_I2c_TargetAddrSet( 0x57 );

    while (1)
    {
        // SDA: (w)8 bits high byte address | (w)8 bits low byte address | (w)8 bits data
        ubaWriteCmd[0] = (uwAddr & 0x3F00) >> 8;            // high byte address
        ubaWriteCmd[1] = uwAddr & 0xFF;                     // low byte address
        ubaWriteCmd[2] = 0xFF - (uwAddr & 0xFF);            // data
        Hal_I2c_MasterTrasmit(&ubaWriteCmd[0], 3, 1);

        osDelay(5); // delay due to EEPROM spec.
        
        // SDA: (w)8 bits high byte address | (w)8 bits low byte address
        // SDA: (r)8 bits data
        // ubaWriteCmd[0] // high byte address
        // ubaWriteCmd[1] // low byte address
        ubaReadCmd[0] = 0;        
        Hal_I2c_MasterTxRestartRx(&ubaWriteCmd[0], 2, &ubaReadCmd[0], 1);

        // show the data
        printf("EEPROM Addr[0x%X], Write[%d], Read[%d]\n", uwAddr, ubaWriteCmd[2], ubaReadCmd[0]);
        
        // the next address
        uwAddr++;
        if (uwAddr >= EEPROM_SIZE)
            uwAddr = 0;
        
        osDelay(1000);
    }
}
