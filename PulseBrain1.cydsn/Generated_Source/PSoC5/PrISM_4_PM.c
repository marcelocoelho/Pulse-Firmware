/*******************************************************************************
* File Name: PrISM_4_PM.c
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

#include "PrISM_4.h"


/***************************************
* Local data allocation
***************************************/
static PrISM_4_BACKUP_STRUCT  PrISM_4_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_4_PULSE_TYPE_HARDCODED)
       (((PrISM_4_GREATERTHAN_OR_EQUAL == PrISM_4_COMPARE0) ? \
                                                PrISM_4_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PrISM_4_GREATERTHAN_OR_EQUAL == PrISM_4_COMPARE1) ? \
                                                PrISM_4_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PrISM_4_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_4_SEED,
    /* seed_copy */    
    PrISM_4_SEED,
    /* polynom */
    PrISM_4_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_4_DENSITY0,
        /* density1 */
        PrISM_4_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_4_SaveConfig
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
*  PrISM_4_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_4_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_4_PULSE_TYPE_HARDCODED)
            PrISM_4_backup.cr = PrISM_4_CONTROL_REG;
        #endif /* End PrISM_4_PULSE_TYPE_HARDCODED */
        PrISM_4_backup.seed = PrISM_4_ReadSeed();
        PrISM_4_backup.seed_copy = CY_GET_REG8(PrISM_4_SEED_COPY_PTR);
        PrISM_4_backup.polynom = PrISM_4_ReadPolynomial();
        PrISM_4_backup.density0 = PrISM_4_ReadPulse0();
        PrISM_4_backup.density1 = PrISM_4_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_4_PULSE_TYPE_HARDCODED)
            PrISM_4_backup.cr = PrISM_4_CONTROL_REG;
        #endif /* End PrISM_4_PULSE_TYPE_HARDCODED */
        PrISM_4_backup.seed = PrISM_4_ReadSeed();
        PrISM_4_backup.seed_copy = CY_GET_REG8(PrISM_4_SEED_COPY_PTR);
        PrISM_4_backup.polynom = PrISM_4_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_4_RestoreConfig
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
*  PrISM_4_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_4_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_4_PULSE_TYPE_HARDCODED)
            PrISM_4_CONTROL_REG = PrISM_4_backup.cr;
        #endif /* End PrISM_4_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_4_SEED_COPY_PTR, PrISM_4_backup.seed_copy);
        CY_SET_REG8(PrISM_4_SEED_PTR, PrISM_4_backup.seed);
        PrISM_4_WritePolynomial(PrISM_4_backup.polynom);
        PrISM_4_WritePulse0(PrISM_4_backup.density0);
        PrISM_4_WritePulse1(PrISM_4_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PrISM_4_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PrISM_4_AUX_CONTROL_PTR) | PrISM_4_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PrISM_4_PULSE_TYPE_HARDCODED)
            PrISM_4_CONTROL_REG = PrISM_4_backup.cr;
        #endif /* End PrISM_4_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_4_SEED_COPY_PTR, PrISM_4_backup.seed_copy);
        CY_SET_REG8(PrISM_4_SEED_PTR, PrISM_4_backup.seed);
        PrISM_4_WritePolynomial(PrISM_4_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_4_Sleep
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
*  PrISM_4_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_4_Sleep(void) 
{
    #if(!PrISM_4_PULSE_TYPE_HARDCODED)
        if((PrISM_4_CONTROL_REG & PrISM_4_CTRL_ENABLE) != 0u) 
        {
            PrISM_4_backup.enableState = 1u;
        }
        else
        {
            PrISM_4_backup.enableState = 0u;
        }
    #endif /* End PrISM_4_PULSE_TYPE_HARDCODED */
    PrISM_4_Stop();
    PrISM_4_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_4_Wakeup
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
*  PrISM_4_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_4_Wakeup(void) 
{
    PrISM_4_RestoreConfig();
    if(PrISM_4_backup.enableState != 0u)
    {
        PrISM_4_Enable();
    } 
}


/* [] END OF FILE */
