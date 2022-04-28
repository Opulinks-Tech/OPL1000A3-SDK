/******************************************************************************
*  Copyright 2017 - 2020, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2020
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_auxadc.c
*
*  Project:
*  --------
*  OPL1000 Project - the AUXADC implement file
*
*  Description:
*  ------------
*  This implement file is include the AUXADC function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <string.h>
#include "aos_reg.h"
#include "rf_reg_lite.h"

#include "hal_auxadc.h"
#include "hal_auxadc_internal.h"
#include "hal_tick.h"
#include "hal_flash.h"
#include "hal_system.h"
#include "cmsis_os.h"
#include "boot_sequence.h"
#include "mw_fim_default_group03.h"
#include "ipc.h"
#include "sys_cfg.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// For FPGA use
// #define __FPGA_RF__

#define RF_RG_EOCB              (1 << 16)   // the bit[16] of RG_AUX_ADC_ECL_OUT: End of conversion sign

#define HAL_AUX_ADC_ERROR_COUNT (3)

/* RF mode table definition */
#define RF_CORE_MODE_NUM 10
#define RF_CTRL_IDX_NUM  30

#define RF_MODE_TABLE_IDX_CLK_GATE_CTRL     15          /* 48 */
#define RF_MODE_TABLE_IDX_POWER_CTRL        20          /* 90 */
#define RF_CORE_MODE_IDLE                   0
#define RF_CORE_MODE_MAX                    8
#define RF_CLK_GATE_CTRL_AUXADC             (1UL << 6)
#define RF_POWER_CTRL_AUXADC                (1UL << 26)
#define HAL_AUX_ADC_READ_ERROR_TIME         300

#define AUXADC_FLASH_START_ADDR             0x8F000

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
// Ori API
uint8_t g_ubHalAux_Init;
E_HalAux_Src_t g_tHalAux_CurrentType;
uint8_t g_ubHalAux_CurrentGpioIdx;
T_HalAuxCalData g_tHalAux_CalData;
osSemaphoreId g_taHalAux_SemaphoreId;
uint32_t g_ulHalAux_AverageCount;

T_Hal_Aux_Init_Fp                 Hal_Aux_Init;
T_Hal_Aux_SourceSelect_Fp         Hal_Aux_SourceSelect;
T_Hal_Aux_AdcValueGet_Fp          Hal_Aux_AdcValueGet;
T_Hal_Aux_AdcUpdateCtrlReg        Hal_Aux_AdcUpdateCtrlReg;
T_Hal_Aux_VbatGet_Fp              Hal_Aux_VbatGet;
T_Hal_Aux_IoVoltageGet_Fp         Hal_Aux_IoVoltageGet;
T_Hal_Aux_VbatCalibration_Fp      Hal_Aux_VbatCalibration;
T_Hal_Aux_IoVoltageCalibration_Fp Hal_Aux_IoVoltageCalibration;

// New API
uint8_t g_ubHalAux_Cal;
uint8_t g_ubHalAux_Pu_WriteDirect;
uint8_t g_ubHalAux_AdcAlwaysOn;
uint32_t g_ulHalAux_PuEnDelay_Us;

S_AuxadcCalTable_t sAuxadcCalTable;
float g_fSlope; // RawData/mv
float g_fOffset;

char *pAuxadcSrcName[ HAL_AUX_SRC_MAX ] =
{
    "GPIO",    // HAL_AUX_SRC_GPIO,
    "VBAT",    // HAL_AUX_SRC_VBAT,
    "LDO_VCO", // HAL_AUX_SRC_LDO_VCO,
    "LDO_RF",  // HAL_AUX_SRC_LDO_RF,
    "TEMP",    // HAL_AUX_SRC_TEMP_SEN,
    "HPBG",    // HAL_AUX_SRC_HPBG_REF,
    "LPBG",    // HAL_AUX_SRC_LPBG_REF,
    "PMU_SF",  // HAL_AUX_SRC_PMU_SF,
    "VSS"      // HAL_AUX_SRC_VSS
};

T_Hal_Aux_AdcUpdateCtrlReg_Fp    Hal_Aux_AdcUpdateCtrlReg;
T_Hal_Aux_LseRegressUpdate_Fp    Hal_Aux_LseRegressUpdate;
T_Hal_Aux_AdcCal_Init_Fp         Hal_Aux_AdcCal_Init;
T_Hal_Aux_AdcCal_LoadDef_Fp      Hal_Aux_AdcCal_LoadDef;
T_Hal_Aux_AdcCal_LoadOtp_Fp      Hal_Aux_AdcCal_LoadOtp;
T_Hal_Aux_AdcCal_LoadFlash_Fp    Hal_Aux_AdcCal_LoadFlash;
T_Hal_Aux_AdcCal_EraseFlash_Fp   Hal_Aux_AdcCal_EraseFlash;
T_Hal_Aux_AdcCal_StoreFlash_Fp   Hal_Aux_AdcCal_StoreFlash;
T_Hal_Aux_AdcVbatInCal_Fp        Hal_Aux_AdcVbatInCal;
T_Hal_Aux_AdcGpioInCal_Fp        Hal_Aux_AdcGpioInCal;
T_Hal_Aux_AdcRawData_Get_Fp      Hal_Aux_AdcRawData_Get;
T_Hal_Aux_AdcMiniVolt_Convert_Fp Hal_Aux_AdcMiniVolt_Convert;
T_Hal_Aux_AdcMiniVolt_Get_Fp     Hal_Aux_AdcMiniVolt_Get;
T_Hal_Aux_AdcConvValue_Get_Fp    Hal_Aux_AdcConvValue_Get;
T_Hal_Aux_AdcConvValue_Get_v2_Fp Hal_Aux_AdcConvValue_Get_v2;
T_Hal_Aux_Adc_AvgCnt_Get_Fp      Hal_Aux_Adc_AvgCnt_Get;
T_Hal_Aux_Adc_AvgCnt_Set_Fp      Hal_Aux_Adc_AvgCnt_Set;
T_Hal_Aux_Adc_PuEnDelay_Get_Fp   Hal_Aux_Adc_PuEnDelay_Get;
T_Hal_Aux_Adc_PuEnDelay_Set_Fp   Hal_Aux_Adc_PuEnDelay_Set;
T_Hal_Aux_Adc_AlwaysOn_Get_Fp    Hal_Aux_Adc_AlwaysOn_Get;
T_Hal_Aux_Adc_AlwaysOn_Set_Fp    Hal_Aux_Adc_AlwaysOn_Set;

