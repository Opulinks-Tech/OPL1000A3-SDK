/******************************************************************************
*  Copyright 2017 - 2020, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2020
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  aos_reg.h
*
*  Project:
*  --------
*  NL1000_Ax series
*
*  Description:
*  ------------
*  This include file defines the registers of AOS .
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

#ifndef __AOS_REG_H__
#define __AOS_REG_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "opl1000.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define AOS             ((S_Aos_Reg_t *) AOS_BASE)


// AOS 0x000
#define AOS_RET_HW_SETTING           1
#define AOS_RET_FW_SETTING           0

// AOS 0x004
#define AOS_SLP_MODE_EN              (0x1 << 0)
#define AOS_SLP_MODE_POS             1
#define AOS_SLP_MODE_MASK            (0x1 << AOS_SLP_MODE_POS)
#define AOS_SLP_MODE_TIMER           (0 << AOS_SLP_MODE_POS)
#define AOS_SLP_MODE_DEEP            (1 << AOS_SLP_MODE_POS)
#define AOS_SLP_MODE_EARLY_WAKE_UP   (0x1 << 2)

// AOS 0x008
#define AOS_OSC_SEL_32K_OSC_SEL_Pos   (0U)
#define AOS_OSC_SEL_32K_OSC_SEL_Msk   (0x00000001U << AOS_OSC_SEL_32K_OSC_SEL_Pos)   /*!< 0x00000001 */
#define AOS_OSC_SEL_32K_OSC_SEL       AOS_OSC_SEL_32K_OSC_SEL_Msk                    /*!< 32kHz clock select for digital logic ? SEQ, RTC timer, etc 1'b0: select 32kHz RC clock 1'b1: select 32kHz crystal osc */
#define AOS_OSC_SEL_RTC_EN_Pos        (1U)
#define AOS_OSC_SEL_RTC_EN_Msk        (0x00000001U << AOS_OSC_SEL_RTC_EN_Pos)        /*!< 0x00000002 */
#define AOS_OSC_SEL_RTC_EN

// AOS 0x014
#define AOS_SLP_TIMER_MASK_L         0xFFFFFFFF

// AOS 0x018
#define AOS_SLP_TIMER_MASK_H         0x1F
#define AOS_SLP_TIMER_MASK_MAX       ((uint64_t)(AOS_SLP_TIMER_MASK_H)<<32 | AOS_SLP_TIMER_MASK_L)

// AOS 0x020
#define AOS_RET_SF_VOL_POS           0
#define AOS_RET_SF_VOL_MSK           (0xF << AOS_RET_SF_VOL_POS)
#define AOS_RET_SF_VOL_0P55          ( 0 << AOS_RET_SF_VOL_POS)
#define AOS_RET_SF_VOL_0P67          ( 3 << AOS_RET_SF_VOL_POS)
#define AOS_RET_SF_VOL_0P86          ( 8 << AOS_RET_SF_VOL_POS)
#define AOS_RET_SF_VOL_1P20          (15 << AOS_RET_SF_VOL_POS)

// AOS 0x028
#define AOS_PTAT_EN                  (0x1 << 17)
#define AOS_TEMP_EN                  (0x1 << 18)

