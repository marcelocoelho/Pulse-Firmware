/*******************************************************************************
* File Name: PrISM_3_PM.c
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

#include "PrISM_3.h"


/***************************************
* Forward function references
***************************************/
void PrISM_3_Enable(void) ;


/***************************************
* Local data allocation
***************************************/
static PrISM_3_BACKUP_STRUCT  PrISM_3_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_3_PULSE_TYPE_HARDCODED)
        (PrISM_3_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_3_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0) |
        (PrISM_3_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_3_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0),
    #endif /* End PrISM_3_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_3_SEED,
    /* seed_copy */    
    PrISM_3_SEED,
    /* polynom */
    PrISM_3_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_3_DENSITY0,
        /* density1 */
        PrISM_3_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_3_SaveConfig
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
*  PrISM_3_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_3_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_3_PULSE_TYPE_HARDCODED)
            PrISM_3_backup.cr = PrISM_3_CONTROL_REG;
        #endif /* End PrISM_3_PULSE_TYPE_HARDCODED */
        PrISM_3_backup.seed = PrISM_3_ReadSeed();
        PrISM_3_backup.seed_copy = CY_GET_REG8(PrISM_3_SEED_COPY_PTR);
        PrISM_3_backup.polynom = PrISM_3_ReadPolynomial();
        PrISM_3_backup.density0 = PrISM_3_ReadPulse0();
        PrISM_3_backup.density1 = PrISM_3_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_3_PULSE_TYPE_HARDCODED)
            PrISM_3_backup.cr = PrISM_3_CONTROL_REG;
        #endif /* End PrISM_3_PULSE_TYPE_HARDCODED */
        PrISM_3_backup.seed = PrISM_3_ReadSeed();
        PrISM_3_backup.seed_copy = CY_GET_REG8(PrISM_3_SEED_COPY_PTR);
        PrISM_3_backup.polynom = PrISM_3_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_3_RestoreConfig
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
*  PrISM_3_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_3_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_3_PULSE_TYPE_HARDCODED)
            PrISM_3_CONTROL_REG = PrISM_3_backup.cr;
        #endif /* End PrISM_3_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_3_SEED_COPY_PTR, PrISM_3_backup.seed_copy);
        CY_SET_REG8(PrISM_3_SEED_PTR, PrISM_3_backup.seed);
        PrISM_3_WritePolynomial(PrISM_3_backup.polynom);
        PrISM_3_WritePulse0(PrISM_3_backup.density0);
        PrISM_3_WritePulse1(PrISM_3_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        #if (PrISM_3_RESOLUTION <= 8u)      /* 8bit - PrISM */
            PrISM_3_AUX_CONTROL_REG |= PrISM_3_FIFO0_CLR;
        #elif (PrISM_3_RESOLUTION <= 16u)   /* 16bit - PrISM */
            CY_SET_REG16(PrISM_3_AUX_CONTROL_PTR, CY_GET_REG16(PrISM_3_AUX_CONTROL_PTR) | 
                                            PrISM_3_FIFO0_CLR | PrISM_3_FIFO0_CLR << 8u);
        #elif (PrISM_3_RESOLUTION <= 24)   /* 24bit - PrISM */
            CY_SET_REG24(PrISM_3_AUX_CONTROL_PTR, CY_GET_REG24(PrISM_3_AUX_CONTROL_PTR) |
                                            PrISM_3_FIFO0_CLR | PrISM_3_FIFO0_CLR << 8u );
            CY_SET_REG24(PrISM_3_AUX_CONTROL2_PTR, CY_GET_REG24(PrISM_3_AUX_CONTROL2_PTR) | 
                                            PrISM_3_FIFO0_CLR );
        #else                                 /* 32bit - PrISM */
            CY_SET_REG32(PrISM_3_AUX_CONTROL_PTR, CY_GET_REG32(PrISM_3_AUX_CONTROL_PTR) |
                                            PrISM_3_FIFO0_CLR | PrISM_3_FIFO0_CLR << 8u );
            CY_SET_REG32(PrISM_3_AUX_CONTROL2_PTR, CY_GET_REG32(PrISM_3_AUX_CONTROL2_PTR) |
                                            PrISM_3_FIFO0_CLR | PrISM_3_FIFO0_CLR << 8u );
        #endif                                /* End PrISM_3_RESOLUTION */
        CyExitCriticalSection(enableInterrupts);
   
    #else   /* CY_UDB_V1 */

        #if(!PrISM_3_PULSE_TYPE_HARDCODED)
            PrISM_3_CONTROL_REG = PrISM_3_backup.cr;
        #endif /* End PrISM_3_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_3_SEED_COPY_PTR, PrISM_3_backup.seed_copy);
        CY_SET_REG8(PrISM_3_SEED_PTR, PrISM_3_backup.seed);
        PrISM_3_WritePolynomial(PrISM_3_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_3_Sleep
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
*  PrISM_3_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_3_Sleep(void) 
{
    #if(!PrISM_3_PULSE_TYPE_HARDCODED)
        if((PrISM_3_CONTROL_REG & PrISM_3_CTRL_ENABLE) != 0u) 
        {
            PrISM_3_backup.enableState = 1u;
        }
        else
        {
            PrISM_3_backup.enableState = 0u;
        }
    #endif /* End PrISM_3_PULSE_TYPE_HARDCODED */
    PrISM_3_Stop();
    PrISM_3_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_3_Wakeup
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
*  PrISM_3_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_3_Wakeup(void) 
{
    PrISM_3_RestoreConfig();
    if(PrISM_3_backup.enableState != 0u)
    {
        PrISM_3_Enable();
    } 
}


/* [] END OF FILE */
