/*******************************************************************************
* File Name: PWM_PulseLEDs_PM.c
* Version 2.20
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cytypes.h"
#include "PWM_PulseLEDs.h"

static PWM_PulseLEDs_backupStruct PWM_PulseLEDs_backup;


/*******************************************************************************
* Function Name: PWM_PulseLEDs_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  PWM_PulseLEDs_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void PWM_PulseLEDs_SaveConfig(void) 
{
    
    #if(!PWM_PulseLEDs_UsingFixedFunction)
        #if (CY_PSOC5A)
            PWM_PulseLEDs_backup.PWMUdb = PWM_PulseLEDs_ReadCounter();
            PWM_PulseLEDs_backup.PWMPeriod = PWM_PulseLEDs_ReadPeriod();
            #if (PWM_PulseLEDs_UseStatus)
                PWM_PulseLEDs_backup.InterruptMaskValue = PWM_PulseLEDs_STATUS_MASK;
            #endif /* (PWM_PulseLEDs_UseStatus) */
            
            #if(PWM_PulseLEDs_UseOneCompareMode)
                PWM_PulseLEDs_backup.PWMCompareValue = PWM_PulseLEDs_ReadCompare();
            #else
                PWM_PulseLEDs_backup.PWMCompareValue1 = PWM_PulseLEDs_ReadCompare1();
                PWM_PulseLEDs_backup.PWMCompareValue2 = PWM_PulseLEDs_ReadCompare2();
            #endif /* (PWM_PulseLEDs_UseOneCompareMode) */
            
           #if(PWM_PulseLEDs_DeadBandUsed)
                PWM_PulseLEDs_backup.PWMdeadBandValue = PWM_PulseLEDs_ReadDeadTime();
            #endif /* (PWM_PulseLEDs_DeadBandUsed) */
          
            #if ( PWM_PulseLEDs_KillModeMinTime)
                PWM_PulseLEDs_backup.PWMKillCounterPeriod = PWM_PulseLEDs_ReadKillTime();
            #endif /* ( PWM_PulseLEDs_KillModeMinTime) */
        #endif /* (CY_PSOC5A) */
        
        #if (CY_PSOC3 || CY_PSOC5LP)
            #if(!PWM_PulseLEDs_PWMModeIsCenterAligned)
                PWM_PulseLEDs_backup.PWMPeriod = PWM_PulseLEDs_ReadPeriod();
            #endif /* (!PWM_PulseLEDs_PWMModeIsCenterAligned) */
            PWM_PulseLEDs_backup.PWMUdb = PWM_PulseLEDs_ReadCounter();
            #if (PWM_PulseLEDs_UseStatus)
                PWM_PulseLEDs_backup.InterruptMaskValue = PWM_PulseLEDs_STATUS_MASK;
            #endif /* (PWM_PulseLEDs_UseStatus) */
            
            #if(PWM_PulseLEDs_DeadBandMode == PWM_PulseLEDs__B_PWM__DBM_256_CLOCKS || \
                PWM_PulseLEDs_DeadBandMode == PWM_PulseLEDs__B_PWM__DBM_2_4_CLOCKS)
                PWM_PulseLEDs_backup.PWMdeadBandValue = PWM_PulseLEDs_ReadDeadTime();
            #endif /*  deadband count is either 2-4 clocks or 256 clocks */
            
            #if(PWM_PulseLEDs_KillModeMinTime)
                 PWM_PulseLEDs_backup.PWMKillCounterPeriod = PWM_PulseLEDs_ReadKillTime();
            #endif /* (PWM_PulseLEDs_KillModeMinTime) */
        #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
        #if(PWM_PulseLEDs_UseControl)
            PWM_PulseLEDs_backup.PWMControlRegister = PWM_PulseLEDs_ReadControlRegister();
        #endif /* (PWM_PulseLEDs_UseControl) */
    #endif  /* (!PWM_PulseLEDs_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_PulseLEDs_RestoreConfig
