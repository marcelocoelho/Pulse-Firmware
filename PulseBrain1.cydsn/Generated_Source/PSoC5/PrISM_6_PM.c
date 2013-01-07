/*******************************************************************************
* File Name: PrISM_6_PM.c
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

#include "PrISM_6.h"


/***************************************
* Local data allocation
***************************************/
static PrISM_6_BACKUP_STRUCT  PrISM_6_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_6_PULSE_TYPE_HARDCODED)
       (((PrISM_6_GREATERTHAN_OR_EQUAL == PrISM_6_COMPARE0) ? \
                                                PrISM_6_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PrISM_6_GREATERTHAN_OR_EQUAL == PrISM_6_COMPARE1) ? \
                                                PrISM_6_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PrISM_6_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_6_SEED,
    /* seed_copy */    
    PrISM_6_SEED,
    /* polynom */
    PrISM_6_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_6_DENSITY0,
        /* density1 */
        PrISM_6_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_6_SaveConfig
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
*  PrISM_6_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_6_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_6_PULSE_TYPE_HARDCODED)
            PrISM_6_backup.cr = PrISM_6_CONTROL_REG;
        #endif /* End PrISM_6_PULSE_TYPE_HARDCODED */
        PrISM_6_backup.seed = PrISM_6_ReadSeed();
        PrISM_6_backup.seed_copy = CY_GET_REG8(PrISM_6_SEED_COPY_PTR);
        PrISM_6_backup.polynom = PrISM_6_ReadPolynomial();
        PrISM_6_backup.density0 = PrISM_6_ReadPulse0();
        PrISM_6_backup.density1 = PrISM_6_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_6_PULSE_TYPE_HARDCODED)
            PrISM_6_backup.cr = PrISM_6_CONTROL_REG;
        #endif /* End PrISM_6_PULSE_TYPE_HARDCODED */
        PrISM_6_backup.seed = PrISM_6_ReadSeed();
        PrISM_6_backup.seed_copy = CY_GET_REG8(PrISM_6_SEED_COPY_PTR);
        PrISM_6_backup.polynom = PrISM_6_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_6_RestoreConfig
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
*  PrISM_6_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_6_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_6_PULSE_TYPE_HARDCODED)
            PrISM_6_CONTROL_REG = PrISM_6_backup.cr;
        #endif /* End PrISM_6_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_6_SEED_COPY_PTR, PrISM_6_backup.seed_copy);
        CY_SET_REG8(PrISM_6_SEED_PTR, PrISM_6_backup.seed);
        PrISM_6_WritePolynomial(PrISM_6_backup.polynom);
        PrISM_6_WritePulse0(PrISM_6_backup.density0);
        PrISM_6_WritePulse1(PrISM_6_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PrISM_6_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PrISM_6_AUX_CONTROL_PTR) | PrISM_6_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PrISM_6_PULSE_TYPE_HARDCODED)
            PrISM_6_CONTROL_REG = PrISM_6_backup.cr;
        #endif /* End PrISM_6_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_6_SEED_COPY_PTR, PrISM_6_backup.seed_copy);
        CY_SET_REG8(PrISM_6_SEED_PTR, PrISM_6_backup.seed);
        PrISM_6_WritePolynomial(PrISM_6_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_6_Sleep
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
*  PrISM_6_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_6_Sleep(void) 
{
    #if(!PrISM_6_PULSE_TYPE_HARDCODED)
        if((PrISM_6_CONTROL_REG & PrISM_6_CTRL_ENABLE) != 0u) 
        {
            PrISM_6_backup.enableState = 1u;
        }
        else
        {
            PrISM_6_backup.enableState = 0u;
        }
    #endif /* End PrISM_6_PULSE_TYPE_HARDCODED */
    PrISM_6_Stop();
    PrISM_6_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_6_Wakeup
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
*  PrISM_6_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_6_Wakeup(void) 
{
    PrISM_6_RestoreConfig();
    if(PrISM_6_backup.enableState != 0u)
    {
        PrISM_6_Enable();
    } 
}


/* [] END OF FILE */
