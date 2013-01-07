/*******************************************************************************
* File Name: Pin_PulseIn_minus_IRD_C.h  
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

#if !defined(CY_PINS_Pin_PulseIn_minus_IRD_C_H) /* Pins Pin_PulseIn_minus_IRD_C_H */
#define CY_PINS_Pin_PulseIn_minus_IRD_C_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_PulseIn_minus_IRD_C_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_80 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_PulseIn_minus_IRD_C__PORT == 15 && ((Pin_PulseIn_minus_IRD_C__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_PulseIn_minus_IRD_C_Write(uint8 value) ;
void    Pin_PulseIn_minus_IRD_C_SetDriveMode(uint8 mode) ;
uint8   Pin_PulseIn_minus_IRD_C_ReadDataReg(void) ;
uint8   Pin_PulseIn_minus_IRD_C_Read(void) ;
uint8   Pin_PulseIn_minus_IRD_C_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_PulseIn_minus_IRD_C_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_PulseIn_minus_IRD_C_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_PulseIn_minus_IRD_C_DM_RES_UP          PIN_DM_RES_UP
#define Pin_PulseIn_minus_IRD_C_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_PulseIn_minus_IRD_C_DM_OD_LO           PIN_DM_OD_LO
#define Pin_PulseIn_minus_IRD_C_DM_OD_HI           PIN_DM_OD_HI
#define Pin_PulseIn_minus_IRD_C_DM_STRONG          PIN_DM_STRONG
#define Pin_PulseIn_minus_IRD_C_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_PulseIn_minus_IRD_C_MASK               Pin_PulseIn_minus_IRD_C__MASK
#define Pin_PulseIn_minus_IRD_C_SHIFT              Pin_PulseIn_minus_IRD_C__SHIFT
#define Pin_PulseIn_minus_IRD_C_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_PulseIn_minus_IRD_C_PS                     (* (reg8 *) Pin_PulseIn_minus_IRD_C__PS)
/* Data Register */
#define Pin_PulseIn_minus_IRD_C_DR                     (* (reg8 *) Pin_PulseIn_minus_IRD_C__DR)
/* Port Number */
#define Pin_PulseIn_minus_IRD_C_PRT_NUM                (* (reg8 *) Pin_PulseIn_minus_IRD_C__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_PulseIn_minus_IRD_C_AG                     (* (reg8 *) Pin_PulseIn_minus_IRD_C__AG)                       
/* Analog MUX bux enable */
#define Pin_PulseIn_minus_IRD_C_AMUX                   (* (reg8 *) Pin_PulseIn_minus_IRD_C__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_PulseIn_minus_IRD_C_BIE                    (* (reg8 *) Pin_PulseIn_minus_IRD_C__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_PulseIn_minus_IRD_C_BIT_MASK               (* (reg8 *) Pin_PulseIn_minus_IRD_C__BIT_MASK)
/* Bypass Enable */
#define Pin_PulseIn_minus_IRD_C_BYP                    (* (reg8 *) Pin_PulseIn_minus_IRD_C__BYP)
/* Port wide control signals */                                                   
#define Pin_PulseIn_minus_IRD_C_CTL                    (* (reg8 *) Pin_PulseIn_minus_IRD_C__CTL)
/* Drive Modes */
#define Pin_PulseIn_minus_IRD_C_DM0                    (* (reg8 *) Pin_PulseIn_minus_IRD_C__DM0) 
#define Pin_PulseIn_minus_IRD_C_DM1                    (* (reg8 *) Pin_PulseIn_minus_IRD_C__DM1)
#define Pin_PulseIn_minus_IRD_C_DM2                    (* (reg8 *) Pin_PulseIn_minus_IRD_C__DM2) 
/* Input Buffer Disable Override */
#define Pin_PulseIn_minus_IRD_C_INP_DIS                (* (reg8 *) Pin_PulseIn_minus_IRD_C__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_PulseIn_minus_IRD_C_LCD_COM_SEG            (* (reg8 *) Pin_PulseIn_minus_IRD_C__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_PulseIn_minus_IRD_C_LCD_EN                 (* (reg8 *) Pin_PulseIn_minus_IRD_C__LCD_EN)
/* Slew Rate Control */
#define Pin_PulseIn_minus_IRD_C_SLW                    (* (reg8 *) Pin_PulseIn_minus_IRD_C__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_PulseIn_minus_IRD_C_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_PulseIn_minus_IRD_C__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_PulseIn_minus_IRD_C_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_PulseIn_minus_IRD_C__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_PulseIn_minus_IRD_C_PRTDSI__OE_SEL0        (* (reg8 *) Pin_PulseIn_minus_IRD_C__PRTDSI__OE_SEL0) 
#define Pin_PulseIn_minus_IRD_C_PRTDSI__OE_SEL1        (* (reg8 *) Pin_PulseIn_minus_IRD_C__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_PulseIn_minus_IRD_C_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_PulseIn_minus_IRD_C__PRTDSI__OUT_SEL0) 
#define Pin_PulseIn_minus_IRD_C_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_PulseIn_minus_IRD_C__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_PulseIn_minus_IRD_C_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_PulseIn_minus_IRD_C__PRTDSI__SYNC_OUT) 


#if defined(Pin_PulseIn_minus_IRD_C__INTSTAT)  /* Interrupt Registers */

    #define Pin_PulseIn_minus_IRD_C_INTSTAT                (* (reg8 *) Pin_PulseIn_minus_IRD_C__INTSTAT)
    #define Pin_PulseIn_minus_IRD_C_SNAP                   (* (reg8 *) Pin_PulseIn_minus_IRD_C__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pin_PulseIn_minus_IRD_C_H */


/* [] END OF FILE */