********************************************************************************
* 
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  PWM_PulseLEDs_backup:  Variables of this global structure are used to  
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_PulseLEDs_RestoreConfig(void) 
{
        #if(!PWM_PulseLEDs_UsingFixedFunction)
            #if (CY_PSOC5A)
                /* Interrupt State Backup for Critical Region*/
                uint8 PWM_PulseLEDs_interruptState;
                /* Enter Critical Region*/
                PWM_PulseLEDs_interruptState = CyEnterCriticalSection();
                #if (PWM_PulseLEDs_UseStatus)
                    /* Use the interrupt output of the status register for IRQ output */
                    PWM_PulseLEDs_STATUS_AUX_CTRL |= PWM_PulseLEDs_STATUS_ACTL_INT_EN_MASK;
                    
                    PWM_PulseLEDs_STATUS_MASK = PWM_PulseLEDs_backup.InterruptMaskValue;
                #endif /* (PWM_PulseLEDs_UseStatus) */
                
                #if (PWM_PulseLEDs_Resolution == 8)
                    /* Set FIFO 0 to 1 byte register for period*/
                    PWM_PulseLEDs_AUX_CONTROLDP0 |= (PWM_PulseLEDs_AUX_CTRL_FIFO0_CLR);
                #else /* (PWM_PulseLEDs_Resolution == 16)*/
                    /* Set FIFO 0 to 1 byte register for period */
                    PWM_PulseLEDs_AUX_CONTROLDP0 |= (PWM_PulseLEDs_AUX_CTRL_FIFO0_CLR);
                    PWM_PulseLEDs_AUX_CONTROLDP1 |= (PWM_PulseLEDs_AUX_CTRL_FIFO0_CLR);
                #endif /* (PWM_PulseLEDs_Resolution == 8) */
                /* Exit Critical Region*/
                CyExitCriticalSection(PWM_PulseLEDs_interruptState);
                
                PWM_PulseLEDs_WriteCounter(PWM_PulseLEDs_backup.PWMUdb);
                PWM_PulseLEDs_WritePeriod(PWM_PulseLEDs_backup.PWMPeriod);
                
                #if(PWM_PulseLEDs_UseOneCompareMode)
                    PWM_PulseLEDs_WriteCompare(PWM_PulseLEDs_backup.PWMCompareValue);
                #else
                    PWM_PulseLEDs_WriteCompare1(PWM_PulseLEDs_backup.PWMCompareValue1);
                    PWM_PulseLEDs_WriteCompare2(PWM_PulseLEDs_backup.PWMCompareValue2);
                #endif /* (PWM_PulseLEDs_UseOneCompareMode) */
                
               #if(PWM_PulseLEDs_DeadBandMode == PWM_PulseLEDs__B_PWM__DBM_256_CLOCKS || \
                   PWM_PulseLEDs_DeadBandMode == PWM_PulseLEDs__B_PWM__DBM_2_4_CLOCKS)
                    PWM_PulseLEDs_WriteDeadTime(PWM_PulseLEDs_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
            
                #if ( PWM_PulseLEDs_KillModeMinTime)
                    PWM_PulseLEDs_WriteKillTime(PWM_PulseLEDs_backup.PWMKillCounterPeriod);
                #endif /* ( PWM_PulseLEDs_KillModeMinTime) */
            #endif /* (CY_PSOC5A) */
            
            #if (CY_PSOC3 || CY_PSOC5LP)
                #if(!PWM_PulseLEDs_PWMModeIsCenterAligned)
                    PWM_PulseLEDs_WritePeriod(PWM_PulseLEDs_backup.PWMPeriod);
                #endif /* (!PWM_PulseLEDs_PWMModeIsCenterAligned) */
                PWM_PulseLEDs_WriteCounter(PWM_PulseLEDs_backup.PWMUdb);
                #if (PWM_PulseLEDs_UseStatus)
                    PWM_PulseLEDs_STATUS_MASK = PWM_PulseLEDs_backup.InterruptMaskValue;
                #endif /* (PWM_PulseLEDs_UseStatus) */
                
                #if(PWM_PulseLEDs_DeadBandMode == PWM_PulseLEDs__B_PWM__DBM_256_CLOCKS || \
                    PWM_PulseLEDs_DeadBandMode == PWM_PulseLEDs__B_PWM__DBM_2_4_CLOCKS)
                    PWM_PulseLEDs_WriteDeadTime(PWM_PulseLEDs_backup.PWMdeadBandValue);
                #endif /* deadband count is either 2-4 clocks or 256 clocks */
                
                #if(PWM_PulseLEDs_KillModeMinTime)
                    PWM_PulseLEDs_WriteKillTime(PWM_PulseLEDs_backup.PWMKillCounterPeriod);
                #endif /* (PWM_PulseLEDs_KillModeMinTime) */
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            
            #if(PWM_PulseLEDs_UseControl)
                PWM_PulseLEDs_WriteControlRegister(PWM_PulseLEDs_backup.PWMControlRegister); 
            #endif /* (PWM_PulseLEDs_UseControl) */
        #endif  /* (!PWM_PulseLEDs_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_PulseLEDs_Sleep
********************************************************************************
* 
* Summary:
*  Disables block's operation and saves the user configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  PWM_PulseLEDs_backup.PWMEnableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_PulseLEDs_Sleep(void) 
{
    #if(PWM_PulseLEDs_UseControl)
        if(PWM_PulseLEDs_CTRL_ENABLE == (PWM_PulseLEDs_CONTROL & PWM_PulseLEDs_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_PulseLEDs_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_PulseLEDs_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_PulseLEDs_UseControl) */
    /* Stop component */
    PWM_PulseLEDs_Stop();
    
    /* Save registers configuration */
    PWM_PulseLEDs_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_PulseLEDs_Wakeup
********************************************************************************
* 
* Summary:
*  Restores and enables the user configuration. Should be called just after 
*  awaking from sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  PWM_PulseLEDs_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_PulseLEDs_Wakeup(void) 
{
     /* Restore registers values */
    PWM_PulseLEDs_RestoreConfig();
    
    if(PWM_PulseLEDs_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_PulseLEDs_Enable();
    } /* Do nothing if component's block was disabled before */
    
}


/* [] END OF FILE */
