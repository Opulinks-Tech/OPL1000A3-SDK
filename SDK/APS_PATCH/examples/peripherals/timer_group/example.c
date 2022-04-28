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
#include "hal_tmr.h"
#include "hal_tick.h"
#include "hal_system.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define APP_MESSAGE_Q_SIZE  16
#define HW_TMR_INTERVAL     (1*1000000) // uint: us.

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list
typedef struct
{
    uint32_t u32TimerIdx;
    uint32_t u32OsTmr;
    uint32_t u32HalTick;
    uint64_t u64SleepTmr;
} S_MessageQ;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
osThreadId g_tAppThread_1;
osThreadId g_tAppThread_2;
osMessageQId g_tAppMessageQ;
osPoolId g_tAppMemPoolId;

// Sec 5: declaration of global function prototype
void IsrLogThread(void *argu);
void Main_AppThread(void *argu);
osStatus IsrMsgQSend(S_MessageQ *ptMsg);

void timer_stamp_log(uint32_t ulTimerIdx);
void timer_callback(uint32_t ulTimerIdx);

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
        printf("OsTick= %u, HalTick= %u and SleepTmr= %llu\n", ptMsgPool->u32OsTmr, ptMsgPool->u32HalTick, ptMsgPool->u64SleepTmr);

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
    Hal_Tmr_Init(0);
    Hal_Tmr_CallBackFuncSet(0, timer_callback);

    printf("Timers Running with %d us\n", HW_TMR_INTERVAL);
    Hal_Tmr_Start(0, HW_TMR_INTERVAL);

    osThreadTerminate( g_tAppThread_1 );
}

void timer_stamp_log(uint32_t ulTimerIdx)
{
    S_MessageQ tMsg;

    // send the result to IsrLogThread
    tMsg.u32TimerIdx = ulTimerIdx;
    tMsg.u32OsTmr    = osKernelSysTick();
    tMsg.u32HalTick  = Hal_Tick_Diff(0);
    tMsg.u64SleepTmr = Hal_Sys_SleepTimerGet();
    IsrMsgQSend(&tMsg);
}

void timer_callback(uint32_t ulTimerIdx)
{
    timer_stamp_log(ulTimerIdx);
//    Hal_Tmr_Stop(0);
}
