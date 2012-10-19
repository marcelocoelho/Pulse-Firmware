/*******************************************************************************
* File Name: PrISM_14_PM.c
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

#include "PrISM_14.h"


/***************************************
* Forward function references
***************************************/
void PrISM_14_Enable(void) ;


/***************************************
* Local data allocation
***************************************/
static PrISM_14_BACKUP_STRUCT  PrISM_14_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_14_PULSE_TYPE_HARDCODED)
        (PrISM_14_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_14_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0) |
        (PrISM_14_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_14_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0),
    #endif /* End PrISM_14_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_14_SEED,
    /* seed_copy */    
    PrISM_14_SEED,
    /* polynom */
    PrISM_14_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_14_DENSITY0,
        /* density1 */
        PrISM_14_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_14_SaveConfig
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
*  PrISM_14_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_14_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_14_PULSE_TYPE_HARDCODED)
            PrISM_14_backup.cr = PrISM_14_CONTROL_REG;
        #endif /* End PrISM_14_PULSE_TYPE_HARDCODED */
        PrISM_14_backup.seed = PrISM_14_ReadSeed();
        PrISM_14_backup.seed_copy = CY_GET_REG8(PrISM_14_SEED_COPY_PTR);
        PrISM_14_backup.polynom = PrISM_14_ReadPolynomial();
        PrISM_14_backup.density0 = PrISM_14_ReadPulse0();
        PrISM_14_backup.density1 = PrISM_14_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_14_PULSE_TYPE_HARDCODED)
            PrISM_14_backup.cr = PrISM_14_CONTROL_REG;
        #endif /* End PrISM_14_PULSE_TYPE_HARDCODED */
        PrISM_14_backup.seed = PrISM_14_ReadSeed();
        PrISM_14_backup.seed_copy = CY_GET_REG8(PrISM_14_SEED_COPY_PTR);
        PrISM_14_backup.polynom = PrISM_14_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_14_RestoreConfig
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
*  PrISM_14_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_14_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_14_PULSE_TYPE_HARDCODED)
            PrISM_14_CONTROL_REG = PrISM_14_backup.cr;
        #endif /* End PrISM_14_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_14_SEED_COPY_PTR, PrISM_14_backup.seed_copy);
        CY_SET_REG8(PrISM_14_SEED_PTR, PrISM_14_backup.seed);
        PrISM_14_WritePolynomial(PrISM_14_backup.polynom);
        PrISM_14_WritePulse0(PrISM_14_backup.density0);
        PrISM_14_WritePulse1(PrISM_14_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        #if (PrISM_14_RESOLUTION <= 8u)      /* 8bit - PrISM */
            PrISM_14_AUX_CONTROL_REG |= PrISM_14_FIFO0_CLR;
        #elif (PrISM_14_RESOLUTION <= 16u)   /* 16bit - PrISM */
            CY_SET_REG16(PrISM_14_AUX_CONTROL_PTR, CY_GET_REG16(PrISM_14_AUX_CONTROL_PTR) | 
                                            PrISM_14_FIFO0_CLR | PrISM_14_FIFO0_CLR << 8u);
        #elif (PrISM_14_RESOLUTION <= 24)   /* 24bit - PrISM */
            CY_SET_REG24(PrISM_14_AUX_CONTROL_PTR, CY_GET_REG24(PrISM_14_AUX_CONTROL_PTR) |
                                            PrISM_14_FIFO0_CLR | PrISM_14_FIFO0_CLR << 8u );
            CY_SET_REG24(PrISM_14_AUX_CONTROL2_PTR, CY_GET_REG24(PrISM_14_AUX_CONTROL2_PTR) | 
                                            PrISM_14_FIFO0_CLR );
        #else                                 /* 32bit - PrISM */
            CY_SET_REG32(PrISM_14_AUX_CONTROL_PTR, CY_GET_REG32(PrISM_14_AUX_CONTROL_PTR) |
                                            PrISM_14_FIFO0_CLR | PrISM_14_FIFO0_CLR << 8u );
            CY_SET_REG32(PrISM_14_AUX_CONTROL2_PTR, CY_GET_REG32(PrISM_14_AUX_CONTROL2_PTR) |
                                            PrISM_14_FIFO0_CLR | PrISM_14_FIFO0_CLR << 8u );
        #endif                                /* End PrISM_14_RESOLUTION */
        CyExitCriticalSection(enableInterrupts);
   
    #else   /* CY_UDB_V1 */

        #if(!PrISM_14_PULSE_TYPE_HARDCODED)
            PrISM_14_CONTROL_REG = PrISM_14_backup.cr;
        #endif /* End PrISM_14_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_14_SEED_COPY_PTR, PrISM_14_backup.seed_copy);
        CY_SET_REG8(PrISM_14_SEED_PTR, PrISM_14_backup.seed);
        PrISM_14_WritePolynomial(PrISM_14_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_14_Sleep
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
*  PrISM_14_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_14_Sleep(void) 
{
    #if(!PrISM_14_PULSE_TYPE_HARDCODED)
        if((PrISM_14_CONTROL_REG & PrISM_14_CTRL_ENABLE) != 0u) 
        {
            PrISM_14_backup.enableState = 1u;
        }
        else
        {
            PrISM_14_backup.enableState = 0u;
        }
    #endif /* End PrISM_14_PULSE_TYPE_HARDCODED */
    PrISM_14_Stop();
    PrISM_14_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_14_Wakeup
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
*  PrISM_14_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_14_Wakeup(void) 
{
    PrISM_14_RestoreConfig();
    if(PrISM_14_backup.enableState != 0u)
    {
        PrISM_14_Enable();
    } 
}


/* [] END OF FILE */
