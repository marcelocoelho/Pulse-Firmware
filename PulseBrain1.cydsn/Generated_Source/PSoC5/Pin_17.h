/*******************************************************************************
* File Name: Pin_17.h  
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

#if !defined(CY_PINS_Pin_17_H) /* Pins Pin_17_H */
#define CY_PINS_Pin_17_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_17_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_70 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_17__PORT == 15 && (Pin_17__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_17_Write(uint8 value) ;
void    Pin_17_SetDriveMode(uint8 mode) ;
uint8   Pin_17_ReadDataReg(void) ;
uint8   Pin_17_Read(void) ;
uint8   Pin_17_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_17_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_17_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_17_DM_RES_UP          PIN_DM_RES_UP
#define Pin_17_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_17_DM_OD_LO           PIN_DM_OD_LO
#define Pin_17_DM_OD_HI           PIN_DM_OD_HI
#define Pin_17_DM_STRONG          PIN_DM_STRONG
#define Pin_17_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_17_MASK               Pin_17__MASK
#define Pin_17_SHIFT              Pin_17__SHIFT
#define Pin_17_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_17_PS                     (* (reg8 *) Pin_17__PS)
/* Data Register */
#define Pin_17_DR                     (* (reg8 *) Pin_17__DR)
/* Port Number */
#define Pin_17_PRT_NUM                (* (reg8 *) Pin_17__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_17_AG                     (* (reg8 *) Pin_17__AG)                       
/* Analog MUX bux enable */
#define Pin_17_AMUX                   (* (reg8 *) Pin_17__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_17_BIE                    (* (reg8 *) Pin_17__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_17_BIT_MASK               (* (reg8 *) Pin_17__BIT_MASK)
/* Bypass Enable */
#define Pin_17_BYP                    (* (reg8 *) Pin_17__BYP)
/* Port wide control signals */                                                   
#define Pin_17_CTL                    (* (reg8 *) Pin_17__CTL)
/* Drive Modes */
#define Pin_17_DM0                    (* (reg8 *) Pin_17__DM0) 
#define Pin_17_DM1                    (* (reg8 *) Pin_17__DM1)
#define Pin_17_DM2                    (* (reg8 *) Pin_17__DM2) 
/* Input Buffer Disable Override */
#define Pin_17_INP_DIS                (* (reg8 *) Pin_17__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_17_LCD_COM_SEG            (* (reg8 *) Pin_17__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_17_LCD_EN                 (* (reg8 *) Pin_17__LCD_EN)
/* Slew Rate Control */
#define Pin_17_SLW                    (* (reg8 *) Pin_17__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_17_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_17__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_17_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_17__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_17_PRTDSI__OE_SEL0        (* (reg8 *) Pin_17__PRTDSI__OE_SEL0) 
#define Pin_17_PRTDSI__OE_SEL1        (* (reg8 *) Pin_17__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_17_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_17__PRTDSI__OUT_SEL0) 
#define Pin_17_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_17__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_17_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_17__PRTDSI__SYNC_OUT) 


#if defined(Pin_17__INTSTAT)  /* Interrupt Registers */

    #define Pin_17_INTSTAT                (* (reg8 *) Pin_17__INTSTAT)
    #define Pin_17_SNAP                   (* (reg8 *) Pin_17__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_17_H */

#endif
/* [] END OF FILE */
