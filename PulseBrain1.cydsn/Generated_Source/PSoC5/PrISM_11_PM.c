/*******************************************************************************
* File Name: PrISM_11_PM.c
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

#include "PrISM_11.h"


/***************************************
* Forward function references
***************************************/
void PrISM_11_Enable(void) ;


/***************************************
* Local data allocation
***************************************/
static PrISM_11_BACKUP_STRUCT  PrISM_11_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_11_PULSE_TYPE_HARDCODED)
        (PrISM_11_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_11_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0) |
        (PrISM_11_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_11_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0),
    #endif /* End PrISM_11_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_11_SEED,
    /* seed_copy */    
    PrISM_11_SEED,
    /* polynom */
    PrISM_11_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_11_DENSITY0,
        /* density1 */
        PrISM_11_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_11_SaveConfig
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
*  PrISM_11_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_11_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_11_PULSE_TYPE_HARDCODED)
            PrISM_11_backup.cr = PrISM_11_CONTROL_REG;
        #endif /* End PrISM_11_PULSE_TYPE_HARDCODED */
        PrISM_11_backup.seed = PrISM_11_ReadSeed();
        PrISM_11_backup.seed_copy = CY_GET_REG8(PrISM_11_SEED_COPY_PTR);
        PrISM_11_backup.polynom = PrISM_11_ReadPolynomial();
        PrISM_11_backup.density0 = PrISM_11_ReadPulse0();
        PrISM_11_backup.density1 = PrISM_11_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_11_PULSE_TYPE_HARDCODED)
            PrISM_11_backup.cr = PrISM_11_CONTROL_REG;
        #endif /* End PrISM_11_PULSE_TYPE_HARDCODED */
        PrISM_11_backup.seed = PrISM_11_ReadSeed();
        PrISM_11_backup.seed_copy = CY_GET_REG8(PrISM_11_SEED_COPY_PTR);
        PrISM_11_backup.polynom = PrISM_11_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_11_RestoreConfig
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
*  PrISM_11_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_11_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_11_PULSE_TYPE_HARDCODED)
            PrISM_11_CONTROL_REG = PrISM_11_backup.cr;
        #endif /* End PrISM_11_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_11_SEED_COPY_PTR, PrISM_11_backup.seed_copy);
        CY_SET_REG8(PrISM_11_SEED_PTR, PrISM_11_backup.seed);
        PrISM_11_WritePolynomial(PrISM_11_backup.polynom);
        PrISM_11_WritePulse0(PrISM_11_backup.density0);
        PrISM_11_WritePulse1(PrISM_11_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        #if (PrISM_11_RESOLUTION <= 8u)      /* 8bit - PrISM */
            PrISM_11_AUX_CONTROL_REG |= PrISM_11_FIFO0_CLR;
        #elif (PrISM_11_RESOLUTION <= 16u)   /* 16bit - PrISM */
            CY_SET_REG16(PrISM_11_AUX_CONTROL_PTR, CY_GET_REG16(PrISM_11_AUX_CONTROL_PTR) | 
                                            PrISM_11_FIFO0_CLR | PrISM_11_FIFO0_CLR << 8u);
        #elif (PrISM_11_RESOLUTION <= 24)   /* 24bit - PrISM */
            CY_SET_REG24(PrISM_11_AUX_CONTROL_PTR, CY_GET_REG24(PrISM_11_AUX_CONTROL_PTR) |
                                            PrISM_11_FIFO0_CLR | PrISM_11_FIFO0_CLR << 8u );
            CY_SET_REG24(PrISM_11_AUX_CONTROL2_PTR, CY_GET_REG24(PrISM_11_AUX_CONTROL2_PTR) | 
                                            PrISM_11_FIFO0_CLR );
        #else                                 /* 32bit - PrISM */
            CY_SET_REG32(PrISM_11_AUX_CONTROL_PTR, CY_GET_REG32(PrISM_11_AUX_CONTROL_PTR) |
                                            PrISM_11_FIFO0_CLR | PrISM_11_FIFO0_CLR << 8u );
            CY_SET_REG32(PrISM_11_AUX_CONTROL2_PTR, CY_GET_REG32(PrISM_11_AUX_CONTROL2_PTR) |
                                            PrISM_11_FIFO0_CLR | PrISM_11_FIFO0_CLR << 8u );
        #endif                                /* End PrISM_11_RESOLUTION */
        CyExitCriticalSection(enableInterrupts);
   
    #else   /* CY_UDB_V1 */

        #if(!PrISM_11_PULSE_TYPE_HARDCODED)
            PrISM_11_CONTROL_REG = PrISM_11_backup.cr;
        #endif /* End PrISM_11_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_11_SEED_COPY_PTR, PrISM_11_backup.seed_copy);
        CY_SET_REG8(PrISM_11_SEED_PTR, PrISM_11_backup.seed);
        PrISM_11_WritePolynomial(PrISM_11_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_11_Sleep
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
*  PrISM_11_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_11_Sleep(void) 
{
    #if(!PrISM_11_PULSE_TYPE_HARDCODED)
        if((PrISM_11_CONTROL_REG & PrISM_11_CTRL_ENABLE) != 0u) 
        {
            PrISM_11_backup.enableState = 1u;
        }
        else
        {
            PrISM_11_backup.enableState = 0u;
        }
    #endif /* End PrISM_11_PULSE_TYPE_HARDCODED */
    PrISM_11_Stop();
    PrISM_11_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_11_Wakeup
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
*  PrISM_11_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_11_Wakeup(void) 
{
    PrISM_11_RestoreConfig();
    if(PrISM_11_backup.enableState != 0u)
    {
        PrISM_11_Enable();
    } 
}


/* [] END OF FILE */
