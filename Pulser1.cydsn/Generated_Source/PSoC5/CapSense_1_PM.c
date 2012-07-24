/*******************************************************************************
* File Name: CapSense_1_PM.c
* Version 3.10
*
* Description:
*  This file provides Sleep APIs for CapSense CSD Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "CapSense_1.h"

CapSense_1_BACKUP_STRUCT CapSense_1_backup =
{   
    0x00u, /* enableState; */
    /* Set ScanSpeed */
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
        CapSense_1_SCANSPEED_VALUE,  /* scan speed value */
    #endif  /* End (CY_PSOC3_ES2 || CY_PSOC5_ES1) */
};


/*******************************************************************************
* Function Name: CapSense_1_SaveConfig
********************************************************************************
*
* Summary:
*  Saves customer configuration of CapSense none-retention registers. Resets 
*  all sensors to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  CapSense_1_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
* Reentrant:
*  No - for PSoC3 ES2 and PSoC5 ES1 silicon, Yes - for PSoC3 ES3.
*
*******************************************************************************/
#if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
    void CapSense_1_SaveConfig(void)
#else
    void CapSense_1_SaveConfig(void) 
#endif  /* End (CY_PSOC3_ES2 || CY_PSOC5_ES1) */
{    
    /* Set ScanSpeed */
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
        CapSense_1_backup.scanspeed = CapSense_1_SCANSPEED_PERIOD_REG;
    #endif  /* End (CY_PSOC3_ES2 || CY_PSOC5_ES1) */

    /* Set CONTROL_REG */
    CapSense_1_backup.ctrlreg = CapSense_1_CONTROL_REG;

    /* Clear all sensors */
    CapSense_1_ClearSensors();
    
    /* The pins disable is customer concern: Cmod and Rb */
}


/*******************************************************************************
* Function Name: CapSense_1_Sleep
********************************************************************************
*
* Summary:
*  Disables Active mode power template bits for number of component used within 
*  CapSense. Calls CapSense_1_SaveConfig() function to save customer 
*  configuration of CapSense none-retention registers and resets all sensors 
*  to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  CapSense_1_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
* Reentrant:
*  No
*
*******************************************************************************/
void CapSense_1_Sleep(void)
{
    /* Check and save enable state */
    if(CapSense_1_IS_CAPSENSE_ENABLE(CapSense_1_CONTROL_REG))
    {
        CapSense_1_backup.enableState = 1u;
        CapSense_1_Stop();
    }
    else
    {
        CapSense_1_backup.enableState = 0u;
    }
    
    CapSense_1_SaveConfig();
}


