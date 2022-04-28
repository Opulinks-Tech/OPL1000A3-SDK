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
*  hal_auxadc_patch.c
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
*  C.C.Wang
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "hal_auxadc_patch.h"
#include "hal_auxadc_internal.h"
#include "aos_reg.h"
#include "rf_reg_lite.h"
#include "hal_tick.h"
#include "cmsis_os.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define HAL_AUX_ADC_READ_ERROR_TIME         300

// Not in rf_reg_lite.h
// 0x08
#define MUX_FOR_TEMP_SEN       (1 << 19)
#define MUX_VCO_LDO_TEST       (1 << 22)
#define MUX_RF_LDO_TEST        (1 << 23)
// 0x48
#define RF_CLK_GATE_CTRL_AUXADC    (1UL << 6)
// 0x5C
#define AUX_GAIN_TUNE_POS      0
#define AUX_GAIN_TUNE_MSK      (0xF << AUX_GAIN_TUNE_POS)
#define AUX_RC_TUNE_POS        4
#define AUX_RC_TUNE_MSK        (0x3 << AUX_RC_TUNE_POS)
#define AUX_REF_ITUNE_POS      16
#define AUX_REF_ITUNE_MSK      (0x3 << UX_REF_ITUNE_POS)
// 0x64
#define AUX_IN_SEL_POS         0
#define AUX_IN_SEL_MSK         (0xF << AUX_IN_SEL_POS)
// 0x90
#define RF_POWER_CTRL_AUXADC   (1UL << 26)
// 0xCC
#define RC_CONVSTB_EN          (0x1 << 0)
// 0xD0
#define AUX_ADC_ECL_OUT_POS    0
#define AUX_ADC_ECL_OUT_MSK    (0x3FF << AUX_ADC_ECL_OUT_POS)
#define AUX_EOCB_FINISH        (0x1 << 16)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
// Ori API
extern uint8_t         g_ubHalAux_Init;
extern E_HalAux_Src_t  g_tHalAux_CurrentType;
extern uint8_t         g_ubHalAux_CurrentGpioIdx;
extern T_HalAuxCalData g_tHalAux_CalData;
extern osSemaphoreId   g_taHalAux_SemaphoreId;
extern uint32_t        g_ulHalAux_AverageCount;

// New API
extern uint8_t  g_ubHalAux_Cal;
extern uint8_t  g_ubHalAux_Pu_WriteDirect;
extern uint8_t  g_ubHalAux_AdcAlwaysOn;
extern uint32_t g_ulHalAux_PuEnDelay_Us;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype
extern void Hal_Aux_Init_impl(void);
extern uint8_t Hal_Aux_SourceSelect_impl(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx);