// AOS 0x030
#define AOS_BUCK_DELAY_POS           0
#define AOS_BUCK_DELAY_MSK           (0x3 << AOS_BUCK_DELAY_POS)
#define AOS_BUCK_DELAY_0             (0 << AOS_BUCK_DELAY_POS)
#define AOS_BUCK_DELAY_1             (1 << AOS_BUCK_DELAY_POS)
#define AOS_BUCK_DELAY_2             (2 << AOS_BUCK_DELAY_POS)
#define AOS_BUCK_DELAY_3             (3 << AOS_BUCK_DELAY_POS)
#define AOS_BUCK_FC_POS              3
#define AOS_BUCK_FC_MSK              (0x7 << AOS_BUCK_FC_POS)
#define AOS_BUCK_FC_1M               (0 << AOS_BUCK_FC_POS)
#define AOS_BUCK_FC_2D4M             (4 << AOS_BUCK_FC_POS)
#define AOS_BUCK_FC_4D4M             (7 << AOS_BUCK_FC_POS)
#define AOS_BUCK_FI_POS              6
#define AOS_BUCK_FI_MSK              (0x7 << AOS_BUCK_FI_POS)
#define AOS_BUCK_FI_1M               (0 << AOS_BUCK_FI_POS)
#define AOS_BUCK_FI_2D4M             (4 << AOS_BUCK_FI_POS)
#define AOS_BUCK_FI_4D4M             (7 << AOS_BUCK_FI_POS)
#define AOS_BUCK_OTA_HS_POS          9
#define AOS_BUCK_OTA_HS_MSK          (0x1 << AOS_BUCK_OTA_HS_POS)
#define AOS_BUCK_OTA_HS_LIGHT        (0 << AOS_BUCK_OTA_HS_POS)
#define AOS_BUCK_OTA_HS_HEAVY        (1 << AOS_BUCK_OTA_HS_POS)
#define AOS_BUCK_PWM_HS_POS          10
#define AOS_BUCK_PWM_HS_MSK          (0x1 << AOS_BUCK_PWM_HS_POS)
#define AOS_BUCK_PWM_HS_LIGHT        (0 << AOS_BUCK_PWM_HS_POS)
#define AOS_BUCK_PWM_HS_HEAVY        (1 << AOS_BUCK_PWM_HS_POS)
#define AOS_BUCK_MODE_POS            15
#define AOS_BUCK_MODE_MSK            (0x1 << AOS_BUCK_MODE_POS)
#define AOS_BUCK_MODE_BURST          (0 << AOS_BUCK_MODE_POS)
#define AOS_BUCK_MODE_PWM            (1 << AOS_BUCK_MODE_POS)
#define AOS_BUCK_ZRC_EN_POS          18
#define AOS_BUCK_ZRC_EN_MSK          (0x1 << AOS_BUCK_ZRC_EN_POS)
#define AOS_BUCK_ZRC_EN_OFF          (0 << AOS_BUCK_ZRC_EN_POS)
#define AOS_BUCK_ZRC_EN_ON           (1 << AOS_BUCK_ZRC_EN_POS)
#define AOS_BUCK_V1D3_VAL_POS        19
#define AOS_BUCK_V1D3_VAL_MSK        (0x3 << AOS_BUCK_V1D3_VAL_POS)
#define AOS_BUCK_V1D3_VAL_1D2V       (0 << AOS_BUCK_V1D3_VAL_POS)
#define AOS_BUCK_V1D3_VAL_1D3V       (1 << AOS_BUCK_V1D3_VAL_POS)
#define AOS_BUCK_V1D3_VAL_1D4V       (2 << AOS_BUCK_V1D3_VAL_POS)
#define AOS_BUCK_V1D3_VAL_1D5V       (3 << AOS_BUCK_V1D3_VAL_POS)
#define AOS_BUCK_V1D1_VAL_POS        21
#define AOS_BUCK_V1D1_VAL_MSK        (0x7 << AOS_BUCK_V1D1_VAL_POS)
#define AOS_BUCK_V1D1_VAL_0D85V      (0 << AOS_BUCK_V1D1_VAL_POS)
#define AOS_BUCK_V1D1_VAL_0D9V       (1 << AOS_BUCK_V1D1_VAL_POS)
#define AOS_BUCK_V1D1_VAL_1V         (2 << AOS_BUCK_V1D1_VAL_POS)
#define AOS_BUCK_V1D1_VAL_1D1V       (4 << AOS_BUCK_V1D1_VAL_POS)
#define AOS_BUCK_V1D1_VAL_1D25V      (7 << AOS_BUCK_V1D1_VAL_POS)

// AOS 0x034-0x04C
#define SEQ_HPBG_ON_POS              10
#define SEQ_HPBG_ON                  (0x1 << SEQ_HPBG_ON_POS)
#define SEQ_BUCK_ON_POS              11
#define SEQ_BUCK_ON                  (0x1 << SEQ_BUCK_ON_POS)
#define SEQ_RET_EN_POS               12
#define SEQ_RET_EN                   (0x1 << SEQ_RET_EN_POS)
#define SEQ_SDM_RST_POS              13
#define SEQ_SDM_RST                  (0x1 << SEQ_SDM_RST_POS)
#define SEQ_SW_ON_POS                14
#define SEQ_SW_ON                    (0x1 << SEQ_SW_ON_POS)
#define SEQ_ISO_EN_POS               15
#define SEQ_ISO_EN                   (0x1 << SEQ_ISO_EN_POS)

