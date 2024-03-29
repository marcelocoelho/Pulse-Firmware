/*******************************************************************************
* File Name: PrISM_LEDBlue_PM.c
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

#include "PrISM_LEDBlue.h"


/***************************************
* Local data allocation
***************************************/
static PrISM_LEDBlue_BACKUP_STRUCT  PrISM_LEDBlue_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_LEDBlue_PULSE_TYPE_HARDCODED)
       (((PrISM_LEDBlue_GREATERTHAN_OR_EQUAL == PrISM_LEDBlue_COMPARE0) ? \
                                                PrISM_LEDBlue_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PrISM_LEDBlue_GREATERTHAN_OR_EQUAL == PrISM_LEDBlue_COMPARE1) ? \
                                                PrISM_LEDBlue_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PrISM_LEDBlue_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_LEDBlue_SEED,
    /* seed_copy */    
    PrISM_LEDBlue_SEED,
    /* polynom */
    PrISM_LEDBlue_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_LEDBlue_DENSITY0,
        /* density1 */
        PrISM_LEDBlue_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_LEDBlue_SaveConfig
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
*  PrISM_LEDBlue_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_LEDBlue_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_LEDBlue_PULSE_TYPE_HARDCODED)
            PrISM_LEDBlue_backup.cr = PrISM_LEDBlue_CONTROL_REG;
        #endif /* End PrISM_LEDBlue_PULSE_TYPE_HARDCODED */
        PrISM_LEDBlue_backup.seed = PrISM_LEDBlue_ReadSeed();
        PrISM_LEDBlue_backup.seed_copy = CY_GET_REG8(PrISM_LEDBlue_SEED_COPY_PTR);
        PrISM_LEDBlue_backup.polynom = PrISM_LEDBlue_ReadPolynomial();
        PrISM_LEDBlue_backup.density0 = PrISM_LEDBlue_ReadPulse0();
        PrISM_LEDBlue_backup.density1 = PrISM_LEDBlue_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_LEDBlue_PULSE_TYPE_HARDCODED)
            PrISM_LEDBlue_backup.cr = PrISM_LEDBlue_CONTROL_REG;
        #endif /* End PrISM_LEDBlue_PULSE_TYPE_HARDCODED */
        PrISM_LEDBlue_backup.seed = PrISM_LEDBlue_ReadSeed();
        PrISM_LEDBlue_backup.seed_copy = CY_GET_REG8(PrISM_LEDBlue_SEED_COPY_PTR);
        PrISM_LEDBlue_backup.polynom = PrISM_LEDBlue_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_LEDBlue_RestoreConfig
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
*  PrISM_LEDBlue_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_LEDBlue_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_LEDBlue_PULSE_TYPE_HARDCODED)
            PrISM_LEDBlue_CONTROL_REG = PrISM_LEDBlue_backup.cr;
        #endif /* End PrISM_LEDBlue_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_LEDBlue_SEED_COPY_PTR, PrISM_LEDBlue_backup.seed_copy);
        CY_SET_REG8(PrISM_LEDBlue_SEED_PTR, PrISM_LEDBlue_backup.seed);
        PrISM_LEDBlue_WritePolynomial(PrISM_LEDBlue_backup.polynom);
        PrISM_LEDBlue_WritePulse0(PrISM_LEDBlue_backup.density0);
        PrISM_LEDBlue_WritePulse1(PrISM_LEDBlue_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PrISM_LEDBlue_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PrISM_LEDBlue_AUX_CONTROL_PTR) | PrISM_LEDBlue_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PrISM_LEDBlue_PULSE_TYPE_HARDCODED)
            PrISM_LEDBlue_CONTROL_REG = PrISM_LEDBlue_backup.cr;
        #endif /* End PrISM_LEDBlue_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_LEDBlue_SEED_COPY_PTR, PrISM_LEDBlue_backup.seed_copy);
        CY_SET_REG8(PrISM_LEDBlue_SEED_PTR, PrISM_LEDBlue_backup.seed);
        PrISM_LEDBlue_WritePolynomial(PrISM_LEDBlue_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_LEDBlue_Sleep
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
*  PrISM_LEDBlue_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_LEDBlue_Sleep(void) 
{
    #if(!PrISM_LEDBlue_PULSE_TYPE_HARDCODED)
        if((PrISM_LEDBlue_CONTROL_REG & PrISM_LEDBlue_CTRL_ENABLE) != 0u) 
        {
            PrISM_LEDBlue_backup.enableState = 1u;
        }
        else
        {
            PrISM_LEDBlue_backup.enableState = 0u;
        }
    #endif /* End PrISM_LEDBlue_PULSE_TYPE_HARDCODED */
    PrISM_LEDBlue_Stop();
    PrISM_LEDBlue_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_LEDBlue_Wakeup
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
*  PrISM_LEDBlue_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_LEDBlue_Wakeup(void) 
{
    PrISM_LEDBlue_RestoreConfig();
    if(PrISM_LEDBlue_backup.enableState != 0u)
    {
        PrISM_LEDBlue_Enable();
    } 
}


/* [] END OF FILE */
