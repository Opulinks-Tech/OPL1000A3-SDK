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
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define APP_MESSAGE_Q_SIZE  16

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list
typedef struct
{
    uint32_t ulGpioIdx;
    uint32_t ulLevel;
} S_MessageQ;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
osThreadId g_tAppThread_1;
osThreadId g_tAppThread_2;
osMessageQId g_tAppMessageQ;
osPoolId g_tAppMemPoolId;

uint32_t gulLv = 0;

// Sec 5: declaration of global function prototype
void IsrLogThread(void *argu);
void Main_AppThread(void *argu);
osStatus IsrMsgQSend(S_MessageQ *ptMsg);
void gpio_int_callback_edge(E_GpioIdx_t tGpioIdx);
void gpio_int_callback_level(E_GpioIdx_t tGpioIdx);

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
    osMessageQDef_t tMessageDef;
    osPoolDef_t tMemPoolDef;

    // create the thread for isr_log
    tThreadDef.name = "log_from_isr";
    tThreadDef.pthread = IsrLogThread;
    tThreadDef.tpriority = OS_TASK_PRIORITY_APP;        // osPriorityNormal
    tThreadDef.instances = 0;                           // reserved, it is no used
    tThreadDef.stacksize = OS_TASK_STACK_SIZE_APP;      // (512), unit: 4-byte, the size is 512*4 bytes
    g_tAppThread_2 = osThreadCreate(&tThreadDef, NULL);
    if (g_tAppThread_2 == NULL)
    {
        printf("To create the thread for IsrLogThread is fail.\n");
    }

    // create the message queue for AppMessageQ
    tMessageDef.queue_sz = APP_MESSAGE_Q_SIZE;          // number of elements in the queue
    tMessageDef.item_sz = sizeof(S_MessageQ);           // size of an item
    tMessageDef.pool = NULL;                            // reserved, it is no used
    g_tAppMessageQ = osMessageCreate(&tMessageDef, g_tAppThread_1);
    if (g_tAppMessageQ == NULL)
    {
        printf("To create the message queue is fail.\n");
    }

    // create the memory pool for MessageQ
    tMemPoolDef.pool_sz = APP_MESSAGE_Q_SIZE;           // number of items (elements) in the pool
    tMemPoolDef.item_sz = sizeof(S_MessageQ);           // size of an item
    tMemPoolDef.pool = NULL;                            // reserved, it is no used
    g_tAppMemPoolId = osPoolCreate(&tMemPoolDef);
    if (g_tAppMemPoolId == NULL)
    {
        printf("To create the memory pool is fail.\n");
    }

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

void IsrLogThread(void *argu)
{
    osEvent tEvent;
    S_MessageQ *ptMsgPool;

    while (1)
    {
        // receive the message from AppMessageQ
        tEvent = osMessageGet(g_tAppMessageQ, osWaitForever);
        if (tEvent.status != osEventMessage)
        {
            printf("To receive the message from AppMessageQ is fail.\n");
            continue;
        }

        // get the content of message
        ptMsgPool = (S_MessageQ *)tEvent.value.p;

        // output the content of message
        printf("--> GPIO[%d] Level[%d]\n", ptMsgPool->ulGpioIdx, ptMsgPool->ulLevel);

        // free the memory pool
        osPoolFree(g_tAppMemPoolId, ptMsgPool);
    }
}

osStatus IsrMsgQSend(S_MessageQ *ptMsg)
{
    osStatus tRet = osErrorOS;
    S_MessageQ *ptMsgPool;

    // allocate the memory pool
    ptMsgPool = (S_MessageQ *)osPoolCAlloc(g_tAppMemPoolId);
    if (ptMsgPool == NULL)
    {
        printf("To allocate the memory pool for AppMessageQ is fail.\n");
        goto done;
    }

    // copy the message content
    memcpy(ptMsgPool, ptMsg, sizeof(S_MessageQ));

    // send the message
    if (osOK != osMessagePut(g_tAppMessageQ, (uint32_t)ptMsgPool, osWaitForever))
    {
        printf("To send the message for AppMessageQ is fail.\n");

        // free the memory pool
        osPoolFree(g_tAppMemPoolId, ptMsgPool);
        goto done;
    }

    tRet = osOK;

done:
    return tRet;
}

void Main_AppThread(void *argu)
{
    uint32_t ulCount = 0;

    // IO_03 : Falling edge
    Hal_Vic_GpioCallBackFuncSet(GPIO_IDX_03, gpio_int_callback_edge);
    Hal_Vic_GpioIntTypeSel(GPIO_IDX_03, INT_TYPE_FALLING_EDGE);
    Hal_Vic_GpioIntInv(GPIO_IDX_03, 0);
    Hal_Vic_GpioIntMask(GPIO_IDX_03, 0);
    Hal_Vic_GpioIntEn(GPIO_IDX_03, 1);

    // IO_04 : Rising edge
    Hal_Vic_GpioCallBackFuncSet(GPIO_IDX_04, gpio_int_callback_edge);
    Hal_Vic_GpioIntTypeSel(GPIO_IDX_04, INT_TYPE_FALLING_EDGE);// Note: Rising not worked. Instead by falling + invert
    Hal_Vic_GpioIntInv(GPIO_IDX_04, 1);
    Hal_Vic_GpioIntMask(GPIO_IDX_04, 0);
    Hal_Vic_GpioIntEn(GPIO_IDX_04, 1);

    // IO_05 : Level
    Hal_Vic_GpioCallBackFuncSet(GPIO_IDX_05, gpio_int_callback_level);
    Hal_Vic_GpioIntTypeSel(GPIO_IDX_05, INT_TYPE_LEVEL);
    Hal_Vic_GpioIntInv(GPIO_IDX_05, gulLv%2);
    Hal_Vic_GpioIntMask(GPIO_IDX_05, 0);
    Hal_Vic_GpioIntEn(GPIO_IDX_05, 1);

    while (1)
    {
        if( (ulCount % 2) == 0)
        {
            Hal_Vic_GpioOutput(GPIO_IDX_02, GPIO_LEVEL_LOW);
            printf("IO_02 Low\n");
        }else{
            Hal_Vic_GpioOutput(GPIO_IDX_02, GPIO_LEVEL_HIGH);
            printf("IO_02 High\n");
        }
        ulCount++;
        osDelay( 1000 );
    }
}

void gpio_int_callback_edge(E_GpioIdx_t tGpioIdx)
{
    S_MessageQ tMsg;

    // send the result to IsrLogThread
    tMsg.ulGpioIdx = tGpioIdx;
    tMsg.ulLevel = Hal_Vic_GpioInput(tGpioIdx);
    IsrMsgQSend(&tMsg);
}

void gpio_int_callback_level(E_GpioIdx_t tGpioIdx)
{
    S_MessageQ tMsg;

    // send the result to IsrLogThread
    tMsg.ulGpioIdx = tGpioIdx;
    tMsg.ulLevel = Hal_Vic_GpioInput(tGpioIdx);
    IsrMsgQSend(&tMsg);

    gulLv++;
    Hal_Vic_GpioIntInv(GPIO_IDX_05, gulLv%2); // Invert level
}
