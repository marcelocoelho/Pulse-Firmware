/*******************************************************************************
* File Name: Pin_LightsOn.h  
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

#if !defined(CY_PINS_Pin_LightsOn_H) /* Pins Pin_LightsOn_H */
#define CY_PINS_Pin_LightsOn_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_LightsOn_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 &&\
     CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_PRODUCTION &&\
	 Pin_LightsOn__PORT == 15 && (Pin_LightsOn__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_LightsOn_Write(uint8 value) ;
void    Pin_LightsOn_SetDriveMode(uint8 mode) ;
uint8   Pin_LightsOn_ReadDataReg(void) ;
uint8   Pin_LightsOn_Read(void) ;
uint8   Pin_LightsOn_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_LightsOn_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_LightsOn_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_LightsOn_DM_RES_UP          PIN_DM_RES_UP
#define Pin_LightsOn_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_LightsOn_DM_OD_LO           PIN_DM_OD_LO
#define Pin_LightsOn_DM_OD_HI           PIN_DM_OD_HI
#define Pin_LightsOn_DM_STRONG          PIN_DM_STRONG
#define Pin_LightsOn_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_LightsOn_MASK               Pin_LightsOn__MASK
#define Pin_LightsOn_SHIFT              Pin_LightsOn__SHIFT
#define Pin_LightsOn_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_LightsOn_PS                     (* (reg8 *) Pin_LightsOn__PS)
/* Data Register */
#define Pin_LightsOn_DR                     (* (reg8 *) Pin_LightsOn__DR)
/* Port Number */
#define Pin_LightsOn_PRT_NUM                (* (reg8 *) Pin_LightsOn__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_LightsOn_AG                     (* (reg8 *) Pin_LightsOn__AG)                       
/* Analog MUX bux enable */
#define Pin_LightsOn_AMUX                   (* (reg8 *) Pin_LightsOn__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_LightsOn_BIE                    (* (reg8 *) Pin_LightsOn__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_LightsOn_BIT_MASK               (* (reg8 *) Pin_LightsOn__BIT_MASK)
/* Bypass Enable */
#define Pin_LightsOn_BYP                    (* (reg8 *) Pin_LightsOn__BYP)
/* Port wide control signals */                                                   
#define Pin_LightsOn_CTL                    (* (reg8 *) Pin_LightsOn__CTL)
/* Drive Modes */
#define Pin_LightsOn_DM0                    (* (reg8 *) Pin_LightsOn__DM0) 
#define Pin_LightsOn_DM1                    (* (reg8 *) Pin_LightsOn__DM1)
#define Pin_LightsOn_DM2                    (* (reg8 *) Pin_LightsOn__DM2) 
/* Input Buffer Disable Override */
#define Pin_LightsOn_INP_DIS                (* (reg8 *) Pin_LightsOn__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_LightsOn_LCD_COM_SEG            (* (reg8 *) Pin_LightsOn__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_LightsOn_LCD_EN                 (* (reg8 *) Pin_LightsOn__LCD_EN)
/* Slew Rate Control */
#define Pin_LightsOn_SLW                    (* (reg8 *) Pin_LightsOn__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_LightsOn_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_LightsOn__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_LightsOn_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_LightsOn__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_LightsOn_PRTDSI__OE_SEL0        (* (reg8 *) Pin_LightsOn__PRTDSI__OE_SEL0) 
#define Pin_LightsOn_PRTDSI__OE_SEL1        (* (reg8 *) Pin_LightsOn__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_LightsOn_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_LightsOn__PRTDSI__OUT_SEL0) 
#define Pin_LightsOn_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_LightsOn__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_LightsOn_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_LightsOn__PRTDSI__SYNC_OUT) 


#if defined(Pin_LightsOn__INTSTAT)  /* Interrupt Registers */

    #define Pin_LightsOn_INTSTAT                (* (reg8 *) Pin_LightsOn__INTSTAT)
    #define Pin_LightsOn_SNAP                   (* (reg8 *) Pin_LightsOn__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_LightsOn_H */

#endif
/* [] END OF FILE */
