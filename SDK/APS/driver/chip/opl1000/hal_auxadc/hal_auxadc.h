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
*  hal_auxadc.h
*
*  Project:
*  --------
*  OPL1000 Project - the AUXADC definition file
*
*  Description:
*  ------------
*  This include file is the AUXADC definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _HAL_AUXADC_H_
#define _HAL_AUXADC_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define HAL_AUX_OK                  1
#define HAL_AUX_FAIL                0

#define HAL_AUX_GPIO_NUM_MAX        16  // support 16 IOs : from 0 to 15

#define HAL_AUX_BASE_VBAT           0   // 0V
#define HAL_AUX_BASE_IO_VOL         0   // 0V

#define HAL_AUX_AVERAGE_COUNT       30  // the average count of ADC value


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// source type
typedef enum
{
    HAL_AUX_SRC_GPIO = 0,
    HAL_AUX_SRC_VBAT,
    HAL_AUX_SRC_LDO_VCO,
    HAL_AUX_SRC_LDO_RF,
    HAL_AUX_SRC_TEMP_SEN,
    HAL_AUX_SRC_HPBG_REF,
    HAL_AUX_SRC_LPBG_REF,
    HAL_AUX_SRC_PMU_SF,
    HAL_AUX_SRC_VSS,

    HAL_AUX_SRC_MAX
} E_HalAux_Src_t;

typedef enum
{
    HAL_AUX_CAL_PTS_L = 0,
    HAL_AUX_CAL_PTS_H,
    HAL_AUX_CAL_PTS_NUM
} E_HalAux_CalIdx_t;

// the calibration data of AUXADC
typedef struct
{
    float fSlopeVbat;
    float fSlopeIo;
    int16_t wDcOffsetVbat;      // 0V
    int16_t wDcOffsetIo;        // 0V
} T_HalAuxCalData;

typedef struct _sAuxadcCal
{
    uint16_t u16MiniVolt;
    uint16_t u16RawData;
}S_AuxadcCal_t;

typedef struct _sAuxadcCalTable
{
    uint32_t      u32Header;    // Reserved for future used
    S_AuxadcCal_t stIntSrc[ HAL_AUX_CAL_PTS_NUM ];
}S_AuxadcCalTable_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
void Hal_Aux_PreInitCold(void);

// Ori API
typedef void (*T_Hal_Aux_Init_Fp)(void);
typedef uint8_t (*T_Hal_Aux_VbatGet_Fp)(float *pfVbat);
typedef uint8_t (*T_Hal_Aux_IoVoltageGet_Fp)(uint8_t ubGpioIdx, float *pfVoltage);
typedef uint8_t (*T_Hal_Aux_VbatCalibration_Fp)(float fVbat);
typedef uint8_t (*T_Hal_Aux_IoVoltageCalibration_Fp)(uint8_t ubGpioIdx, float fVoltage);
typedef void (*T_Hal_Aux_AdcUpdateCtrlReg)(uint32_t u32Enable);

// New API
typedef void (*T_Hal_Aux_AdcUpdateCtrlReg_Fp)(uint32_t u32Enable);
typedef void (*T_Hal_Aux_LseRegressUpdate_Fp)(uint8_t u8Num, S_AuxadcCal_t *puStAdcCalPts);
typedef void (*T_Hal_Aux_AdcCal_Init_Fp)( void );
typedef uint32_t (*T_Hal_Aux_AdcCal_LoadDef_Fp)( void );
typedef uint32_t (*T_Hal_Aux_AdcCal_LoadOtp_Fp)( void );
typedef uint32_t (*T_Hal_Aux_AdcCal_LoadFlash_Fp)( void );
typedef uint32_t (*T_Hal_Aux_AdcCal_EraseFlash_Fp)( void );
typedef uint32_t (*T_Hal_Aux_AdcCal_StoreFlash_Fp)( void );
typedef uint32_t (*T_Hal_Aux_AdcVbatInCal_Fp)(uint16_t u16MiniVolt, uint8_t u8PtsIdx);
typedef uint32_t (*T_Hal_Aux_AdcGpioInCal_Fp)(uint8_t u8GpioIdx, uint16_t u16MiniVolt, uint8_t u8PtsIdx);
typedef uint8_t (*T_Hal_Aux_AdcRawData_Get_Fp)( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t *pulRaw);
typedef float (*T_Hal_Aux_AdcMiniVolt_Convert_Fp)(uint32_t u32RawData);
typedef uint8_t(*T_Hal_Aux_AdcMiniVolt_Get_Fp)( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, float *pfVbat);
typedef uint8_t (*T_Hal_Aux_AdcConvValue_Get_Fp)( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t *pulValue);

