/*******************************************************************************
* File Name: PrISM_8_PM.c
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

#include "PrISM_8.h"


/***************************************
* Local data allocation
***************************************/
static PrISM_8_BACKUP_STRUCT  PrISM_8_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_8_PULSE_TYPE_HARDCODED)
       (((PrISM_8_GREATERTHAN_OR_EQUAL == PrISM_8_COMPARE0) ? \
                                                PrISM_8_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PrISM_8_GREATERTHAN_OR_EQUAL == PrISM_8_COMPARE1) ? \
                                                PrISM_8_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PrISM_8_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_8_SEED,
    /* seed_copy */    
    PrISM_8_SEED,
    /* polynom */
    PrISM_8_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_8_DENSITY0,
        /* density1 */
        PrISM_8_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_8_SaveConfig
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
*  PrISM_8_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_8_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_8_PULSE_TYPE_HARDCODED)
            PrISM_8_backup.cr = PrISM_8_CONTROL_REG;
        #endif /* End PrISM_8_PULSE_TYPE_HARDCODED */
        PrISM_8_backup.seed = PrISM_8_ReadSeed();
        PrISM_8_backup.seed_copy = CY_GET_REG8(PrISM_8_SEED_COPY_PTR);
        PrISM_8_backup.polynom = PrISM_8_ReadPolynomial();
        PrISM_8_backup.density0 = PrISM_8_ReadPulse0();
        PrISM_8_backup.density1 = PrISM_8_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_8_PULSE_TYPE_HARDCODED)
            PrISM_8_backup.cr = PrISM_8_CONTROL_REG;
        #endif /* End PrISM_8_PULSE_TYPE_HARDCODED */
        PrISM_8_backup.seed = PrISM_8_ReadSeed();
        PrISM_8_backup.seed_copy = CY_GET_REG8(PrISM_8_SEED_COPY_PTR);
        PrISM_8_backup.polynom = PrISM_8_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_8_RestoreConfig
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
*  PrISM_8_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_8_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_8_PULSE_TYPE_HARDCODED)
            PrISM_8_CONTROL_REG = PrISM_8_backup.cr;
        #endif /* End PrISM_8_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_8_SEED_COPY_PTR, PrISM_8_backup.seed_copy);
        CY_SET_REG8(PrISM_8_SEED_PTR, PrISM_8_backup.seed);
        PrISM_8_WritePolynomial(PrISM_8_backup.polynom);
        PrISM_8_WritePulse0(PrISM_8_backup.density0);
        PrISM_8_WritePulse1(PrISM_8_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PrISM_8_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PrISM_8_AUX_CONTROL_PTR) | PrISM_8_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PrISM_8_PULSE_TYPE_HARDCODED)
            PrISM_8_CONTROL_REG = PrISM_8_backup.cr;
        #endif /* End PrISM_8_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_8_SEED_COPY_PTR, PrISM_8_backup.seed_copy);
        CY_SET_REG8(PrISM_8_SEED_PTR, PrISM_8_backup.seed);
        PrISM_8_WritePolynomial(PrISM_8_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_8_Sleep
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
*  PrISM_8_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_8_Sleep(void) 
{
    #if(!PrISM_8_PULSE_TYPE_HARDCODED)
        if((PrISM_8_CONTROL_REG & PrISM_8_CTRL_ENABLE) != 0u) 
        {
            PrISM_8_backup.enableState = 1u;
        }
        else
        {
            PrISM_8_backup.enableState = 0u;
        }
    #endif /* End PrISM_8_PULSE_TYPE_HARDCODED */
    PrISM_8_Stop();
    PrISM_8_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_8_Wakeup
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
*  PrISM_8_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_8_Wakeup(void) 
{
    PrISM_8_RestoreConfig();
    if(PrISM_8_backup.enableState != 0u)
    {
        PrISM_8_Enable();
    } 
}


/* [] END OF FILE */
