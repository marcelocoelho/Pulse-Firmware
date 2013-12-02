/*******************************************************************************
* File Name: PrISM_5_PM.c
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

#include "PrISM_5.h"


/***************************************
* Local data allocation
***************************************/
static PrISM_5_BACKUP_STRUCT  PrISM_5_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_5_PULSE_TYPE_HARDCODED)
       (((PrISM_5_GREATERTHAN_OR_EQUAL == PrISM_5_COMPARE0) ? \
                                                PrISM_5_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PrISM_5_GREATERTHAN_OR_EQUAL == PrISM_5_COMPARE1) ? \
                                                PrISM_5_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PrISM_5_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_5_SEED,
    /* seed_copy */    
    PrISM_5_SEED,
    /* polynom */
    PrISM_5_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_5_DENSITY0,
        /* density1 */
        PrISM_5_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_5_SaveConfig
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
*  PrISM_5_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_5_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_5_PULSE_TYPE_HARDCODED)
            PrISM_5_backup.cr = PrISM_5_CONTROL_REG;
        #endif /* End PrISM_5_PULSE_TYPE_HARDCODED */
        PrISM_5_backup.seed = PrISM_5_ReadSeed();
        PrISM_5_backup.seed_copy = CY_GET_REG8(PrISM_5_SEED_COPY_PTR);
        PrISM_5_backup.polynom = PrISM_5_ReadPolynomial();
        PrISM_5_backup.density0 = PrISM_5_ReadPulse0();
        PrISM_5_backup.density1 = PrISM_5_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_5_PULSE_TYPE_HARDCODED)
            PrISM_5_backup.cr = PrISM_5_CONTROL_REG;
        #endif /* End PrISM_5_PULSE_TYPE_HARDCODED */
        PrISM_5_backup.seed = PrISM_5_ReadSeed();
        PrISM_5_backup.seed_copy = CY_GET_REG8(PrISM_5_SEED_COPY_PTR);
        PrISM_5_backup.polynom = PrISM_5_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_5_RestoreConfig
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
*  PrISM_5_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_5_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_5_PULSE_TYPE_HARDCODED)
            PrISM_5_CONTROL_REG = PrISM_5_backup.cr;
        #endif /* End PrISM_5_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_5_SEED_COPY_PTR, PrISM_5_backup.seed_copy);
        CY_SET_REG8(PrISM_5_SEED_PTR, PrISM_5_backup.seed);
        PrISM_5_WritePolynomial(PrISM_5_backup.polynom);
        PrISM_5_WritePulse0(PrISM_5_backup.density0);
        PrISM_5_WritePulse1(PrISM_5_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PrISM_5_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PrISM_5_AUX_CONTROL_PTR) | PrISM_5_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PrISM_5_PULSE_TYPE_HARDCODED)
            PrISM_5_CONTROL_REG = PrISM_5_backup.cr;
        #endif /* End PrISM_5_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_5_SEED_COPY_PTR, PrISM_5_backup.seed_copy);
        CY_SET_REG8(PrISM_5_SEED_PTR, PrISM_5_backup.seed);
        PrISM_5_WritePolynomial(PrISM_5_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_5_Sleep
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
*  PrISM_5_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_5_Sleep(void) 
{
    #if(!PrISM_5_PULSE_TYPE_HARDCODED)
        if((PrISM_5_CONTROL_REG & PrISM_5_CTRL_ENABLE) != 0u) 
        {
            PrISM_5_backup.enableState = 1u;
        }
        else
        {
            PrISM_5_backup.enableState = 0u;
        }
    #endif /* End PrISM_5_PULSE_TYPE_HARDCODED */
    PrISM_5_Stop();
    PrISM_5_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_5_Wakeup
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
*  PrISM_5_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_5_Wakeup(void) 
{
    PrISM_5_RestoreConfig();
    if(PrISM_5_backup.enableState != 0u)
    {
        PrISM_5_Enable();
    } 
}


/* [] END OF FILE */
