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
*  hal_i2c.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This source file defines the functions of I2C .
*  Ref. document is << DesignWare DW_apb_i2c Databook >>
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
#include "hal_i2c.h"
#include "hal_tick.h"
#include "cmsis_os.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


#define SYSYEM_TICK_PER_US          ( SystemCoreClockGet()/1000000 )
#define US_TO_NS                    1000



#define I2C_SDA_HOLD_NS             650

#define I2C_TIMEOUT_COUNT_MAX       (0x30000)

#define I2C_FIFO_SIZE               8

#define IRQ_PRIORITY_I2C_PATCH      0x08

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list...


typedef uint32_t (*T_Hal_I2c_Eanble)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_I2c_WaitForMasterCompleted)(void);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
/* Internal */
RET_DATA T_Hal_I2c_Eanble                 _Hal_I2c_Eanble;
RET_DATA T_Hal_I2c_WaitForMasterCompleted _Hal_I2c_WaitForMasterCompleted;

RET_DATA T_Hal_I2c_SpeedGet   Hal_I2c_SpeedGet;
RET_DATA T_Hal_I2c_SpeedSet   Hal_I2c_SpeedSet;
RET_DATA T_Hal_I2c_FsClockSet Hal_I2c_FsClockSet;
RET_DATA T_Hal_I2c_IntHandler Hal_I2c_IntHandler;

/* Master mode relative */
RET_DATA T_Hal_I2c_TargetAddrSet        Hal_I2c_TargetAddrSet;
RET_DATA T_Hal_I2c_MasterReceive        Hal_I2c_MasterReceive;
RET_DATA T_Hal_I2c_MasterTrasmit        Hal_I2c_MasterTrasmit;
RET_DATA T_Hal_I2c_MasterTxRestartRx    Hal_I2c_MasterTxRestartRx;
RET_DATA T_Hal_I2c_MasterInit           Hal_I2c_MasterInit;

