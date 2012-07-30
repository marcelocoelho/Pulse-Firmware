/*******************************************************************************
* File Name: ShiftReg_DelaySenseIR_PM.c
* Version 2.0
*
* Description:
*  This file provides the API source code for sleep mode support for Shift 
*  Register component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ShiftReg_DelaySenseIR.h"


static ShiftReg_DelaySenseIR_BACKUP_STRUCT ShiftReg_DelaySenseIR_backup = \
{
    /* enable state - disabled */
    0u
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
    /* Store A0, A1 and Status Mask registers */
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
       ShiftReg_DelaySenseIR_backup.saveSrA0Reg   = CY_GET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_LSB_PTR);
       ShiftReg_DelaySenseIR_backup.saveSrA1Reg   = CY_GET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_VALUE_LSB_PTR);
       ShiftReg_DelaySenseIR_backup.saveSrIntMask = ShiftReg_DelaySenseIR_SR_STATUS_MASK;

    #else
    /* Store A0, A1 only (not need to save Status Mask register  in ES3 silicon) */
       ShiftReg_DelaySenseIR_backup.saveSrA0Reg   = CY_GET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_LSB_PTR);
       ShiftReg_DelaySenseIR_backup.saveSrA1Reg   = CY_GET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_VALUE_LSB_PTR);

    #endif /*(CY_PSOC3_ES2 || CY_PSOC5_ES1)*/
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
    /* Restore tha data, saved by SaveConfig()function */
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
        CY_SET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_LSB_PTR, ShiftReg_DelaySenseIR_backup.saveSrA0Reg);
            CY_SET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_VALUE_LSB_PTR, ShiftReg_DelaySenseIR_backup.saveSrA1Reg);
            ShiftReg_DelaySenseIR_SR_STATUS_MASK = ShiftReg_DelaySenseIR_backup.saveSrIntMask;
    #else
            CY_SET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_LSB_PTR, ShiftReg_DelaySenseIR_backup.saveSrA0Reg);
            CY_SET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_VALUE_LSB_PTR, ShiftReg_DelaySenseIR_backup.saveSrA1Reg);

    #endif /*(CY_PSOC3_ES2 || CY_PSOC5_ES1)*/
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
    if((ShiftReg_DelaySenseIR_SR_CONTROL & ShiftReg_DelaySenseIR_CLK_EN) == ShiftReg_DelaySenseIR_CLK_EN)
    {
        ShiftReg_DelaySenseIR_backup.enableState = 1u;
    }
    else
    {
        ShiftReg_DelaySenseIR_backup.enableState = 0u;
    }
    
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
    
    if(ShiftReg_DelaySenseIR_backup.enableState == 1u)
    {
        ShiftReg_DelaySenseIR_Enable();   
    }
}

/* [] END OF FILE */
