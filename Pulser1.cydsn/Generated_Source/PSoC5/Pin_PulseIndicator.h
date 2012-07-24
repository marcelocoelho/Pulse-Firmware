/*******************************************************************************
* File Name: Pin_PulseIndicator.h  
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

#if !defined(CY_PINS_Pin_PulseIndicator_H) /* Pins Pin_PulseIndicator_H */
#define CY_PINS_Pin_PulseIndicator_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_PulseIndicator_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 &&\
     CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_PRODUCTION &&\
	 Pin_PulseIndicator__PORT == 15 && (Pin_PulseIndicator__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_PulseIndicator_Write(uint8 value) ;
void    Pin_PulseIndicator_SetDriveMode(uint8 mode) ;
uint8   Pin_PulseIndicator_ReadDataReg(void) ;
uint8   Pin_PulseIndicator_Read(void) ;
uint8   Pin_PulseIndicator_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_PulseIndicator_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_PulseIndicator_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_PulseIndicator_DM_RES_UP          PIN_DM_RES_UP
#define Pin_PulseIndicator_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_PulseIndicator_DM_OD_LO           PIN_DM_OD_LO
#define Pin_PulseIndicator_DM_OD_HI           PIN_DM_OD_HI
#define Pin_PulseIndicator_DM_STRONG          PIN_DM_STRONG
#define Pin_PulseIndicator_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_PulseIndicator_MASK               Pin_PulseIndicator__MASK
#define Pin_PulseIndicator_SHIFT              Pin_PulseIndicator__SHIFT
#define Pin_PulseIndicator_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_PulseIndicator_PS                     (* (reg8 *) Pin_PulseIndicator__PS)
/* Data Register */
#define Pin_PulseIndicator_DR                     (* (reg8 *) Pin_PulseIndicator__DR)
/* Port Number */
#define Pin_PulseIndicator_PRT_NUM                (* (reg8 *) Pin_PulseIndicator__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_PulseIndicator_AG                     (* (reg8 *) Pin_PulseIndicator__AG)                       
/* Analog MUX bux enable */
#define Pin_PulseIndicator_AMUX                   (* (reg8 *) Pin_PulseIndicator__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_PulseIndicator_BIE                    (* (reg8 *) Pin_PulseIndicator__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_PulseIndicator_BIT_MASK               (* (reg8 *) Pin_PulseIndicator__BIT_MASK)
/* Bypass Enable */
#define Pin_PulseIndicator_BYP                    (* (reg8 *) Pin_PulseIndicator__BYP)
/* Port wide control signals */                                                   
#define Pin_PulseIndicator_CTL                    (* (reg8 *) Pin_PulseIndicator__CTL)
/* Drive Modes */
#define Pin_PulseIndicator_DM0                    (* (reg8 *) Pin_PulseIndicator__DM0) 
#define Pin_PulseIndicator_DM1                    (* (reg8 *) Pin_PulseIndicator__DM1)
#define Pin_PulseIndicator_DM2                    (* (reg8 *) Pin_PulseIndicator__DM2) 
/* Input Buffer Disable Override */
#define Pin_PulseIndicator_INP_DIS                (* (reg8 *) Pin_PulseIndicator__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_PulseIndicator_LCD_COM_SEG            (* (reg8 *) Pin_PulseIndicator__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_PulseIndicator_LCD_EN                 (* (reg8 *) Pin_PulseIndicator__LCD_EN)
/* Slew Rate Control */
#define Pin_PulseIndicator_SLW                    (* (reg8 *) Pin_PulseIndicator__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_PulseIndicator_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_PulseIndicator__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_PulseIndicator_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_PulseIndicator__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_PulseIndicator_PRTDSI__OE_SEL0        (* (reg8 *) Pin_PulseIndicator__PRTDSI__OE_SEL0) 
#define Pin_PulseIndicator_PRTDSI__OE_SEL1        (* (reg8 *) Pin_PulseIndicator__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_PulseIndicator_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_PulseIndicator__PRTDSI__OUT_SEL0) 
#define Pin_PulseIndicator_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_PulseIndicator__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_PulseIndicator_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_PulseIndicator__PRTDSI__SYNC_OUT) 


#if defined(Pin_PulseIndicator__INTSTAT)  /* Interrupt Registers */

    #define Pin_PulseIndicator_INTSTAT                (* (reg8 *) Pin_PulseIndicator__INTSTAT)
    #define Pin_PulseIndicator_SNAP                   (* (reg8 *) Pin_PulseIndicator__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_PulseIndicator_H */

#endif
/* [] END OF FILE */
