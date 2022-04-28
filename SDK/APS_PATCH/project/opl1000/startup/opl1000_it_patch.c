/* *****************************************************************************
 *  Copyright 2017-2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file opl1000_it_patch.c
 * 
 *  @brief 
 *  
 ******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "opl1000_it_patch.h"
#include "cmsis_os.h"
#include "hal_wdt.h"
#include "hal_system_patch.h"
#include "hal_vic_patch.h"
#include "ps_patch.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define HARD_FAULT_HANDLER_VECT_ADDR  0x0000000C
#define APS_IPATCH_126_ENTRY_ADDR     0x3000E248
#define APS_IPATCH_126_DATA_ADDR      0x004001F8
#define APS_IPATCH_EN_3_ADDR          0x3000E260
#define APS_IPATCH_126_MSK            (1 << (126 & 0x1F))
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef void (*T_InterruptHandler)(void);
typedef void (*T_ExceptionDumpStack)(uint32_t u32RegPsp, uint32_t u32RegMsp, uint32_t u32RegLr);
/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
void WDT_IRQHandler_Entry_patch(void);
void HardFault_Handler_patch(void);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 

extern T_InterruptHandler WDT_IRQHandler_Entry;
extern T_ExceptionDumpStack ExceptionDumpStack;


 
 
 
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */


void ISR_PatchInit(void)
{
    WDT_IRQHandler_Entry     = WDT_IRQHandler_Entry_patch;
}
/* Setup Hard fault handler to HW patch entry 126 */
void ISR_SetupHardfaultPatch(void)
{
    reg_write(APS_IPATCH_126_ENTRY_ADDR, HARD_FAULT_HANDLER_VECT_ADDR);
    reg_write(APS_IPATCH_126_DATA_ADDR, (uint32_t)&HardFault_Handler_patch);
    reg_write(APS_IPATCH_EN_3_ADDR, reg_read(APS_IPATCH_EN_3_ADDR) | APS_IPATCH_126_MSK);
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */



void WDT_PostProcess(uint32_t u32RegPsp, uint32_t u32RegMsp, uint32_t u32RegLr)
{
    Hal_Vic_IntClear(WDT_IRQn);
    tracer_drct_printf("WDT\n");
    ExceptionDumpStack(u32RegPsp, u32RegMsp, u32RegLr);

    Hal_Sys_SwResetAll();
}
void HardFault_PostProcess(uint32_t u32RegPsp, uint32_t u32RegMsp, uint32_t u32RegLr)
{
    /* Stop WDT first, avoid WDT reset when clockswitch is from 1p2g */
    Hal_Wdt_Stop();
    
    tracer_drct_printf("Hard fault\nE000ED28: %08X %08X", reg_read(0xE000ED28), reg_read(0xE000ED2C));
    tracer_drct_printf(" %08X %08X %08X %08X\n\n", reg_read(0xE000ED30), reg_read(0xE000ED34), reg_read(0xE000ED38), reg_read(0xE000ED3C));
    ExceptionDumpStack(u32RegPsp, u32RegMsp, u32RegLr);
    
    Hal_Sys_SwResetAll();
}

#if defined(GCC)
void WDT_IRQHandler_Entry_patch(void)
{
    __asm volatile (
        "MRS r0, psp\n\t"
        "MRS r1, msp\n\t"
        "MOV r2, lr\n\t"
        "BL WDT_PostProcess\n\t"
        );
}
void HardFault_Handler_patch(void)
{
    __asm volatile (
        "MRS r0, psp\n\t"
        "MRS r1, msp\n\t"
        "MOV r2, lr\n\t"
        "BL HardFault_PostProcess\n\t"
        );
}
#else
__ASM void WDT_IRQHandler_Entry_patch(void)
{
    IMPORT WDT_PostProcess
    
    MRS r0, psp
    MRS r1, msp
    MOV r2, lr
    BL WDT_PostProcess
}
__ASM void HardFault_Handler_patch(void)
{
    IMPORT HardFault_PostProcess
    
    MRS r0, psp
    MRS r1, msp
    MOV r2, lr
    BL HardFault_PostProcess
}
#endif
