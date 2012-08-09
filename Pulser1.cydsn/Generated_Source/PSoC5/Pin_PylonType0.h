/*******************************************************************************
* File Name: Pin_PylonType0.h  
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

#if !defined(CY_PINS_Pin_PylonType0_H) /* Pins Pin_PylonType0_H */
#define CY_PINS_Pin_PylonType0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_PylonType0_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 &&\
     CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_PRODUCTION &&\
	 Pin_PylonType0__PORT == 15 && (Pin_PylonType0__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_PylonType0_Write(uint8 value) ;
void    Pin_PylonType0_SetDriveMode(uint8 mode) ;
uint8   Pin_PylonType0_ReadDataReg(void) ;
uint8   Pin_PylonType0_Read(void) ;
uint8   Pin_PylonType0_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_PylonType0_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_PylonType0_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_PylonType0_DM_RES_UP          PIN_DM_RES_UP
#define Pin_PylonType0_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_PylonType0_DM_OD_LO           PIN_DM_OD_LO
#define Pin_PylonType0_DM_OD_HI           PIN_DM_OD_HI
#define Pin_PylonType0_DM_STRONG          PIN_DM_STRONG
#define Pin_PylonType0_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_PylonType0_MASK               Pin_PylonType0__MASK
#define Pin_PylonType0_SHIFT              Pin_PylonType0__SHIFT
#define Pin_PylonType0_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_PylonType0_PS                     (* (reg8 *) Pin_PylonType0__PS)
/* Data Register */
#define Pin_PylonType0_DR                     (* (reg8 *) Pin_PylonType0__DR)
/* Port Number */
#define Pin_PylonType0_PRT_NUM                (* (reg8 *) Pin_PylonType0__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_PylonType0_AG                     (* (reg8 *) Pin_PylonType0__AG)                       
/* Analog MUX bux enable */
#define Pin_PylonType0_AMUX                   (* (reg8 *) Pin_PylonType0__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_PylonType0_BIE                    (* (reg8 *) Pin_PylonType0__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_PylonType0_BIT_MASK               (* (reg8 *) Pin_PylonType0__BIT_MASK)
/* Bypass Enable */
#define Pin_PylonType0_BYP                    (* (reg8 *) Pin_PylonType0__BYP)
/* Port wide control signals */                                                   
#define Pin_PylonType0_CTL                    (* (reg8 *) Pin_PylonType0__CTL)
/* Drive Modes */
#define Pin_PylonType0_DM0                    (* (reg8 *) Pin_PylonType0__DM0) 
#define Pin_PylonType0_DM1                    (* (reg8 *) Pin_PylonType0__DM1)
#define Pin_PylonType0_DM2                    (* (reg8 *) Pin_PylonType0__DM2) 
/* Input Buffer Disable Override */
#define Pin_PylonType0_INP_DIS                (* (reg8 *) Pin_PylonType0__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_PylonType0_LCD_COM_SEG            (* (reg8 *) Pin_PylonType0__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_PylonType0_LCD_EN                 (* (reg8 *) Pin_PylonType0__LCD_EN)
/* Slew Rate Control */
#define Pin_PylonType0_SLW                    (* (reg8 *) Pin_PylonType0__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_PylonType0_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_PylonType0__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_PylonType0_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_PylonType0__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_PylonType0_PRTDSI__OE_SEL0        (* (reg8 *) Pin_PylonType0__PRTDSI__OE_SEL0) 
#define Pin_PylonType0_PRTDSI__OE_SEL1        (* (reg8 *) Pin_PylonType0__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_PylonType0_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_PylonType0__PRTDSI__OUT_SEL0) 
#define Pin_PylonType0_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_PylonType0__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_PylonType0_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_PylonType0__PRTDSI__SYNC_OUT) 


#if defined(Pin_PylonType0__INTSTAT)  /* Interrupt Registers */

    #define Pin_PylonType0_INTSTAT                (* (reg8 *) Pin_PylonType0__INTSTAT)
    #define Pin_PylonType0_SNAP                   (* (reg8 *) Pin_PylonType0__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_PylonType0_H */

#endif
/* [] END OF FILE */
