/*******************************************************************************
* File Name: Pin_DevId_3.h  
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

#if !defined(CY_PINS_Pin_DevId_3_H) /* Pins Pin_DevId_3_H */
#define CY_PINS_Pin_DevId_3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_DevId_3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 &&\
     CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_PRODUCTION &&\
	 Pin_DevId_3__PORT == 15 && (Pin_DevId_3__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_DevId_3_Write(uint8 value) ;
void    Pin_DevId_3_SetDriveMode(uint8 mode) ;
uint8   Pin_DevId_3_ReadDataReg(void) ;
uint8   Pin_DevId_3_Read(void) ;
uint8   Pin_DevId_3_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_DevId_3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_DevId_3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_DevId_3_DM_RES_UP          PIN_DM_RES_UP
#define Pin_DevId_3_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_DevId_3_DM_OD_LO           PIN_DM_OD_LO
#define Pin_DevId_3_DM_OD_HI           PIN_DM_OD_HI
#define Pin_DevId_3_DM_STRONG          PIN_DM_STRONG
#define Pin_DevId_3_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_DevId_3_MASK               Pin_DevId_3__MASK
#define Pin_DevId_3_SHIFT              Pin_DevId_3__SHIFT
#define Pin_DevId_3_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_DevId_3_PS                     (* (reg8 *) Pin_DevId_3__PS)
/* Data Register */
#define Pin_DevId_3_DR                     (* (reg8 *) Pin_DevId_3__DR)
/* Port Number */
#define Pin_DevId_3_PRT_NUM                (* (reg8 *) Pin_DevId_3__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_DevId_3_AG                     (* (reg8 *) Pin_DevId_3__AG)                       
/* Analog MUX bux enable */
#define Pin_DevId_3_AMUX                   (* (reg8 *) Pin_DevId_3__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_DevId_3_BIE                    (* (reg8 *) Pin_DevId_3__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_DevId_3_BIT_MASK               (* (reg8 *) Pin_DevId_3__BIT_MASK)
/* Bypass Enable */
#define Pin_DevId_3_BYP                    (* (reg8 *) Pin_DevId_3__BYP)
/* Port wide control signals */                                                   
#define Pin_DevId_3_CTL                    (* (reg8 *) Pin_DevId_3__CTL)
/* Drive Modes */
#define Pin_DevId_3_DM0                    (* (reg8 *) Pin_DevId_3__DM0) 
#define Pin_DevId_3_DM1                    (* (reg8 *) Pin_DevId_3__DM1)
#define Pin_DevId_3_DM2                    (* (reg8 *) Pin_DevId_3__DM2) 
/* Input Buffer Disable Override */
#define Pin_DevId_3_INP_DIS                (* (reg8 *) Pin_DevId_3__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_DevId_3_LCD_COM_SEG            (* (reg8 *) Pin_DevId_3__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_DevId_3_LCD_EN                 (* (reg8 *) Pin_DevId_3__LCD_EN)
/* Slew Rate Control */
#define Pin_DevId_3_SLW                    (* (reg8 *) Pin_DevId_3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_DevId_3_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_DevId_3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_DevId_3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_DevId_3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_DevId_3_PRTDSI__OE_SEL0        (* (reg8 *) Pin_DevId_3__PRTDSI__OE_SEL0) 
#define Pin_DevId_3_PRTDSI__OE_SEL1        (* (reg8 *) Pin_DevId_3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_DevId_3_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_DevId_3__PRTDSI__OUT_SEL0) 
#define Pin_DevId_3_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_DevId_3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_DevId_3_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_DevId_3__PRTDSI__SYNC_OUT) 


#if defined(Pin_DevId_3__INTSTAT)  /* Interrupt Registers */

    #define Pin_DevId_3_INTSTAT                (* (reg8 *) Pin_DevId_3__INTSTAT)
    #define Pin_DevId_3_SNAP                   (* (reg8 *) Pin_DevId_3__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_DevId_3_H */

#endif
/* [] END OF FILE */