// Sec 5: declaration of global function prototype
extern void Hal_Aux_CmdParse_impl(char *sCmd);
extern void Hal_Aux_Pinmux_impl(uint8_t ubIdx);

uint32_t *g_u32aMsqRfModeTable = NULL; /* From MSQ, assigned in patch or ROM release */

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
/* Config Average times*/ 
uint32_t Hal_Aux_Adc_AvgCnt_Get_impl( void )
{
    return g_ulHalAux_AverageCount;
}

/* Config Average times*/ 
uint32_t Hal_Aux_Adc_AvgCnt_Set_impl(uint32_t u32AvgCnt)
{
    if( u32AvgCnt == 0 )
        return HAL_AUX_FAIL;

    g_ulHalAux_AverageCount = u32AvgCnt;

    return HAL_AUX_OK;
}

uint32_t Hal_Aux_Adc_PuEnDelay_Get_impl( void )
{
    return g_ulHalAux_PuEnDelay_Us;
}

/* Config PuEn delay. This value caculate from Application Note*/
uint32_t Hal_Aux_Adc_PuEnDelay_Set_impl(uint32_t u32DelayUs)
{
    #if 0
    if( u32DelayUs > DEFAULT_AUXADC_PU_EN_DELAY_US_MAX )
        return HAL_AUX_FAIL;
    #endif

    g_ulHalAux_PuEnDelay_Us = u32DelayUs;

    return HAL_AUX_OK;
}

uint8_t Hal_Aux_Adc_AlwaysOn_Get_impl( void )
{
    return g_ubHalAux_AdcAlwaysOn;
}


uint32_t Hal_Aux_Adc_AlwaysOn_Set_impl(uint8_t u8IsAlwaysOn)
{
    g_ubHalAux_AdcAlwaysOn = u8IsAlwaysOn;

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_Init
*
* DESCRIPTION:
*   AUXADC init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_Init_impl(void)
{
    osSemaphoreDef_t tSemaphoreDef;
    volatile uint32_t tmp;

    if (g_u32aMsqRfModeTable == NULL)
        g_u32aMsqRfModeTable = (uint32_t *)(0x20000000 + AOS->RG_SPARE_2);

    // before init
    g_ubHalAux_Init = 0;

    // cold boot
//    if (0 == Boot_CheckWarmBoot())
//    {
//        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP03_CAL_AUXADC, 0, MW_FIM_CAL_AUXADC_SIZE, (uint8_t*)&g_tHalAux_CalData))
//        {
//            // if fail, get the default value
//            memcpy(&g_tHalAux_CalData, &g_tMwFimDefaultCalAuxadc, MW_FIM_CAL_AUXADC_SIZE);
//        }
//    }

    // AUXADC and HPBG bias
    tmp = AOS->ADC_CTL;
    tmp &= ~((0x1F << 7) | (0x1F << 2));
    tmp |= ((0x10 << 7) | (0x10 << 2));
    AOS->ADC_CTL = tmp;

    // Disable AUXADC
    tmp = AOS->ADC_CTL;
    tmp &= ~(0x1 << 0);
    tmp |= (0x0 << 0);
    AOS->ADC_CTL = tmp;
    
    // Disable the internal temperature sensor
    tmp = AOS->HPBG_CTL;
    tmp &= ~(0x1 << 18);
    tmp |= (0x0 << 18);
    AOS->HPBG_CTL = tmp;

    // Select output from PMU side
    tmp = AOS->PMS_SPARE;
    tmp &= ~((0x1 << 7) | (0x1 << 5) | (0x7 << 1) | (0x1 << 0));
    tmp |= ((0x1 << 7) | (0x1 << 5) | (0x0 << 1) | (0x0 << 0));
    AOS->PMS_SPARE = tmp;

#if defined ( __FPGA_RF__ )
    // Do nothing
#else
    // Turn off PU of AUXADC
    tmp = RF->PU_VAL;
    tmp &= ~(0x1 << 26);
    tmp |= (0x0 << 26);
    RF->PU_VAL = tmp;

    // Turn off clock to AUXADC
    tmp = RF->RG_CK_GATE_CTRL;
    tmp &= ~(0x1 << 6);
    tmp |= (0x0 << 6);
    RF->RG_CK_GATE_CTRL = tmp;
#endif

    // Select input to AUXADC
    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        g_tHalAux_CurrentType = HAL_AUX_SRC_GPIO;
        g_ubHalAux_CurrentGpioIdx = 0;
    }
    Hal_Aux_SourceSelect(g_tHalAux_CurrentType, g_ubHalAux_CurrentGpioIdx);

#if defined ( __FPGA_RF__ )
    // Do nothing
#else
    // Adjust AUXADC performance
    tmp = RF->AUXADC_CTRL0;
    tmp &= ~((0x3 << 4) | (0xF << 0));
    tmp |= ((0x1 << 4) | (0x9 << 0));
    RF->AUXADC_CTRL0 = tmp;

    // Idle (non-trigger)
    tmp = RF->AUX_ADC_CK_GEN_CTL;
    tmp &= ~(0x1 << 0);
    tmp |= (0x0 << 0);
    RF->AUX_ADC_CK_GEN_CTL = tmp;
#endif

    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        // create the semaphore
        tSemaphoreDef.dummy = 0;                            // reserved, it is no used
        g_taHalAux_SemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
        if (g_taHalAux_SemaphoreId == NULL)
        {
            printf("To create the semaphore for Hal_Aux is fail.\n");
            return;
        }
    }

    // after init
    g_ubHalAux_Init = 1;
}

