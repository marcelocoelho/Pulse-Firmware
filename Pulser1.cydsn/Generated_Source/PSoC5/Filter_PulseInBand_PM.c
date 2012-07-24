/*******************************************************************************
* File Name: Filter_PulseInBand_PM.c
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

#include "Filter_PulseInBand_PVT.h"

static Filter_PulseInBand_BACKUP_STRUCT  Filter_PulseInBand_backup = 
{
    Filter_PulseInBand_DISABLED,
    Filter_PulseInBand_RUN_MASK,
    
};


/*******************************************************************************
* Function Name: Filter_PulseInBand_SaveConfig
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
*  Filter_PulseInBand_backup:  This global structure variable is used to store 
*  configuration registers which are non retention whenever user wants to go 
*  sleep mode by calling Sleep() API.
*
* Reentrance:
*  No
*
*******************************************************************************/
void Filter_PulseInBand_SaveConfig(void)
{
    Filter_PulseInBand_backup.cr = Filter_PulseInBand_CR_REG;
    Filter_PulseInBand_backup.sr = Filter_PulseInBand_SR_REG;
    Filter_PulseInBand_backup.sema = Filter_PulseInBand_SEMA_REG;
}


/*******************************************************************************
* Function Name: Filter_PulseInBand_RestoreConfig
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
*  Filter_PulseInBand_backup:  This global structure variable is used to restore 
*  configuration registers which are non retention whenever user wants to switch 
*  to active power mode by calling Wakeup() API.
*
*******************************************************************************/
void Filter_PulseInBand_RestoreConfig(void) 
{
    Filter_PulseInBand_CR_REG = Filter_PulseInBand_backup.cr;
    Filter_PulseInBand_SR_REG = Filter_PulseInBand_backup.sr;
    Filter_PulseInBand_SEMA_REG = Filter_PulseInBand_backup.sema;
    // Restore ACU RAM as this is not retension
    Filter_PulseInBand_RestoreACURam();
}


/*******************************************************************************
* Function Name: Filter_PulseInBand_RestoreACURAM
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
void Filter_PulseInBand_RestoreACURam() 
{
    /* Power on DFB before initializing the RAMs */
    Filter_PulseInBand_PM_ACT_CFG_REG |= Filter_PulseInBand_PM_ACT_MSK;

    /* Put DFB RAM on the bus */
    Filter_PulseInBand_RAM_DIR_REG = Filter_PulseInBand_RAM_DIR_BUS;

    /* ACU RAM */
    memcpy(Filter_PulseInBand_ACU_RAM,
        Filter_PulseInBand_acu, Filter_PulseInBand_ACU_RAM_SIZE); 

    /* Take DFB RAM off the bus */
    Filter_PulseInBand_RAM_DIR_REG = Filter_PulseInBand_RAM_DIR_DFB;
}


/*******************************************************************************
* Function Name: Filter_PulseInBand_Sleep
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
*  Filter_PulseInBand_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrance:
*  No
*
*******************************************************************************/
void Filter_PulseInBand_Sleep(void)
{
    /* Save Filter enable state */
    if(Filter_PulseInBand_PM_ACT_MSK == (Filter_PulseInBand_PM_ACT_CFG_REG & Filter_PulseInBand_PM_ACT_MSK))
    {
        /* Component is enabled */
        Filter_PulseInBand_backup.enableState = Filter_PulseInBand_ENABLED;
    }
    else
    {
        /* Component is disabled */
        Filter_PulseInBand_backup.enableState = Filter_PulseInBand_DISABLED;
    }

    /* Stop the configuration */
    Filter_PulseInBand_Stop();

    /* Save the configuration */
    Filter_PulseInBand_SaveConfig();
}


/*******************************************************************************
* Function Name: Filter_PulseInBand_Wakeup
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
*  Filter_PulseInBand_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void Filter_PulseInBand_Wakeup(void) 
{
    /* Restore the configuration */
    Filter_PulseInBand_RestoreConfig();
    
    /* Enable's the component operation */
    if(Filter_PulseInBand_backup.enableState == Filter_PulseInBand_ENABLED)
    {
        Filter_PulseInBand_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
