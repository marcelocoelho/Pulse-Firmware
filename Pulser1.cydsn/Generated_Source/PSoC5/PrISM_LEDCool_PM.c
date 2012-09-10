/*******************************************************************************
* File Name: PrISM_LEDCool_PM.c
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

#include "PrISM_LEDCool.h"


/***************************************
* Forward function references
***************************************/
void PrISM_LEDCool_Enable(void) ;


/***************************************
* Local data allocation
***************************************/
static PrISM_LEDCool_BACKUP_STRUCT  PrISM_LEDCool_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)
        (PrISM_LEDCool_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_LEDCool_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0) |
        (PrISM_LEDCool_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PrISM_LEDCool_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0),
    #endif /* End PrISM_LEDCool_PULSE_TYPE_HARDCODED */
    /* seed */    
    PrISM_LEDCool_SEED,
    /* seed_copy */    
    PrISM_LEDCool_SEED,
    /* polynom */
    PrISM_LEDCool_POLYNOM,
    #if(CY_UDB_V0)
        /* density0 */
        PrISM_LEDCool_DENSITY0,
        /* density1 */
        PrISM_LEDCool_DENSITY1,
    #endif /*End CY_UDB_V0*/
};


/*******************************************************************************
* Function Name: PrISM_LEDCool_SaveConfig
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
*  PrISM_LEDCool_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_LEDCool_SaveConfig(void) 
{
    #if (CY_UDB_V0)
        #if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)
            PrISM_LEDCool_backup.cr = PrISM_LEDCool_CONTROL_REG;
        #endif /* End PrISM_LEDCool_PULSE_TYPE_HARDCODED */
        PrISM_LEDCool_backup.seed = PrISM_LEDCool_ReadSeed();
        PrISM_LEDCool_backup.seed_copy = CY_GET_REG8(PrISM_LEDCool_SEED_COPY_PTR);
        PrISM_LEDCool_backup.polynom = PrISM_LEDCool_ReadPolynomial();
        PrISM_LEDCool_backup.density0 = PrISM_LEDCool_ReadPulse0();
        PrISM_LEDCool_backup.density1 = PrISM_LEDCool_ReadPulse1();
    #else /* CY_UDB_V1 */
        #if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)
            PrISM_LEDCool_backup.cr = PrISM_LEDCool_CONTROL_REG;
        #endif /* End PrISM_LEDCool_PULSE_TYPE_HARDCODED */
        PrISM_LEDCool_backup.seed = PrISM_LEDCool_ReadSeed();
        PrISM_LEDCool_backup.seed_copy = CY_GET_REG8(PrISM_LEDCool_SEED_COPY_PTR);
        PrISM_LEDCool_backup.polynom = PrISM_LEDCool_ReadPolynomial();
    #endif  /* CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_LEDCool_RestoreConfig
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
*  PrISM_LEDCool_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_LEDCool_RestoreConfig(void) 
{
    #if (CY_UDB_V0)
    
        uint8 enableInterrupts;
        
        #if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)
            PrISM_LEDCool_CONTROL_REG = PrISM_LEDCool_backup.cr;
        #endif /* End PrISM_LEDCool_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_LEDCool_SEED_COPY_PTR, PrISM_LEDCool_backup.seed_copy);
        CY_SET_REG8(PrISM_LEDCool_SEED_PTR, PrISM_LEDCool_backup.seed);
        PrISM_LEDCool_WritePolynomial(PrISM_LEDCool_backup.polynom);
        PrISM_LEDCool_WritePulse0(PrISM_LEDCool_backup.density0);
        PrISM_LEDCool_WritePulse1(PrISM_LEDCool_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        #if (PrISM_LEDCool_RESOLUTION <= 8u)      /* 8bit - PrISM */
            PrISM_LEDCool_AUX_CONTROL_REG |= PrISM_LEDCool_FIFO0_CLR;
        #elif (PrISM_LEDCool_RESOLUTION <= 16u)   /* 16bit - PrISM */
            CY_SET_REG16(PrISM_LEDCool_AUX_CONTROL_PTR, CY_GET_REG16(PrISM_LEDCool_AUX_CONTROL_PTR) | 
                                            PrISM_LEDCool_FIFO0_CLR | PrISM_LEDCool_FIFO0_CLR << 8u);
        #elif (PrISM_LEDCool_RESOLUTION <= 24)   /* 24bit - PrISM */
            CY_SET_REG24(PrISM_LEDCool_AUX_CONTROL_PTR, CY_GET_REG24(PrISM_LEDCool_AUX_CONTROL_PTR) |
                                            PrISM_LEDCool_FIFO0_CLR | PrISM_LEDCool_FIFO0_CLR << 8u );
            CY_SET_REG24(PrISM_LEDCool_AUX_CONTROL2_PTR, CY_GET_REG24(PrISM_LEDCool_AUX_CONTROL2_PTR) | 
                                            PrISM_LEDCool_FIFO0_CLR );
        #else                                 /* 32bit - PrISM */
            CY_SET_REG32(PrISM_LEDCool_AUX_CONTROL_PTR, CY_GET_REG32(PrISM_LEDCool_AUX_CONTROL_PTR) |
                                            PrISM_LEDCool_FIFO0_CLR | PrISM_LEDCool_FIFO0_CLR << 8u );
            CY_SET_REG32(PrISM_LEDCool_AUX_CONTROL2_PTR, CY_GET_REG32(PrISM_LEDCool_AUX_CONTROL2_PTR) |
                                            PrISM_LEDCool_FIFO0_CLR | PrISM_LEDCool_FIFO0_CLR << 8u );
        #endif                                /* End PrISM_LEDCool_RESOLUTION */
        CyExitCriticalSection(enableInterrupts);
   
    #else   /* CY_UDB_V1 */

        #if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)
            PrISM_LEDCool_CONTROL_REG = PrISM_LEDCool_backup.cr;
        #endif /* End PrISM_LEDCool_PULSE_TYPE_HARDCODED */

        CY_SET_REG8(PrISM_LEDCool_SEED_COPY_PTR, PrISM_LEDCool_backup.seed_copy);
        CY_SET_REG8(PrISM_LEDCool_SEED_PTR, PrISM_LEDCool_backup.seed);
        PrISM_LEDCool_WritePolynomial(PrISM_LEDCool_backup.polynom);
    
    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: PrISM_LEDCool_Sleep
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
*  PrISM_LEDCool_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_LEDCool_Sleep(void) 
{
    #if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)
        if((PrISM_LEDCool_CONTROL_REG & PrISM_LEDCool_CTRL_ENABLE) != 0u) 
        {
            PrISM_LEDCool_backup.enableState = 1u;
        }
        else
        {
            PrISM_LEDCool_backup.enableState = 0u;
        }
    #endif /* End PrISM_LEDCool_PULSE_TYPE_HARDCODED */
    PrISM_LEDCool_Stop();
    PrISM_LEDCool_SaveConfig();
}


/*******************************************************************************
* Function Name: PrISM_LEDCool_Wakeup
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
*  PrISM_LEDCool_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PrISM_LEDCool_Wakeup(void) 
{
    PrISM_LEDCool_RestoreConfig();
    if(PrISM_LEDCool_backup.enableState != 0u)
    {
        PrISM_LEDCool_Enable();
    } 
}


/* [] END OF FILE */
