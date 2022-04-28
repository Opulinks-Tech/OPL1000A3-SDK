/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_spi.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This source file defines the functions of spi.
*  Ref. document are:
*    [1] << DesignWare DW_apb_ssi Databook >>
*    [2] SPI tag bits - User manual.docx
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "opl1000.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_spi.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
uint32_t g_Spi_u32aSpiSpeed[SPI_IDX_MAX] = {0,0,0};
S_Spi_Reg_t *g_Spi_psaSpiReg[SPI_IDX_MAX] = {SPI_0, SPI_1, SPI_2};

T_Hal_Spi_BaudRateGet Hal_Spi_BaudRateGet;
T_Hal_Spi_BaudRateSet Hal_Spi_BaudRateSet;
T_Hal_Spi_DividerUpdate Hal_Spi_DividerUpdate;
T_Hal_Spi_Init Hal_Spi_Init;
T_Hal_Spi_Data_Send Hal_Spi_Data_Send;
T_Hal_Spi_Data_Recv Hal_Spi_Data_Recv;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
/*************************************************************************
* FUNCTION:
*  Hal_Spi_BaudRateGet
*
* DESCRIPTION:
*   1. Get SPI working baud rate
*      SPI baud rate = (SPI source clock) / (SPI divider)
*      This value might not equal to the setting baud rate in Hal_Spi_BaudRateSet
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   Baud-rate value. 1 for error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_BaudRateGet_impl(E_SpiIdx_t eSpiIdx)
{
    S_Spi_Reg_t *pSpi;

    if(eSpiIdx >= SPI_IDX_MAX)
        return 1;
    
    pSpi = g_Spi_psaSpiReg[eSpiIdx];
    return Hal_Sys_ApsPclkGet() / pSpi->BAUDR;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_BaudRateSet
*
* DESCRIPTION:
*   1. Set baud-rate of SPI
*        SPI clock source is from APS PCLK, and the divider range is any even value between 2 and 65534.
*        This function will find the closet baud rate and set the divider to SPI.
*        Using Hal_Spi_BaudRateGet can get current real SPI baud rate.
*        SPI baud rate = (SPI source clock) / Divider
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*   2. u32Baud  : Clock of SPI. 
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_BaudRateSet_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Baud)
{
    S_Spi_Reg_t *pSpi;
    uint32_t u32Div;
    uint32_t u32SsiEnr;
    uint32_t u32Ret = 1;
    
    if(eSpiIdx >= SPI_IDX_MAX)
        return 1;
    
    if (u32Baud == 0)
        return 1;
    
    pSpi = g_Spi_psaSpiReg[eSpiIdx];
    
    /* Disable SPI first */
    u32SsiEnr = pSpi->SSIENR;
    pSpi->SSIENR = 0;
    
    /* Find closet even value. Bit 0 is always truncated
     * Page 106, range is 0x0002~0xFFFE, even only (ie. last bit always truncated)
     * SPI source clock is from APS PCLK */
    u32Div = ( Hal_Sys_ApsPclkGet() + u32Baud - 1 )/ u32Baud;
    
    if( u32Div < SPI_BUADR_MIN )
        u32Div = SPI_BUADR_MIN;
    else if( u32Div > SPI_BUADR_MAX )
        u32Div = SPI_BUADR_MAX;
    else    /* Valid range */
        u32Ret = 0;
    
    /* Set baud rate and store it */
    pSpi->BAUDR = u32Div;
    g_Spi_u32aSpiSpeed[eSpiIdx] = u32Baud;
    
    /* Recover SPI */
    pSpi->SSIENR = u32SsiEnr;
    
    return u32Ret;
}

/**
 * @brief Update SPI divider.
 *        When SPI clock source changed, call this function to update SPI divider.
 *        This function will use previous SPI setting baud rate to update divider.
 * @param eSpiIdx [in] Index of SPI.
 *                      SPI_IDX_0, SPI_IDX_1, SPI_IDX_2
 * @return Setting status
 * @retval 0 setting complete
 * @retval 1 error 
 */
uint32_t Hal_Spi_DividerUpdate_impl(E_SpiIdx_t eSpiIdx)
{  
    if (eSpiIdx >= SPI_IDX_MAX)
        return 1;
    
    Hal_Spi_BaudRateSet(eSpiIdx, g_Spi_u32aSpiSpeed[eSpiIdx]);
    return 0;
}




