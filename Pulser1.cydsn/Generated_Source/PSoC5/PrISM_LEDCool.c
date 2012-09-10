/*******************************************************************************
* File Name: PrISM_LEDCool.c
* Version 2.10
*
* Description:
*  This file provides the source code of the API for the PrISM Component.
*
* Note:
*  The PRiSM Component consists of a 8, 16, 24, 32 - bit PrISM, it
*  depends on length Polynomial value and user selected Seed Value. 
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "PrISM_LEDCool.h"


/***************************************
* Local data allocation
***************************************/

uint8 PrISM_LEDCool_initVar = 0u;


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_Start
********************************************************************************
*
* Summary:
*  The start function sets Polynomial, Seed and Pulse Density registers provided
*  by customizer. PrISM computation starts on rising edge of input clock.
*
* Parameters:  
*  None.
*
* Return:  
*  None.
*
* Global variables:
*  The PrISM_LEDCool_initVar variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and
*  set to 1 the first time PrISM_LEDCool_Start() is called. This allows for
*  component initialization without re-initialization in all subsequent calls
*  to the PrISM_LEDCool_Start() routine. 

* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_LEDCool_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(PrISM_LEDCool_initVar == 0u)
    {
        PrISM_LEDCool_Init();
        PrISM_LEDCool_initVar = 1u;
    }
    PrISM_LEDCool_Enable();

}


