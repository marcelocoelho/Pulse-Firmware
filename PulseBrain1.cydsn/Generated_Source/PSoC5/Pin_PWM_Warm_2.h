/*******************************************************************************
* File Name: Pin_PWM_Warm_2.h  
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

#if !defined(CY_PINS_Pin_PWM_Warm_2_H) /* Pins Pin_PWM_Warm_2_H */
#define CY_PINS_Pin_PWM_Warm_2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_PWM_Warm_2_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_70 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_PWM_Warm_2__PORT == 15 && (Pin_PWM_Warm_2__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_PWM_Warm_2_Write(uint8 value) ;
void    Pin_PWM_Warm_2_SetDriveMode(uint8 mode) ;
uint8   Pin_PWM_Warm_2_ReadDataReg(void) ;
uint8   Pin_PWM_Warm_2_Read(void) ;
uint8   Pin_PWM_Warm_2_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_PWM_Warm_2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_PWM_Warm_2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_PWM_Warm_2_DM_RES_UP          PIN_DM_RES_UP
#define Pin_PWM_Warm_2_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_PWM_Warm_2_DM_OD_LO           PIN_DM_OD_LO
#define Pin_PWM_Warm_2_DM_OD_HI           PIN_DM_OD_HI
#define Pin_PWM_Warm_2_DM_STRONG          PIN_DM_STRONG
#define Pin_PWM_Warm_2_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_PWM_Warm_2_MASK               Pin_PWM_Warm_2__MASK
#define Pin_PWM_Warm_2_SHIFT              Pin_PWM_Warm_2__SHIFT
#define Pin_PWM_Warm_2_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_PWM_Warm_2_PS                     (* (reg8 *) Pin_PWM_Warm_2__PS)
/* Data Register */
#define Pin_PWM_Warm_2_DR                     (* (reg8 *) Pin_PWM_Warm_2__DR)
/* Port Number */
#define Pin_PWM_Warm_2_PRT_NUM                (* (reg8 *) Pin_PWM_Warm_2__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_PWM_Warm_2_AG                     (* (reg8 *) Pin_PWM_Warm_2__AG)                       
/* Analog MUX bux enable */
#define Pin_PWM_Warm_2_AMUX                   (* (reg8 *) Pin_PWM_Warm_2__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_PWM_Warm_2_BIE                    (* (reg8 *) Pin_PWM_Warm_2__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_PWM_Warm_2_BIT_MASK               (* (reg8 *) Pin_PWM_Warm_2__BIT_MASK)
/* Bypass Enable */
#define Pin_PWM_Warm_2_BYP                    (* (reg8 *) Pin_PWM_Warm_2__BYP)
/* Port wide control signals */                                                   
#define Pin_PWM_Warm_2_CTL                    (* (reg8 *) Pin_PWM_Warm_2__CTL)
/* Drive Modes */
#define Pin_PWM_Warm_2_DM0                    (* (reg8 *) Pin_PWM_Warm_2__DM0) 
#define Pin_PWM_Warm_2_DM1                    (* (reg8 *) Pin_PWM_Warm_2__DM1)
#define Pin_PWM_Warm_2_DM2                    (* (reg8 *) Pin_PWM_Warm_2__DM2) 
/* Input Buffer Disable Override */
#define Pin_PWM_Warm_2_INP_DIS                (* (reg8 *) Pin_PWM_Warm_2__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_PWM_Warm_2_LCD_COM_SEG            (* (reg8 *) Pin_PWM_Warm_2__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_PWM_Warm_2_LCD_EN                 (* (reg8 *) Pin_PWM_Warm_2__LCD_EN)
/* Slew Rate Control */
#define Pin_PWM_Warm_2_SLW                    (* (reg8 *) Pin_PWM_Warm_2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_PWM_Warm_2_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_PWM_Warm_2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_PWM_Warm_2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_PWM_Warm_2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_PWM_Warm_2_PRTDSI__OE_SEL0        (* (reg8 *) Pin_PWM_Warm_2__PRTDSI__OE_SEL0) 
#define Pin_PWM_Warm_2_PRTDSI__OE_SEL1        (* (reg8 *) Pin_PWM_Warm_2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_PWM_Warm_2_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_PWM_Warm_2__PRTDSI__OUT_SEL0) 
#define Pin_PWM_Warm_2_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_PWM_Warm_2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_PWM_Warm_2_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_PWM_Warm_2__PRTDSI__SYNC_OUT) 


#if defined(Pin_PWM_Warm_2__INTSTAT)  /* Interrupt Registers */

    #define Pin_PWM_Warm_2_INTSTAT                (* (reg8 *) Pin_PWM_Warm_2__INTSTAT)
    #define Pin_PWM_Warm_2_SNAP                   (* (reg8 *) Pin_PWM_Warm_2__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_PWM_Warm_2_H */

#endif
/* [] END OF FILE */
