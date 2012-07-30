/*******************************************************************************
* File Name: Status_Reg_DevID.h  
* Version 1.60
*
* Description:
*  This file containts Status Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_STATUS_REG_Status_Reg_DevID_H) /* CY_STATUS_REG_Status_Reg_DevID_H */
#define CY_STATUS_REG_Status_Reg_DevID_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 Status_Reg_DevID_Read(void) ;


/***************************************
*             Registers
***************************************/

/* Status Register */
#define Status_Reg_DevID_Status    (* (reg8 *) Status_Reg_DevID_sts_reg__STATUS_REG )

#endif /* End CY_STATUS_REG_Status_Reg_DevID_H */


/* [] END OF FILE */
