/*******************************************************************************
* File Name: Pin_27.h  
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

#if !defined(CY_PINS_Pin_27_H) /* Pins Pin_27_H */
#define CY_PINS_Pin_27_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_27_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_70 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_27__PORT == 15 && (Pin_27__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_27_Write(uint8 value) ;
void    Pin_27_SetDriveMode(uint8 mode) ;
uint8   Pin_27_ReadDataReg(void) ;
uint8   Pin_27_Read(void) ;
uint8   Pin_27_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_27_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_27_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_27_DM_RES_UP          PIN_DM_RES_UP
#define Pin_27_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_27_DM_OD_LO           PIN_DM_OD_LO
#define Pin_27_DM_OD_HI           PIN_DM_OD_HI
#define Pin_27_DM_STRONG          PIN_DM_STRONG
#define Pin_27_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_27_MASK               Pin_27__MASK
#define Pin_27_SHIFT              Pin_27__SHIFT
#define Pin_27_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_27_PS                     (* (reg8 *) Pin_27__PS)
/* Data Register */
#define Pin_27_DR                     (* (reg8 *) Pin_27__DR)
/* Port Number */
#define Pin_27_PRT_NUM                (* (reg8 *) Pin_27__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_27_AG                     (* (reg8 *) Pin_27__AG)                       
/* Analog MUX bux enable */
#define Pin_27_AMUX                   (* (reg8 *) Pin_27__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_27_BIE                    (* (reg8 *) Pin_27__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_27_BIT_MASK               (* (reg8 *) Pin_27__BIT_MASK)
/* Bypass Enable */
#define Pin_27_BYP                    (* (reg8 *) Pin_27__BYP)
/* Port wide control signals */                                                   
#define Pin_27_CTL                    (* (reg8 *) Pin_27__CTL)
/* Drive Modes */
#define Pin_27_DM0                    (* (reg8 *) Pin_27__DM0) 
#define Pin_27_DM1                    (* (reg8 *) Pin_27__DM1)
#define Pin_27_DM2                    (* (reg8 *) Pin_27__DM2) 
/* Input Buffer Disable Override */
#define Pin_27_INP_DIS                (* (reg8 *) Pin_27__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_27_LCD_COM_SEG            (* (reg8 *) Pin_27__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_27_LCD_EN                 (* (reg8 *) Pin_27__LCD_EN)
/* Slew Rate Control */
#define Pin_27_SLW                    (* (reg8 *) Pin_27__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_27_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_27__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_27_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_27__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_27_PRTDSI__OE_SEL0        (* (reg8 *) Pin_27__PRTDSI__OE_SEL0) 
#define Pin_27_PRTDSI__OE_SEL1        (* (reg8 *) Pin_27__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_27_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_27__PRTDSI__OUT_SEL0) 
#define Pin_27_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_27__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_27_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_27__PRTDSI__SYNC_OUT) 


#if defined(Pin_27__INTSTAT)  /* Interrupt Registers */

    #define Pin_27_INTSTAT                (* (reg8 *) Pin_27__INTSTAT)
    #define Pin_27_SNAP                   (* (reg8 *) Pin_27__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_27_H */

#endif
/* [] END OF FILE */