/*******************************************************************************
* Function Name: CapSense_1_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores CapSense configuration and non-retention register values.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Must be called only after CapSense_1_SaveConfig() routine. Otherwise 
*  the component configuration will be overwritten with its initial setting.  
*
* Global Variables:
*  CapSense_1_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void CapSense_1_RestoreConfig(void) 
{   
    #if ( ((CapSense_1_PRS_OPTIONS) || \
           (CapSense_1_IMPLEMENTATION_CH0 == CapSense_1_MEASURE_IMPLEMENTATION_UDB) || \
           ( (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN) && \
             (CapSense_1_IMPLEMENTATION_CH1 == CapSense_1_MEASURE_IMPLEMENTATION_UDB))) && \
          (CY_PSOC3_ES2 || CY_PSOC5_ES1) )
        
        uint8 enableInterrupts;
    #endif /* End ( ((CapSense_1_PRS_OPTIONS) || \
                     (CapSense_1_IMPLEMENTATION_CH0 == CapSense_1_MEASURE_IMPLEMENTATION_UDB) || \
                     ( (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN) && \
                       (CapSense_1_IMPLEMENTATION_CH1 == CapSense_1_MEASURE_IMPLEMENTATION_UDB))) && \
                    (CY_PSOC3_ES2 || CY_PSOC5_ES1) ) */
    
    /* Set PRS */
    #if (CapSense_1_PRS_OPTIONS == CapSense_1_PRS_8BITS)
        #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
            /* Aux control set FIFO as REG */ 
            enableInterrupts = CyEnterCriticalSection();
            CapSense_1_AUX_CONTROL_A_REG |= CapSense_1_AUXCTRL_FIFO_SINGLE_REG;
            CyExitCriticalSection(enableInterrupts);
            
            /* Write polynomial */
            CapSense_1_POLYNOM_REG   = CapSense_1_PRS8_DEFAULT_POLYNOM;             /* D0 register */
        #endif  /* End (CY_PSOC3_ES2 || CY_PSOC5_ES1) */
        
        /* Write FIFO with seed */
        CapSense_1_SEED_COPY_REG = CapSense_1_MEASURE_FULL_RANGE_LOW;               /* F0 register */
    
    #elif (CapSense_1_PRS_OPTIONS == CapSense_1_PRS_16BITS)
        #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
            /* Aux control set FIFO as REG */
            enableInterrupts = CyEnterCriticalSection();
            CapSense_1_AUX_CONTROL_A_REG |= CapSense_1_AUXCTRL_FIFO_SINGLE_REG;
            CapSense_1_AUX_CONTROL_B_REG |= CapSense_1_AUXCTRL_FIFO_SINGLE_REG;
            CyExitCriticalSection(enableInterrupts);
            
            /* Write polynomial */
            CY_SET_REG16(CapSense_1_POLYNOM_PTR, CapSense_1_PRS16_DEFAULT_POLYNOM); /* D0 register */
        #endif  /* End (CY_PSOC3_ES2 || CY_PSOC5_ES1) */
        
        /* Write FIFO with seed */
        CY_SET_REG16(CapSense_1_SEED_COPY_PTR, CapSense_1_MEASURE_FULL_RANGE);      /* F0 register */
                
    #elif (CapSense_1_PRS_OPTIONS == CapSense_1_PRS_16BITS_4X)
        #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
            /* Aux control set FIFO as REG */
            enableInterrupts = CyEnterCriticalSection();
            CapSense_1_AUX_CONTROL_A_REG  |= CapSense_1_AUXCTRL_FIFO_SINGLE_REG;
            CyExitCriticalSection(enableInterrupts);
            
            /* Write polynomial */
            CapSense_1_POLYNOM_A__D1_REG   = HI8(CapSense_1_PRS16_DEFAULT_POLYNOM); /* D0 register */
            CapSense_1_POLYNOM_A__D0_REG   = LO8(CapSense_1_PRS16_DEFAULT_POLYNOM); /* D1 register */
        #endif  /* End (CY_PSOC3_ES2 || CY_PSOC5_ES1) */
        
        /* Write FIFO with seed */
        CapSense_1_SEED_COPY_A__F1_REG = CapSense_1_MEASURE_FULL_RANGE_LOW;         /* F0 register */
        CapSense_1_SEED_COPY_A__F0_REG =CapSense_1_MEASURE_FULL_RANGE_LOW;          /* F1 register */
        
    #else
        /* Do nothing = config without PRS */
    #endif  /* End (CapSense_1_PRS_OPTIONS == CapSense_1_PRS_8BITS) */
    
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
        /* Set ScanSpeed */
        CapSense_1_SCANSPEED_PERIOD_REG = CapSense_1_backup.scanspeed;       /* Counter7_PERIOD register */
    #endif  /* End (CY_PSOC3_ES2 || CY_PSOC5_ES1) */
    
    /* Set the Measure */
    #if (CapSense_1_IMPLEMENTATION_CH0 == CapSense_1_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM  - FF Timer register are retention */
        /* Raw Counter - FF Timer register are retention */
    #else
        /* Window PWM and Raw Counter AUX and D0 set */ 
        #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
            enableInterrupts = CyEnterCriticalSection();
            CapSense_1_PWM_CH0_AUX_CONTROL_REG |= CapSense_1_AUXCTRL_FIFO_SINGLE_REG;   /* AUX register */
            CapSense_1_RAW_CH0_AUX_CONTROL_REG |= CapSense_1_AUXCTRL_FIFO_SINGLE_REG;   /* AUX register */
            CyExitCriticalSection(enableInterrupts);
            
            CapSense_1_PWM_CH0_ADD_VALUE_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;    /* D0 register */
            CapSense_1_RAW_CH0_ADD_VALUE_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;    /* D0 register */
            
        #endif  /* End (CY_PSOC3_ES2 || CY_PSOC5_ES1) */
        
        /* Window PWM */
        CapSense_1_PWM_CH0_PERIOD_LO_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;        /* F0 register */
        
        /* Raw Counter */
        CapSense_1_RAW_CH0_PERIOD_HI_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;        /* F1 register */
        CapSense_1_RAW_CH0_PERIOD_LO_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;        /* F0 register */
    
    #endif  /* (CapSense_1_IMPLEMENTATION_CH0 == CapSense_1_MEASURE_IMPLEMENTATION_FF) */ 
    
    #if (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN)
        #if (CapSense_1_IMPLEMENTATION_CH1 == CapSense_1_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM  - FF Timer register are retention */
            /* Raw Counter - FF Timer register are retention */
        #else
            /* Window PWM and Raw Counter AUX and D0 set */ 
            #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
                enableInterrupts = CyEnterCriticalSection();
                CapSense_1_PWM_CH1_AUX_CONTROL_REG |= CapSense_1_AUXCTRL_FIFO_SINGLE_REG; /* AUX register */
                CapSense_1_RAW_CH1_AUX_CONTROL_REG |= CapSense_1_AUXCTRL_FIFO_SINGLE_REG; /* AUX register */
                CyExitCriticalSection(enableInterrupts);
                
                CapSense_1_RAW_CH1_ADD_VALUE_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;   /* D0 register */
                CapSense_1_PWM_CH1_ADD_VALUE_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;   /* D0 register */
            #endif  /* End (CY_PSOC3_ES2 || CY_PSOC5_ES1) */
            
            /* Window PWM */
            CapSense_1_PWM_CH1_PERIOD_LO_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;       /* F0 register */
            
            /* Raw Counter */
            CapSense_1_RAW_CH1_PERIOD_HI_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;       /* F1 register */
            CapSense_1_RAW_CH1_PERIOD_LO_REG    = CapSense_1_MEASURE_FULL_RANGE_LOW;       /* F0 register */
            
        #endif  /* End (CapSense_1_IMPLEMENTATION_CH1 == CapSense_1_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* End (CapSense_1_DESIGN_TYPE == TWO_CHANNELS_DESIGN)*/

    /* Set CONTROL_REG */
    CapSense_1_CONTROL_REG = CapSense_1_backup.ctrlreg;

    /* Enable window generation */
    CapSense_1_CONTROL_REG |= CapSense_1_CTRL_WINDOW_EN__CH0;
    #if (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN) 
        CapSense_1_CONTROL_REG |= CapSense_1_CTRL_WINDOW_EN__CH1; 
    #endif  /* End CapSense_1_DESIGN_TYPE */
 
    /* The pins enable are customer concern: Cmod and Rb */
 }
 

/*******************************************************************************
* Function Name: CapSense_1_Wakeup
********************************************************************************
*
* Summary:
*  Restores CapSense configuration and non-retention register values. 
*  Restores enabled state of component by setting Active mode power template 
*  bits for number of component used within CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  CapSense_1_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void CapSense_1_Wakeup(void) 
{
    CapSense_1_RestoreConfig();
    
    /* Restore CapSense Enable state */
    if (CapSense_1_backup.enableState != 0u)
    {
        CapSense_1_Enable();
    }
}


/* [] END OF FILE */
