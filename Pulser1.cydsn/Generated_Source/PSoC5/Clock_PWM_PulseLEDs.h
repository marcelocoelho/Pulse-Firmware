/*******************************************************************************
* File Name: Clock_PWM_PulseLEDs.h
* Version 1.60
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_CLOCK_Clock_PWM_PulseLEDs_H)
#define CY_CLOCK_Clock_PWM_PulseLEDs_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/

void Clock_PWM_PulseLEDs_Start(void) ;
void Clock_PWM_PulseLEDs_Stop(void) ;

#if(!(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3 && \
    CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_3A_ES2) && \
	!(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 && \
	CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_ES1))
void Clock_PWM_PulseLEDs_StopBlock(void) ;
#endif

void Clock_PWM_PulseLEDs_StandbyPower(uint8 state) ;
void Clock_PWM_PulseLEDs_SetDividerRegister(uint16 clkDivider, uint8 reset) ;
uint16 Clock_PWM_PulseLEDs_GetDividerRegister(void) ;
void Clock_PWM_PulseLEDs_SetModeRegister(uint8 modeBitMask) ;
void Clock_PWM_PulseLEDs_ClearModeRegister(uint8 modeBitMask) ;
uint8 Clock_PWM_PulseLEDs_GetModeRegister(void) ;
void Clock_PWM_PulseLEDs_SetSourceRegister(uint8 clkSource) ;
uint8 Clock_PWM_PulseLEDs_GetSourceRegister(void) ;
#if defined(Clock_PWM_PulseLEDs__CFG3)
void Clock_PWM_PulseLEDs_SetPhaseRegister(uint8 clkPhase) ;
uint8 Clock_PWM_PulseLEDs_GetPhaseRegister(void) ;
#endif

#define Clock_PWM_PulseLEDs_Enable()                       Clock_PWM_PulseLEDs_Start()
#define Clock_PWM_PulseLEDs_Disable()                      Clock_PWM_PulseLEDs_Stop()
#define Clock_PWM_PulseLEDs_SetDivider(clkDivider)         Clock_PWM_PulseLEDs_SetDividerRegister(clkDivider, 1)
#define Clock_PWM_PulseLEDs_SetDividerValue(clkDivider)    Clock_PWM_PulseLEDs_SetDividerRegister((clkDivider) - 1, 1)
#define Clock_PWM_PulseLEDs_SetMode(clkMode)               Clock_PWM_PulseLEDs_SetModeRegister(clkMode)
#define Clock_PWM_PulseLEDs_SetSource(clkSource)           Clock_PWM_PulseLEDs_SetSourceRegister(clkSource)
#if defined(Clock_PWM_PulseLEDs__CFG3)
#define Clock_PWM_PulseLEDs_SetPhase(clkPhase)             Clock_PWM_PulseLEDs_SetPhaseRegister(clkPhase)
#define Clock_PWM_PulseLEDs_SetPhaseValue(clkPhase)        Clock_PWM_PulseLEDs_SetPhaseRegister((clkPhase) + 1)
#endif


/***************************************
*           API Constants
***************************************/

/* Constants SetPhase(), clkPhase parameter. Only valid for PSoC 3 ES2 and earlier. See datasheet for details. */
#if CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3 && \
   (CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_3A_ES1 || \
    CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_3A_ES2)
#define CYCLK_2_5NS             0x01 /* 2.5 ns delay. */
#define CYCLK_3_5NS             0x02 /* 3.5 ns delay. */
#define CYCLK_4_5NS             0x03 /* 4.5 ns delay. */
#define CYCLK_5_5NS             0x04 /* 5.5 ns delay. */
#define CYCLK_6_5NS             0x05 /* 6.5 ns delay. */
#define CYCLK_7_5NS             0x06 /* 7.5 ns delay. */
#define CYCLK_8_5NS             0x07 /* 8.5 ns delay. */
#define CYCLK_9_5NS             0x08 /* 9.5 ns delay. */
#define CYCLK_10_5NS            0x09 /* 10.5 ns delay. */
#define CYCLK_11_5NS            0x0A /* 11.5 ns delay. */
#define CYCLK_12_5NS            0x0B /* 12.5 ns delay. */
#endif


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Clock_PWM_PulseLEDs_CLKEN              (* (reg8 *) Clock_PWM_PulseLEDs__PM_ACT_CFG)
#define Clock_PWM_PulseLEDs_CLKEN_PTR          ((reg8 *) Clock_PWM_PulseLEDs__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Clock_PWM_PulseLEDs_CLKSTBY            (* (reg8 *) Clock_PWM_PulseLEDs__PM_STBY_CFG)
#define Clock_PWM_PulseLEDs_CLKSTBY_PTR        ((reg8 *) Clock_PWM_PulseLEDs__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Clock_PWM_PulseLEDs_DIV_LSB            (* (reg8 *) Clock_PWM_PulseLEDs__CFG0)
#define Clock_PWM_PulseLEDs_DIV_LSB_PTR        ((reg8 *) Clock_PWM_PulseLEDs__CFG0)
#define Clock_PWM_PulseLEDs_DIV_PTR            ((reg16 *) Clock_PWM_PulseLEDs__CFG0)

/* Clock MSB divider configuration register. */
#define Clock_PWM_PulseLEDs_DIV_MSB            (* (reg8 *) Clock_PWM_PulseLEDs__CFG1)
#define Clock_PWM_PulseLEDs_DIV_MSB_PTR        ((reg8 *) Clock_PWM_PulseLEDs__CFG1)

/* Mode and source configuration register */
#define Clock_PWM_PulseLEDs_MOD_SRC            (* (reg8 *) Clock_PWM_PulseLEDs__CFG2)
#define Clock_PWM_PulseLEDs_MOD_SRC_PTR        ((reg8 *) Clock_PWM_PulseLEDs__CFG2)

#if defined(Clock_PWM_PulseLEDs__CFG3)
/* Analog clock phase configuration register */
#define Clock_PWM_PulseLEDs_PHASE              (* (reg8 *) Clock_PWM_PulseLEDs__CFG3)
#define Clock_PWM_PulseLEDs_PHASE_PTR          ((reg8 *) Clock_PWM_PulseLEDs__CFG3)
#endif


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Clock_PWM_PulseLEDs_CLKEN_MASK         Clock_PWM_PulseLEDs__PM_ACT_MSK
#define Clock_PWM_PulseLEDs_CLKSTBY_MASK       Clock_PWM_PulseLEDs__PM_STBY_MSK

/* CFG2 field masks */
#define Clock_PWM_PulseLEDs_SRC_SEL_MSK        Clock_PWM_PulseLEDs__CFG2_SRC_SEL_MASK
#define Clock_PWM_PulseLEDs_MODE_MASK          (~(Clock_PWM_PulseLEDs_SRC_SEL_MSK))

#if defined(Clock_PWM_PulseLEDs__CFG3)
/* CFG3 phase mask */
#define Clock_PWM_PulseLEDs_PHASE_MASK         Clock_PWM_PulseLEDs__CFG3_PHASE_DLY_MASK
#endif

#endif /* CY_CLOCK_Clock_PWM_PulseLEDs_H */


/* [] END OF FILE */
