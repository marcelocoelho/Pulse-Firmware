/*******************************************************************************
* File Name: ADC_PulseIn_PM.c  
* Version 2.20
*
* Description:
*  This file provides the power manager source code to the API for the 
*  Delta-Sigma ADC  Component.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "ADC_PulseIn.h"

static ADC_PulseIn_BACKUP_STRUCT ADC_PulseIn_backup = 
{
    ADC_PulseIn_DISABLED,
    ADC_PulseIn_CFG1_DEC_CR
};


/*******************************************************************************  
* Function Name: ADC_PulseIn_SaveConfig
********************************************************************************
*
* Summary:
*  Save the register configuration which are not retention.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
* Global variables:
*  ADC_PulseIn_backup:  This global structure variable is used to store 
*  configuration registers which are non retention whenever user wants to go 
*  sleep mode by calling Sleep() API.

*
* Reentrance:
*  No
*
*******************************************************************************/
void ADC_PulseIn_SaveConfig(void)
{
    ADC_PulseIn_backup.deccr = ADC_PulseIn_DEC_CR_REG;
}


/*******************************************************************************  
* Function Name: ADC_PulseIn_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore the register configurations which are not retention.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
* Global variables:
*  ADC_PulseIn_backup:  This global structure variable is used to restore 
*  configuration registers which are non retention whenever user wants to switch 
*  to active power mode by calling Wakeup() API.
*
* Reentrance:
*  No
*
*******************************************************************************/
void ADC_PulseIn_RestoreConfig(void) 
{
    ADC_PulseIn_DEC_CR_REG = ADC_PulseIn_backup.deccr;
}


/******************************************************************************* 
* Function Name: ADC_PulseIn_Sleep
********************************************************************************
*
* Summary:
*  Stops the operation of the block and saves the user configuration. 
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  ADC_PulseIn_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.

*
* Reentrance: 
*  No
*
*******************************************************************************/
void ADC_PulseIn_Sleep(void)
{
    /* Save ADC enable state */
    if((ADC_PulseIn_ACT_PWR_DEC_EN == (ADC_PulseIn_PWRMGR_DEC_REG & ADC_PulseIn_ACT_PWR_DEC_EN)) &&
       (ADC_PulseIn_ACT_PWR_DSM_EN == (ADC_PulseIn_PWRMGR_DSM_REG & ADC_PulseIn_ACT_PWR_DSM_EN)))
    {
        /* Component is enabled */
        ADC_PulseIn_backup.enableState = ADC_PulseIn_ENABLED;
    }
    else
    {
        /* Component is disabled */
        ADC_PulseIn_backup.enableState = ADC_PulseIn_DISABLED;
    }

    /* Stop the configuration */
    ADC_PulseIn_Stop();

    /* Save the user configuration */
    ADC_PulseIn_SaveConfig();
}


/******************************************************************************* 
* Function Name: ADC_PulseIn_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and enables the power to the block.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  ADC_PulseIn_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrance: 
*  No
*
*******************************************************************************/
void ADC_PulseIn_Wakeup(void) 
{
    /* Restore the configuration */
    ADC_PulseIn_RestoreConfig();

    /* Enable's the component operation */
    if(ADC_PulseIn_backup.enableState == ADC_PulseIn_ENABLED)
    {
        ADC_PulseIn_Enable();

        /* Start the conversion only if conversion is not triggered by the hardware */
        if(!(ADC_PulseIn_DEC_CR_REG & ADC_PulseIn_DEC_XSTART_EN))
        {
            ADC_PulseIn_StartConvert();
        }

    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
