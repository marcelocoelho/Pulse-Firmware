/*******************************************************************************
* File Name: Status_Reg_DevID.h  
* Version 1.70
*
* Description:
*  This file containts Status Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_STATUS_REG_Status_Reg_DevID_H) /* CY_STATUS_REG_Status_Reg_DevID_H */
#define CY_STATUS_REG_Status_Reg_DevID_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 Status_Reg_DevID_Read(void) ;
void Status_Reg_DevID_InterruptEnable(void) ;
void Status_Reg_DevID_InterruptDisable(void) ;
void Status_Reg_DevID_WriteMask(uint8) ;
uint8 Status_Reg_DevID_ReadMask(void) ;


/***************************************
*           API Constants
***************************************/

#define Status_Reg_DevID_STATUS_INTR_ENBL    0x10u


/***************************************
*         Parameter Constants
***************************************/

/* Status Register Inputs */
#define Status_Reg_DevID_INPUTS              8


/***************************************
*             Registers
***************************************/

/* Status Register */
#define Status_Reg_DevID_Status             (* (reg8 *) Status_Reg_DevID_sts_sts_reg__STATUS_REG )
#define Status_Reg_DevID_Status_PTR         (  (reg8 *) Status_Reg_DevID_sts_sts_reg__STATUS_REG )
#define Status_Reg_DevID_Status_Mask        (* (reg8 *) Status_Reg_DevID_sts_sts_reg__MASK_REG )
#define Status_Reg_DevID_Status_Aux_Ctrl    (* (reg8 *) Status_Reg_DevID_sts_sts_reg__STATUS_AUX_CTL_REG )

#endif /* End CY_STATUS_REG_Status_Reg_DevID_H */


/* [] END OF FILE */
