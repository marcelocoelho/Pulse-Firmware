/*******************************************************************************
* File Name: Pin_14.h  
* Version 1.70
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

#if !defined(CY_PINS_Pin_14_H) /* Pins Pin_14_H */
#define CY_PINS_Pin_14_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_14_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_70 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_14__PORT == 15 && (Pin_14__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_14_Write(uint8 value) ;
void    Pin_14_SetDriveMode(uint8 mode) ;
uint8   Pin_14_ReadDataReg(void) ;
uint8   Pin_14_Read(void) ;
uint8   Pin_14_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_14_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_14_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_14_DM_RES_UP          PIN_DM_RES_UP
#define Pin_14_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_14_DM_OD_LO           PIN_DM_OD_LO
#define Pin_14_DM_OD_HI           PIN_DM_OD_HI
#define Pin_14_DM_STRONG          PIN_DM_STRONG
#define Pin_14_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_14_MASK               Pin_14__MASK
#define Pin_14_SHIFT              Pin_14__SHIFT
#define Pin_14_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_14_PS                     (* (reg8 *) Pin_14__PS)
/* Data Register */
#define Pin_14_DR                     (* (reg8 *) Pin_14__DR)
/* Port Number */
#define Pin_14_PRT_NUM                (* (reg8 *) Pin_14__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_14_AG                     (* (reg8 *) Pin_14__AG)                       
/* Analog MUX bux enable */
#define Pin_14_AMUX                   (* (reg8 *) Pin_14__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_14_BIE                    (* (reg8 *) Pin_14__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_14_BIT_MASK               (* (reg8 *) Pin_14__BIT_MASK)
/* Bypass Enable */
#define Pin_14_BYP                    (* (reg8 *) Pin_14__BYP)
/* Port wide control signals */                                                   
#define Pin_14_CTL                    (* (reg8 *) Pin_14__CTL)
/* Drive Modes */
#define Pin_14_DM0                    (* (reg8 *) Pin_14__DM0) 
#define Pin_14_DM1                    (* (reg8 *) Pin_14__DM1)
#define Pin_14_DM2                    (* (reg8 *) Pin_14__DM2) 
/* Input Buffer Disable Override */
#define Pin_14_INP_DIS                (* (reg8 *) Pin_14__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_14_LCD_COM_SEG            (* (reg8 *) Pin_14__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_14_LCD_EN                 (* (reg8 *) Pin_14__LCD_EN)
/* Slew Rate Control */
#define Pin_14_SLW                    (* (reg8 *) Pin_14__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_14_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_14__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_14_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_14__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_14_PRTDSI__OE_SEL0        (* (reg8 *) Pin_14__PRTDSI__OE_SEL0) 
#define Pin_14_PRTDSI__OE_SEL1        (* (reg8 *) Pin_14__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_14_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_14__PRTDSI__OUT_SEL0) 
#define Pin_14_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_14__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_14_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_14__PRTDSI__SYNC_OUT) 


#if defined(Pin_14__INTSTAT)  /* Interrupt Registers */

    #define Pin_14_INTSTAT                (* (reg8 *) Pin_14__INTSTAT)
    #define Pin_14_SNAP                   (* (reg8 *) Pin_14__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_14_H */

#endif
/* [] END OF FILE */
