/*******************************************************************************
* File Name: PrISM_LEDWarm_PM.c
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

#include "PrISM_LEDWarm.h"


/***************************************
* Local data allocation
***************************************/
static PrISM_LEDWarm_BACKUP_STRUCT  PrISM_LEDWarm_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_LEDWarm_PULSE_TYPE_HARDCODED)
       (((PrISM_LEDWarm_GREATERTHAN_OR_EQUAL == PrISM_LEDWarm_COMPARE0) ? \
                                                PrISM_LEDWarm_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0u) |
        ((PrISM_LEDWarm_GREATERTHAN_OR_EQUAL == PrISM_LEDWarm_COMPARE1) ? \
                                                PrISM_LEDWarm_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0u)),
    #endif /* End PrISM_LEDWarm_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_LEDWarm_SEED,
    /* seed_copy */    
    PrISM_LEDWarm_SEED,
    /* polynom */
    PrISM_LEDWarm_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_LEDWarm_DENSITY0,
        /* density1 */
        PrISM_LEDWarm_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_LEDWarm_SaveConfig
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
*  PrISM_LEDWarm_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_LEDWarm_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_LEDWarm_PULSE_TYPE_HARDCODED)
            PrISM_LEDWarm_backup.cr = PrISM_LEDWarm_CONTROL_REG;
        #endif /* End PrISM_LEDWarm_PULSE_TYPE_HARDCODED */
        PrISM_LEDWarm_backup.seed = PrISM_LEDWarm_ReadSeed();
        PrISM_LEDWarm_backup.seed_copy = CY_GET_REG8(PrISM_LEDWarm_SEED_COPY_PTR);
        PrISM_LEDWarm_backup.polynom = PrISM_LEDWarm_ReadPolynomial();
        PrISM_LEDWarm_backup.density0 = PrISM_LEDWarm_ReadPulse0();
        PrISM_LEDWarm_backup.density1 = PrISM_LEDWarm_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_LEDWarm_PULSE_TYPE_HARDCODED)
            PrISM_LEDWarm_backup.cr = PrISM_LEDWarm_CONTROL_REG;
        #endif /* End PrISM_LEDWarm_PULSE_TYPE_HARDCODED */
        PrISM_LEDWarm_backup.seed = PrISM_LEDWarm_ReadSeed();
        PrISM_LEDWarm_backup.seed_copy = CY_GET_REG8(PrISM_LEDWarm_SEED_COPY_PTR);
        PrISM_LEDWarm_backup.polynom = PrISM_LEDWarm_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_LEDWarm_RestoreConfig
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
*  PrISM_LEDWarm_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_LEDWarm_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_LEDWarm_PULSE_TYPE_HARDCODED)
            PrISM_LEDWarm_CONTROL_REG = PrISM_LEDWarm_backup.cr;
        #endif /* End PrISM_LEDWarm_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_LEDWarm_SEED_COPY_PTR, PrISM_LEDWarm_backup.seed_copy);
        CY_SET_REG8(PrISM_LEDWarm_SEED_PTR, PrISM_LEDWarm_backup.seed);
        PrISM_LEDWarm_WritePolynomial(PrISM_LEDWarm_backup.polynom);
        PrISM_LEDWarm_WritePulse0(PrISM_LEDWarm_backup.density0);
        PrISM_LEDWarm_WritePulse1(PrISM_LEDWarm_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        CY_SET_REG8(PrISM_LEDWarm_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PrISM_LEDWarm_AUX_CONTROL_PTR) | PrISM_LEDWarm_FIFO0_CLR);
        CyExitCriticalSection(enableInterrupts);

    #else   /* CY_UDB_V1 */

        #if(!PrISM_LEDWarm_PULSE_TYPE_HARDCODED)
            PrISM_LEDWarm_CONTROL_REG = PrISM_LEDWarm_backup.cr;
        #endif /* End PrISM_LEDWarm_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_LEDWarm_SEED_COPY_PTR, PrISM_LEDWarm_backup.seed_copy);
        CY_SET_REG8(PrISM_LEDWarm_SEED_PTR, PrISM_LEDWarm_backup.seed);
        PrISM_LEDWarm_WritePolynomial(PrISM_LEDWarm_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_LEDWarm_Sleep
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
*  PrISM_LEDWarm_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_LEDWarm_Sleep(void) 
{
    #if(!PrISM_LEDWarm_PULSE_TYPE_HARDCODED)
        if((PrISM_LEDWarm_CONTROL_REG & PrISM_LEDWarm_CTRL_ENABLE) != 0u) 
        {
            PrISM_LEDWarm_backup.enableState = 1u;
        }
        else
        {
            PrISM_LEDWarm_backup.enableState = 0u;
        }
    #endif /* End PrISM_LEDWarm_PULSE_TYPE_HARDCODED */
    PrISM_LEDWarm_Stop();
    PrISM_LEDWarm_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_LEDWarm_Wakeup
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
*  PrISM_LEDWarm_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_LEDWarm_Wakeup(void) 
{
    PrISM_LEDWarm_RestoreConfig();
    if(PrISM_LEDWarm_backup.enableState != 0u)
    {
        PrISM_LEDWarm_Enable();
    } 
}


/* [] END OF FILE */
