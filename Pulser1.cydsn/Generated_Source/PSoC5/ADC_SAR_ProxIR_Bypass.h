/*******************************************************************************
* File Name: ADC_SAR_ProxIR_Bypass.h  
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

#if !defined(CY_PINS_ADC_SAR_ProxIR_Bypass_H) /* Pins ADC_SAR_ProxIR_Bypass_H */
#define CY_PINS_ADC_SAR_ProxIR_Bypass_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ADC_SAR_ProxIR_Bypass_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 &&\
     CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_PRODUCTION &&\
	 ADC_SAR_ProxIR_Bypass__PORT == 15 && (ADC_SAR_ProxIR_Bypass__MASK & 0xC0))

/***************************************
*        Function Prototypes             
***************************************/    

void    ADC_SAR_ProxIR_Bypass_Write(uint8 value) ;
void    ADC_SAR_ProxIR_Bypass_SetDriveMode(uint8 mode) ;
uint8   ADC_SAR_ProxIR_Bypass_ReadDataReg(void) ;
uint8   ADC_SAR_ProxIR_Bypass_Read(void) ;
uint8   ADC_SAR_ProxIR_Bypass_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define ADC_SAR_ProxIR_Bypass_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define ADC_SAR_ProxIR_Bypass_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define ADC_SAR_ProxIR_Bypass_DM_RES_UP          PIN_DM_RES_UP
#define ADC_SAR_ProxIR_Bypass_DM_RES_DWN         PIN_DM_RES_DWN
#define ADC_SAR_ProxIR_Bypass_DM_OD_LO           PIN_DM_OD_LO
#define ADC_SAR_ProxIR_Bypass_DM_OD_HI           PIN_DM_OD_HI
#define ADC_SAR_ProxIR_Bypass_DM_STRONG          PIN_DM_STRONG
#define ADC_SAR_ProxIR_Bypass_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define ADC_SAR_ProxIR_Bypass_MASK               ADC_SAR_ProxIR_Bypass__MASK
#define ADC_SAR_ProxIR_Bypass_SHIFT              ADC_SAR_ProxIR_Bypass__SHIFT
#define ADC_SAR_ProxIR_Bypass_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ADC_SAR_ProxIR_Bypass_PS                     (* (reg8 *) ADC_SAR_ProxIR_Bypass__PS)
/* Data Register */
#define ADC_SAR_ProxIR_Bypass_DR                     (* (reg8 *) ADC_SAR_ProxIR_Bypass__DR)
/* Port Number */
#define ADC_SAR_ProxIR_Bypass_PRT_NUM                (* (reg8 *) ADC_SAR_ProxIR_Bypass__PRT) 
/* Connect to Analog Globals */                                                  
#define ADC_SAR_ProxIR_Bypass_AG                     (* (reg8 *) ADC_SAR_ProxIR_Bypass__AG)                       
/* Analog MUX bux enable */
#define ADC_SAR_ProxIR_Bypass_AMUX                   (* (reg8 *) ADC_SAR_ProxIR_Bypass__AMUX) 
/* Bidirectional Enable */                                                        
#define ADC_SAR_ProxIR_Bypass_BIE                    (* (reg8 *) ADC_SAR_ProxIR_Bypass__BIE)
/* Bit-mask for Aliased Register Access */
#define ADC_SAR_ProxIR_Bypass_BIT_MASK               (* (reg8 *) ADC_SAR_ProxIR_Bypass__BIT_MASK)
/* Bypass Enable */
#define ADC_SAR_ProxIR_Bypass_BYP                    (* (reg8 *) ADC_SAR_ProxIR_Bypass__BYP)
/* Port wide control signals */                                                   
#define ADC_SAR_ProxIR_Bypass_CTL                    (* (reg8 *) ADC_SAR_ProxIR_Bypass__CTL)
/* Drive Modes */
#define ADC_SAR_ProxIR_Bypass_DM0                    (* (reg8 *) ADC_SAR_ProxIR_Bypass__DM0) 
#define ADC_SAR_ProxIR_Bypass_DM1                    (* (reg8 *) ADC_SAR_ProxIR_Bypass__DM1)
#define ADC_SAR_ProxIR_Bypass_DM2                    (* (reg8 *) ADC_SAR_ProxIR_Bypass__DM2) 
/* Input Buffer Disable Override */
#define ADC_SAR_ProxIR_Bypass_INP_DIS                (* (reg8 *) ADC_SAR_ProxIR_Bypass__INP_DIS)
/* LCD Common or Segment Drive */
#define ADC_SAR_ProxIR_Bypass_LCD_COM_SEG            (* (reg8 *) ADC_SAR_ProxIR_Bypass__LCD_COM_SEG)
/* Enable Segment LCD */
#define ADC_SAR_ProxIR_Bypass_LCD_EN                 (* (reg8 *) ADC_SAR_ProxIR_Bypass__LCD_EN)
/* Slew Rate Control */
#define ADC_SAR_ProxIR_Bypass_SLW                    (* (reg8 *) ADC_SAR_ProxIR_Bypass__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ADC_SAR_ProxIR_Bypass_PRTDSI__CAPS_SEL       (* (reg8 *) ADC_SAR_ProxIR_Bypass__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ADC_SAR_ProxIR_Bypass_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ADC_SAR_ProxIR_Bypass__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ADC_SAR_ProxIR_Bypass_PRTDSI__OE_SEL0        (* (reg8 *) ADC_SAR_ProxIR_Bypass__PRTDSI__OE_SEL0) 
#define ADC_SAR_ProxIR_Bypass_PRTDSI__OE_SEL1        (* (reg8 *) ADC_SAR_ProxIR_Bypass__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ADC_SAR_ProxIR_Bypass_PRTDSI__OUT_SEL0       (* (reg8 *) ADC_SAR_ProxIR_Bypass__PRTDSI__OUT_SEL0) 
#define ADC_SAR_ProxIR_Bypass_PRTDSI__OUT_SEL1       (* (reg8 *) ADC_SAR_ProxIR_Bypass__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ADC_SAR_ProxIR_Bypass_PRTDSI__SYNC_OUT       (* (reg8 *) ADC_SAR_ProxIR_Bypass__PRTDSI__SYNC_OUT) 


#if defined(ADC_SAR_ProxIR_Bypass__INTSTAT)  /* Interrupt Registers */

    #define ADC_SAR_ProxIR_Bypass_INTSTAT                (* (reg8 *) ADC_SAR_ProxIR_Bypass__INTSTAT)
    #define ADC_SAR_ProxIR_Bypass_SNAP                   (* (reg8 *) ADC_SAR_ProxIR_Bypass__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins ADC_SAR_ProxIR_Bypass_H */

#endif
/* [] END OF FILE */