/* Slave mode relative */
RET_DATA T_Hal_I2c_SlaveIntTxCallBack        g_tHalI2cIntTxCallback;
RET_DATA T_Hal_I2c_SlaveIntRxCallBack        g_tHalI2cIntRxCallback;
RET_DATA T_Hal_I2c_SlaveReceive              Hal_I2c_SlaveReceive;
RET_DATA T_Hal_I2c_SlaveTrasmit              Hal_I2c_SlaveTrasmit;
RET_DATA T_Hal_I2c_SlaveInit                 Hal_I2c_SlaveInit;
RET_DATA T_Hal_I2c_SlaveIntTxEn              Hal_I2c_SlaveIntTxEn;
RET_DATA T_Hal_I2c_SlaveIntRxEn              Hal_I2c_SlaveIntRxEn;
RET_DATA T_Hal_I2c_SlaveIntTxCallBackFuncSet Hal_I2c_SlaveIntTxCallBackFuncSet;
RET_DATA T_Hal_I2c_SlaveIntRxCallBackFuncSet Hal_I2c_SlaveIntRxCallBackFuncSet;

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
*  _Hal_I2c_Eanble
* 
* DESCRIPTION:
*   1. Enable/Disable I2C
* 
* CALLS
* 
* PARAMETERS
*   1. u8Enable: 1 for enable/0 for disable
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_I2c_Eanble_impl(uint8_t u8Enable)
{
    uint32_t u32Count = 0;
    uint32_t u32DisableTimeout = 10*( SystemCoreClockGet()/I2C_CLK_STD );

    if(u8Enable)
    {
        // Enable
        I2C->ENABLE |= I2C_ENABLE_EN;
    }else{
        // Disable, ref to << 3.8.3 Disabling DW_apb_i2c >>
        I2C->ENABLE &= ~I2C_ENABLE_EN;
        
        while( I2C->ENABLE_STATUS & I2C_ENABLE_STATUS_EN )
        {
            if(u32Count++ > u32DisableTimeout)
                return 1;
        }
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_I2c_WaitForMasterCompleted
* 
* DESCRIPTION:
*   1. Wait for master mode no activity
* 
* CALLS
* 
* PARAMETERS
*   None
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_I2c_WaitForMasterCompleted_impl(void)
{
    uint32_t u32Count = 0;

    while(I2C->STATUS & I2C_STATUS_MST_ACTIVITY)
    {
        if(u32Count > I2C_TIMEOUT_COUNT_MAX)
            return 1;
        u32Count++;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SpeedGet
* 
* DESCRIPTION:
*   1. Get I2C speed.
* 
* CALLS
* 
* PARAMETERS
*   None
* 
* RETURNS
*   refer to E_I2cSpeed_t
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
E_I2cSpeed_t Hal_I2c_SpeedGet_impl(void)
{
    switch(I2C->CON & I2C_CON_SPEED_MASK)
    {
        case I2C_CON_SPEED_STD : return I2C_SPEED_STANDARD;
        case I2C_CON_SPEED_FAST: return I2C_SPEED_FAST;
        //case I2C_CON_SPEED_HIGH: return I2C_SPEED_HIGH;
    }
    return I2C_SPEED_STANDARD;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SpeedSet
* 
* DESCRIPTION:
*   1. Setup I2C speed.
* 
* CALLS
* 
* PARAMETERS
*   1. eSpeed : Standard(100K)/Fast(400K)/High speed(3.4M) mode. refer to E_I2cSpeed_t
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SpeedSet_impl(E_I2cSpeed_t eSpeed)
{
    uint32_t u32EnStatus = 0;
    uint32_t u32Temp = 0;

    // Disable before set
    u32EnStatus = I2C->ENABLE;
    if( u32EnStatus & I2C_ENABLE_EN )
        _Hal_I2c_Eanble(0);

    u32Temp = I2C->CON & ~(I2C_CON_SPEED_MASK);
    switch(eSpeed)
    {
        case I2C_SPEED_STANDARD:
            I2C->CON = u32Temp | I2C_CON_SPEED_STD;
            I2C->SS_SCL_HCNT = I2C_STD_SCL_HIGH_MIN * SYSYEM_TICK_PER_US / US_TO_NS;
            I2C->SS_SCL_LCNT = I2C_STD_SCL_LOW_MIN * SYSYEM_TICK_PER_US / US_TO_NS;
            I2C->FS_SPKLEN = I2C_FS_SPIKE_MAX * SYSYEM_TICK_PER_US / US_TO_NS;
            break;
        case I2C_SPEED_FAST:
            I2C->CON = u32Temp | I2C_CON_SPEED_FAST;
            I2C->FS_SCL_HCNT = I2C_FS_SCL_HIGH_MIN * SYSYEM_TICK_PER_US / US_TO_NS;
            I2C->FS_SCL_LCNT = I2C_FS_SCL_LOW_MIN * SYSYEM_TICK_PER_US / US_TO_NS;
            I2C->FS_SPKLEN = I2C_FS_SPIKE_MAX * SYSYEM_TICK_PER_US / US_TO_NS;
            break;
        //case I2C_SPEED_HIGH:
        default:
            if( u32EnStatus & I2C_ENABLE_EN )
                _Hal_I2c_Eanble(1);
            return 1;
    }

    I2C->SDA_HOLD = (I2C_SDA_HOLD_NS * SYSYEM_TICK_PER_US + US_TO_NS/2) / US_TO_NS;
    
    // Enable if need
    if( u32EnStatus & I2C_ENABLE_EN )
        _Hal_I2c_Eanble(1);
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_FsSpeedSet
* 
* DESCRIPTION:
*   1. Setup mode to Fast-mode and given the High/Low counters
*      model: 600ns + HCNT time + LCNT time = 1 / Freq
* 
* CALLS
* 
* PARAMETERS
*   1. u16Hcnt : High voltage time, unit: ticks
*   2. u16Lcnt : Low voltage time, unit: ticks
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_FsClockSet_impl(uint16_t u16Hcnt, uint16_t u16Lcnt)
{
    uint32_t u32EnStatus = 0;
    uint32_t u32Temp = 0;

    // Disable before set
    u32EnStatus = I2C->ENABLE;
    if( u32EnStatus & I2C_ENABLE_EN )
        _Hal_I2c_Eanble(0);

    // Force to Fast mode
    u32Temp = I2C->CON & ~(I2C_CON_SPEED_MASK);
    I2C->CON = u32Temp | I2C_CON_SPEED_FAST;
    
    // Set CNTs
    I2C->FS_SCL_HCNT = u16Hcnt;
    I2C->FS_SCL_LCNT = u16Lcnt;
    
    // Enable if need
    if( u32EnStatus & I2C_ENABLE_EN )
        _Hal_I2c_Eanble(1);

    return 0;
}

/*************************************************************************
* FUNCTION:
*   Hal_I2c_IntHandler
*
* DESCRIPTION:
*   the handler of I2C
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_I2c_IntHandler_impl(void)
{
    uint32_t ulIrqClr;
    uint8_t u8Data;

    // slave write
	if (I2C->INTR_STAT & I2C_INT_RD_REQ)
    {
        if (g_tHalI2cIntTxCallback != 0)
            g_tHalI2cIntTxCallback();
        
        // clear IRQ
        ulIrqClr = I2C->CLR_RD_REQ;
    }
    
    // slave read
    if (I2C->INTR_STAT & I2C_INT_RX_FULL)
    {
        Hal_I2c_SlaveReceive(&u8Data, 1);
        if (g_tHalI2cIntRxCallback != 0)
            g_tHalI2cIntRxCallback(u8Data);
        
        // automatically clear IRQ
    }
    
    // tx abort
	if (I2C->INTR_STAT & I2C_INT_TX_ABRT)
    {
        // clear IRQ
        ulIrqClr = I2C->CLR_TX_ABRT;
    }
    
    (void)ulIrqClr;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_TargetAddrSet
* 
* DESCRIPTION:
*   1. Setup master's target address
* 
* CALLS
* 
* PARAMETERS
*   1. u16TargetAddr : Target address
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_TargetAddrSet_impl(uint16_t u16TargetAddr)
{
    uint32_t u32EnStatus = 0;
    
    // Wait for previous action complete, generate START bit
    if( _Hal_I2c_WaitForMasterCompleted() )
        return 1;

    // Disable before set
    u32EnStatus = I2C->ENABLE;
    if( u32EnStatus & I2C_ENABLE_EN )
        _Hal_I2c_Eanble(0);
    
    I2C->TAR &= ~I2C_TAR_TARGET_ADDR_MASK;
    I2C->TAR |= (u16TargetAddr & I2C_TAR_TARGET_ADDR_MASK);
    
    // Enable if need
    if( u32EnStatus & I2C_ENABLE_EN )
        _Hal_I2c_Eanble(1);
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_MasterTrasmit
* 
* DESCRIPTION:
*   1. Master transmit, used before Hal_I2c_TargetAddrSet
* 
* CALLS
* 
* PARAMETERS
*   1. pu8Data      : Data to transmit
*   2. u32Length    : Data length
*   3. u8NeedStopBit: Need Stop bit when finished
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_MasterTrasmit_impl(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit)
{
    uint32_t u32Count = 0;
    uint32_t u32Temp = 0;
    uint32_t u32Idx = 0;

    // Wait for previous action complete, generate START bit
    if( _Hal_I2c_WaitForMasterCompleted() )
        return 1;

    // !!! enter the critical section
    osEnterCritical();

    // Transmit data
    while(u32Idx < u32Length)
    {
        u32Temp = (uint32_t)pu8Data[u32Idx] | I2C_DATA_CMD_WRITE;

        // Check last data
        if(u32Idx == u32Length-1)
        {
            //Check for stop bit
            if(u8NeedStopBit)
                u32Temp |= I2C_DATA_STOP_BIT;
        }
        // Wait for TX not full
        while( !(I2C->STATUS & I2C_STATUS_TX_NOT_FULL) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
            {
                // !!! exit the critical section
                osExitCritical();
                return 1;
            }
            u32Count++;
        }
        
        // Send data
        I2C->DATA_CMD = u32Temp;
        u32Idx++;
    }

    // !!! exit the critical section
    osExitCritical();
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_MasterRecieve
* 
* DESCRIPTION:
*   1. Master receive, used before Hal_I2c_TargetAddrSet
* 
* CALLS
* 
* PARAMETERS
*   1. pu8Data      : Data buff to receive
*   2. u32Length    : Data length
*   3. u8NeedStopBit: Need Stop bit when finished
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_MasterReceive_impl(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit)
{
    uint32_t u32Count = 0;
    uint32_t u32Temp = 0;
    uint32_t u32Idx = 0;
    uint32_t u32WrIdx = 0;

    // Wait for previous action complete, generate START bit
    if( _Hal_I2c_WaitForMasterCompleted() )
        return 1;

    // !!! enter the critical section
    osEnterCritical();
    
    // Tx dummy only
    while ((u32WrIdx < I2C_FIFO_SIZE) && (u32WrIdx < u32Length))
    {
        u32Temp = I2C_DATA_CMD_READ;

        // Check STOP bit
        if(u32WrIdx == u32Length-1)
        {
            //Last data, check for stop bit
            if(u8NeedStopBit)
                u32Temp |= I2C_DATA_STOP_BIT;
        }
        
        // Wait for TX not full
        u32Count = 0;
        while( !(I2C->STATUS & I2C_STATUS_TX_NOT_FULL) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
            {
                // !!! exit the critical section
                osExitCritical();
                return 1;
            }
            u32Count++;
        }
        
        // Send data
        I2C->DATA_CMD = u32Temp;
        u32WrIdx++;
    }
    
    // Tx and Rx
    while (u32WrIdx < u32Length)
    {
        // Rx
        // Wait for RX not empty
        u32Count = 0;
        while( !(I2C->STATUS & I2C_STATUS_RX_NOT_EMPTY) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
            {
                // !!! exit the critical section
                osExitCritical();
                return 1;
            }
            u32Count++;
        }
        // Get data
        pu8Data[u32Idx] = (uint8_t)(I2C->DATA_CMD & I2C_DATA_CMD_DATA_MASK);
        u32Idx++;
        
        // Tx
        u32Temp = I2C_DATA_CMD_READ;

        // Check STOP bit
        if(u32WrIdx == u32Length-1)
        {
            //Last data, check for stop bit
            if(u8NeedStopBit)
                u32Temp |= I2C_DATA_STOP_BIT;
        }
        
        // Wait for TX not full
        u32Count = 0;
        while( !(I2C->STATUS & I2C_STATUS_TX_NOT_FULL) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
            {
                // !!! exit the critical section
                osExitCritical();
                return 1;
            }
            u32Count++;
        }
        
        // Send data
        I2C->DATA_CMD = u32Temp;
        u32WrIdx++;
    }
    
    // Rx only
    while (u32Idx < u32Length)
    {
        // Rx
        // Wait for RX not empty
        u32Count = 0;
        while( !(I2C->STATUS & I2C_STATUS_RX_NOT_EMPTY) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
            {
                // !!! exit the critical section
                osExitCritical();
                return 1;
            }
            u32Count++;
        }
        // Get data
        pu8Data[u32Idx] = (uint8_t)(I2C->DATA_CMD & I2C_DATA_CMD_DATA_MASK);
        u32Idx++;
    }
    
    // !!! exit the critical section
    osExitCritical();
    return 0;
}

/**
 * @brief I2C master TX command, restart, then RX command
 *   @verbatim
 *                |----------------------------------------     ------------------------------------------------------------     -------------------------|
 *    Data format | START | ADDR+w | ACK | WData[0] | ACK | ... | WData[m] | ACK | RESTART | ADDR+r | ACK | RData[0] | ACK | ... | RData[n] | NACK | STOP |
 *                |-------+--------+-----+----------+-----+     +----------+-----+---------+--------+-----+----------+-----+     +----------+------+------|
 *    Transmitter |   M   |   M    |  S  |     M    |  S  | ... |     M    |  S  |    M    |    M   |  S  |     S    |  M  | ... |     S    |  M   |   M  |
 *                |----------------------------------------     ------------------------------------------------------------     -------------------------|
 *                                                                                  ^^^^^Without STOP bit, send START bit
 *   @endverbatim 
 *   Only needs this function in OPL1000. Because IC_EMPTYFIFO_HOLD_MASTER_EN is 0 in OPL1000.
 *
 * @param pu8WriteData [in] TX data
 * @param u32WriteLength [in] Length of TX data
 * @param pu8ReadData [out] Buffer to store RX data
 * @param u32ReadLength [in] Length of RX data
 * @return Status. 1: Error, 0: success
 */
uint32_t Hal_I2c_MasterTxRestartRx_impl(uint8_t *pu8WriteData, uint32_t u32WriteLength, uint8_t *pu8ReadData, uint32_t u32ReadLength)
{
    uint32_t u32CmdIdx = 0;
    uint32_t u32DataIdx = 0;
    uint32_t u32Status = 1;
    uint32_t u32Count = 0;

    // Wait for previous action complete, generate START bit
    if( _Hal_I2c_WaitForMasterCompleted() )
        return 1;
    
    osEnterCritical();
    
    /* TX command */
    while(u32CmdIdx < u32WriteLength)
    {
        u32Count = 0;
        while( !(I2C->STATUS & I2C_STATUS_TX_NOT_FULL) )
        {
            if(u32Count++ > I2C_TIMEOUT_COUNT_MAX)
                goto end;
        }
        I2C->DATA_CMD = I2C_DATA_CMD_WRITE | pu8WriteData[u32CmdIdx++];
    }

    u32CmdIdx = 0;
    while((u32CmdIdx < u32ReadLength) || (u32DataIdx < u32ReadLength))
    {
        if (u32CmdIdx < u32ReadLength)
        {
            /* TX command */
            u32Count = 0;
            while( !(I2C->STATUS & I2C_STATUS_TX_NOT_FULL) )
            {
                if(u32Count++ > I2C_TIMEOUT_COUNT_MAX)
                    goto end;
            }
            I2C->DATA_CMD = I2C_DATA_CMD_READ;
            
            /* avoid TX FIFO empty, send TX FIFO full first */
            if ((++u32CmdIdx + u32WriteLength) < I2C_FIFO_SIZE)
                continue;
        }
                
        /* RX data */
        u32Count = 0;
        while( !(I2C->STATUS & I2C_STATUS_RX_NOT_EMPTY) )
        {
            if(u32Count++ > I2C_TIMEOUT_COUNT_MAX)
                goto end;
        }
        pu8ReadData[u32DataIdx++] = I2C->DATA_CMD;
    }
    u32Status = 0;
    
end:
    osExitCritical();
    return u32Status;
}


/*************************************************************************
* FUNCTION:
*  Hal_I2c_MasterInit
* 
* DESCRIPTION:
*   1. Initial I2c as master mode
* 
* CALLS
* 
* PARAMETERS
*   1. eAddrMode: 7/10bit Address mode. refer to E_I2cAddrMode_t
*   2. eSpeed   : Standard(100K)/Fast(400K)/High speed(3.4M) mode. refer to E_I2cSpeed_t
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_MasterInit_impl(E_I2cAddrMode_t eAddrMode, E_I2cSpeed_t eSpeed)
{
    // Enable clock of module
    Hal_Sys_ApsClkEn(1, APS_CLK_I2C);
    // Reset module
    Hal_Sys_ApsModuleRst(APS_RST_I2C);
    
    // ---> Disable first, to set registers 
    _Hal_I2c_Eanble( 0 );
    
    // Master Mode, and restart enable
    I2C->CON = I2C_CON_MASTER_MODE | I2C_CON_SLAVE_DISABLE | I2C_CON_RESTART_EN;
    
    // Address Bits
    if(eAddrMode == I2C_10BIT)
    {
        // 10-Bit Master
        I2C->CON |= I2C_CON_MASTER_10BIT;
    }else{
        // 7-Bit Master
        I2C->CON |= I2C_CON_MASTER_07BIT;
    }
    
    // Set FIFO
    I2C->RX_TL = 0;
    I2C->TX_TL = 0;
    
    // Speed
    Hal_I2c_SpeedSet( eSpeed );

    // Mask all interrupt, handle Tx abort
    I2C->INTR_MASK = I2C_INT_TX_ABRT;
    
    // <--- Enable modules
    _Hal_I2c_Eanble( 1 );
    
    // VIC 1) Clear interrupt
    Hal_Vic_IntClear(I2C_IRQn);
    // VIC 2) un-Mask VIC
    Hal_Vic_IntMask(I2C_IRQn, 0);
    // VIC 3) Enable VIC
    Hal_Vic_IntEn(I2C_IRQn, 1);

    // NVIC 1) Clean NVIC
    NVIC_ClearPendingIRQ(I2C_IRQn);
    // NVIC 2) Set prority
    NVIC_SetPriority(I2C_IRQn, IRQ_PRIORITY_I2C);
    // NVIC 3) Enable NVIC
    NVIC_EnableIRQ(I2C_IRQn);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveTrasmit
* 
* DESCRIPTION:
*   1. Slave transmit
* 
* CALLS
* 
* PARAMETERS
*   1. pu8Data      : Data to transmit
*   2. u32Length    : Data length
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveTrasmit_impl(uint8_t *pu8Data, uint32_t u32Length)
{
    uint32_t u32Count = 0;
    uint32_t u32Idx = 0;

    // Wait for previous action complete, generate START bit
    if( _Hal_I2c_WaitForMasterCompleted() )
        return 1;

    // Transmit data
    while(u32Idx < u32Length)
    {
        // Wait for TX not full
        while( !(I2C->STATUS & I2C_STATUS_TX_NOT_FULL) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
                return 1;
            u32Count++;
        }
        
        // Send data
        I2C->DATA_CMD = pu8Data[u32Idx];
        u32Idx++;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveReceive
* 
* DESCRIPTION:
*   1. Slave receive
* 
* CALLS
* 
* PARAMETERS
*   1. pu8Data      : Data buff to receive
*   2. u32Length    : Data length
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveReceive_impl(uint8_t *pu8Data, uint32_t u32Length)
{
    uint32_t u32Count = 0;
    uint32_t u32Idx = 0;

    // Wait for previous action complete, generate START bit
    if( _Hal_I2c_WaitForMasterCompleted() )
        return 1;
    
    while(u32Idx < u32Length)
    {
        // Wait for RX not empty
        while( !(I2C->STATUS & I2C_STATUS_RX_NOT_EMPTY) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
                return 1;
            u32Count++;
        }
        // Get data
        pu8Data[u32Idx] = (uint8_t)(I2C->DATA_CMD & I2C_DATA_CMD_DATA_MASK);
        u32Idx++;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_MasterInit
* 
* DESCRIPTION:
*   1. Initial I2c as master mode
* 
* CALLS
* 
* PARAMETERS
*   1. eAddrMode      : 7/10bit Address mode. refer to E_I2cAddrMode_t
*   2. u16SlaveAddr   : Slave address
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveInit_impl(E_I2cAddrMode_t eAddrMode, uint16_t u16SlaveAddr)
{
    // Enable clock of module
    Hal_Sys_ApsClkEn(1, APS_CLK_I2C);
    // Reset module
    Hal_Sys_ApsModuleRst(APS_RST_I2C);
    
    // ---> Disable first, to set registers 
    _Hal_I2c_Eanble( 0 );
    
    // Slave Mode, and restart enable
    I2C->CON = I2C_CON_RESTART_EN;
    
    // Address Bits
    if(eAddrMode == I2C_10BIT)
    {
        // 10-Bit Master
        I2C->CON |= I2C_CON_SLAVE_10BIT;
    }
    
    // Set FIFO
    I2C->RX_TL = 0;
    I2C->TX_TL = 0;
    
    // Speed, recommend set to max speed available 
    Hal_I2c_SpeedSet( I2C_SPEED_FAST );

    // Mask all interrupt, handle Tx abort
    I2C->INTR_MASK = I2C_INT_TX_ABRT;
    
    // Slave address
    I2C->SAR = u16SlaveAddr & I2C_SAR_SLAVE_ADDR_MASK;
    
    // <--- Enable modules
    _Hal_I2c_Eanble( 1 );
    
    // VIC 1) Clear interrupt
    Hal_Vic_IntClear(I2C_IRQn);
    // VIC 2) un-Mask VIC
    Hal_Vic_IntMask(I2C_IRQn, 0);
    // VIC 3) Enable VIC
    Hal_Vic_IntEn(I2C_IRQn, 1);

    // NVIC 1) Clean NVIC
    NVIC_ClearPendingIRQ(I2C_IRQn);
    // NVIC 2) Set prority
    NVIC_SetPriority(I2C_IRQn, IRQ_PRIORITY_I2C);
    // NVIC 3) Enable NVIC
    NVIC_EnableIRQ(I2C_IRQn);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveIntTxEn
* 
* DESCRIPTION:
*   1. enable / disable the slave Tx interrupt
* 
* CALLS
* 
* PARAMETERS
*   1. u8Enable: 1 for enable/0 for disable
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveIntTxEn_impl(uint8_t u8Enable)
{
    if(u8Enable)
    {
        I2C->INTR_MASK |= I2C_INT_RD_REQ;
    }
    else
    {
        I2C->INTR_MASK &= ~I2C_INT_RD_REQ;
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveIntRxEn
* 
* DESCRIPTION:
*   1. enable / disable the slave Rx interrupt
* 
* CALLS
* 
* PARAMETERS
*   1. u8Enable: 1 for enable/0 for disable
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveIntRxEn_impl(uint8_t u8Enable)
{
    if(u8Enable)
    {
        I2C->INTR_MASK |= I2C_INT_RX_FULL;
    }
    else
    {
        I2C->INTR_MASK &= ~I2C_INT_RX_FULL;
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveIntTxCallBackFuncSet
* 
* DESCRIPTION:
*   1. set the slave Tx callback fucntion
* 
* CALLS
* 
* PARAMETERS
*   1. tFunc: the callback function for slave Tx
* 
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_I2c_SlaveIntTxCallBackFuncSet_impl(T_Hal_I2c_SlaveIntTxCallBack tFunc)
{
    g_tHalI2cIntTxCallback = tFunc;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveIntRxCallBackFuncSet
* 
* DESCRIPTION:
*   1. set the slave Rx callback fucntion
* 
* CALLS
* 
* PARAMETERS
*   1. tFunc: the callback function for slave Rx
* 
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_I2c_SlaveIntRxCallBackFuncSet_impl(T_Hal_I2c_SlaveIntRxCallBack tFunc)
{
    g_tHalI2cIntRxCallback = tFunc;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_Pre_Init
*
* DESCRIPTION:
*   1. Inital for functuin pointers and glabal parameters
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
void Hal_I2c_Pre_Init(void)
{
    /* Internal */
    _Hal_I2c_Eanble                 = _Hal_I2c_Eanble_impl;
    _Hal_I2c_WaitForMasterCompleted = _Hal_I2c_WaitForMasterCompleted_impl;

    Hal_I2c_SpeedGet   = Hal_I2c_SpeedGet_impl;
    Hal_I2c_SpeedSet   = Hal_I2c_SpeedSet_impl;
	Hal_I2c_FsClockSet = Hal_I2c_FsClockSet_impl;
    Hal_I2c_IntHandler = Hal_I2c_IntHandler_impl;

    /* Master mode relative */
    Hal_I2c_TargetAddrSet       = Hal_I2c_TargetAddrSet_impl;
    Hal_I2c_MasterReceive       = Hal_I2c_MasterReceive_impl;
    Hal_I2c_MasterTrasmit       = Hal_I2c_MasterTrasmit_impl;
    Hal_I2c_MasterTxRestartRx   = Hal_I2c_MasterTxRestartRx_impl;
    Hal_I2c_MasterInit          = Hal_I2c_MasterInit_impl;

    /* Slave mode relative */
    g_tHalI2cIntTxCallback = 0;
    g_tHalI2cIntRxCallback = 0;
    Hal_I2c_SlaveReceive              = Hal_I2c_SlaveReceive_impl;
    Hal_I2c_SlaveTrasmit              = Hal_I2c_SlaveTrasmit_impl;
    Hal_I2c_SlaveInit                 = Hal_I2c_SlaveInit_impl;
    Hal_I2c_SlaveIntTxEn              = Hal_I2c_SlaveIntTxEn_impl;
    Hal_I2c_SlaveIntRxEn              = Hal_I2c_SlaveIntRxEn_impl;
    Hal_I2c_SlaveIntTxCallBackFuncSet = Hal_I2c_SlaveIntTxCallBackFuncSet_impl;
    Hal_I2c_SlaveIntRxCallBackFuncSet = Hal_I2c_SlaveIntRxCallBackFuncSet_impl;
}