void Hal_Aux_Init_ts(void)
{
    osSemaphoreDef_t tSemaphoreDef;

    if( g_ubHalAux_Init )
        return;

    // Set default source
    Hal_Aux_SourceSelect( HAL_AUX_SRC_GPIO, 0);

    // create the semaphore
    tSemaphoreDef.dummy = 0; // reserved, it is no used
    g_taHalAux_SemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
    if (g_taHalAux_SemaphoreId == NULL)
    {
        printf("To create the semaphore for Hal_Aux is fail.\n");
        return;
    }

    // after init
    g_ubHalAux_Init = 1;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_SourceSelect
*
* DESCRIPTION:
*   select the source of AUXADC
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_SourceSelect_impl(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx)
{
    volatile uint32_t tmp;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check the source type
    if (tSrc >= HAL_AUX_SRC_MAX)
        goto done;

    // check the index of GPIO
    if (tSrc == HAL_AUX_SRC_GPIO)
    {
        if (ubGpioIdx >= HAL_AUX_GPIO_NUM_MAX)
            goto done;
    }

    // Select output from PMU side
    tmp = AOS->PMS_SPARE;
    tmp &= ~(0x7 << 1);
    if(tSrc == HAL_AUX_SRC_GPIO)
        tmp |= (1 << 1);
    else if(tSrc == HAL_AUX_SRC_VBAT)
        tmp |= (0 << 1);
    else if(tSrc == HAL_AUX_SRC_VSS)
        tmp |= (5<< 1);
    else
        tmp |= (tSrc << 1);
    AOS->PMS_SPARE = tmp;
    
    g_tHalAux_CurrentType = tSrc;

    // Select input to AUXADC
    // GPIO
    if (tSrc == HAL_AUX_SRC_GPIO)
    {
    #if defined ( __FPGA_RF__ )
        // Do nothing
    #else
        // External
        tmp = RF->RG_SPARE;
        tmp &= ~(0x1 << 19);
        tmp |= (0x0 << 19);
        RF->RG_SPARE = tmp;
        // Select the IO pin
        tmp = RF->RG_AUX_IN_SEL;
        tmp &= ~(0xF << 0);
        tmp |= (ubGpioIdx << 0);
        RF->RG_AUX_IN_SEL = tmp;
    #endif
        g_ubHalAux_CurrentGpioIdx = ubGpioIdx;
    }
    // others
    else
    {
    #if defined ( __FPGA_RF__ )
    // Do nothing
    #else
        // Select input to AUXADC
        // Internal
        tmp = RF->RG_SPARE;
        tmp &= ~(0x1 << 19);
        tmp |= (0x1 << 19);
        RF->RG_SPARE = tmp;
        // Fix to the channel 1
        tmp = RF->RG_AUX_IN_SEL;
        tmp &= ~(0xF << 0);
        tmp |= (0x1 << 0);
        RF->RG_AUX_IN_SEL = tmp;
    #endif
    }

    ubRet = HAL_AUX_OK;

done:
    return ubRet;
}

uint8_t Hal_Aux_SourceSelect_ts(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx)
{
    uint8_t ubRet = HAL_AUX_FAIL;

    // check the source type
    if (tSrc >= HAL_AUX_SRC_MAX)
        goto done;

    // check the index of GPIO
    if (tSrc == HAL_AUX_SRC_GPIO)
    {
        if (ubGpioIdx >= HAL_AUX_GPIO_NUM_MAX)
            goto done;
    }

    g_tHalAux_CurrentType = tSrc;
    g_ubHalAux_CurrentGpioIdx = ubGpioIdx;

    ubRet = HAL_AUX_OK;

done:
    return ubRet;
}

/* Update RF ctrl table before AUXADC starts */
void Hal_Aux_AdcUpdateCtrlReg_impl(uint32_t u32Enable)
{
    uint8_t u8ModeIdx;

    if (g_u32aMsqRfModeTable == NULL)
        return;
    
    if (u32Enable)
    {
        for (u8ModeIdx = 0; u8ModeIdx<RF_CORE_MODE_MAX; u8ModeIdx++)
        {
            //g_u32aMsqRfModeTable[u8ModeIdx][RF_MODE_TABLE_IDX_CLK_GATE_CTRL] |= RF_CLK_GATE_CTRL_AUXADC;
            //g_u32aMsqRfModeTable[u8ModeIdx][RF_MODE_TABLE_IDX_POWER_CTRL] |= RF_POWER_CTRL_AUXADC;
            *( g_u32aMsqRfModeTable + u8ModeIdx*RF_CTRL_IDX_NUM + RF_MODE_TABLE_IDX_CLK_GATE_CTRL) |= RF_CLK_GATE_CTRL_AUXADC;
            *( g_u32aMsqRfModeTable + u8ModeIdx*RF_CTRL_IDX_NUM + RF_MODE_TABLE_IDX_POWER_CTRL) |= RF_POWER_CTRL_AUXADC;
        }
    }
    else
    {
        if( !g_ubHalAux_AdcAlwaysOn )
        {
            for (u8ModeIdx = 0; u8ModeIdx<RF_CORE_MODE_MAX; u8ModeIdx++)
            {
                //g_u32aMsqRfModeTable[u8ModeIdx][RF_MODE_TABLE_IDX_CLK_GATE_CTRL] &= ~RF_CLK_GATE_CTRL_AUXADC;
                //g_u32aMsqRfModeTable[u8ModeIdx][RF_MODE_TABLE_IDX_POWER_CTRL] &= ~RF_POWER_CTRL_AUXADC;
                *( g_u32aMsqRfModeTable + u8ModeIdx*RF_CTRL_IDX_NUM + RF_MODE_TABLE_IDX_CLK_GATE_CTRL) &= ~RF_CLK_GATE_CTRL_AUXADC;
                *( g_u32aMsqRfModeTable + u8ModeIdx*RF_CTRL_IDX_NUM + RF_MODE_TABLE_IDX_POWER_CTRL) &= ~RF_POWER_CTRL_AUXADC;
            }
        }
    }
}

void Hal_Aux_AdcUpdateCtrlReg_ts(uint32_t u32Enable)
{
    // Do nothing
}

/* Regression function for AUXADC */
void Hal_Aux_LseRegressUpdate_impl(uint8_t u8Num, S_AuxadcCal_t *puStAdcCalPts)
{
    uint8_t u8Idx = 0;
    float fAvgMiniVol = 0;
    float fAvgAdcCode = 0;
    float fTemp_diff_x = 0, fTemp_diff_y = 0;
    float fTemp_den = 0, fTemp_num = 0;
    
    // LSE regression
    // Step 1: compute AVG
    for(u8Idx = 0; u8Idx<u8Num ; u8Idx++)
    {
        fAvgMiniVol += (float)puStAdcCalPts[ u8Idx ].u16MiniVolt;
        fAvgAdcCode += (float)puStAdcCalPts[ u8Idx ].u16RawData;
    }
    fAvgMiniVol /= u8Num;
    fAvgAdcCode /= u8Num;

    // Step 2: compute slove and offset
    for(u8Idx = 0; u8Idx<u8Num ; u8Idx++)
    {
        fTemp_diff_x = (float)puStAdcCalPts[ u8Idx ].u16MiniVolt - fAvgMiniVol;
        fTemp_diff_y = (float)puStAdcCalPts[ u8Idx ].u16RawData  - fAvgAdcCode;
        
        fTemp_den += fTemp_diff_x * fTemp_diff_y;
        fTemp_num += fTemp_diff_x * fTemp_diff_x;
    }
    
    g_fSlope  = fTemp_den/fTemp_num; // RawData/mv
    g_fOffset = fAvgAdcCode - g_fSlope*fAvgMiniVol;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcValueGet
*
* DESCRIPTION:
*   get the ADC value from AUXADC
*
* PARAMETERS
*   1. pulValue : [Out] the ADC value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcValueGet_impl(uint32_t *pulValue)
{
    volatile uint32_t tmp;
    volatile uint32_t i;
    uint8_t ubRet = HAL_AUX_FAIL;

    uint32_t ulAdcValue = 0;
    uint32_t ulAdcDiff = 0;
    uint32_t ulAdcPrevious = 0;
    uint32_t ulAdcCurrent = 0;
    uint32_t ulRepeatCount = 0;
    uint32_t j = 0;
    uint32_t AUXEqualZeroCounter = 0;

    uint32_t ulCurrentTick = 0;
    uint32_t ulDiffTick = 0;
    uint32_t ulEscapeTicks = HAL_AUX_ADC_READ_ERROR_TIME * Hal_Tick_PerMilliSec();
    uint32_t ulDelayTicks = ( Hal_Tick_PerMilliSec() / 1000 ) * g_ulHalAux_PuEnDelay_Us;

    Hal_Aux_AdcUpdateCtrlReg(1);

    // Enable AUXADC
    tmp = AOS->ADC_CTL;
    tmp &= ~(0x1 << 0);
    tmp |= (0x1 << 0);
    AOS->ADC_CTL = tmp;

    // Enable the internal temperature sensor
    if (g_tHalAux_CurrentType == HAL_AUX_SRC_TEMP_SEN)
    {
            tmp = AOS->HPBG_CTL;
            tmp &= ~(0x1 << 18);
            tmp |= (0x1 << 18);
            AOS->HPBG_CTL = tmp;
    }else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_VCO)
    {
        #ifdef __FPGA_RF__
            // Do nothing
        #else
            tmp = RF->RG_SPARE;
            tmp &= ~(0x1 << 22);
            tmp |= (0x1 << 22);
            RF->RG_SPARE = tmp;
        #endif
    }
    else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_RF)
    {
        #ifdef __FPGA_RF__
            // Do nothing
        #else
            tmp = RF->RG_SPARE;
            tmp &= ~(0x1 << 23);
            tmp |= (0x1 << 23);
            RF->RG_SPARE = tmp;
        #endif
    }else if (g_tHalAux_CurrentType == HAL_AUX_SRC_HPBG_REF)
    {
        tmp = AOS->PMS_SPARE;
        tmp &= ~(0x1 << 4);
        tmp |= (0x1 << 4);
        AOS->PMS_SPARE = tmp;
    }

    // Check delay time start
    ulCurrentTick = Hal_Tick_Diff(0);

    if (g_ulHalAux_AverageCount == 0)
        g_ulHalAux_AverageCount = 1;

    ulAdcValue = 0;
    ulRepeatCount = g_ulHalAux_AverageCount;

#ifdef __FPGA_RF__
    // Do nothing
#else
    // while(RF->PU_VAL != g_u32aMsqRfModeTable[RF_CORE_MODE_IDLE][RF_MODE_TABLE_IDX_POWER_CTRL])
    while(RF->PU_VAL != *( g_u32aMsqRfModeTable + RF_MODE_TABLE_IDX_CLK_GATE_CTRL) )
    {
        // wait fro RF_IDLE
        osDelay(1);
    }
#endif

    // Check delay time
    ulDiffTick = Hal_Tick_Diff( ulCurrentTick );
    if( ulDiffTick < ulDelayTicks )
    {
        osDelay( (ulDelayTicks - ulDiffTick) / Hal_Tick_PerMilliSec() );
        do
        {
            ulDiffTick = Hal_Tick_Diff( ulCurrentTick );
        }while( ulDiffTick < ulDelayTicks );
    }

    // Average
    for (j=0; j<ulRepeatCount; j++)
    {
        ulCurrentTick = Hal_Tick_Diff(0);
        do
        {
            #ifdef __FPGA_RF__
            // Do nothing
            ulAdcCurrent = 1;
            #else
            // Trigger
            tmp = RF->AUX_ADC_CK_GEN_CTL;
            tmp &= ~(0x1 << 0);
            tmp |= (0x1 << 0);
            RF->AUX_ADC_CK_GEN_CTL = tmp;

            // get the ADC value
            i = 0;
            while (RF->RG_AUX_ADC_ECL_OUT & RF_RG_EOCB)
            {
                if (i >= 0xFF)
                    goto done;
                i++;
            }
            ulAdcCurrent = RF->RG_AUX_ADC_ECL_OUT & 0x03FF;

            // Idle (non-trigger)
            tmp = RF->AUX_ADC_CK_GEN_CTL;
            tmp &= ~(0x1 << 0);
            tmp |= (0x0 << 0);
            RF->AUX_ADC_CK_GEN_CTL = tmp;
            #endif

            // error handle if always zero
            ulDiffTick = Hal_Tick_Diff(ulCurrentTick);
            if (ulDiffTick >= ulEscapeTicks)
            {
                osDelay(10);
                break;
            }

        } while (ulAdcCurrent == 0);

        // Time-out, always got zero
        if (ulAdcCurrent == 0)
            AUXEqualZeroCounter++;

        // workaround 2: compare diff with previous, small than 10
        if(ulAdcPrevious == 0)
        {
            // init case
            ulAdcValue += ulAdcCurrent;
     	    ulAdcPrevious = ulAdcCurrent;
        }
        else
        {
            if( ulAdcPrevious > ulAdcCurrent )
                ulAdcDiff = ulAdcPrevious - ulAdcCurrent;
            else
                ulAdcDiff = ulAdcCurrent - ulAdcPrevious;
            
            if( ulAdcDiff <= 10 )
            {
                ulAdcValue += ulAdcCurrent;
                ulAdcPrevious = ulAdcCurrent;
            }else{
            	AUXEqualZeroCounter++;
            }
        }
    }

    if( !g_ubHalAux_AdcAlwaysOn )
    {
        // Disable AUXADC
        tmp = AOS->ADC_CTL;
        tmp &= ~(0x1 << 0);
        tmp |= (0x0 << 0);
        AOS->ADC_CTL = tmp;

        // Disable the internal temperature sensor
        if (g_tHalAux_CurrentType == HAL_AUX_SRC_TEMP_SEN)
        {
            tmp = AOS->HPBG_CTL;
            tmp &= ~(0x1 << 18);
            tmp |= (0x0 << 18);
            AOS->HPBG_CTL = tmp;
        }else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_VCO)
        {
            #ifdef __FPGA_RF__
                // Do nothing
            #else
                tmp = RF->RG_SPARE;
                tmp &= ~(0x1 << 22);
                tmp |= (0x0 << 22);
                RF->RG_SPARE = tmp;
            #endif
        }
        else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_RF)
        {
            #ifdef __FPGA_RF__
                // Do nothing
            #else
                tmp = RF->RG_SPARE;
                tmp &= ~(0x1 << 23);
                tmp |= (0x0 << 23);
                RF->RG_SPARE = tmp;
            #endif
        }else if (g_tHalAux_CurrentType == HAL_AUX_SRC_HPBG_REF)
        {
            tmp = AOS->PMS_SPARE;
            tmp &= ~(0x1 << 4);
            tmp |= (0x0 << 4);
            AOS->PMS_SPARE = tmp;
        }

        Hal_Aux_AdcUpdateCtrlReg(0);
    }