// AOS 0x0x80
#define AOS_OSC_CTL_LPO_ON_Pos            (0U)
#define AOS_OSC_CTL_LPO_ON_Msk            (0x00000001U << AOS_OSC_CTL_LPO_ON_Pos)            /*!< 0x00000001 */
#define AOS_OSC_CTL_LPO_ON                AOS_OSC_CTL_LPO_ON_Msk                             /*!< override enable (refer to bit2 for override value)  1'b1: use RG_32K_LPO_EN to control LPO 1'b0: use SEQ state to control LPO */
#define AOS_OSC_CTL_32K_XTAL_EN_Pos       (1U)
#define AOS_OSC_CTL_32K_XTAL_EN_Msk       (0x00000001U << AOS_OSC_CTL_32K_XTAL_EN_Pos)       /*!< 0x00000002 */
#define AOS_OSC_CTL_32K_XTAL_EN           AOS_OSC_CTL_32K_XTAL_EN_Msk                        /*!< override value (refer to bit 10 override enable)  1'b1: turn on XTAL  1'b0: turn off XTAL */
#define AOS_OSC_CTL_32K_LPO_EN_Pos        (2U)
#define AOS_OSC_CTL_32K_LPO_EN_Msk        (0x00000001U << AOS_OSC_CTL_32K_LPO_EN_Pos)        /*!< 0x00000004 */
#define AOS_OSC_CTL_32K_LPO_EN            AOS_OSC_CTL_32K_LPO_EN_Msk                         /*!< override value (refer to bit 0) 1'b1: turn on LPO (default) 1'b0: turn off LPO */
#define AOS_OSC_CTL_32K_XTAL_GM_Pos       (3U)
#define AOS_OSC_CTL_32K_XTAL_GM_Msk       (0x0000000FU << AOS_OSC_CTL_32K_XTAL_GM_Pos)       /*!< 0x00000078 */
#define AOS_OSC_CTL_32K_XTAL_GM           AOS_OSC_CTL_32K_XTAL_GM_Msk                        /*!< 32k XTAL loop gain control C: for start up 1: for low power oscillate maintain Such value will be determined by CL of xtal */
#define AOS_OSC_CTL_32K_LPO_FC_Pos        (7U)
#define AOS_OSC_CTL_32K_LPO_FC_Msk        (0x00000007U << AOS_OSC_CTL_32K_LPO_FC_Pos)        /*!< 0x00000380 */
#define AOS_OSC_CTL_32K_LPO_FC            AOS_OSC_CTL_32K_LPO_FC_Msk                         /*!< LPO frequency control 1:16Khz 2:22Khz 4:32Khz 7:46Khz */
#define AOS_OSC_CTL_XTAL_ON_Pos           (10U)
#define AOS_OSC_CTL_XTAL_ON_Msk           (0x00000001U << AOS_OSC_CTL_XTAL_ON_Pos)           /*!< 0x00000400 */
#define AOS_OSC_CTL_XTAL_ON               AOS_OSC_CTL_XTAL_ON_Msk                            /*!< override enable (refer to bit1 for the override value)  1'b1: use RG_32K_XTAL_EN to control XTAL 1'b0: use SEQ state to control XTAL */
#define AOS_OSC_CTL_32K_XTAL_CAP_Pos      (11U)
#define AOS_OSC_CTL_32K_XTAL_CAP_Msk      (0x0000000FU << AOS_OSC_CTL_32K_XTAL_CAP_Pos)      /*!< 0x00007800 */
#define AOS_OSC_CTL_32K_XTAL_CAP          AOS_OSC_CTL_32K_XTAL_CAP_Msk                       /*!< Crystal cap adjustment 0: 1pF highest frequency error/ lowest current consumption F: 12pF lowest frequency/ current consumption */
#define AOS_OSC_CTL_32K_XTAL_BYPASS_Pos   (15U)
#define AOS_OSC_CTL_32K_XTAL_BYPASS_Msk   (0x00000001U << AOS_OSC_CTL_32K_XTAL_BYPASS_Pos)   /*!< 0x00008000 */
#define AOS_OSC_CTL_32K_XTAL_BYPASS       AOS_OSC_CTL_32K_XTAL_BYPASS_Msk                    /*!< 1'b1: bypass 32k xtal, use external clk; 1'b0: not bypass 32k xtal; */

