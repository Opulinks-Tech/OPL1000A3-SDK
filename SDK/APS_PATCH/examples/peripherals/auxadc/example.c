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
#include "hal_auxadc_patch.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define AUX_GPIO_IDX    3

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
    float fVbat;
    float fIoVoltage;
    uint8_t ret;

    Hal_Aux_Init();
    Hal_Aux_AdcCal_Init();

    printf("Auxadc Running \n");

    while (1) {
        fVbat = 0;
        ret = Hal_Aux_AdcMiniVolt_Get(HAL_AUX_SRC_VBAT, 0, &fVbat);
        printf("VBAT : ");
        if (ret == HAL_AUX_OK) {
            printf("%.0f mV\n", fVbat);
        } else {
            printf("error...\n");
        }

        fIoVoltage = 0;
        printf("IO_%02d: ", AUX_GPIO_IDX);
        ret = Hal_Aux_AdcMiniVolt_Get(HAL_AUX_SRC_GPIO, AUX_GPIO_IDX, &fIoVoltage);
        if (ret == HAL_AUX_OK) {
            printf("%.0f mV\n", AUX_GPIO_IDX, fIoVoltage);
        } else {
            printf("error...\n");
        }

        printf("\n");
        osDelay(1000);
    }
}
