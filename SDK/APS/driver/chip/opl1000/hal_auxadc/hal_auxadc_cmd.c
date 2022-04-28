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
#include <stdlib.h>
#include <string.h>
#include "aos_reg.h"
#include "hal_auxadc.h"
#include "hal_auxadc_internal.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
RET_DATA T_Hal_Aux_CmdParse_Fp Hal_Aux_CmdParse;
RET_DATA T_Hal_Aux_Pinmux_Fp Hal_Aux_Pinmux;

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

/*************************************************************************
* FUNCTION:
*   Hal_Aux_CmdParse
*
* DESCRIPTION:
*   Parse the AUXADC cmd
*
* PARAMETERS
*   1. sCmd    : [In] the input string
*
* RETURNS
*
*************************************************************************/
void Hal_Aux_CmdParse_impl(char *sCmd)
{
    float fValue;
    uint32_t ulValue;
    uint8_t ubType;
    uint8_t ubIdx;

    uint8_t ucaSeps[] = " \t\r\n";         // space, tab, new line
    uint8_t *pucaToken[8] = {0};
    uint8_t i;
    char *pSavedPtr = NULL;

    printf("\n");

    // get the first token
    pucaToken[0] = (uint8_t*)strtok_r(sCmd, (const char*)ucaSeps, &pSavedPtr);

    // get the other token
    if (NULL != pucaToken[0])
    {
        for (i=1; i<8; i++)
        {
            pucaToken[i] = (uint8_t*)strtok_r(NULL, (const char*)ucaSeps, &pSavedPtr);
            if (NULL == pucaToken[i])
                break;
        }
    }

    // select the source type
    if ((strcmp((const char*)pucaToken[1], "0") == 0) && ((i == 3) || (i == 4)))
    {
        // the type
        ubType = strtoul((const char*)pucaToken[2], NULL, 0);
        
        // the index
        if (i == 4)
            ubIdx = strtoul((const char*)pucaToken[3], NULL, 0);

        // select source
        if ((HAL_AUX_OK == Hal_Aux_SourceSelect((E_HalAux_Src_t)ubType, ubIdx)) && (i == 4))
        {
            Hal_Aux_Pinmux(ubIdx);
        }
    }
    // get the ADC value
    else if ((strcmp((const char*)pucaToken[1], "1") == 0) && (i == 2))
    {
        if (HAL_AUX_OK == Hal_Aux_AdcValueGet(&ulValue))
        {
            printf("AUXADC [%u]\n", ulValue);
        }
    }
    // get the VBAT voltage
    else if ((strcmp((const char*)pucaToken[1], "2") == 0) && (i == 2))
    {
        if (HAL_AUX_OK == Hal_Aux_VbatGet(&fValue))
        {
            ulValue = (uint32_t)(fValue * 1000);
            printf("VBAT [%u.%03u]\n", ulValue / 1000, ulValue % 1000);
        }
    }
    // get the IO voltage
    else if ((strcmp((const char*)pucaToken[1], "3") == 0) && (i == 3))
    {
        ubIdx = strtoul((const char*)pucaToken[2], NULL, 0);
    
        if (HAL_AUX_OK == Hal_Aux_IoVoltageGet(ubIdx, &fValue))
        {
            ulValue = (uint32_t)(fValue * 1000);
            printf("IO %u [%u.%03u]\n", ubIdx, ulValue / 1000, ulValue % 1000);
        }
    }
    else
    {
        printf("It is an invalid command.\n");
    }
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_Pinmux
*
* DESCRIPTION:
*   config the IO pad to AUXADC
*
* PARAMETERS
*   1. ubIdx    : [In] the number of IO pad
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_Pinmux_impl(uint8_t ubIdx)
{
    volatile uint32_t tmp;

// AUXADC
    // IO2(AUXADC), IO3(AUXADC)
    // output source
    // input IO
    
    // input
    tmp = AOS->RG_PD_DIR;
    tmp |= (0x1 << ubIdx);
    AOS->RG_PD_DIR = tmp;

    // input disable
    tmp = AOS->RG_PD_IE;
    tmp &= ~(0x1 << ubIdx);
    AOS->RG_PD_IE = tmp;

    // pull-up / pull-down
    tmp = AOS->RG_PD_PE;
    tmp &= ~(0x1 << ubIdx);
    AOS->RG_PD_PE = tmp;

    // GPIO pin
    tmp = AOS->RG_PDOC_MODE;
    tmp &= ~(0x1 << ubIdx);
    AOS->RG_PDOC_MODE = tmp;

    tmp = AOS->RG_PDOV_MODE;
    tmp &= ~(0x1 << ubIdx);
    AOS->RG_PDOV_MODE = tmp;
}
