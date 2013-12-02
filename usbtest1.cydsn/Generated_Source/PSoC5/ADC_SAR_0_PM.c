/*******************************************************************************
* File Name: ADC_SAR_0_PM.c
* Version 2.0
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_SAR_0.h"


/***************************************
* Local data allocation
***************************************/

static ADC_SAR_0_BACKUP_STRUCT  ADC_SAR_0_backup =
{
    ADC_SAR_0_DISABLED
};


/*******************************************************************************
* Function Name: ADC_SAR_0_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_SAR_0_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_0_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_SAR_0_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_0_Sleep
********************************************************************************
*
* Summary:
*  Stops and saves the user configuration
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SAR_0_backup - modified.
*
*******************************************************************************/
void ADC_SAR_0_Sleep(void)
{
    if((ADC_SAR_0_PWRMGR_SAR_REG  & ADC_SAR_0_ACT_PWR_SAR_EN) != 0u)
    {
        if((ADC_SAR_0_SAR_CSR0_REG & ADC_SAR_0_SAR_SOF_START_CONV) != 0u)
        {
            ADC_SAR_0_backup.enableState = ADC_SAR_0_ENABLED | ADC_SAR_0_STARTED;
        }
        else
        {
            ADC_SAR_0_backup.enableState = ADC_SAR_0_ENABLED;
        }
        ADC_SAR_0_Stop();
    }
    else
    {
        ADC_SAR_0_backup.enableState = ADC_SAR_0_DISABLED;
    }
    /*ADC_SAR_0_SaveConfig();*/
}


/*******************************************************************************
* Function Name: ADC_SAR_0_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SAR_0_backup - used.
*
*******************************************************************************/
void ADC_SAR_0_Wakeup(void)
{
    /*ADC_SAR_0_RestoreConfig();*/
    if(ADC_SAR_0_backup.enableState != ADC_SAR_0_DISABLED)
    {
        ADC_SAR_0_Enable();
        #if(ADC_SAR_0_DEFAULT_CONV_MODE != ADC_SAR_0__HARDWARE_TRIGGER)
            if((ADC_SAR_0_backup.enableState & ADC_SAR_0_STARTED) != 0u)
            {
                ADC_SAR_0_StartConvert();
            }
        #endif /* End ADC_SAR_0_DEFAULT_CONV_MODE != ADC_SAR_0__HARDWARE_TRIGGER */
        
    }
}


/* [] END OF FILE */
