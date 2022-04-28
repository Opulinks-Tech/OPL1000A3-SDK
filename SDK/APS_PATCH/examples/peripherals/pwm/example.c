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
#include "hal_pwm.h"

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
    S_Hal_Pwm_Config_t tPwmCfg;

    Hal_Pwm_Init();
    Hal_Pwm_ClockSourceSet(HAL_PWM_CLK_22M);

    /* Simple mode config */
    Hal_Pwm_SimpleConfigSet(HAL_PWM_IDX_4, 50, 5000);  /* IO19: PWM4, duty = 50, clock = 5K Hz */
    Hal_Pwm_SimpleConfigSet(HAL_PWM_IDX_5, 20, 10000); /* IO18: PWM5, duty = 20, clock = 10K Hz */

    /* Complex mode config */
    /* IO20: PWM3, complex mode, duty = 80, clock = 100K Hz */
    tPwmCfg.ulPeriod = 22*10;
    tPwmCfg.ulDutyBright = 22*8;
    tPwmCfg.ulDutyDull   = 22*8;
    tPwmCfg.ulRampUp   = 0;
    tPwmCfg.ulRampDown = 0;
    tPwmCfg.ulHoldBright = 1;
    tPwmCfg.ulHoldDull   = 1;
    Hal_Pwm_ComplexConfigSet(HAL_PWM_IDX_3, tPwmCfg);

    Hal_Pwm_Enable( HAL_PWM_IDX_3 | HAL_PWM_IDX_4 | HAL_PWM_IDX_5);

    printf("PWNs Running\n");
}
