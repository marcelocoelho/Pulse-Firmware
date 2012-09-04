/*******************************************************************************
* File Name: IDAC8_PulseIR.c
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  IDAC8.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include "IDAC8_PulseIR.h"

static IDAC8_PulseIR_backupStruct IDAC8_PulseIR_backup;


/*******************************************************************************
* Function Name: IDAC8_PulseIR_SaveConfig
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
void IDAC8_PulseIR_SaveConfig(void) 
{
    if (!((IDAC8_PulseIR_CR1 & IDAC8_PulseIR_SRC_MASK) == IDAC8_PulseIR_SRC_UDB))
    {
        IDAC8_PulseIR_backup.data_value = IDAC8_PulseIR_Data;
    }
}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_RestoreConfig
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
void IDAC8_PulseIR_RestoreConfig(void) 
{
    if (!((IDAC8_PulseIR_CR1 & IDAC8_PulseIR_SRC_MASK) == IDAC8_PulseIR_SRC_UDB))
    {
        if((IDAC8_PulseIR_Strobe & IDAC8_PulseIR_STRB_MASK) == IDAC8_PulseIR_STRB_EN)
        {
            IDAC8_PulseIR_Strobe &= ~IDAC8_PulseIR_STRB_MASK;
            IDAC8_PulseIR_Data = IDAC8_PulseIR_backup.data_value;
            IDAC8_PulseIR_Strobe |= IDAC8_PulseIR_STRB_EN;
        }
        else
        {
            IDAC8_PulseIR_Data = IDAC8_PulseIR_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:
*  void:
*
* Return:
*  void
*
* Global variables:
*  IDAC8_PulseIR_backup.enableState: Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void IDAC8_PulseIR_Sleep(void) 
{
    if(IDAC8_PulseIR_ACT_PWR_EN == (IDAC8_PulseIR_PWRMGR & IDAC8_PulseIR_ACT_PWR_EN))
    {
        /* IDAC8 is enabled */
        IDAC8_PulseIR_backup.enableState = 1u;
    }
    else
    {
        /* IDAC8 is disabled */
        IDAC8_PulseIR_backup.enableState = 0u;
    }

    IDAC8_PulseIR_Stop();
    IDAC8_PulseIR_SaveConfig();
}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_Wakeup
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
*  IDAC8_PulseIR_backup.enableState: Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void IDAC8_PulseIR_Wakeup(void) 
{
    IDAC8_PulseIR_RestoreConfig();
    
    if(IDAC8_PulseIR_backup.enableState == 1u)
    {
        /* Enable IDAC8's operation */
        IDAC8_PulseIR_Enable();
        
        /* Set the data register */
        IDAC8_PulseIR_SetValue(IDAC8_PulseIR_Data);
    } /* Do nothing if IDAC8 was disabled before */    
}


/* [] END OF FILE */