// API changed, given all param once
typedef uint8_t (*T_Hal_Aux_AdcConvValue_Get_v2_Fp)( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t ulAvgCnt, uint32_t ulDelayUs, uint32_t *pulValue);

typedef uint32_t (*T_Hal_Aux_Adc_AvgCnt_Get_Fp)( void );
typedef uint32_t (*T_Hal_Aux_Adc_AvgCnt_Set_Fp)(uint32_t u32AvgCnt);
typedef uint32_t (*T_Hal_Aux_Adc_PuEnDelay_Get_Fp)( void );
typedef uint32_t (*T_Hal_Aux_Adc_PuEnDelay_Set_Fp)(uint32_t u32DelayUs);
typedef uint8_t (*T_Hal_Aux_Adc_AlwaysOn_Get_Fp)( void );
typedef uint32_t (*T_Hal_Aux_Adc_AlwaysOn_Set_Fp)(uint8_t u8IsAlwaysOn);

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
extern char *pAuxadcSrcName[ HAL_AUX_SRC_MAX ];
extern S_AuxadcCalTable_t sAuxadcCalTable;
extern uint8_t g_ubHalAux_Pu_WriteDirect; // Only enable for calibration
extern uint32_t g_ulHalAux_AverageCount;
extern uint32_t g_ulHalAux_SkipCount;
extern uint32_t g_ulHalAux_DelayUs;

// Sec 7: declaration of static function prototype
// Ori API
extern T_Hal_Aux_Init_Fp                 Hal_Aux_Init;
extern T_Hal_Aux_VbatGet_Fp              Hal_Aux_VbatGet;
extern T_Hal_Aux_IoVoltageGet_Fp         Hal_Aux_IoVoltageGet;
extern T_Hal_Aux_VbatCalibration_Fp      Hal_Aux_VbatCalibration;
extern T_Hal_Aux_IoVoltageCalibration_Fp Hal_Aux_IoVoltageCalibration;
extern T_Hal_Aux_AdcUpdateCtrlReg        Hal_Aux_AdcUpdateCtrlReg;

// New API
extern T_Hal_Aux_AdcUpdateCtrlReg_Fp    Hal_Aux_AdcUpdateCtrlReg;
extern T_Hal_Aux_LseRegressUpdate_Fp    Hal_Aux_LseRegressUpdate;
extern T_Hal_Aux_AdcCal_Init_Fp         Hal_Aux_AdcCal_Init;
extern T_Hal_Aux_AdcCal_LoadDef_Fp      Hal_Aux_AdcCal_LoadDef;
extern T_Hal_Aux_AdcCal_LoadOtp_Fp      Hal_Aux_AdcCal_LoadOtp;
extern T_Hal_Aux_AdcCal_LoadFlash_Fp    Hal_Aux_AdcCal_LoadFlash;
extern T_Hal_Aux_AdcCal_EraseFlash_Fp   Hal_Aux_AdcCal_EraseFlash;
extern T_Hal_Aux_AdcCal_StoreFlash_Fp   Hal_Aux_AdcCal_StoreFlash;
extern T_Hal_Aux_AdcVbatInCal_Fp        Hal_Aux_AdcVbatInCal;
extern T_Hal_Aux_AdcGpioInCal_Fp        Hal_Aux_AdcGpioInCal;
extern T_Hal_Aux_AdcRawData_Get_Fp      Hal_Aux_AdcRawData_Get;
extern T_Hal_Aux_AdcMiniVolt_Convert_Fp Hal_Aux_AdcMiniVolt_Convert;
extern T_Hal_Aux_AdcMiniVolt_Get_Fp     Hal_Aux_AdcMiniVolt_Get;
extern T_Hal_Aux_AdcConvValue_Get_Fp    Hal_Aux_AdcConvValue_Get;
extern T_Hal_Aux_AdcConvValue_Get_v2_Fp Hal_Aux_AdcConvValue_Get_v2;

extern T_Hal_Aux_Adc_AvgCnt_Get_Fp      Hal_Aux_Adc_AvgCnt_Get;
extern T_Hal_Aux_Adc_AvgCnt_Set_Fp      Hal_Aux_Adc_AvgCnt_Set;
extern T_Hal_Aux_Adc_PuEnDelay_Get_Fp   Hal_Aux_Adc_PuEnDelay_Get;
extern T_Hal_Aux_Adc_PuEnDelay_Set_Fp   Hal_Aux_Adc_PuEnDelay_Set;
extern T_Hal_Aux_Adc_AlwaysOn_Get_Fp    Hal_Aux_Adc_AlwaysOn_Get;
extern T_Hal_Aux_Adc_AlwaysOn_Set_Fp    Hal_Aux_Adc_AlwaysOn_Set;

#ifdef __cplusplus
}
#endif

#endif // _HAL_AUXADC_H_