/*************************************************************************
* FUNCTION:
*  Hal_Spi_Init
*
* DESCRIPTION:
*   1. Init SPI module
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*   2. u32Baud  : Clock of SPI.
*   3. ePolar   : Polarity of SPI clock. refert to E_SpiClkPolarity_t
*   4. ePhase   : Pase of SPI clock. refert to E_SpiClkPhase_t
*   5. eFormat  : Transmit protocal. refert to E_SpiFrameFormat_t
*   6. eDataSize: Transmit data size 
*   7. u8QModeEn: Qaud-mode select. 1 for enable/0 for disable
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Init_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn)
{
    S_Spi_Reg_t *pSpi = 0;

    if(eSpiIdx == SPI_IDX_0)
	{
        pSpi = SPI_0;
        Hal_Sys_ApsClkEn(1, APS_CLK_SPI_0);
        Hal_Sys_ApsModuleRst(APS_RST_SPI_0);
	}
    else if(eSpiIdx == SPI_IDX_1)
    {
        pSpi = SPI_1;
        Hal_Sys_ApsClkEn(1, APS_CLK_SPI_1);
        Hal_Sys_ApsModuleRst(APS_RST_SPI_1);
    }
    else if(eSpiIdx == SPI_IDX_2)
    {
        pSpi = SPI_2;
        Hal_Sys_ApsClkEn(1, APS_CLK_SPI_2);
        Hal_Sys_ApsModuleRst(APS_RST_SPI_2);
    }
    else
    {
        return 1;
    }
    
	// Disable SPI
	pSpi->SSIENR = 0;
    
    // Control setting
    pSpi->CTRLR0 = SPI_CTL_OPR_MODE_TX_RX|
                   (ePolar<< 7) |
                   (ePhase << 6) | 
                   (eFormat<<4) | 
                   (eDataSize);
    
    // Set speed
    Hal_Spi_BaudRateSet(eSpiIdx, u32Baud);    
    
    // Set slave number, only 1 slave
    pSpi->SER = 0x0001;

    // Rx buffer threshold
        // 0000_0000 ssi_rxf_intr is asserted when 1 or more data entry is present in receive FIFO
        // 0000_0001 ssi_rxf_intr is asserted when 2 or more data entries are present in receive FIFO
        // 0000_0010 ssi_rxf_intr is asserted when 3 or more data entries are present in receive FIFO
        // 0000_0011 ssi_rxf_intr is asserted when 4 or more data entries are present in receive FIFO
    // RX FIFO size is 4, so the max value could be set to 3.
    // try to set to 4, the behavior is abnormal.
    pSpi->RXFTLR = 2;
    
    // Tx buffer threshold
    pSpi->TXFTLR = 2;
    
    // Interrupt mask (Need any interrupt?)
    pSpi->IMR = 0x10;
    
    // Enable SPI
    pSpi->SSIENR = SPI_SSIER_EN;
    if(u8QModeEn)
        pSpi->SSIENR |= SPI_SSIER_TAG;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Data_Send
*
* DESCRIPTION:
*   1. SPI send data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*   2. u32Data  : Data to send
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Data_Send_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Data)
{
    S_Spi_Reg_t *pSpi = 0;
    uint32_t u32TimeOut = 0;
    
    if(eSpiIdx >= SPI_IDX_MAX)
        return 1;
    
    pSpi = g_Spi_psaSpiReg[eSpiIdx];
    
    while( !(pSpi->SR & SPI_SR_TX_NOT_FULL) )
    {
        // Wait for TX not full...
        
        // Time-out check
        if( u32TimeOut > SPI_TIMEOUT )
            return 1;
        u32TimeOut++;
    }
    pSpi->DR[0] = u32Data;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Data_Recv
*
* DESCRIPTION:
*   1. SPI receive data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*   2. pu32Data : Pointer to receive data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Data_Recv_impl(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data)
{
    S_Spi_Reg_t *pSpi = 0;
    uint32_t u32TimeOut = 0;
    

    if(eSpiIdx >= SPI_IDX_MAX)
        return 1;
    
    pSpi = g_Spi_psaSpiReg[eSpiIdx];
    
    while( !(pSpi->SR & SPI_SR_RX_NOT_EMPTY) )
    {
        // Wait for RX not Empty...
        
        //Time-out check
        if( u32TimeOut > SPI_TIMEOUT )
            return 1;
        u32TimeOut++;
    }
    *pu32Data = pSpi->DR[0];
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Pre_Init
*
* DESCRIPTION:
*   1. Inital of functuin pointers
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Spi_Pre_Init(void)
{
    Hal_Spi_BaudRateGet = Hal_Spi_BaudRateGet_impl;
    Hal_Spi_BaudRateSet = Hal_Spi_BaudRateSet_impl;
    Hal_Spi_DividerUpdate = Hal_Spi_DividerUpdate_impl;
    Hal_Spi_Init = Hal_Spi_Init_impl;
    Hal_Spi_Data_Send = Hal_Spi_Data_Send_impl;
    Hal_Spi_Data_Recv = Hal_Spi_Data_Recv_impl;
}

