/*******************************************************************************
* File Name: Pin_PulseIndicator_1.h  
* Version 1.60
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PINS_Pin_PulseIndicator_1_H) /* Pins Pin_PulseIndicator_1_H */
#define CY_PINS_Pin_PulseIndicator_1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_PulseIndicator_1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 &&\
     CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_PRODUCTION &&\
	 Pin_PulseIndicator_1__PORT == 15 && (Pin_PulseIndicator_1__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_PulseIndicator_1_Write(uint8 value) ;
void    Pin_PulseIndicator_1_SetDriveMode(uint8 mode) ;
uint8   Pin_PulseIndicator_1_ReadDataReg(void) ;
uint8   Pin_PulseIndicator_1_Read(void) ;
uint8   Pin_PulseIndicator_1_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_PulseIndicator_1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_PulseIndicator_1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_PulseIndicator_1_DM_RES_UP          PIN_DM_RES_UP
#define Pin_PulseIndicator_1_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_PulseIndicator_1_DM_OD_LO           PIN_DM_OD_LO
#define Pin_PulseIndicator_1_DM_OD_HI           PIN_DM_OD_HI
#define Pin_PulseIndicator_1_DM_STRONG          PIN_DM_STRONG
#define Pin_PulseIndicator_1_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_PulseIndicator_1_MASK               Pin_PulseIndicator_1__MASK
#define Pin_PulseIndicator_1_SHIFT              Pin_PulseIndicator_1__SHIFT
#define Pin_PulseIndicator_1_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_PulseIndicator_1_PS                     (* (reg8 *) Pin_PulseIndicator_1__PS)
/* Data Register */
#define Pin_PulseIndicator_1_DR                     (* (reg8 *) Pin_PulseIndicator_1__DR)
/* Port Number */
#define Pin_PulseIndicator_1_PRT_NUM                (* (reg8 *) Pin_PulseIndicator_1__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_PulseIndicator_1_AG                     (* (reg8 *) Pin_PulseIndicator_1__AG)                       
/* Analog MUX bux enable */
#define Pin_PulseIndicator_1_AMUX                   (* (reg8 *) Pin_PulseIndicator_1__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_PulseIndicator_1_BIE                    (* (reg8 *) Pin_PulseIndicator_1__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_PulseIndicator_1_BIT_MASK               (* (reg8 *) Pin_PulseIndicator_1__BIT_MASK)
/* Bypass Enable */
#define Pin_PulseIndicator_1_BYP                    (* (reg8 *) Pin_PulseIndicator_1__BYP)
/* Port wide control signals */                                                   
#define Pin_PulseIndicator_1_CTL                    (* (reg8 *) Pin_PulseIndicator_1__CTL)
/* Drive Modes */
#define Pin_PulseIndicator_1_DM0                    (* (reg8 *) Pin_PulseIndicator_1__DM0) 
#define Pin_PulseIndicator_1_DM1                    (* (reg8 *) Pin_PulseIndicator_1__DM1)
#define Pin_PulseIndicator_1_DM2                    (* (reg8 *) Pin_PulseIndicator_1__DM2) 
/* Input Buffer Disable Override */
#define Pin_PulseIndicator_1_INP_DIS                (* (reg8 *) Pin_PulseIndicator_1__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_PulseIndicator_1_LCD_COM_SEG            (* (reg8 *) Pin_PulseIndicator_1__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_PulseIndicator_1_LCD_EN                 (* (reg8 *) Pin_PulseIndicator_1__LCD_EN)
/* Slew Rate Control */
#define Pin_PulseIndicator_1_SLW                    (* (reg8 *) Pin_PulseIndicator_1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_PulseIndicator_1_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_PulseIndicator_1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_PulseIndicator_1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_PulseIndicator_1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_PulseIndicator_1_PRTDSI__OE_SEL0        (* (reg8 *) Pin_PulseIndicator_1__PRTDSI__OE_SEL0) 
#define Pin_PulseIndicator_1_PRTDSI__OE_SEL1        (* (reg8 *) Pin_PulseIndicator_1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_PulseIndicator_1_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_PulseIndicator_1__PRTDSI__OUT_SEL0) 
#define Pin_PulseIndicator_1_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_PulseIndicator_1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_PulseIndicator_1_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_PulseIndicator_1__PRTDSI__SYNC_OUT) 


#if defined(Pin_PulseIndicator_1__INTSTAT)  /* Interrupt Registers */

    #define Pin_PulseIndicator_1_INTSTAT                (* (reg8 *) Pin_PulseIndicator_1__INTSTAT)
    #define Pin_PulseIndicator_1_SNAP                   (* (reg8 *) Pin_PulseIndicator_1__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_PulseIndicator_1_H */

#endif
/* [] END OF FILE */
