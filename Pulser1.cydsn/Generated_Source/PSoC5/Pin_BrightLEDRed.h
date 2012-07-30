/*******************************************************************************
* File Name: Pin_BrightLEDRed.h  
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

#if !defined(CY_PINS_Pin_BrightLEDRed_H) /* Pins Pin_BrightLEDRed_H */
#define CY_PINS_Pin_BrightLEDRed_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_BrightLEDRed_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 &&\
     CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_PRODUCTION &&\
	 Pin_BrightLEDRed__PORT == 15 && (Pin_BrightLEDRed__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_BrightLEDRed_Write(uint8 value) ;
void    Pin_BrightLEDRed_SetDriveMode(uint8 mode) ;
uint8   Pin_BrightLEDRed_ReadDataReg(void) ;
uint8   Pin_BrightLEDRed_Read(void) ;
uint8   Pin_BrightLEDRed_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_BrightLEDRed_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_BrightLEDRed_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_BrightLEDRed_DM_RES_UP          PIN_DM_RES_UP
#define Pin_BrightLEDRed_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_BrightLEDRed_DM_OD_LO           PIN_DM_OD_LO
#define Pin_BrightLEDRed_DM_OD_HI           PIN_DM_OD_HI
#define Pin_BrightLEDRed_DM_STRONG          PIN_DM_STRONG
#define Pin_BrightLEDRed_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_BrightLEDRed_MASK               Pin_BrightLEDRed__MASK
#define Pin_BrightLEDRed_SHIFT              Pin_BrightLEDRed__SHIFT
#define Pin_BrightLEDRed_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_BrightLEDRed_PS                     (* (reg8 *) Pin_BrightLEDRed__PS)
/* Data Register */
#define Pin_BrightLEDRed_DR                     (* (reg8 *) Pin_BrightLEDRed__DR)
/* Port Number */
#define Pin_BrightLEDRed_PRT_NUM                (* (reg8 *) Pin_BrightLEDRed__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_BrightLEDRed_AG                     (* (reg8 *) Pin_BrightLEDRed__AG)                       
/* Analog MUX bux enable */
#define Pin_BrightLEDRed_AMUX                   (* (reg8 *) Pin_BrightLEDRed__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_BrightLEDRed_BIE                    (* (reg8 *) Pin_BrightLEDRed__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_BrightLEDRed_BIT_MASK               (* (reg8 *) Pin_BrightLEDRed__BIT_MASK)
/* Bypass Enable */
#define Pin_BrightLEDRed_BYP                    (* (reg8 *) Pin_BrightLEDRed__BYP)
/* Port wide control signals */                                                   
#define Pin_BrightLEDRed_CTL                    (* (reg8 *) Pin_BrightLEDRed__CTL)
/* Drive Modes */
#define Pin_BrightLEDRed_DM0                    (* (reg8 *) Pin_BrightLEDRed__DM0) 
#define Pin_BrightLEDRed_DM1                    (* (reg8 *) Pin_BrightLEDRed__DM1)
#define Pin_BrightLEDRed_DM2                    (* (reg8 *) Pin_BrightLEDRed__DM2) 
/* Input Buffer Disable Override */
#define Pin_BrightLEDRed_INP_DIS                (* (reg8 *) Pin_BrightLEDRed__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_BrightLEDRed_LCD_COM_SEG            (* (reg8 *) Pin_BrightLEDRed__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_BrightLEDRed_LCD_EN                 (* (reg8 *) Pin_BrightLEDRed__LCD_EN)
/* Slew Rate Control */
#define Pin_BrightLEDRed_SLW                    (* (reg8 *) Pin_BrightLEDRed__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_BrightLEDRed_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_BrightLEDRed__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_BrightLEDRed_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_BrightLEDRed__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_BrightLEDRed_PRTDSI__OE_SEL0        (* (reg8 *) Pin_BrightLEDRed__PRTDSI__OE_SEL0) 
#define Pin_BrightLEDRed_PRTDSI__OE_SEL1        (* (reg8 *) Pin_BrightLEDRed__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_BrightLEDRed_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_BrightLEDRed__PRTDSI__OUT_SEL0) 
#define Pin_BrightLEDRed_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_BrightLEDRed__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_BrightLEDRed_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_BrightLEDRed__PRTDSI__SYNC_OUT) 


#if defined(Pin_BrightLEDRed__INTSTAT)  /* Interrupt Registers */

    #define Pin_BrightLEDRed_INTSTAT                (* (reg8 *) Pin_BrightLEDRed__INTSTAT)
    #define Pin_BrightLEDRed_SNAP                   (* (reg8 *) Pin_BrightLEDRed__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_BrightLEDRed_H */

#endif
/* [] END OF FILE */