// AOS 0x084
#define AOS_TEST_PAD_TO_MUX2_EN      (0x1 << 0)
#define AOS_TEST_PAD_SRC_POS         1
#define AOS_TEST_PAD_SRC_MSK         (0x7 << AOS_TEST_PAD_SRC_POS)
#define AOS_TEST_PAD_SRC_VBAT        (0 << AOS_TEST_PAD_SRC_POS)  // HW Document fixed
#define AOS_TEST_PAD_SRC_GPIO        (1 << AOS_TEST_PAD_SRC_POS)  // HW Document fixed
#define AOS_TEST_PAD_SRC_LDO_VCO     (2 << AOS_TEST_PAD_SRC_POS)
#define AOS_TEST_PAD_SRC_LDO_RF      (3 << AOS_TEST_PAD_SRC_POS)
#define AOS_TEST_PAD_SRC_TEMP_SEN    (4 << AOS_TEST_PAD_SRC_POS)
#define AOS_TEST_PAD_SRC_HPBG_REF    (5 << AOS_TEST_PAD_SRC_POS)
#define AOS_TEST_PAD_SRC_LPBG_REF    (6 << AOS_TEST_PAD_SRC_POS)
#define AOS_TEST_PAD_SRC_PMU_SF      (7 << AOS_TEST_PAD_SRC_POS)
#define AOS_HPBG_TEST_REF_EN         (0x1 << 4)
#define AOS_AUXADC_CPOR_N_EN         (0x1 << 5)
#define AOS_HPBG_CAL_3_EN            (0x1 << 7)

// AOS 0x088
#define AOS_AUXADC_EN                (0x1 << 0)
#define AOS_SARADC_EN                (0x1 << 1)
#define AOS_AUX_REF_VTUNE_POS        2
#define AOS_AUX_REF_VTUNE_MSK        (0x1F << AOS_AUX_REF_VTUNE_POS)
#define AOS_ADC_IBIAS_POS            7
#define AOS_ADC_IBIAS_MSK            (0x1F << AOS_ADC_IBIAS_POS)
#define AOS_ADC_IBIAS_06UA           ( 0 << AOS_ADC_IBIAS_POS)
#define AOS_ADC_IBIAS_06P25UA        ( 1 << AOS_ADC_IBIAS_POS)
#define AOS_ADC_IBIAS_06P75UA        ( 3 << AOS_ADC_IBIAS_POS)
#define AOS_ADC_IBIAS_07P75UA        ( 5 << AOS_ADC_IBIAS_POS)
#define AOS_ADC_IBIAS_09P75UA        ( 7 << AOS_ADC_IBIAS_POS)
#define AOS_ADC_IBIAS_10UA           (16 << AOS_ADC_IBIAS_POS)
#define AOS_ADC_IBIAS_13P75UA        (31 << AOS_ADC_IBIAS_POS)

// AOS 0x08C
#define LDO_VCO_IBIAS_POS            0
#define LDO_VCO_IBIAS_MSK            0x1F
#define LDO_VCO_IBIAS_6UA            ( 0 << LDO_VCO_IBIAS_POS)
#define LDO_VCO_IBIAS_6P25UA         ( 1 << LDO_VCO_IBIAS_POS)
#define LDO_VCO_IBIAS_6P75UA         ( 3 << LDO_VCO_IBIAS_POS)
#define LDO_VCO_IBIAS_7P75UA         ( 5 << LDO_VCO_IBIAS_POS)
#define LDO_VCO_IBIAS_9P75UA         ( 7 << LDO_VCO_IBIAS_POS)
#define LDO_VCO_IBIAS_10UA           (16 << LDO_VCO_IBIAS_POS)
#define LDO_VCO_IBIAS_13P75UA        (31 << LDO_VCO_IBIAS_POS)
#define LDO_RF_EN                    (0x1 << 5)
#define LDO_VCO_EN                   (0x1 << 6)
#define LDO_RF_VAL_POS               7
#define LDO_RF_VAL_MSK               (0x3 << LDO_RF_VAL_POS)
#define LDO_RF_VAL_1V                (0 << LDO_RF_VAL_POS)
#define LDO_RF_VAL_1P1V              (1 << LDO_RF_VAL_POS)
#define LDO_RF_VAL_1P2V              (2 << LDO_RF_VAL_POS)
#define LDO_RF_VAL_1P3V              (3 << LDO_RF_VAL_POS)
#define LDO_RF_EN_POS                9
#define LDO_RF_EN_MSK                (0x1 << LDO_RF_EN_POS)
#define LDO_VCO_EN_POS               10
#define LDO_VCO_EN_MSK               (0x1 << LDO_VCO_EN_POS)
#define LDO_RF_IBIAS_POS             11
#define LDO_RF_IBIAS_MSK             (0xF << LDO_RF_IBIAS_POS)
#define LDO_VCO_VAL_POS              15
#define LDO_VCO_VAL_MSK              (0x3 << LDO_VCO_VAL_POS)
#define LDO_VCO_VAL_1V               (0 << LDO_VCO_VAL_POS)
#define LDO_VCO_VAL_1P1V             (1 << LDO_VCO_VAL_POS)
#define LDO_VCO_VAL_1P2V             (2 << LDO_VCO_VAL_POS)
#define LDO_VCO_VAL_1P3V             (3 << LDO_VCO_VAL_POS)

