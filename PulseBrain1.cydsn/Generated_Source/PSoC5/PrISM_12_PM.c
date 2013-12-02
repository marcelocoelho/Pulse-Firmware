/*******************************************************************************
* File Name: PrISM_12_PM.c
* Version 2.20
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

#include "PrISM_12.h"


/***************************************
* Local data allocation
***************************************/
static PrISM_12_BACKUP_STRUCT  PrISM_12_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_12_PULSE_TYPE_HARDCODED)
       (((PrISM_12_GREATERTHAN_OR_EQUAL == PrISM_12_COMPARE0) ? \
                                                PrISM_12_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PrISM_12_GREATERTHAN_OR_EQUAL == PrISM_12_COMPARE1) ? \
                                                PrISM_12_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PrISM_12_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_12_SEED,
    /* seed_copy */    
    PrISM_12_SEED,
    /* polynom */
    PrISM_12_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_12_DENSITY0,
        /* density1 */
        PrISM_12_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_12_SaveConfig
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
*  PrISM_12_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_12_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_12_PULSE_TYPE_HARDCODED)
            PrISM_12_backup.cr = PrISM_12_CONTROL_REG;
        #endif /* End PrISM_12_PULSE_TYPE_HARDCODED */
        PrISM_12_backup.seed = PrISM_12_ReadSeed();
        PrISM_12_backup.seed_copy = CY_GET_REG8(PrISM_12_SEED_COPY_PTR);
        PrISM_12_backup.polynom = PrISM_12_ReadPolynomial();
        PrISM_12_backup.density0 = PrISM_12_ReadPulse0();
        PrISM_12_backup.density1 = PrISM_12_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_12_PULSE_TYPE_HARDCODED)
            PrISM_12_backup.cr = PrISM_12_CONTROL_REG;
        #endif /* End PrISM_12_PULSE_TYPE_HARDCODED */
        PrISM_12_backup.seed = PrISM_12_ReadSeed();
        PrISM_12_backup.seed_copy = CY_GET_REG8(PrISM_12_SEED_COPY_PTR);
        PrISM_12_backup.polynom = PrISM_12_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_12_RestoreConfig
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
*  PrISM_12_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_12_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_12_PULSE_TYPE_HARDCODED)
            PrISM_12_CONTROL_REG = PrISM_12_backup.cr;
        #endif /* End PrISM_12_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_12_SEED_COPY_PTR, PrISM_12_backup.seed_copy);
        CY_SET_REG8(PrISM_12_SEED_PTR, PrISM_12_backup.seed);
        PrISM_12_WritePolynomial(PrISM_12_backup.polynom);
        PrISM_12_WritePulse0(PrISM_12_backup.density0);
        PrISM_12_WritePulse1(PrISM_12_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PrISM_12_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PrISM_12_AUX_CONTROL_PTR) | PrISM_12_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PrISM_12_PULSE_TYPE_HARDCODED)
            PrISM_12_CONTROL_REG = PrISM_12_backup.cr;
        #endif /* End PrISM_12_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_12_SEED_COPY_PTR, PrISM_12_backup.seed_copy);
        CY_SET_REG8(PrISM_12_SEED_PTR, PrISM_12_backup.seed);
        PrISM_12_WritePolynomial(PrISM_12_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_12_Sleep
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
*  PrISM_12_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_12_Sleep(void) 
{
    #if(!PrISM_12_PULSE_TYPE_HARDCODED)
        if((PrISM_12_CONTROL_REG & PrISM_12_CTRL_ENABLE) != 0u) 
        {
            PrISM_12_backup.enableState = 1u;
        }
        else
        {
            PrISM_12_backup.enableState = 0u;
        }
    #endif /* End PrISM_12_PULSE_TYPE_HARDCODED */
    PrISM_12_Stop();
    PrISM_12_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_12_Wakeup
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
*  PrISM_12_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_12_Wakeup(void) 
{
    PrISM_12_RestoreConfig();
    if(PrISM_12_backup.enableState != 0u)
    {
        PrISM_12_Enable();
    } 
}


/* [] END OF FILE */
