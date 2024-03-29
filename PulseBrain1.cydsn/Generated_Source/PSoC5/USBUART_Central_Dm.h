/*******************************************************************************
* File Name: USBUART_Central_Dm.h  
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

#if !defined(CY_PINS_USBUART_Central_Dm_H) /* Pins USBUART_Central_Dm_H */
#define CY_PINS_USBUART_Central_Dm_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "USBUART_Central_Dm_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_80 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 USBUART_Central_Dm__PORT == 15 && ((USBUART_Central_Dm__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    USBUART_Central_Dm_Write(uint8 value) ;
void    USBUART_Central_Dm_SetDriveMode(uint8 mode) ;
uint8   USBUART_Central_Dm_ReadDataReg(void) ;
uint8   USBUART_Central_Dm_Read(void) ;
uint8   USBUART_Central_Dm_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define USBUART_Central_Dm_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define USBUART_Central_Dm_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define USBUART_Central_Dm_DM_RES_UP          PIN_DM_RES_UP
#define USBUART_Central_Dm_DM_RES_DWN         PIN_DM_RES_DWN
#define USBUART_Central_Dm_DM_OD_LO           PIN_DM_OD_LO
#define USBUART_Central_Dm_DM_OD_HI           PIN_DM_OD_HI
#define USBUART_Central_Dm_DM_STRONG          PIN_DM_STRONG
#define USBUART_Central_Dm_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define USBUART_Central_Dm_MASK               USBUART_Central_Dm__MASK
#define USBUART_Central_Dm_SHIFT              USBUART_Central_Dm__SHIFT
#define USBUART_Central_Dm_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define USBUART_Central_Dm_PS                     (* (reg8 *) USBUART_Central_Dm__PS)
/* Data Register */
#define USBUART_Central_Dm_DR                     (* (reg8 *) USBUART_Central_Dm__DR)
/* Port Number */
#define USBUART_Central_Dm_PRT_NUM                (* (reg8 *) USBUART_Central_Dm__PRT) 
/* Connect to Analog Globals */                                                  
#define USBUART_Central_Dm_AG                     (* (reg8 *) USBUART_Central_Dm__AG)                       
/* Analog MUX bux enable */
#define USBUART_Central_Dm_AMUX                   (* (reg8 *) USBUART_Central_Dm__AMUX) 
/* Bidirectional Enable */                                                        
#define USBUART_Central_Dm_BIE                    (* (reg8 *) USBUART_Central_Dm__BIE)
/* Bit-mask for Aliased Register Access */
#define USBUART_Central_Dm_BIT_MASK               (* (reg8 *) USBUART_Central_Dm__BIT_MASK)
/* Bypass Enable */
#define USBUART_Central_Dm_BYP                    (* (reg8 *) USBUART_Central_Dm__BYP)
/* Port wide control signals */                                                   
#define USBUART_Central_Dm_CTL                    (* (reg8 *) USBUART_Central_Dm__CTL)
/* Drive Modes */
#define USBUART_Central_Dm_DM0                    (* (reg8 *) USBUART_Central_Dm__DM0) 
#define USBUART_Central_Dm_DM1                    (* (reg8 *) USBUART_Central_Dm__DM1)
#define USBUART_Central_Dm_DM2                    (* (reg8 *) USBUART_Central_Dm__DM2) 
/* Input Buffer Disable Override */
#define USBUART_Central_Dm_INP_DIS                (* (reg8 *) USBUART_Central_Dm__INP_DIS)
/* LCD Common or Segment Drive */
#define USBUART_Central_Dm_LCD_COM_SEG            (* (reg8 *) USBUART_Central_Dm__LCD_COM_SEG)
/* Enable Segment LCD */
#define USBUART_Central_Dm_LCD_EN                 (* (reg8 *) USBUART_Central_Dm__LCD_EN)
/* Slew Rate Control */
#define USBUART_Central_Dm_SLW                    (* (reg8 *) USBUART_Central_Dm__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define USBUART_Central_Dm_PRTDSI__CAPS_SEL       (* (reg8 *) USBUART_Central_Dm__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define USBUART_Central_Dm_PRTDSI__DBL_SYNC_IN    (* (reg8 *) USBUART_Central_Dm__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define USBUART_Central_Dm_PRTDSI__OE_SEL0        (* (reg8 *) USBUART_Central_Dm__PRTDSI__OE_SEL0) 
#define USBUART_Central_Dm_PRTDSI__OE_SEL1        (* (reg8 *) USBUART_Central_Dm__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define USBUART_Central_Dm_PRTDSI__OUT_SEL0       (* (reg8 *) USBUART_Central_Dm__PRTDSI__OUT_SEL0) 
#define USBUART_Central_Dm_PRTDSI__OUT_SEL1       (* (reg8 *) USBUART_Central_Dm__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define USBUART_Central_Dm_PRTDSI__SYNC_OUT       (* (reg8 *) USBUART_Central_Dm__PRTDSI__SYNC_OUT) 


#if defined(USBUART_Central_Dm__INTSTAT)  /* Interrupt Registers */

    #define USBUART_Central_Dm_INTSTAT                (* (reg8 *) USBUART_Central_Dm__INTSTAT)
    #define USBUART_Central_Dm_SNAP                   (* (reg8 *) USBUART_Central_Dm__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_USBUART_Central_Dm_H */


/* [] END OF FILE */