// AOS 0x0E0, 0x0E4 and 0x118
#define AOS_RET_RAM_MASK             0xFFFFF /* 20 bits*/
#define AOS_RET_RAM_TIMEOUT          0x500

// AOS 0x0F0
#define RC_CAL_PATTERN_LEN           12
#define RC_CAL_PATTERN_MSK           ( (1 << RC_CAL_PATTERN_LEN) - 1)

// AOS 0x100
#define AOS_STRAP_MODE_MAX           0xF
#define AOS_STRAP_MODE_MASK          0xF
#define AOS_STRAP_MODE_OVERWRITE_EN  (0x1<<4)

// AOS 0x104
#define AOS_APS_RST_RFM              (0x1<<0)
#define AOS_APS_RST_I2C              (0x1<<2)
#define AOS_APS_RST_PWM              (0x1<<3)
#define AOS_APS_RST_TMR_0            (0x1<<4)
#define AOS_APS_RST_TMR_1            (0x1<<5)
#define AOS_APS_RST_WDT              (0x1<<6)
#define AOS_APS_RST_VIC              (0x1<<7)
#define AOS_APS_RST_SYS_BUS          (0x1<<8)
#define AOS_APS_RST_SPI_0            (0x1<<9)
#define AOS_APS_RST_SPI_1            (0x1<<10)
#define AOS_APS_RST_SPI_2            (0x1<<11)
#define AOS_APS_RST_UART_0           (0x1<<12)
#define AOS_APS_RST_UART_1           (0x1<<13)
#define AOS_APS_RST_DBG_UART         (0x1<<14)
#define AOS_APS_RST_OTP              (0x1<<15)
#define AOS_APS_RST_DMA              (0x1<<16)
#define AOS_APS_RST_ROM              (0x1<<17)
#define AOS_APS_RST_RAM              (0x1<<18)
#define AOS_APS_RST_JTAG             (0x1<<19)
#define AOS_APS_RST_AHB_TO_APB       (0x1<<20)
#define AOS_APS_RST_BRIDGE_APS       (0x1<<21)
#define AOS_APS_RST_BRIDGE_MSQ       (0x1<<22)
#define AOS_APS_RST_CM3              (0x1<<23)
#define AOS_APS_RST_SCRT             (0x1<<24)
#define AOS_APS_RST_TAP              (0x1<<25)

#define AOS_RST_DELAY                30

// AOS 0x108
#define AOS_MSQ_RST_ROM              (0x1<<0)
#define AOS_MSQ_RST_SDM_XTAL         (0x1<<1)
#define AOS_MSQ_RST_BRIDGE_MSQ       (0x1<<3)
#define AOS_MSQ_RST_BRIDGE_APS       (0x1<<4)
#define AOS_MSQ_RST_MAC              (0x1<<5)
#define AOS_MSQ_RST_PHY              (0x1<<6)
#define AOS_MSQ_RST_WDT              (0x1<<7)
#define AOS_MSQ_RST_VIC              (0x1<<8)
#define AOS_MSQ_RST_PHY_REG          (0x1<<9)
#define AOS_MSQ_RST_AOS              (0x1<<10)
#define AOS_MSQ_RST_CM0              (0x1<<11)
#define AOS_MSQ_RST_RFPHY            (0x1<<12)
#define AOS_MSQ_RST_SPI              (0x1<<13)
#define AOS_MSQ_RST_DBG_UART         (0x1<<14)
#define AOS_MSQ_RST_PROG_PU          (0x1<<15)

// AOS 0x11C
#define AOS_MSQ_ROM_REMAP_ALL_EN     0x1

