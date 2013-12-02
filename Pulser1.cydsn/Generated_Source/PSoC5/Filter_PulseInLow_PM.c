/*******************************************************************************
* File Name: Filter_PulseInLow_PM.c
* Version 2.0
*
* Description:
*  This file provides the power managaer API source code for the FILT component.
*
* Note:
*  
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Filter_PulseInLow_PVT.h"

static Filter_PulseInLow_BACKUP_STRUCT  Filter_PulseInLow_backup = 
{
    Filter_PulseInLow_DISABLED,
    Filter_PulseInLow_RUN_MASK,
    
};


/*******************************************************************************
* Function Name: Filter_PulseInLow_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Filter_PulseInLow_backup:  This global structure variable is used to store 
*  configuration registers which are non retention whenever user wants to go 
*  sleep mode by calling Sleep() API.
*
* Reentrance:
*  No
*
*******************************************************************************/
void Filter_PulseInLow_SaveConfig(void)
{
    Filter_PulseInLow_backup.cr = Filter_PulseInLow_CR_REG;
    Filter_PulseInLow_backup.sr = Filter_PulseInLow_SR_REG;
    Filter_PulseInLow_backup.sema = Filter_PulseInLow_SEMA_REG;
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_RestoreConfig
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
* Global variables:
*  Filter_PulseInLow_backup:  This global structure variable is used to restore 
*  configuration registers which are non retention whenever user wants to switch 
*  to active power mode by calling Wakeup() API.
*
*******************************************************************************/
void Filter_PulseInLow_RestoreConfig(void) 
{
    Filter_PulseInLow_CR_REG = Filter_PulseInLow_backup.cr;
    Filter_PulseInLow_SR_REG = Filter_PulseInLow_backup.sr;
    Filter_PulseInLow_SEMA_REG = Filter_PulseInLow_backup.sema;
    // Restore ACU RAM as this is not retension
    Filter_PulseInLow_RestoreACURam();
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_RestoreACURAM
********************************************************************************
*
* Summary:
*  Restores the contents of ACU ram.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  None.
*
*******************************************************************************/
void Filter_PulseInLow_RestoreACURam() 
{
    /* Power on DFB before initializing the RAMs */
    Filter_PulseInLow_PM_ACT_CFG_REG |= Filter_PulseInLow_PM_ACT_MSK;

    /* Put DFB RAM on the bus */
    Filter_PulseInLow_RAM_DIR_REG = Filter_PulseInLow_RAM_DIR_BUS;

    /* ACU RAM */
    memcpy(Filter_PulseInLow_ACU_RAM,
        Filter_PulseInLow_acu, Filter_PulseInLow_ACU_RAM_SIZE); 

    /* Take DFB RAM off the bus */
    Filter_PulseInLow_RAM_DIR_REG = Filter_PulseInLow_RAM_DIR_DFB;
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Filter_PulseInLow_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrance:
*  No
*
*******************************************************************************/
void Filter_PulseInLow_Sleep(void)
{
    /* Save Filter enable state */
    if(Filter_PulseInLow_PM_ACT_MSK == (Filter_PulseInLow_PM_ACT_CFG_REG & Filter_PulseInLow_PM_ACT_MSK))
    {
        /* Component is enabled */
        Filter_PulseInLow_backup.enableState = Filter_PulseInLow_ENABLED;
    }
    else
    {
        /* Component is disabled */
        Filter_PulseInLow_backup.enableState = Filter_PulseInLow_DISABLED;
    }

    /* Stop the configuration */
    Filter_PulseInLow_Stop();

    /* Save the configuration */
    Filter_PulseInLow_SaveConfig();
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Filter_PulseInLow_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void Filter_PulseInLow_Wakeup(void) 
{
    /* Restore the configuration */
    Filter_PulseInLow_RestoreConfig();
    
    /* Enable's the component operation */
    if(Filter_PulseInLow_backup.enableState == Filter_PulseInLow_ENABLED)
    {
        Filter_PulseInLow_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
