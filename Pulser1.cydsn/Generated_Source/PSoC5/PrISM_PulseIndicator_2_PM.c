/*******************************************************************************
* File Name: PrISM_PulseIndicator_2.c
* Version 2.0
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality of the PrISM component
*
* Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "PrISM_PulseIndicator_2.h"


/***************************************
* Forward function references
***************************************/
void PrISM_PulseIndicator_2_Enable(void);


/***************************************
* Local data allocation
***************************************/
static PrISM_PulseIndicator_2_BACKUP_STRUCT  PrISM_PulseIndicator_2_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED)
        (PrISM_PulseIndicator_2_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_PulseIndicator_2_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0) |
        (PrISM_PulseIndicator_2_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_PulseIndicator_2_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0),
    #endif /* End PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_PulseIndicator_2_SEED,
    /* seed_copy */    
    PrISM_PulseIndicator_2_SEED,
    /* polynom */
    PrISM_PulseIndicator_2_POLYNOM,
    #if(PrISM_PulseIndicator_2_PSOC3_ES2 || PrISM_PulseIndicator_2_PSOC5_ES1) /* PSoC3 ES2 or early, PSoC5 ES1*/
        /* density0 */
        PrISM_PulseIndicator_2_DENSITY0,
        /* density1 */
        PrISM_PulseIndicator_2_DENSITY1,
    #endif /*End PrISM_PulseIndicator_2_PSOC3_ES2 || PrISM_PulseIndicator_2_PSOC5_ES1*/
};


/*******************************************************************************
* Function Name: PrISM_PulseIndicator_2_SaveConfig
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
* Global Variables:
*  PrISM_PulseIndicator_2_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_PulseIndicator_2_SaveConfig(void)
{
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if (PrISM_PulseIndicator_2_PSOC3_ES2 || PrISM_PulseIndicator_2_PSOC5_ES1)

        #if(!PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED)
            PrISM_PulseIndicator_2_backup.cr = PrISM_PulseIndicator_2_CONTROL_REG;
        #endif /* End PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED */
        PrISM_PulseIndicator_2_backup.seed = PrISM_PulseIndicator_2_ReadSeed();
        PrISM_PulseIndicator_2_backup.seed_copy = CY_GET_REG8(PrISM_PulseIndicator_2_SEED_COPY_PTR);
        PrISM_PulseIndicator_2_backup.polynom = PrISM_PulseIndicator_2_ReadPolynomial();
        PrISM_PulseIndicator_2_backup.density0 = PrISM_PulseIndicator_2_ReadPulse0();
        PrISM_PulseIndicator_2_backup.density1 = PrISM_PulseIndicator_2_ReadPulse1();
    
    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (PrISM_PulseIndicator_2_PSOC3_ES3 || PrISM_PulseIndicator_2_PSOC5_ES2)

        #if(!PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED)
            PrISM_PulseIndicator_2_backup.cr = PrISM_PulseIndicator_2_CONTROL_REG;
        #endif /* End PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED */
        PrISM_PulseIndicator_2_backup.seed = PrISM_PulseIndicator_2_ReadSeed();
        PrISM_PulseIndicator_2_backup.seed_copy = CY_GET_REG8(PrISM_PulseIndicator_2_SEED_COPY_PTR);
        PrISM_PulseIndicator_2_backup.polynom = PrISM_PulseIndicator_2_ReadPolynomial();
    
    #endif  /* Unknown chip die is not taken into account */
}