// AOS 0x134
#define AOS_APS_CLK_SRC_RC           0
#define AOS_APS_CLK_SRC_XTAL         1
#define AOS_APS_CLK_SRC_XTAL_X2      2
#define AOS_APS_CLK_SRC_176M_SWITCH  3
#define AOS_APS_CLK_SRC_MASK         0x3
#define AOS_APS_CLK_176M_SRC_XTAL_X4 ( ((uint32_t)0<<31) | (0<<29) )
#define AOS_APS_CLK_176M_SRC_DECI    ( ((uint32_t)0<<31) | (1<<29) )
#define AOS_APS_CLK_176M_SRC_1P2G    ( ((uint32_t)1<<31) | (0<<29) )
#define AOS_APS_CLK_176M_SRC_EXT     ( ((uint32_t)1<<31) | (1<<29) )
#define AOS_APS_CLK_176M_SRC_MASK    ( ((uint32_t)0x1<<31) | (0x1<<29) )
#define AOS_APS_CLK_EN_GLOBAL        (0x1<<4)
#define AOS_APS_CLK_EN_I2C_PCLK      (0x1<<5)
#define AOS_APS_CLK_EN_TMR_0_PCLK    (0x1<<6)
#define AOS_APS_CLK_EN_TMR_1_PCLK    (0x1<<7)
#define AOS_APS_CLK_EN_WDT_PCLK      (0x1<<8)
#define AOS_APS_CLK_EN_VIC_PCLK      (0x1<<9)
#define AOS_APS_CLK_EN_SPI_0_PCLK    (0x1<<10)
#define AOS_APS_CLK_EN_SPI_1_PCLK    (0x1<<11)
#define AOS_APS_CLK_EN_SPI_2_PCLK    (0x1<<12)
#define AOS_APS_CLK_EN_UART_0_PCLK   (0x1<<13)
#define AOS_APS_CLK_EN_UART_1_PCLK   (0x1<<14)
#define AOS_APS_CLK_EN_DBG_UART_PCLK (0x1<<15)
#define AOS_APS_CLK_EN_OTP_PCLK      (0x1<<16)
#define AOS_APS_CLK_EN_CM3_HCLK      (0x1<<17)
#define AOS_APS_CLK_EN_DMA_HCLK      (0x1<<18)
#define AOS_APS_CLK_EN_ROM_HCLK      (0x1<<19)
#define AOS_APS_CLK_EN_RAM_HCLK      (0x1<<20)
#define AOS_APS_CLK_EN_APB_HCLK      (0x1<<21)
#define AOS_APS_CLK_EN_APS_HCLK      (0x1<<22)
#define AOS_APS_CLK_EN_MSQ_HCLK      (0x1<<23)
#define AOS_APS_CLK_EN_SCRT_HCLK     (0x1<<24)
#define AOS_APS_CLK_EN_FCLK          (0x1<<25)
#define AOS_APS_CLK_EN_PWM_CLK       (0x1<<26)
#define AOS_APS_CLK_EN_JTAG_HCLK     (0x1<<28)
#define AOS_APS_CLK_EN_WDT_INTERNAL  (0x1<<30)
#define AOS_APS_CLK_DIV2             (0x1<<2)
#define AOS_APS_PCLK_DIV2            (0x1<<3)

#define AOS_PWM_CLK_MASK             (0x1<<27)
#define AOS_PWM_CLK_32KHZ            (0<<27)
#define AOS_PWM_CLK_22MHZ            (1<<27)

// AOS 0x138
#define AOS_MSQ_SRC_RC               0
#define AOS_MSQ_SRC_XTAL             1
#define AOS_MSQ_SRC_XTAL_X2          2
#define AOS_MSQ_SRC_EXT              3
#define AOS_MSQ_CLK_SRC_MASK         0x3
#define AOS_MSQ_CLK_EN_GLOBAL        (0x1<<2)
#define AOS_MSQ_CLK_EN_ROM_HCLK      (0x1<<4)
#define AOS_MSQ_CLK_EN_RAM_HCLK      (0x1<<5)
#define AOS_MSQ_CLK_EN_MSQ_HCLK      (0x1<<7)
#define AOS_MSQ_CLK_EN_APS_HCLK      (0x1<<8)
#define AOS_MSQ_CLK_EN_MAC_HCLK      (0x1<<9)
#define AOS_MSQ_CLK_EN_PHY_HCLK      (0x1<<10)
#define AOS_MSQ_CLK_EN_PHY_REG_HCLK  (0x1<<11)
#define AOS_MSQ_CLK_EN_AOS_HCLK      (0x1<<12)
#define AOS_MSQ_CLK_EN_WDT_HCLK      (0x1<<13)
#define AOS_MSQ_CLK_EN_VIC_HCLK      (0x1<<14)
#define AOS_MSQ_CLK_EN_FCLK          (0x1<<15)
#define AOS_MSQ_CLK_EN_DCLK          (0x1<<16)
#define AOS_MSQ_CLK_EN_SCLK          (0x1<<17)
#define AOS_MSQ_CLK_EN_PU_HCLK       (0x1<<18)
#define AOS_MSQ_CLK_EN_CM0_HCLK      (0x1<<19)
#define AOS_MSQ_CLK_EN_WDT           (0x1<<20)
#define AOS_MSQ_CLK_EN_SPI           (0x1<<21)
#define AOS_MSQ_CLK_EN_UART          (0x1<<22)
#define AOS_MSQ_CLK_DIV2             (0x1<<3)

