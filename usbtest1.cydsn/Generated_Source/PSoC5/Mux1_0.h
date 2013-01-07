/*******************************************************************************
* File Name: Mux1_0.h  
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

#if !defined(CY_PINS_Mux1_0_H) /* Pins Mux1_0_H */
#define CY_PINS_Mux1_0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Mux1_0_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_80 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Mux1_0__PORT == 15 && ((Mux1_0__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Mux1_0_Write(uint8 value) ;
void    Mux1_0_SetDriveMode(uint8 mode) ;
uint8   Mux1_0_ReadDataReg(void) ;
uint8   Mux1_0_Read(void) ;
uint8   Mux1_0_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Mux1_0_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Mux1_0_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Mux1_0_DM_RES_UP          PIN_DM_RES_UP
#define Mux1_0_DM_RES_DWN         PIN_DM_RES_DWN
#define Mux1_0_DM_OD_LO           PIN_DM_OD_LO
#define Mux1_0_DM_OD_HI           PIN_DM_OD_HI
#define Mux1_0_DM_STRONG          PIN_DM_STRONG
#define Mux1_0_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Mux1_0_MASK               Mux1_0__MASK
#define Mux1_0_SHIFT              Mux1_0__SHIFT
#define Mux1_0_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Mux1_0_PS                     (* (reg8 *) Mux1_0__PS)
/* Data Register */
#define Mux1_0_DR                     (* (reg8 *) Mux1_0__DR)
/* Port Number */
#define Mux1_0_PRT_NUM                (* (reg8 *) Mux1_0__PRT) 
/* Connect to Analog Globals */                                                  
#define Mux1_0_AG                     (* (reg8 *) Mux1_0__AG)                       
/* Analog MUX bux enable */
#define Mux1_0_AMUX                   (* (reg8 *) Mux1_0__AMUX) 
/* Bidirectional Enable */                                                        
#define Mux1_0_BIE                    (* (reg8 *) Mux1_0__BIE)
/* Bit-mask for Aliased Register Access */
#define Mux1_0_BIT_MASK               (* (reg8 *) Mux1_0__BIT_MASK)
/* Bypass Enable */
#define Mux1_0_BYP                    (* (reg8 *) Mux1_0__BYP)
/* Port wide control signals */                                                   
#define Mux1_0_CTL                    (* (reg8 *) Mux1_0__CTL)
/* Drive Modes */
#define Mux1_0_DM0                    (* (reg8 *) Mux1_0__DM0) 
#define Mux1_0_DM1                    (* (reg8 *) Mux1_0__DM1)
#define Mux1_0_DM2                    (* (reg8 *) Mux1_0__DM2) 
/* Input Buffer Disable Override */
#define Mux1_0_INP_DIS                (* (reg8 *) Mux1_0__INP_DIS)
/* LCD Common or Segment Drive */
#define Mux1_0_LCD_COM_SEG            (* (reg8 *) Mux1_0__LCD_COM_SEG)
/* Enable Segment LCD */
#define Mux1_0_LCD_EN                 (* (reg8 *) Mux1_0__LCD_EN)
/* Slew Rate Control */
#define Mux1_0_SLW                    (* (reg8 *) Mux1_0__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Mux1_0_PRTDSI__CAPS_SEL       (* (reg8 *) Mux1_0__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Mux1_0_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Mux1_0__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Mux1_0_PRTDSI__OE_SEL0        (* (reg8 *) Mux1_0__PRTDSI__OE_SEL0) 
#define Mux1_0_PRTDSI__OE_SEL1        (* (reg8 *) Mux1_0__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Mux1_0_PRTDSI__OUT_SEL0       (* (reg8 *) Mux1_0__PRTDSI__OUT_SEL0) 
#define Mux1_0_PRTDSI__OUT_SEL1       (* (reg8 *) Mux1_0__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Mux1_0_PRTDSI__SYNC_OUT       (* (reg8 *) Mux1_0__PRTDSI__SYNC_OUT) 


#if defined(Mux1_0__INTSTAT)  /* Interrupt Registers */

    #define Mux1_0_INTSTAT                (* (reg8 *) Mux1_0__INTSTAT)
    #define Mux1_0_SNAP                   (* (reg8 *) Mux1_0__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Mux1_0_H */


/* [] END OF FILE */
