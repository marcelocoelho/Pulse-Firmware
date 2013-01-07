/*******************************************************************************
* File Name: PrISM_10_PM.c
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

#include "PrISM_10.h"


/***************************************
* Local data allocation
***************************************/
static PrISM_10_BACKUP_STRUCT  PrISM_10_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_10_PULSE_TYPE_HARDCODED)
       (((PrISM_10_GREATERTHAN_OR_EQUAL == PrISM_10_COMPARE0) ? \
                                                PrISM_10_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PrISM_10_GREATERTHAN_OR_EQUAL == PrISM_10_COMPARE1) ? \
                                                PrISM_10_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PrISM_10_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_10_SEED,
    /* seed_copy */    
    PrISM_10_SEED,
    /* polynom */
    PrISM_10_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_10_DENSITY0,
        /* density1 */
        PrISM_10_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_10_SaveConfig
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
*  PrISM_10_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_10_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_10_PULSE_TYPE_HARDCODED)
            PrISM_10_backup.cr = PrISM_10_CONTROL_REG;
        #endif /* End PrISM_10_PULSE_TYPE_HARDCODED */
        PrISM_10_backup.seed = PrISM_10_ReadSeed();
        PrISM_10_backup.seed_copy = CY_GET_REG8(PrISM_10_SEED_COPY_PTR);
        PrISM_10_backup.polynom = PrISM_10_ReadPolynomial();
        PrISM_10_backup.density0 = PrISM_10_ReadPulse0();
        PrISM_10_backup.density1 = PrISM_10_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_10_PULSE_TYPE_HARDCODED)
            PrISM_10_backup.cr = PrISM_10_CONTROL_REG;
        #endif /* End PrISM_10_PULSE_TYPE_HARDCODED */
        PrISM_10_backup.seed = PrISM_10_ReadSeed();
        PrISM_10_backup.seed_copy = CY_GET_REG8(PrISM_10_SEED_COPY_PTR);
        PrISM_10_backup.polynom = PrISM_10_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_10_RestoreConfig
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
*  PrISM_10_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_10_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_10_PULSE_TYPE_HARDCODED)
            PrISM_10_CONTROL_REG = PrISM_10_backup.cr;
        #endif /* End PrISM_10_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_10_SEED_COPY_PTR, PrISM_10_backup.seed_copy);
        CY_SET_REG8(PrISM_10_SEED_PTR, PrISM_10_backup.seed);
        PrISM_10_WritePolynomial(PrISM_10_backup.polynom);
        PrISM_10_WritePulse0(PrISM_10_backup.density0);
        PrISM_10_WritePulse1(PrISM_10_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PrISM_10_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PrISM_10_AUX_CONTROL_PTR) | PrISM_10_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PrISM_10_PULSE_TYPE_HARDCODED)
            PrISM_10_CONTROL_REG = PrISM_10_backup.cr;
        #endif /* End PrISM_10_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_10_SEED_COPY_PTR, PrISM_10_backup.seed_copy);
        CY_SET_REG8(PrISM_10_SEED_PTR, PrISM_10_backup.seed);
        PrISM_10_WritePolynomial(PrISM_10_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_10_Sleep
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
*  PrISM_10_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_10_Sleep(void) 
{
    #if(!PrISM_10_PULSE_TYPE_HARDCODED)
        if((PrISM_10_CONTROL_REG & PrISM_10_CTRL_ENABLE) != 0u) 
        {
            PrISM_10_backup.enableState = 1u;
        }
        else
        {
            PrISM_10_backup.enableState = 0u;
        }
    #endif /* End PrISM_10_PULSE_TYPE_HARDCODED */
    PrISM_10_Stop();
    PrISM_10_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_10_Wakeup
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
*  PrISM_10_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_10_Wakeup(void) 
{
    PrISM_10_RestoreConfig();
    if(PrISM_10_backup.enableState != 0u)
    {
        PrISM_10_Enable();
    } 
}


/* [] END OF FILE */