#define AOS_APS_CLK_DIV2_UNGATED     (0x1<<23)
#define AOS_APS_PCLK_DIV2_UNGATED    (0x1<<24)
#define AOS_MSQ_CLK_DIV2_UNGATED     (0x1<<25)

// AOS 0x13C
#define AOS_PHY_RF_TEST_SRC_S_22M    (0<<4)
#define AOS_PHY_RF_TEST_SRC_S_44M    (1<<4)
#define AOS_PHY_RF_TEST_SRC_Q_44M    (2<<4)
#define AOS_PHY_RF_TEST_SRC_RFMMD    (3<<4)
#define AOS_PHY_RF_TEST_SRC_MASK     (0x3<<4)
#define AOS_PHY_RF_CAP_SRC_S_044M    (0<<6)
#define AOS_PHY_RF_CAP_SRC_Q_176M    (1<<6)
#define AOS_PHY_RF_CAP_SRC_MASK      (0x3<<6)
#define AOS_PHY_CLK_S_EN             (0x1<<0)
#define AOS_PHY_CLK_Q_EN             (0x1<<1)
#define AOS_PHY_CLK_RFMMD_EN         (0x1<<2)
#define AOS_PHY_CLK_S_22M_TX_EN      (0x1<<8)
#define AOS_PHY_CLK_S_22M_RX_EN      (0x1<<9)
#define AOS_PHY_CLK_S_44M_TX_EN      (0x1<<10)
#define AOS_PHY_CLK_S_44M_RX_EN      (0x1<<11)
#define AOS_PHY_CLK_Q_176M_TX_EN     (0x1<<12)
#define AOS_PHY_CLK_Q_176M_RX_EN     (0x1<<13)
#define AOS_PHY_CLK_Q_088M_TX_EN     (0x1<<15)
#define AOS_PHY_CLK_Q_088M_RX_EN     (0x1<<16)
#define AOS_PHY_CLK_Q_044M_TX_EN     (0x1<<17)
#define AOS_PHY_CLK_Q_044M_RX_EN     (0x1<<18)
#define AOS_PHY_CLK_CAP_FIFO2SRAM_EN (0x1<<14)
#define AOS_PHY_CLK_CAP_DATA2FIFO_EN (0x1<<19)
#define AOS_PHY_CLK_RFMMD_INV        (0x1<<20)
#define AOS_PHY_CLK_Q_INV            (0x1<<21)
#define AOS_PHY_CLK_S_INV            (0x1<<22)
#define AOS_PHY_CLK_S_DIV2           (0x1<<3)