/*******************************************************************************
* Function Name: PrISM_LEDCool_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  PrISM_LEDCool_Start().
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PrISM_LEDCool_Init(void) 
{
    uint8 enableInterrupts;
    
    /* Writes Seed value, polynom value and density provided by customizer */
    PrISM_LEDCool_WriteSeed(PrISM_LEDCool_SEED);
    PrISM_LEDCool_WritePolynomial(PrISM_LEDCool_POLYNOM);
    PrISM_LEDCool_WritePulse0(PrISM_LEDCool_DENSITY0);
    PrISM_LEDCool_WritePulse1(PrISM_LEDCool_DENSITY1);
    
    enableInterrupts = CyEnterCriticalSection();
    /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
    #if (PrISM_LEDCool_RESOLUTION <= 8u)      /* 8bit - PrISM */
        PrISM_LEDCool_AUX_CONTROL_REG |= PrISM_LEDCool_FIFO0_CLR;
    #elif (PrISM_LEDCool_RESOLUTION <= 16u)   /* 16bit - PrISM */
        CY_SET_REG16(PrISM_LEDCool_AUX_CONTROL_PTR, CY_GET_REG16(PrISM_LEDCool_AUX_CONTROL_PTR) | 
                                        PrISM_LEDCool_FIFO0_CLR | PrISM_LEDCool_FIFO0_CLR << 8u);
    #elif (PrISM_LEDCool_RESOLUTION <= 24u)   /* 24bit - PrISM */
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
    
    #if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)
        /* Writes density type provided by customizer */
        if(PrISM_LEDCool_GREATERTHAN_OR_EQUAL == 0 )
        {
            PrISM_LEDCool_CONTROL_REG |= PrISM_LEDCool_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PrISM_LEDCool_CONTROL_REG &= ~PrISM_LEDCool_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
    
        if(PrISM_LEDCool_GREATERTHAN_OR_EQUAL == 0)
        {
            PrISM_LEDCool_CONTROL_REG |= PrISM_LEDCool_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PrISM_LEDCool_CONTROL_REG &= ~PrISM_LEDCool_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
    #endif /* End PrISM_LEDCool_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* Function Name: PrISM_LEDCool_Enable
********************************************************************************
*  
* Summary: 
*  Enables the PrISM block operation
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PrISM_LEDCool_Enable(void) 
{
    #if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)
        /* Enable the PrISM computation */
        PrISM_LEDCool_CONTROL_REG |= PrISM_LEDCool_CTRL_ENABLE;
    #endif /* End PrISM_LEDCool_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_Stop
********************************************************************************
*
* Summary:
*  Stops PrISM computation. Outputs remain constant.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PrISM_LEDCool_Stop(void) 
{
    #if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)
        PrISM_LEDCool_CONTROL_REG &= ~PrISM_LEDCool_CTRL_ENABLE;
    #else
        /* PulseTypeHardoded option enabled - to stop PrISM use enable input */
    #endif /* End $INSTANCE_NAME`_PULSE_TYPE_HARDCODED */
}

#if(!PrISM_LEDCool_PULSE_TYPE_HARDCODED)


    /***************************************************************************
    * FUNCTION NAME:     PrISM_LEDCool_SetPulse0Mode
    ****************************************************************************
    *
    * Summary:
    *  Sets the pulse density type for Density0. Less than or Equal(<=) or 
    *  Greater than or Equal(>=).
    *
    * Parameters:
    *  pulse0Type: Selected pulse density type.
    *
    * Return:
    *  None.
    *
    ***************************************************************************/
    void PrISM_LEDCool_SetPulse0Mode(uint8 pulse0Type) 
    {
        if(pulse0Type == PrISM_LEDCool_GREATERTHAN_OR_EQUAL)
        {
            PrISM_LEDCool_CONTROL_REG |= PrISM_LEDCool_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PrISM_LEDCool_CONTROL_REG &= ~PrISM_LEDCool_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
    }
    
    
    /***************************************************************************
    * FUNCTION NAME:   PrISM_LEDCool_SetPulse1Mode
    ****************************************************************************
    *
    * Summary:
    *  Sets the pulse density type for Density1. Less than or Equal(<=) or 
    *  Greater than or Equal(>=).
    *
    * Parameters:  
    *  pulse1Type: Selected pulse density type.
    *
    * Return:
    *  None.
    *
    ***************************************************************************/
    void PrISM_LEDCool_SetPulse1Mode(uint8 pulse1Type) 
    {
        if(pulse1Type == PrISM_LEDCool_GREATERTHAN_OR_EQUAL)
        {
            PrISM_LEDCool_CONTROL_REG |= PrISM_LEDCool_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PrISM_LEDCool_CONTROL_REG &= ~PrISM_LEDCool_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
    }

#endif /* End PrISM_LEDCool_PULSE_TYPE_HARDCODED == 0 */


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_ReadSeed
********************************************************************************
*
* Summary:
*  Reads the PrISM Seed register.
*
* Parameters:
*  None.
*
* Return:
*  Current Period register value.
*
*******************************************************************************/
uint8 PrISM_LEDCool_ReadSeed(void) 
{
    return( CY_GET_REG8(PrISM_LEDCool_SEED_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_WriteSeed
********************************************************************************
*
* Summary:
*  Writes the PrISM Seed register with the start value.
*
* Parameters:
*  seed: Seed register value.
*
* Return:
*  None.
*
*******************************************************************************/
void PrISM_LEDCool_WriteSeed(uint8 seed) 
{
    if(seed != 0u)
    {
        CY_SET_REG8(PrISM_LEDCool_SEED_COPY_PTR, seed);
        CY_SET_REG8(PrISM_LEDCool_SEED_PTR, seed);
    }
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_ReadPolynomial
********************************************************************************
*
* Summary:
*  Reads the PrISM polynomial.
*
* Parameters:
*  None.
*
* Return:
*  PrISM polynomial.
*
*******************************************************************************/
uint8 PrISM_LEDCool_ReadPolynomial(void) 
{
    return( CY_GET_REG8(PrISM_LEDCool_POLYNOM_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_WritePolynomial
********************************************************************************
*
* Summary:
*  Writes the PrISM polynomial.
*
* Parameters:
*  polynomial: PrISM polynomial.
*
* Return:
*  None.
*
*******************************************************************************/
void PrISM_LEDCool_WritePolynomial(uint8 polynomial) \
                                                                
{
    CY_SET_REG8(PrISM_LEDCool_POLYNOM_PTR, polynomial);
    
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_ReadPulse0
********************************************************************************
*
* Summary:
*  Reads the PrISM Pulse Density0 register.
*
* Parameters:
*  None.
*
* Return:
*  Pulse Density0 register value.
*
*******************************************************************************/
uint8 PrISM_LEDCool_ReadPulse0(void) 
{
    return( CY_GET_REG8(PrISM_LEDCool_DENSITY0_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_WritePulse0
********************************************************************************
*
* Summary:
*  Writes the PrISM Pulse Density0 register with the Pulse Density value.
*
* Parameters:
*  pulseDensity0: Pulse Density value.
*
* Return:
*  None.
*
*******************************************************************************/
void PrISM_LEDCool_WritePulse0(uint8 pulseDensity0) \
                                                                
{
    CY_SET_REG8(PrISM_LEDCool_DENSITY0_PTR, pulseDensity0);
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_ReadPulse1
********************************************************************************
*
* Summary:
*  Reads the PrISM Pulse Density1 register.
*
* Parameters:
*  None.
*
* Return:
*  Pulse Density1 register value.
*
*******************************************************************************/
uint8 PrISM_LEDCool_ReadPulse1(void) 
{
    return( CY_GET_REG8(PrISM_LEDCool_DENSITY1_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_LEDCool_WritePulse1
********************************************************************************
*
* Summary:
*  Writes the PrISM Pulse Density1 register with the Pulse Density value.
*
* Parameters:
*  pulseDensity1: Pulse Density value.
*
* Return:
*  None.
*
*******************************************************************************/
void PrISM_LEDCool_WritePulse1(uint8 pulseDensity1) \
                                                                    
{
    CY_SET_REG8(PrISM_LEDCool_DENSITY1_PTR, pulseDensity1);
}


/* [] END OF FILE */