/***********
C Functions
***********/
// Sec 8: C Functions
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
uint8_t Hal_Aux_AdcConvValue_Get_patch( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t *pulValue)
{
    float fAdcVol = 0;

    if( HAL_AUX_OK != Hal_Aux_AdcMiniVolt_Get(tSrc, ubGpioIdx, &fAdcVol) )
        return HAL_AUX_FAIL;

    // mVlot convert to ideal ADC code
    *pulValue = (uint32_t)( (fAdcVol * 1023) + 3000/2 ) / 3000;
    if( *pulValue >0x3FF)
    {
        *pulValue = 0x3FF;
    }

    return HAL_AUX_OK;
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
uint8_t Hal_Aux_AdcMiniVolt_Get_patch( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, float *pfVbat)
{
    uint32_t ulAdcValue = 0;

    if( HAL_AUX_OK != Hal_Aux_AdcRawData_Get(tSrc, ubGpioIdx, &ulAdcValue) )
        return HAL_AUX_FAIL;

    // Convert RawData to mVlot
    *pfVbat = (float)Hal_Aux_AdcMiniVolt_Convert(ulAdcValue);
    if(*pfVbat < 0)
    {
        *pfVbat = 0;
    }

    return HAL_AUX_OK;
}

/* Update RF ctrl table before AUXADC starts */
void Hal_Aux_AdcUpdateCtrlReg_patch(uint32_t u32Enable)
{
    if (u32Enable)
    {
        // Turn on PU of AUXADC
        RF->PU_VAL |= RF_POWER_CTRL_AUXADC;
        // Turn on clock to AUXADC
        RF->RG_CK_GATE_CTRL |= RF_CLK_GATE_CTRL_AUXADC;
    }
    else
    {
        // Turn off PU of AUXADC
        RF->PU_VAL &= ~RF_POWER_CTRL_AUXADC;
        // Turn off clock to AUXADC
        RF->RG_CK_GATE_CTRL &= ~RF_CLK_GATE_CTRL_AUXADC;
    }
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
uint8_t Hal_Aux_AdcValueGet_onlyaps(uint32_t *pulValue)
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

    // Avoid to DIV_0
    if (g_ulHalAux_AverageCount == 0)
        goto done;

    Hal_Aux_AdcUpdateCtrlReg(1);

    // Enable AUXADC
    AOS->ADC_CTL |= AOS_AUXADC_EN;

    // Enable the internal temperature sensor
    if (g_tHalAux_CurrentType == HAL_AUX_SRC_TEMP_SEN)
    {
        AOS->HPBG_CTL |= AOS_TEMP_EN;
    }
    else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_VCO)
    {
        RF->RG_SPARE |= MUX_VCO_LDO_TEST;
    }
    else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_RF)
    {
        RF->RG_SPARE |= MUX_RF_LDO_TEST;
    }
    else if (g_tHalAux_CurrentType == HAL_AUX_SRC_HPBG_REF)
    {
        AOS->PMS_SPARE |= AOS_HPBG_TEST_REF_EN;
    }

    // Check delay time start
    ulCurrentTick = Hal_Tick_Diff(0);

    ulAdcValue = 0;
    ulRepeatCount = g_ulHalAux_AverageCount;

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
            // Trigger
            RF->AUX_ADC_CK_GEN_CTL |= RC_CONVSTB_EN;

            // get the ADC value
            i = 0;
            while (RF->RG_AUX_ADC_ECL_OUT & AUX_EOCB_FINISH)
            {
                if (i >= 0xFF)
                    goto done;
                i++;
            }
            ulAdcCurrent = RF->RG_AUX_ADC_ECL_OUT & AUX_ADC_ECL_OUT_MSK;

            // Idle (non-trigger)
            RF->AUX_ADC_CK_GEN_CTL &= ~RC_CONVSTB_EN;

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

    // Compute average
    if(g_ulHalAux_AverageCount > AUXEqualZeroCounter)
    {    
        *pulValue = (ulAdcValue + ((g_ulHalAux_AverageCount - AUXEqualZeroCounter) / 2)) / (g_ulHalAux_AverageCount - AUXEqualZeroCounter);
        ubRet = HAL_AUX_OK;
    }

done:
    // Alway-on or turn-off after used
    if( !g_ubHalAux_AdcAlwaysOn )
    {
        // Disable AUXADC
        AOS->ADC_CTL &= ~AOS_AUXADC_EN;

        // Disable the internal temperature sensor
        if (g_tHalAux_CurrentType == HAL_AUX_SRC_TEMP_SEN)
        {
            AOS->HPBG_CTL &= ~AOS_TEMP_EN;
        }
        else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_VCO)
        {
            RF->RG_SPARE &= ~MUX_VCO_LDO_TEST;
        }
        else if (g_tHalAux_CurrentType == HAL_AUX_SRC_LDO_RF)
        {
            RF->RG_SPARE &= ~MUX_RF_LDO_TEST;
        }
        else if (g_tHalAux_CurrentType == HAL_AUX_SRC_HPBG_REF)
        {
            AOS->PMS_SPARE &= ~AOS_HPBG_TEST_REF_EN;
        }

        Hal_Aux_AdcUpdateCtrlReg(0);
    }

    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_Patch_Init
