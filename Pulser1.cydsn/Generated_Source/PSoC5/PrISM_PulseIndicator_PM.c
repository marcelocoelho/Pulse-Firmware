/*******************************************************************************
* File Name: PrISM_PulseIndicator_PM.c
* Version 2.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality of the PrISM component
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "PrISM_PulseIndicator.h"


/***************************************
* Forward function references
***************************************/
void PrISM_PulseIndicator_Enable(void) ;


/***************************************
* Local data allocation
***************************************/
static PrISM_PulseIndicator_BACKUP_STRUCT  PrISM_PulseIndicator_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_PulseIndicator_PULSE_TYPE_HARDCODED)
        (PrISM_PulseIndicator_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_PulseIndicator_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0) |
        (PrISM_PulseIndicator_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_PulseIndicator_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0),
    #endif /* End PrISM_PulseIndicator_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_PulseIndicator_SEED,
    /* seed_copy */    
    PrISM_PulseIndicator_SEED,
    /* polynom */
    PrISM_PulseIndicator_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_PulseIndicator_DENSITY0,
        /* density1 */
        PrISM_PulseIndicator_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_PulseIndicator_SaveConfig
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
*  PrISM_PulseIndicator_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_PulseIndicator_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_PulseIndicator_PULSE_TYPE_HARDCODED)
            PrISM_PulseIndicator_backup.cr = PrISM_PulseIndicator_CONTROL_REG;
        #endif /* End PrISM_PulseIndicator_PULSE_TYPE_HARDCODED */
        PrISM_PulseIndicator_backup.seed = PrISM_PulseIndicator_ReadSeed();
        PrISM_PulseIndicator_backup.seed_copy = CY_GET_REG8(PrISM_PulseIndicator_SEED_COPY_PTR);
        PrISM_PulseIndicator_backup.polynom = PrISM_PulseIndicator_ReadPolynomial();
        PrISM_PulseIndicator_backup.density0 = PrISM_PulseIndicator_ReadPulse0();
        PrISM_PulseIndicator_backup.density1 = PrISM_PulseIndicator_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_PulseIndicator_PULSE_TYPE_HARDCODED)
            PrISM_PulseIndicator_backup.cr = PrISM_PulseIndicator_CONTROL_REG;
        #endif /* End PrISM_PulseIndicator_PULSE_TYPE_HARDCODED */
        PrISM_PulseIndicator_backup.seed = PrISM_PulseIndicator_ReadSeed();
        PrISM_PulseIndicator_backup.seed_copy = CY_GET_REG8(PrISM_PulseIndicator_SEED_COPY_PTR);
        PrISM_PulseIndicator_backup.polynom = PrISM_PulseIndicator_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_PulseIndicator_RestoreConfig
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
*  PrISM_PulseIndicator_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_PulseIndicator_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_PulseIndicator_PULSE_TYPE_HARDCODED)
            PrISM_PulseIndicator_CONTROL_REG = PrISM_PulseIndicator_backup.cr;
        #endif /* End PrISM_PulseIndicator_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_PulseIndicator_SEED_COPY_PTR, PrISM_PulseIndicator_backup.seed_copy);
        CY_SET_REG8(PrISM_PulseIndicator_SEED_PTR, PrISM_PulseIndicator_backup.seed);
        PrISM_PulseIndicator_WritePolynomial(PrISM_PulseIndicator_backup.polynom);
        PrISM_PulseIndicator_WritePulse0(PrISM_PulseIndicator_backup.density0);
        PrISM_PulseIndicator_WritePulse1(PrISM_PulseIndicator_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        #if (PrISM_PulseIndicator_RESOLUTION <= 8u)      /* 8bit - PrISM */
            PrISM_PulseIndicator_AUX_CONTROL_REG |= PrISM_PulseIndicator_FIFO0_CLR;
        #elif (PrISM_PulseIndicator_RESOLUTION <= 16u)   /* 16bit - PrISM */
            CY_SET_REG16(PrISM_PulseIndicator_AUX_CONTROL_PTR, CY_GET_REG16(PrISM_PulseIndicator_AUX_CONTROL_PTR) | 
                                            PrISM_PulseIndicator_FIFO0_CLR | PrISM_PulseIndicator_FIFO0_CLR << 8u);
        #elif (PrISM_PulseIndicator_RESOLUTION <= 24)   /* 24bit - PrISM */
            CY_SET_REG24(PrISM_PulseIndicator_AUX_CONTROL_PTR, CY_GET_REG24(PrISM_PulseIndicator_AUX_CONTROL_PTR) |
                                            PrISM_PulseIndicator_FIFO0_CLR | PrISM_PulseIndicator_FIFO0_CLR << 8u );
            CY_SET_REG24(PrISM_PulseIndicator_AUX_CONTROL2_PTR, CY_GET_REG24(PrISM_PulseIndicator_AUX_CONTROL2_PTR) | 
                                            PrISM_PulseIndicator_FIFO0_CLR );
        #else                                 /* 32bit - PrISM */
            CY_SET_REG32(PrISM_PulseIndicator_AUX_CONTROL_PTR, CY_GET_REG32(PrISM_PulseIndicator_AUX_CONTROL_PTR) |
                                            PrISM_PulseIndicator_FIFO0_CLR | PrISM_PulseIndicator_FIFO0_CLR << 8u );
            CY_SET_REG32(PrISM_PulseIndicator_AUX_CONTROL2_PTR, CY_GET_REG32(PrISM_PulseIndicator_AUX_CONTROL2_PTR) |
                                            PrISM_PulseIndicator_FIFO0_CLR | PrISM_PulseIndicator_FIFO0_CLR << 8u );
        #endif                                /* End PrISM_PulseIndicator_RESOLUTION */
        CyExitCriticalSection(enableInterrupts);
   
    #else   /* CY_UDB_V1 */

        #if(!PrISM_PulseIndicator_PULSE_TYPE_HARDCODED)
            PrISM_PulseIndicator_CONTROL_REG = PrISM_PulseIndicator_backup.cr;
        #endif /* End PrISM_PulseIndicator_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_PulseIndicator_SEED_COPY_PTR, PrISM_PulseIndicator_backup.seed_copy);
        CY_SET_REG8(PrISM_PulseIndicator_SEED_PTR, PrISM_PulseIndicator_backup.seed);
        PrISM_PulseIndicator_WritePolynomial(PrISM_PulseIndicator_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_PulseIndicator_Sleep
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
*  PrISM_PulseIndicator_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_PulseIndicator_Sleep(void) 
{
    #if(!PrISM_PulseIndicator_PULSE_TYPE_HARDCODED)
        if((PrISM_PulseIndicator_CONTROL_REG & PrISM_PulseIndicator_CTRL_ENABLE) != 0u) 
        {
            PrISM_PulseIndicator_backup.enableState = 1u;
        }
        else
        {
            PrISM_PulseIndicator_backup.enableState = 0u;
        }
    #endif /* End PrISM_PulseIndicator_PULSE_TYPE_HARDCODED */
    PrISM_PulseIndicator_Stop();
    PrISM_PulseIndicator_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_PulseIndicator_Wakeup
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
*  PrISM_PulseIndicator_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_PulseIndicator_Wakeup(void) 
{
    PrISM_PulseIndicator_RestoreConfig();
    if(PrISM_PulseIndicator_backup.enableState != 0u)
    {
        PrISM_PulseIndicator_Enable();
    } 
}


/* [] END OF FILE */
