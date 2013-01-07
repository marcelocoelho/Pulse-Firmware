/*******************************************************************************
* File Name: ShiftReg_DelaySenseIR_PM.c
* Version 2.20
*
* Description:
*  This file provides the API source code for sleep mode support for Shift
*  Register component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ShiftReg_DelaySenseIR.h"

static ShiftReg_DelaySenseIR_BACKUP_STRUCT ShiftReg_DelaySenseIR_backup =
{
    ShiftReg_DelaySenseIR_DISABLED,

    ((uint32) ShiftReg_DelaySenseIR_DEFAULT_A0),
    ((uint32) ShiftReg_DelaySenseIR_DEFAULT_A1),

    #if(CY_UDB_V0)
        ((uint32) ShiftReg_DelaySenseIR_INT_SRC),
    #endif /* (CY_UDB_V0) */
};


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_SaveConfig
********************************************************************************
*
* Summary:
*  Saves Shift Register configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_SaveConfig(void) 
{
    /* Store working registers A0 and A1 */
    ShiftReg_DelaySenseIR_backup.saveSrA0Reg   = CY_GET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_LSB_PTR);
    ShiftReg_DelaySenseIR_backup.saveSrA1Reg   = CY_GET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_VALUE_LSB_PTR);

    #if(CY_UDB_V0)
        ShiftReg_DelaySenseIR_backup.saveSrIntMask = ShiftReg_DelaySenseIR_SR_STATUS_MASK;
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores Shift Register configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_RestoreConfig(void) 
{
    /* Restore working registers A0 and A1 */
    CY_SET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_LSB_PTR, ShiftReg_DelaySenseIR_backup.saveSrA0Reg);
    CY_SET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_VALUE_LSB_PTR, ShiftReg_DelaySenseIR_backup.saveSrA1Reg);

    #if(CY_UDB_V0)
        ShiftReg_DelaySenseIR_SR_STATUS_MASK = ((uint8) ShiftReg_DelaySenseIR_backup.saveSrIntMask);
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_Sleep
********************************************************************************
*
* Summary:
*  Prepare the component to enter a Sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_Sleep(void) 
{
    ShiftReg_DelaySenseIR_backup.enableState = ((uint8) ShiftReg_DelaySenseIR_IS_ENABLED);

    ShiftReg_DelaySenseIR_Stop();
    ShiftReg_DelaySenseIR_SaveConfig();
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_Wakeup(void) 
{
    ShiftReg_DelaySenseIR_RestoreConfig();

    if(0u != ShiftReg_DelaySenseIR_backup.enableState)
    {
        ShiftReg_DelaySenseIR_Enable();
    }
}


/* [] END OF FILE */