*
* DESCRIPTION:
*   function patch init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_Patch_Init(void)
{
    // Ori API
//    g_tHalAux_CurrentType = HAL_AUX_SRC_GPIO;
//    g_ubHalAux_CurrentGpioIdx = 0;
//    g_taHalAux_SemaphoreId = NULL;
//    g_ulHalAux_AverageCount = HAL_AUX_AVERAGE_COUNT;

    Hal_Aux_AdcUpdateCtrlReg     = Hal_Aux_AdcUpdateCtrlReg_patch;
//    Hal_Aux_VbatGet              = Hal_Aux_VbatGet_impl;
//    Hal_Aux_IoVoltageGet         = Hal_Aux_IoVoltageGet_impl;
//    Hal_Aux_VbatCalibration      = Hal_Aux_VbatCalibration_impl;
//    Hal_Aux_IoVoltageCalibration = Hal_Aux_IoVoltageCalibration_impl;

//    Hal_Aux_CmdParse = Hal_Aux_CmdParse_impl;
//    Hal_Aux_Pinmux   = Hal_Aux_Pinmux_impl;

    // New API
//    g_ubHalAux_Cal = 0;
//    g_ubHalAux_Pu_WriteDirect = 0;
//    g_ubHalAux_AdcAlwaysOn = 0;
//    g_ulHalAux_PuEnDelay_Us = 0;

//    Hal_Aux_LseRegressUpdate    = Hal_Aux_LseRegressUpdate_impl;
//    Hal_Aux_AdcCal_Init         = Hal_Aux_AdcCal_Init_impl;
//    Hal_Aux_AdcCal_LoadDef      = Hal_Aux_AdcCal_LoadDef_impl;
//    Hal_Aux_AdcCal_LoadOtp      = Hal_Aux_AdcCal_LoadOtp_impl;
//    Hal_Aux_AdcCal_LoadFlash    = Hal_Aux_AdcCal_LoadFlash_impl;
//    Hal_Aux_AdcCal_EraseFlash   = Hal_Aux_AdcCal_EraseFlash_impl;
//    Hal_Aux_AdcCal_StoreFlash   = Hal_Aux_AdcCal_StoreFlash_impl;
//    Hal_Aux_AdcVbatInCal        = Hal_Aux_AdcVbatInCal_impl;
//    Hal_Aux_AdcGpioInCal        = Hal_Aux_AdcGpioInCal_impl;
//    Hal_Aux_AdcMiniVolt_Convert = Hal_Aux_AdcMiniVolt_Convert_impl;
    Hal_Aux_AdcMiniVolt_Get     = Hal_Aux_AdcMiniVolt_Get_patch;
    Hal_Aux_AdcConvValue_Get    = Hal_Aux_AdcConvValue_Get_patch;
//    Hal_Aux_AdcConvValue_Get_v2 = Hal_Aux_AdcConvValue_Get_v2_impl;

//    Hal_Aux_Adc_AvgCnt_Get      = Hal_Aux_Adc_AvgCnt_Get_impl;
//    Hal_Aux_Adc_AvgCnt_Set      = Hal_Aux_Adc_AvgCnt_Set_impl;
//    Hal_Aux_Adc_PuEnDelay_Get   = Hal_Aux_Adc_PuEnDelay_Get_impl;
//    Hal_Aux_Adc_PuEnDelay_Set   = Hal_Aux_Adc_PuEnDelay_Set_impl;
//    Hal_Aux_Adc_AlwaysOn_Get    = Hal_Aux_Adc_AlwaysOn_Get_impl;
//    Hal_Aux_Adc_AlwaysOn_Set    = Hal_Aux_Adc_AlwaysOn_Set_impl;

//    Hal_Aux_AdcRawData_Get   = Hal_Aux_AdcRawData_Get_ts;
#if 1
    // Token-switch Ver.
//    Hal_Aux_Init             = Hal_Aux_Init_ts;
//    Hal_Aux_SourceSelect     = Hal_Aux_SourceSelect_ts;
//    Hal_Aux_AdcValueGet      = Hal_Aux_AdcValueGet_ts;
#else
    // FT/Two-stage Ver.
    Hal_Aux_Init             = Hal_Aux_Init_impl;
    Hal_Aux_SourceSelect     = Hal_Aux_SourceSelect_impl;
    Hal_Aux_AdcValueGet      = Hal_Aux_AdcValueGet_onlyaps;
#endif
}