#define AOS_PHY_CLK_S_DIV2_UNGATED   (0x1<<23)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef struct
{
    volatile uint32_t RET_MUX;            // 0x000
    volatile uint32_t MODE_CTL;           // 0x004
    volatile uint32_t OSC_SEL;            // 0x008
    volatile uint32_t SLP_TIMER_CURR_L;   // 0x00C
    volatile uint32_t SLP_TIMER_CURR_H;   // 0x010
    volatile uint32_t SLP_TIMER_PRESET_L; // 0x014
    volatile uint32_t SLP_TIMER_PRESET_H; // 0x018
    volatile uint32_t PS_TIMER_PRESET;    // 0x01C
    volatile uint32_t RET_SF_VAL_CTL;     // 0x020
    volatile uint32_t PMU_SF_VAL_CTL;     // 0x024
    volatile uint32_t HPBG_CTL;           // 0x028
    volatile uint32_t LPBG_CTL;           // 0x02C
    volatile uint32_t BUCK_CTL;           // 0x030
    volatile uint32_t ON1_TIME;           // 0x034
    volatile uint32_t ON2_TIME;           // 0x038
    volatile uint32_t ON3_TIME;           // 0x03C
    volatile uint32_t ON4_TIME;           // 0x040
    volatile uint32_t ON5_TIME;           // 0x044
    volatile uint32_t ON6_TIME;           // 0x048
    volatile uint32_t ON7_TIME;           // 0x04C
    volatile uint32_t CPOR_N_ON_TIME;     // 0x050
    volatile uint32_t reserve_054;        // 0x054, reserved
    volatile uint32_t SPS_TIMER_PRESET;   // 0x058
    volatile uint32_t SON1_TIME;          // 0x05C
    volatile uint32_t SON2_TIME;          // 0x060
    volatile uint32_t SON3_TIME;          // 0x064
    volatile uint32_t SON4_TIME;          // 0x068
    volatile uint32_t SON5_TIME;          // 0x06C
    volatile uint32_t SON6_TIME;          // 0x070
    volatile uint32_t SON7_TIME;          // 0x074
    volatile uint32_t SCPOR_N_ON_TIME;    // 0x078
    volatile uint32_t PU_CTL;             // 0x07C
    volatile uint32_t OSC_CTL;            // 0x080
    volatile uint32_t PMS_SPARE;          // 0x084, HW reservd for debug
    volatile uint32_t ADC_CTL;            // 0x088
    volatile uint32_t LDO_CTL;            // 0x08C
    volatile uint32_t RG_PD_IE;           // 0x090
    volatile uint32_t RG_PD_PE;           // 0x094
    volatile uint32_t RG_PD_O_INV;        // 0x098
    volatile uint32_t RG_PD_DS;           // 0x09C
    volatile uint32_t RG_GPO;             // 0x0A0
    volatile uint32_t RG_PD_I_INV;        // 0x0A4
    volatile uint32_t RG_PDOV_MODE;       // 0x0A8
    volatile uint32_t RG_PD_DIR;          // 0x0AC
    volatile uint32_t RG_PD_OENP_INV;     // 0x0B0
    volatile uint32_t RG_PDOC_MODE;       // 0x0B4
    volatile uint32_t RG_GPI;             // 0x0B8
    volatile uint32_t reserve_0bc;        // 0x0BC, reserved
    volatile uint32_t RG_PDI_SRC_IO_A;    // 0x0C0
    volatile uint32_t RG_PDI_SRC_IO_B;    // 0x0C4
    volatile uint32_t RG_PDI_SRC_IO_C;    // 0x0C8
    volatile uint32_t RG_PDI_SRC_IO_D;    // 0x0CC
    volatile uint32_t RG_PTS_INMUX_A;     // 0x0D0
    volatile uint32_t RG_PTS_INMUX_B;     // 0x0D4
    volatile uint32_t RG_PTS_INMUX_C;     // 0x0D8
    volatile uint32_t RG_PTS_INMUX_D;     // 0x0DC
    volatile uint32_t RG_SRAM_IOS_EN;     // 0x0E0
    volatile uint32_t RG_SRAM_RET_OFF;    // 0x0E4
    volatile uint32_t RG_PHY_WR_SRAM;     // 0x0E8
    volatile uint32_t RG_PHY_RD_SRAM;     // 0x0EC
    volatile uint32_t CAL_CEN;            // 0x0F0
    volatile uint32_t CAL_STR;            // 0x0F4
    volatile uint32_t SDM_PT_SEL;         // 0x0F8
    volatile uint32_t SDM_CTL;            // 0x0FC
    volatile uint32_t R_STRAP_MODE_CTL;   // 0x100
    volatile uint32_t R_APS_SWRST;        // 0x104
    volatile uint32_t R_MSQ_SWRST;        // 0x108
    volatile uint32_t RG_SPARE;           // 0x10C
    volatile uint32_t RG_PTS_INMUX_E;     // 0x110
    volatile uint32_t RG_PTS_INMUX_F;     // 0x114
    volatile uint32_t RG_SRAM_RET_ACK;    // 0x118
    volatile uint32_t RG_MSQ_ROM_MAP;     // 0x11C
    volatile uint32_t RG_AOS_ID;          // 0x120
    volatile uint32_t RG_SPARE_1;         // 0x124
    volatile uint32_t RG_RSTS;            // 0x128
    volatile uint32_t RG_SPARE_2;         // 0x12C
    volatile uint32_t RG_SPARE_3;         // 0x130
    volatile uint32_t R_M3CLK_SEL;        // 0x134
    volatile uint32_t R_M0CLK_SEL;        // 0x138
    volatile uint32_t R_RFPHY_SEL;        // 0x13C
    volatile uint32_t R_SCRT_EN;          // 0x140
    volatile uint32_t reserve_144[21];    // 0x144 ~ 0x194, move to sys_reg
    volatile uint32_t R_CLK_MMFACTOR_CM3; // 0x198
} S_Aos_Reg_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable


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

#endif
