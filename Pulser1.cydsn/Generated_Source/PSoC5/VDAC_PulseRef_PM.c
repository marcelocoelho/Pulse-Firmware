/*******************************************************************************
* File Name: VDAC_PulseRef_PM.c  
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

#include "VDAC_PulseRef.h"

static VDAC_PulseRef_backupStruct VDAC_PulseRef_backup;


/*******************************************************************************
* Function Name: VDAC_PulseRef_SaveConfig
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
void VDAC_PulseRef_SaveConfig(void)
{
    VDAC_PulseRef_backup.data_value = VDAC_PulseRef_Data;
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_RestoreConfig
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
void VDAC_PulseRef_RestoreConfig(void)
{
    VDAC_PulseRef_Data = VDAC_PulseRef_backup.data_value;
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_Sleep
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
*  VDAC_PulseRef_backup.enableState:  Is modified depending on the enable 
* state  of the block before entering sleep mode.
*
* Reenrancy: 
*  No
*
*******************************************************************************/
void VDAC_PulseRef_Sleep(void)
{
    /* Save VDAC8's enable state */    
    if(VDAC_PulseRef_ACT_PWR_EN == (VDAC_PulseRef_PWRMGR & VDAC_PulseRef_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VDAC_PulseRef_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VDAC_PulseRef_backup.enableState = 0u;
    }
    
    VDAC_PulseRef_Stop();
    VDAC_PulseRef_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_Wakeup
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
*  VDAC_PulseRef_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC_PulseRef_Wakeup(void) 
{
    VDAC_PulseRef_RestoreConfig();
    
    if(VDAC_PulseRef_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VDAC_PulseRef_Enable();

        /* Restore the data register */
        VDAC_PulseRef_SetValue(VDAC_PulseRef_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