#ifdef __FPGA_RF__
    // Do nothing
    *pulValue = 0;
    ubRet = HAL_AUX_OK;
#else
    if(g_ulHalAux_AverageCount != AUXEqualZeroCounter)
    {    
        *pulValue = (ulAdcValue + ((g_ulHalAux_AverageCount - AUXEqualZeroCounter) / 2)) / (g_ulHalAux_AverageCount - AUXEqualZeroCounter);
        ubRet = HAL_AUX_OK;
    }
done:
#endif
    return ubRet;
}

uint8_t Hal_Aux_AdcValueGet_ts(uint32_t *pulValue)
{
    uint8_t ubRet = HAL_AUX_FAIL;
    T_AuxAdcReq tReq = {0};
    T_AuxAdcRsp tRsp = {0};

    tReq.u8Type          = SYS_CFG_AUXADC_VAL_GET;
    tReq.u8Src           = g_tHalAux_CurrentType;
    tReq.u8GpioIdx       = g_ubHalAux_CurrentGpioIdx;
    tReq.u8AdcAlwaysOn   = g_ubHalAux_AdcAlwaysOn;

    tReq.u32RepeatCnt    = g_ulHalAux_AverageCount;
    tReq.u32PuEnDelay_Us = g_ulHalAux_PuEnDelay_Us;

    if(sys_cfg_auxadc_req(&tReq, &tRsp))
    {
        goto done;
    }

    *pulValue = tRsp.u32Value;

    ubRet = HAL_AUX_OK;

done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_VbatGet
*
* DESCRIPTION:
*   get the VBAT value from AUXADC
*
* PARAMETERS
*   1. pfVbat : [Out] the VBAT value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_VbatGet_impl(float *pfVbat)
{
    uint32_t ulAdcValue;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if (g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    if (HAL_AUX_OK != Hal_Aux_SourceSelect(HAL_AUX_SRC_VBAT, 0))
        goto done;

    if (HAL_AUX_OK != Hal_Aux_AdcValueGet(&ulAdcValue))
        goto done;

    // check the base(DC offset)
    if (ulAdcValue <= g_tHalAux_CalData.wDcOffsetVbat)
    {
        *pfVbat = HAL_AUX_BASE_VBAT;
    }
    // others
    else
    {
        *pfVbat = HAL_AUX_BASE_VBAT + (ulAdcValue - g_tHalAux_CalData.wDcOffsetVbat) * g_tHalAux_CalData.fSlopeVbat;
    }

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_IoVoltageGet
*
* DESCRIPTION:
*   get the IO voltage value from AUXADC
*
* PARAMETERS
*   1. ubGpioIdx : [In] the index of GPIO
*   2. pfVoltage : [Out] the IO voltage value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_IoVoltageGet_impl(uint8_t ubGpioIdx, float *pfVoltage)
{
    uint32_t ulAdcValue;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if (g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    // check IO number
    if (ubGpioIdx >= HAL_AUX_GPIO_NUM_MAX)
        goto done;

    if (HAL_AUX_OK != Hal_Aux_SourceSelect(HAL_AUX_SRC_GPIO, ubGpioIdx))
        goto done;

    if (HAL_AUX_OK != Hal_Aux_AdcValueGet(&ulAdcValue))
        goto done;

    // check the base(DC offset)
    if (ulAdcValue <= g_tHalAux_CalData.wDcOffsetIo)
    {
        *pfVoltage = HAL_AUX_BASE_IO_VOL;
    }
    // others
    else
    {
        *pfVoltage = HAL_AUX_BASE_IO_VOL + (ulAdcValue - g_tHalAux_CalData.wDcOffsetIo) * g_tHalAux_CalData.fSlopeIo;
    }

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_VbatCalibration
*
* DESCRIPTION:
*   do the calibration of VBAT
*
* PARAMETERS
*   1. fVbat : [In] the voltage of VBAT
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_VbatCalibration_impl(float fVbat)
{
    uint32_t ulAdcValue;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if (g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    if (HAL_AUX_OK != Hal_Aux_SourceSelect(HAL_AUX_SRC_VBAT, 0))
        goto done;

    if (HAL_AUX_OK != Hal_Aux_AdcValueGet(&ulAdcValue))
        goto done;

    // Vbat = base voltage + (ADC value - base ADC value) * slope
    // base ADC value = ADC value - (Vbat - base voltage) / slope
    g_tHalAux_CalData.wDcOffsetVbat = ulAdcValue - (fVbat - HAL_AUX_BASE_VBAT) / g_tHalAux_CalData.fSlopeVbat;

    if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_IDX_GP03_CAL_AUXADC, 0, MW_FIM_CAL_AUXADC_SIZE, (uint8_t*)&g_tHalAux_CalData))
        goto done;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_IoVoltageCalibration
*
* DESCRIPTION:
*   do the calibration of the IO voltage
*
* PARAMETERS
*   1. ubGpioIdx : [In] the index of GPIO
*   2. fVoltage  : [In] the IO voltage value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_IoVoltageCalibration_impl(uint8_t ubGpioIdx, float fVoltage)
{
    uint32_t ulAdcValue;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if (g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    // check IO number
    if (ubGpioIdx >= HAL_AUX_GPIO_NUM_MAX)
        goto done;

    if (HAL_AUX_OK != Hal_Aux_SourceSelect(HAL_AUX_SRC_GPIO, ubGpioIdx))
        goto done;

    if (HAL_AUX_OK != Hal_Aux_AdcValueGet(&ulAdcValue))
        goto done;

    // IoVoltage = base voltage + (ADC value - base ADC value) * slope
    // base ADC value = ADC value - (IoVoltage - base voltage) / slope
    g_tHalAux_CalData.wDcOffsetIo = ulAdcValue - (fVoltage - HAL_AUX_BASE_IO_VOL) / g_tHalAux_CalData.fSlopeIo;

    if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_IDX_GP03_CAL_AUXADC, 0, MW_FIM_CAL_AUXADC_SIZE, (uint8_t*)&g_tHalAux_CalData))
        goto done;
    
    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_Init
*
* DESCRIPTION:
*   init data for convert raw-data to mini-volt
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_AdcCal_Init_impl( void )
{
    if(g_ubHalAux_Cal)
        return;

    if( Hal_Aux_AdcCal_LoadFlash() == HAL_AUX_OK )
    {
        // Load from flash
    }
    else if( Hal_Aux_AdcCal_LoadOtp() == HAL_AUX_OK )
    {
        // Load from OTP
    }
    else
    {
        // Load from internal sources
        Hal_Aux_AdcCal_LoadDef();
    }
    g_ubHalAux_Cal = 1;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_LoadDef
*
* DESCRIPTION:
*   Init default setting from internal sources
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_LoadDef_impl( void )
{
    sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt = 0;
    sAuxadcCalTable.stIntSrc[ 0 ].u16RawData  = 0;

    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = 0;
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData  = 0;

    g_fSlope = 0.3; // RawData/mv
    g_fOffset = 64;

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_LoadOtp
*
* DESCRIPTION:
*   Load default setting form OTP
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_LoadOtp_impl( void )
{
    uint8_t u8Idx = 0;
    uint32_t u32Otp_temp;
    S_AuxadcCal_t stOtpRef[5];

    for(u8Idx = 0; u8Idx<5; u8Idx++)
    {
        Hal_Sys_OtpRead((0x140 + 4*u8Idx), (uint8_t *)&u32Otp_temp, 4);
        
        stOtpRef[ u8Idx ].u16MiniVolt = (u8Idx + 1)*500;
        stOtpRef[ u8Idx ].u16RawData = u32Otp_temp;
    }

    if(  stOtpRef[ 0 ].u16RawData ==  stOtpRef[ 4 ].u16RawData )
        return HAL_AUX_FAIL;

    sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt = stOtpRef[ 0 ].u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ 0 ].u16RawData  = stOtpRef[ 0 ].u16RawData;
    
    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = stOtpRef[ 4 ].u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData  = stOtpRef[ 4 ].u16RawData;
    
    // Updated slope and offset
    Hal_Aux_LseRegressUpdate(5, stOtpRef);

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_LoadTable
*
* DESCRIPTION:
*   Load calibration data from flash
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_LoadFlash_impl( void )
{
    uint32_t u32Res = 0;
    uint32_t u32Header = 0;
    
    u32Res = Hal_Flash_AddrRead(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable.u32Header), (uint8_t *)&u32Header);
    if( u32Header == 0xFFFFFFFF)
        return HAL_AUX_FAIL;

    u32Res = Hal_Flash_AddrRead(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable), (uint8_t *)&sAuxadcCalTable);

    if( u32Res != 0 )
    {
        // Error happen
        return HAL_AUX_FAIL;
    }else{
        // No error, updated slope and offset
        Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);
        return HAL_AUX_OK;
    }
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_EraseFlash
*
* DESCRIPTION:
*   Erase calibration data to flash
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_EraseFlash_impl( void )
{
    uint32_t u32Res = 0;
    
    u32Res = Hal_Flash_4KSectorAddrErase( SPI_IDX_0, AUXADC_FLASH_START_ADDR);
    if( u32Res != 0)
        return HAL_AUX_FAIL;
    else
        return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_StoreFlash
*
* DESCRIPTION:
*   Store calibration data to flash
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_StoreFlash_impl( void )
{
    uint32_t u32Res = 0;
    
    u32Res = Hal_Aux_AdcCal_EraseFlash();
    if( u32Res != HAL_AUX_OK)
        return HAL_AUX_FAIL;

    u32Res = Hal_Flash_AddrProgram(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable), (uint8_t *)&sAuxadcCalTable);
    if( u32Res != 0)
        return HAL_AUX_FAIL;
    else
        return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcVbatInCal
*
* DESCRIPTION:
*   Cal auxadc value of given Vbat
*
* PARAMETERS
*   1. u16MiniVolt : [In] the input mVolt
*   2. u8PtsIdx    : [In] the index of calibration pts
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcVbatInCal_impl(uint16_t u16MiniVolt, uint8_t u8PtsIdx)
{
    uint8_t u8WriteDirect_bak = g_ubHalAux_Pu_WriteDirect;
    uint32_t u32Temp = 0;
    uint32_t u32Res = 0;

    // Check input valid
    if(u8PtsIdx >= HAL_AUX_CAL_PTS_NUM)
        return HAL_AUX_FAIL;

    // Get ADC raw-data
    g_ubHalAux_Pu_WriteDirect = 1;
    u32Res = Hal_Aux_AdcRawData_Get( HAL_AUX_SRC_VBAT, 0, &u32Temp);
    g_ubHalAux_Pu_WriteDirect = u8WriteDirect_bak;
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;

    // Update table
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16MiniVolt = u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16RawData = u32Temp;
    Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcGpioInCal
*
* DESCRIPTION:
*   Cal auxadc value of given GPIO
*
* PARAMETERS
*   1. u8GpioIdx   : [In] the given GPIO
*   2. u16MiniVolt : [In] the input mVolt
*   3. u8PtsIdx    : [In] the index of calibration pts
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcGpioInCal_impl(uint8_t u8GpioIdx, uint16_t u16MiniVolt, uint8_t u8PtsIdx)
{
    uint8_t u8WriteDirect_bak = g_ubHalAux_Pu_WriteDirect;
    uint32_t u32Temp = 0;
    uint32_t u32Res = 0;

    // Check input valid
    if(u8GpioIdx >= HAL_AUX_GPIO_NUM_MAX)
        return HAL_AUX_FAIL;
    if(u8PtsIdx >= HAL_AUX_CAL_PTS_NUM)
        return HAL_AUX_FAIL;

    // Get ADC raw-data
    g_ubHalAux_Pu_WriteDirect = 1;
    u32Res = Hal_Aux_AdcRawData_Get( HAL_AUX_SRC_GPIO, u8GpioIdx, &u32Temp);
    g_ubHalAux_Pu_WriteDirect = u8WriteDirect_bak;
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;

    // Update table
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16MiniVolt = u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16RawData = u32Temp;
    Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcRawData_Get
*
* DESCRIPTION:
*   Got AUXADC raw-data
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. pulRaw  : [Out] the ADC rawData 
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcRawData_Get_impl( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t *pulRaw)
{
    uint32_t ulRegTemp = 0;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if(g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    // wait for m0 AUXADC idle
    do{
        Hal_Sys_SpareRegRead(SPARE_0, &ulRegTemp);
    }while( ulRegTemp & IPC_SPARE0_AUXADC_M0_ACT);

    if( HAL_AUX_OK != Hal_Aux_SourceSelect( (E_HalAux_Src_t)tSrc, ubGpioIdx) )
        goto done;

    if( HAL_AUX_OK != Hal_Aux_AdcValueGet(pulRaw) )
        goto done;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

uint8_t Hal_Aux_AdcRawData_Get_ts( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t *pulRaw)
{
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if(g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    if( HAL_AUX_OK != Hal_Aux_SourceSelect( (E_HalAux_Src_t)tSrc, ubGpioIdx) )
        goto done;

    if( HAL_AUX_OK != Hal_Aux_AdcValueGet(pulRaw) )
        goto done;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcMiniVolt_Convert
*
* DESCRIPTION:
*   Convert given AUXADC raw-data to mVolt
*
* PARAMETERS
*   1. u32RawData: [In] the given ADC rawData 
*
* RETURNS
*   1. float : mVolt
*
*************************************************************************/
float Hal_Aux_AdcMiniVolt_Convert_impl(uint32_t u32RawData)
{
    return ((float)u32RawData - g_fOffset)/g_fSlope;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcConvValue_Get
*
* DESCRIPTION:
*   Got the converted ADC value (map 0x000~0x3FF to 0 ~ 3000 mV)
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. pulValue  : [Out] the converted ADC value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcConvValue_Get_impl( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t *pulValue)
{
    float fAdcVol = 0;

    if( HAL_AUX_OK != Hal_Aux_AdcMiniVolt_Get(tSrc, ubGpioIdx, &fAdcVol) )
        return HAL_AUX_FAIL;

    // mVlot convert to ideal ADC code
    *pulValue = (uint32_t)( (fAdcVol * 1023) + 3/2 ) / 3;
    if( *pulValue >0x3FF)
    {
        *pulValue = 0x3FF;
    }

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcConvValue_Get_v2
*
* DESCRIPTION:
*   New API for gotting the converted ADC value (map 0x000~0x3FF to 0 ~ 3000 mV)
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. ulAvgCnt  : [In] the average counts of the source
*   4. ulDelayUs : [In] the delay times for R-R-C type source
*   5. pulValue  : [Out] the converted ADC value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcConvValue_Get_v2_impl( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t ulAvgCnt, uint32_t ulDelayUs, uint32_t *pulValue)
{
    g_ulHalAux_AverageCount = ulAvgCnt;
    g_ulHalAux_PuEnDelay_Us = ulDelayUs;

    return Hal_Aux_AdcConvValue_Get( tSrc, ubGpioIdx, pulValue);
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcMiniVolt_Get
*
* DESCRIPTION:
*   Got AUXADC voltage in mVolt
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. pfVbat    : [Out] the mVolt of the source
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcMiniVolt_Get_impl( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, float *pfVbat)
{
    uint32_t ulAdcValue = 0;

    if( HAL_AUX_OK != Hal_Aux_AdcRawData_Get(tSrc, ubGpioIdx, &ulAdcValue) )
        return HAL_AUX_FAIL;

    // Convert RawData to mVlot
    *pfVbat = (float)Hal_Aux_AdcMiniVolt_Convert(ulAdcValue)/(float)1000;
    if(*pfVbat < 0)
    {
        *pfVbat = 0;
    }

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_PreInitCold
*
* DESCRIPTION:
*   the previous initial for cold boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_PreInitCold(void)
{
    // Ori API
    g_tHalAux_CurrentType = HAL_AUX_SRC_GPIO;
    g_ubHalAux_CurrentGpioIdx = 0;
    //memcpy(&g_tHalAux_CalData, &g_tMwFimDefaultCalAuxadc, MW_FIM_CAL_AUXADC_SIZE);
    g_taHalAux_SemaphoreId = NULL;
    g_ulHalAux_AverageCount = HAL_AUX_AVERAGE_COUNT;

    //Hal_Aux_Init                 = Hal_Aux_Init_impl;
    //Hal_Aux_SourceSelect         = Hal_Aux_SourceSelect_impl;
    Hal_Aux_AdcUpdateCtrlReg     = Hal_Aux_AdcUpdateCtrlReg_impl;
    //Hal_Aux_AdcValueGet          = Hal_Aux_AdcValueGet_impl;
    Hal_Aux_VbatGet              = Hal_Aux_VbatGet_impl;
    Hal_Aux_IoVoltageGet         = Hal_Aux_IoVoltageGet_impl;
    Hal_Aux_VbatCalibration      = Hal_Aux_VbatCalibration_impl;
    Hal_Aux_IoVoltageCalibration = Hal_Aux_IoVoltageCalibration_impl;

    Hal_Aux_CmdParse = Hal_Aux_CmdParse_impl;
    Hal_Aux_Pinmux   = Hal_Aux_Pinmux_impl;

    // New API
    g_ubHalAux_Cal = 0;
    g_ubHalAux_Pu_WriteDirect = 0;
    g_ubHalAux_AdcAlwaysOn = 0;
    g_ulHalAux_PuEnDelay_Us = 0;

    //Hal_Aux_AdcUpdateCtrlReg    = Hal_Aux_AdcUpdateCtrlReg_impl;
    Hal_Aux_LseRegressUpdate    = Hal_Aux_LseRegressUpdate_impl;
    Hal_Aux_AdcCal_Init         = Hal_Aux_AdcCal_Init_impl;
    Hal_Aux_AdcCal_LoadDef      = Hal_Aux_AdcCal_LoadDef_impl;
    Hal_Aux_AdcCal_LoadOtp      = Hal_Aux_AdcCal_LoadOtp_impl;
    Hal_Aux_AdcCal_LoadFlash    = Hal_Aux_AdcCal_LoadFlash_impl;
    Hal_Aux_AdcCal_EraseFlash   = Hal_Aux_AdcCal_EraseFlash_impl;
    Hal_Aux_AdcCal_StoreFlash   = Hal_Aux_AdcCal_StoreFlash_impl;
    Hal_Aux_AdcVbatInCal        = Hal_Aux_AdcVbatInCal_impl;
    Hal_Aux_AdcGpioInCal        = Hal_Aux_AdcGpioInCal_impl;
    //Hal_Aux_AdcRawData_Get      = Hal_Aux_AdcRawData_Get_impl;
    Hal_Aux_AdcMiniVolt_Convert = Hal_Aux_AdcMiniVolt_Convert_impl;
    Hal_Aux_AdcMiniVolt_Get     = Hal_Aux_AdcMiniVolt_Get_impl;
    Hal_Aux_AdcConvValue_Get    = Hal_Aux_AdcConvValue_Get_impl;
    Hal_Aux_AdcConvValue_Get_v2 = Hal_Aux_AdcConvValue_Get_v2_impl;

    Hal_Aux_Adc_AvgCnt_Get      = Hal_Aux_Adc_AvgCnt_Get_impl;
    Hal_Aux_Adc_AvgCnt_Set      = Hal_Aux_Adc_AvgCnt_Set_impl;
    Hal_Aux_Adc_PuEnDelay_Get   = Hal_Aux_Adc_PuEnDelay_Get_impl;
    Hal_Aux_Adc_PuEnDelay_Set   = Hal_Aux_Adc_PuEnDelay_Set_impl;
    Hal_Aux_Adc_AlwaysOn_Get    = Hal_Aux_Adc_AlwaysOn_Get_impl;
    Hal_Aux_Adc_AlwaysOn_Set    = Hal_Aux_Adc_AlwaysOn_Set_impl;

#if 1
    // For new API
    Hal_Aux_AdcUpdateCtrlReg = Hal_Aux_AdcUpdateCtrlReg_ts;
    Hal_Aux_Init             = Hal_Aux_Init_ts;
    Hal_Aux_SourceSelect     = Hal_Aux_SourceSelect_ts;
    Hal_Aux_AdcValueGet      = Hal_Aux_AdcValueGet_ts;
    Hal_Aux_AdcRawData_Get   = Hal_Aux_AdcRawData_Get_ts;
#else
    Hal_Aux_AdcUpdateCtrlReg = Hal_Aux_AdcUpdateCtrlReg_impl;
    Hal_Aux_Init             = Hal_Aux_Init_impl;
    Hal_Aux_SourceSelect     = Hal_Aux_SourceSelect_impl;
    Hal_Aux_AdcValueGet      = Hal_Aux_AdcValueGet_impl;
    Hal_Aux_AdcRawData_Get   = Hal_Aux_AdcRawData_Get_impl;
#endif
}
