/*******************************************************************************
* File Name: VDAC8_PulseIR_PM.c  
* Version 1.70
*
*  Description:
*     This file provides the power management source code to API for the
*     VDAC8.  
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VDAC8_PulseIR.h"

static VDAC8_PulseIR_backupStruct VDAC8_PulseIR_backup;


/*******************************************************************************
* Function Name: VDAC8_PulseIR_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Reenrancy: 
*  No
*
*******************************************************************************/
void VDAC8_PulseIR_SaveConfig(void)
{
    VDAC8_PulseIR_backup.data_value = VDAC8_PulseIR_Data;
}


/*******************************************************************************
* Function Name: VDAC8_PulseIR_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Reenrancy: 
*  No
*
*******************************************************************************/
void VDAC8_PulseIR_RestoreConfig(void)
{
    VDAC8_PulseIR_Data = VDAC8_PulseIR_backup.data_value;
}


/*******************************************************************************
* Function Name: VDAC8_PulseIR_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  VDAC8_PulseIR_backup.enableState:  Is modified depending on the enable 
* state  of the block before entering sleep mode.
*
* Reenrancy: 
*  No
*
*******************************************************************************/
void VDAC8_PulseIR_Sleep(void)
{
    /* Save VDAC8's enable state */    
    if(VDAC8_PulseIR_ACT_PWR_EN == (VDAC8_PulseIR_PWRMGR & VDAC8_PulseIR_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VDAC8_PulseIR_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VDAC8_PulseIR_backup.enableState = 0u;
    }
    
    VDAC8_PulseIR_Stop();
    VDAC8_PulseIR_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC8_PulseIR_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  VDAC8_PulseIR_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_PulseIR_Wakeup(void) 
{
    VDAC8_PulseIR_RestoreConfig();
    
    if(VDAC8_PulseIR_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VDAC8_PulseIR_Enable();

        /* Restore the data register */
        VDAC8_PulseIR_SetValue(VDAC8_PulseIR_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
