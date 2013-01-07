/*******************************************************************************
* File Name: Current_Source.h  
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

#if !defined(CY_PINS_Current_Source_H) /* Pins Current_Source_H */
#define CY_PINS_Current_Source_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Current_Source_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_80 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Current_Source__PORT == 15 && ((Current_Source__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Current_Source_Write(uint8 value) ;
void    Current_Source_SetDriveMode(uint8 mode) ;
uint8   Current_Source_ReadDataReg(void) ;
uint8   Current_Source_Read(void) ;
uint8   Current_Source_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Current_Source_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Current_Source_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Current_Source_DM_RES_UP          PIN_DM_RES_UP
#define Current_Source_DM_RES_DWN         PIN_DM_RES_DWN
#define Current_Source_DM_OD_LO           PIN_DM_OD_LO
#define Current_Source_DM_OD_HI           PIN_DM_OD_HI
#define Current_Source_DM_STRONG          PIN_DM_STRONG
#define Current_Source_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Current_Source_MASK               Current_Source__MASK
#define Current_Source_SHIFT              Current_Source__SHIFT
#define Current_Source_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Current_Source_PS                     (* (reg8 *) Current_Source__PS)
/* Data Register */
#define Current_Source_DR                     (* (reg8 *) Current_Source__DR)
/* Port Number */
#define Current_Source_PRT_NUM                (* (reg8 *) Current_Source__PRT) 
/* Connect to Analog Globals */                                                  
#define Current_Source_AG                     (* (reg8 *) Current_Source__AG)                       
/* Analog MUX bux enable */
#define Current_Source_AMUX                   (* (reg8 *) Current_Source__AMUX) 
/* Bidirectional Enable */                                                        
#define Current_Source_BIE                    (* (reg8 *) Current_Source__BIE)
/* Bit-mask for Aliased Register Access */
#define Current_Source_BIT_MASK               (* (reg8 *) Current_Source__BIT_MASK)
/* Bypass Enable */
#define Current_Source_BYP                    (* (reg8 *) Current_Source__BYP)
/* Port wide control signals */                                                   
#define Current_Source_CTL                    (* (reg8 *) Current_Source__CTL)
/* Drive Modes */
#define Current_Source_DM0                    (* (reg8 *) Current_Source__DM0) 
#define Current_Source_DM1                    (* (reg8 *) Current_Source__DM1)
#define Current_Source_DM2                    (* (reg8 *) Current_Source__DM2) 
/* Input Buffer Disable Override */
#define Current_Source_INP_DIS                (* (reg8 *) Current_Source__INP_DIS)
/* LCD Common or Segment Drive */
#define Current_Source_LCD_COM_SEG            (* (reg8 *) Current_Source__LCD_COM_SEG)
/* Enable Segment LCD */
#define Current_Source_LCD_EN                 (* (reg8 *) Current_Source__LCD_EN)
/* Slew Rate Control */
#define Current_Source_SLW                    (* (reg8 *) Current_Source__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Current_Source_PRTDSI__CAPS_SEL       (* (reg8 *) Current_Source__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Current_Source_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Current_Source__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Current_Source_PRTDSI__OE_SEL0        (* (reg8 *) Current_Source__PRTDSI__OE_SEL0) 
#define Current_Source_PRTDSI__OE_SEL1        (* (reg8 *) Current_Source__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Current_Source_PRTDSI__OUT_SEL0       (* (reg8 *) Current_Source__PRTDSI__OUT_SEL0) 
#define Current_Source_PRTDSI__OUT_SEL1       (* (reg8 *) Current_Source__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Current_Source_PRTDSI__SYNC_OUT       (* (reg8 *) Current_Source__PRTDSI__SYNC_OUT) 


#if defined(Current_Source__INTSTAT)  /* Interrupt Registers */

    #define Current_Source_INTSTAT                (* (reg8 *) Current_Source__INTSTAT)
    #define Current_Source_SNAP                   (* (reg8 *) Current_Source__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Current_Source_H */


/* [] END OF FILE */
