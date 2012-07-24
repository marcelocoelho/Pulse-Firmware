/*******************************************************************************
* File Name: IDAC8_PulseRed_PM.c  
* Version 1.80
*
*  Description:
*     This file provides the power management source code to API for the
*     IDAC8.  
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/


#include "IDAC8_PulseRed.h"

static IDAC8_PulseRed_backupStruct IDAC8_PulseRed_backup;


/*******************************************************************************
* Function Name: IDAC8_PulseRed_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void IDAC8_PulseRed_SaveConfig(void)
{
    IDAC8_PulseRed_backup.data_value = IDAC8_PulseRed_Data;
}


/*******************************************************************************
* Function Name: IDAC8_PulseRed_RestoreConfig
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
*******************************************************************************/
void IDAC8_PulseRed_RestoreConfig(void)
{
    IDAC8_PulseRed_Data = IDAC8_PulseRed_backup.data_value;
}


/*******************************************************************************
* Function Name: IDAC8_PulseRed_Sleep
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
*  IDAC8_PulseRed_backup.enableState:  Is modified depending on the enable 
*   state of the block before entering sleep mode.
*
*******************************************************************************/
void IDAC8_PulseRed_Sleep(void)
{
    if(IDAC8_PulseRed_ACT_PWR_EN == (IDAC8_PulseRed_PWRMGR & IDAC8_PulseRed_ACT_PWR_EN))
    {
        /* IDAC8 is enabled */
        IDAC8_PulseRed_backup.enableState = 1u;
    }
    else
    {
        /* IDAC8 is disabled */
        IDAC8_PulseRed_backup.enableState = 0u;
    }
    
    IDAC8_PulseRed_Stop();
    IDAC8_PulseRed_SaveConfig();
}


/*******************************************************************************
* Function Name: IDAC8_PulseRed_Wakeup
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
*  IDAC8_PulseRed_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void IDAC8_PulseRed_Wakeup(void)
{
    IDAC8_PulseRed_RestoreConfig();
    
    if(IDAC8_PulseRed_backup.enableState == 1u)
    {
        /* Enable IDAC8's operation */
        IDAC8_PulseRed_Enable();
       
        /* Set the data register */
        IDAC8_PulseRed_SetValue(IDAC8_PulseRed_Data);
    } /* Do nothing if IDAC8 was disabled before */    
}


/* [] END OF FILE */
