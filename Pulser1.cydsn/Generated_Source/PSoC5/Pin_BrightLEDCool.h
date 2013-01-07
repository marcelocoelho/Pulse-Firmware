/*******************************************************************************
* File Name: Pin_BrightLEDCool.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pin_BrightLEDCool_H) /* Pins Pin_BrightLEDCool_H */
#define CY_PINS_Pin_BrightLEDCool_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_BrightLEDCool_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_80 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_BrightLEDCool__PORT == 15 && ((Pin_BrightLEDCool__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_BrightLEDCool_Write(uint8 value) ;
void    Pin_BrightLEDCool_SetDriveMode(uint8 mode) ;
uint8   Pin_BrightLEDCool_ReadDataReg(void) ;
uint8   Pin_BrightLEDCool_Read(void) ;
uint8   Pin_BrightLEDCool_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_BrightLEDCool_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_BrightLEDCool_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_BrightLEDCool_DM_RES_UP          PIN_DM_RES_UP
#define Pin_BrightLEDCool_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_BrightLEDCool_DM_OD_LO           PIN_DM_OD_LO
#define Pin_BrightLEDCool_DM_OD_HI           PIN_DM_OD_HI
#define Pin_BrightLEDCool_DM_STRONG          PIN_DM_STRONG
#define Pin_BrightLEDCool_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_BrightLEDCool_MASK               Pin_BrightLEDCool__MASK
#define Pin_BrightLEDCool_SHIFT              Pin_BrightLEDCool__SHIFT
#define Pin_BrightLEDCool_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_BrightLEDCool_PS                     (* (reg8 *) Pin_BrightLEDCool__PS)
/* Data Register */
#define Pin_BrightLEDCool_DR                     (* (reg8 *) Pin_BrightLEDCool__DR)
/* Port Number */
#define Pin_BrightLEDCool_PRT_NUM                (* (reg8 *) Pin_BrightLEDCool__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_BrightLEDCool_AG                     (* (reg8 *) Pin_BrightLEDCool__AG)                       
/* Analog MUX bux enable */
#define Pin_BrightLEDCool_AMUX                   (* (reg8 *) Pin_BrightLEDCool__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_BrightLEDCool_BIE                    (* (reg8 *) Pin_BrightLEDCool__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_BrightLEDCool_BIT_MASK               (* (reg8 *) Pin_BrightLEDCool__BIT_MASK)
/* Bypass Enable */
#define Pin_BrightLEDCool_BYP                    (* (reg8 *) Pin_BrightLEDCool__BYP)
/* Port wide control signals */                                                   
#define Pin_BrightLEDCool_CTL                    (* (reg8 *) Pin_BrightLEDCool__CTL)
/* Drive Modes */
#define Pin_BrightLEDCool_DM0                    (* (reg8 *) Pin_BrightLEDCool__DM0) 
#define Pin_BrightLEDCool_DM1                    (* (reg8 *) Pin_BrightLEDCool__DM1)
#define Pin_BrightLEDCool_DM2                    (* (reg8 *) Pin_BrightLEDCool__DM2) 
/* Input Buffer Disable Override */
#define Pin_BrightLEDCool_INP_DIS                (* (reg8 *) Pin_BrightLEDCool__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_BrightLEDCool_LCD_COM_SEG            (* (reg8 *) Pin_BrightLEDCool__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_BrightLEDCool_LCD_EN                 (* (reg8 *) Pin_BrightLEDCool__LCD_EN)
/* Slew Rate Control */
#define Pin_BrightLEDCool_SLW                    (* (reg8 *) Pin_BrightLEDCool__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_BrightLEDCool_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_BrightLEDCool__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_BrightLEDCool_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_BrightLEDCool__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_BrightLEDCool_PRTDSI__OE_SEL0        (* (reg8 *) Pin_BrightLEDCool__PRTDSI__OE_SEL0) 
#define Pin_BrightLEDCool_PRTDSI__OE_SEL1        (* (reg8 *) Pin_BrightLEDCool__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_BrightLEDCool_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_BrightLEDCool__PRTDSI__OUT_SEL0) 
#define Pin_BrightLEDCool_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_BrightLEDCool__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_BrightLEDCool_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_BrightLEDCool__PRTDSI__SYNC_OUT) 


#if defined(Pin_BrightLEDCool__INTSTAT)  /* Interrupt Registers */

    #define Pin_BrightLEDCool_INTSTAT                (* (reg8 *) Pin_BrightLEDCool__INTSTAT)
    #define Pin_BrightLEDCool_SNAP                   (* (reg8 *) Pin_BrightLEDCool__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pin_BrightLEDCool_H */


/* [] END OF FILE */