/*******************************************************************************
* Function Name: PrISM_PulseIndicator_2_RestoreConfig
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
* Global Variables:
*  PrISM_PulseIndicator_2_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_PulseIndicator_2_RestoreConfig(void) 
{
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if (PrISM_PulseIndicator_2_PSOC3_ES2 || PrISM_PulseIndicator_2_PSOC5_ES1)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED)
            PrISM_PulseIndicator_2_CONTROL_REG = PrISM_PulseIndicator_2_backup.cr;
        #endif /* End PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_PulseIndicator_2_SEED_COPY_PTR, PrISM_PulseIndicator_2_backup.seed_copy);
        CY_SET_REG8(PrISM_PulseIndicator_2_SEED_PTR, PrISM_PulseIndicator_2_backup.seed);
        PrISM_PulseIndicator_2_WritePolynomial(PrISM_PulseIndicator_2_backup.polynom);
        PrISM_PulseIndicator_2_WritePulse0(PrISM_PulseIndicator_2_backup.density0);
        PrISM_PulseIndicator_2_WritePulse1(PrISM_PulseIndicator_2_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        #if (PrISM_PulseIndicator_2_RESOLUTION <= 8u)      /* 8bit - PrISM */
            PrISM_PulseIndicator_2_AUX_CONTROL_REG |= PrISM_PulseIndicator_2_FIFO0_CLR;
        #elif (PrISM_PulseIndicator_2_RESOLUTION <= 16u)   /* 16bit - PrISM */
            CY_SET_REG16(PrISM_PulseIndicator_2_AUX_CONTROL_PTR, CY_GET_REG16(PrISM_PulseIndicator_2_AUX_CONTROL_PTR) | 
                                            PrISM_PulseIndicator_2_FIFO0_CLR | PrISM_PulseIndicator_2_FIFO0_CLR << 8u);
        #elif (PrISM_PulseIndicator_2_RESOLUTION <= 24)   /* 24bit - PrISM */
            CY_SET_REG24(PrISM_PulseIndicator_2_AUX_CONTROL_PTR, CY_GET_REG24(PrISM_PulseIndicator_2_AUX_CONTROL_PTR) |
                                            PrISM_PulseIndicator_2_FIFO0_CLR | PrISM_PulseIndicator_2_FIFO0_CLR << 8u );
            CY_SET_REG24(PrISM_PulseIndicator_2_AUX_CONTROL2_PTR, CY_GET_REG24(PrISM_PulseIndicator_2_AUX_CONTROL2_PTR) | 
                                            PrISM_PulseIndicator_2_FIFO0_CLR );
        #else                                 /* 32bit - PrISM */
            CY_SET_REG32(PrISM_PulseIndicator_2_AUX_CONTROL_PTR, CY_GET_REG32(PrISM_PulseIndicator_2_AUX_CONTROL_PTR) |
                                            PrISM_PulseIndicator_2_FIFO0_CLR | PrISM_PulseIndicator_2_FIFO0_CLR << 8u );
            CY_SET_REG32(PrISM_PulseIndicator_2_AUX_CONTROL2_PTR, CY_GET_REG32(PrISM_PulseIndicator_2_AUX_CONTROL2_PTR) |
                                            PrISM_PulseIndicator_2_FIFO0_CLR | PrISM_PulseIndicator_2_FIFO0_CLR << 8u );
        #endif                                /* End PrISM_PulseIndicator_2_RESOLUTION */
        CyExitCriticalSection(enableInterrupts);
   
    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (PrISM_PulseIndicator_2_PSOC3_ES3 || PrISM_PulseIndicator_2_PSOC5_ES2)

        #if(!PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED)
            PrISM_PulseIndicator_2_CONTROL_REG = PrISM_PulseIndicator_2_backup.cr;
        #endif /* End PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_PulseIndicator_2_SEED_COPY_PTR, PrISM_PulseIndicator_2_backup.seed_copy);
        CY_SET_REG8(PrISM_PulseIndicator_2_SEED_PTR, PrISM_PulseIndicator_2_backup.seed);
        PrISM_PulseIndicator_2_WritePolynomial(PrISM_PulseIndicator_2_backup.polynom);
    
    #endif  /* End PrISM_PulseIndicator_2_PSOC3_ES2 || PrISM_PulseIndicator_2_PSOC5_ES1 */
}


/*******************************************************************************
* Function Name: PrISM_PulseIndicator_2_Sleep
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
*  PrISM_PulseIndicator_2_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_PulseIndicator_2_Sleep(void)
{
    #if(!PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED)
        if((PrISM_PulseIndicator_2_CONTROL_REG & PrISM_PulseIndicator_2_CTRL_ENABLE) != 0u) 
        {
            PrISM_PulseIndicator_2_backup.enableState = 1u;
        }
        else
        {
            PrISM_PulseIndicator_2_backup.enableState = 0u;
        }
    #endif /* End PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED */
    PrISM_PulseIndicator_2_Stop();
    PrISM_PulseIndicator_2_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_PulseIndicator_2_Wakeup
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
*  PrISM_PulseIndicator_2_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_PulseIndicator_2_Wakeup(void) 
{
    PrISM_PulseIndicator_2_RestoreConfig();
    if(PrISM_PulseIndicator_2_backup.enableState != 0u)
    {
        PrISM_PulseIndicator_2_Enable();
    } 
}


/* [] END OF FILE */
