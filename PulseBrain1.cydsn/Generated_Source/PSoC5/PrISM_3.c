/*******************************************************************************
* File Name: PrISM_3.c
* Version 2.20
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

#include "PrISM_3.h"


/***************************************
* Local data allocation
***************************************/

uint8 PrISM_3_initVar = 0u;


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_Start
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
*  The PrISM_3_initVar variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and
*  set to 1 the first time PrISM_3_Start() is called. This allows for
*  component initialization without re-initialization in all subsequent calls
*  to the PrISM_3_Start() routine. 

* Reentrant:
*  No.
*
*******************************************************************************/
void PrISM_3_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(PrISM_3_initVar == 0u)
    {
        PrISM_3_Init();
        PrISM_3_initVar = 1u;
    }
    PrISM_3_Enable();
}


/*******************************************************************************
* Function Name: PrISM_3_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  PrISM_3_Start().
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PrISM_3_Init(void) 
{
    uint8 enableInterrupts;
    
    /* Writes Seed value, polynom value and density provided by customizer */
    PrISM_3_WriteSeed(PrISM_3_SEED);
    PrISM_3_WritePolynomial(PrISM_3_POLYNOM);
    PrISM_3_WritePulse0(PrISM_3_DENSITY0);
    PrISM_3_WritePulse1(PrISM_3_DENSITY1);
    
    enableInterrupts = CyEnterCriticalSection();
    /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
    CY_SET_REG8(PrISM_3_AUX_CONTROL_PTR, 
                        CY_GET_REG8(PrISM_3_AUX_CONTROL_PTR) | PrISM_3_FIFO0_CLR);
    CyExitCriticalSection(enableInterrupts);
    
    #if(!PrISM_3_PULSE_TYPE_HARDCODED)
        /* Writes density type provided by customizer */
        #if(PrISM_3_GREATERTHAN_OR_EQUAL == PrISM_3_COMPARE0)
            PrISM_3_CONTROL_REG |= PrISM_3_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        #else
            PrISM_3_CONTROL_REG &= (uint8)~PrISM_3_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        #endif /* End PrISM_3_COMPARE0 */    
    
        #if(PrISM_3_GREATERTHAN_OR_EQUAL == PrISM_3_COMPARE1)
            PrISM_3_CONTROL_REG |= PrISM_3_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        #else
            PrISM_3_CONTROL_REG &= (uint8)~PrISM_3_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        #endif /* End PrISM_3_COMPARE1 */    
    #endif /* End PrISM_3_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* Function Name: PrISM_3_Enable
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
void PrISM_3_Enable(void) 
{
    #if(!PrISM_3_PULSE_TYPE_HARDCODED)
        /* Enable the PrISM computation */
        PrISM_3_CONTROL_REG |= PrISM_3_CTRL_ENABLE;
    #endif /* End PrISM_3_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_Stop
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
void PrISM_3_Stop(void) 
{
    #if(!PrISM_3_PULSE_TYPE_HARDCODED)
        PrISM_3_CONTROL_REG &= (uint8)~PrISM_3_CTRL_ENABLE;
    #else
        /* PulseTypeHardoded option enabled - to stop PrISM use enable input */
    #endif /* End $INSTANCE_NAME`_PULSE_TYPE_HARDCODED */
}

#if(!PrISM_3_PULSE_TYPE_HARDCODED)


    /***************************************************************************
    * FUNCTION NAME:     PrISM_3_SetPulse0Mode
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
    void PrISM_3_SetPulse0Mode(uint8 pulse0Type) 
    {
        if(pulse0Type == PrISM_3_GREATERTHAN_OR_EQUAL)
        {
            PrISM_3_CONTROL_REG |= PrISM_3_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PrISM_3_CONTROL_REG &= (uint8)~PrISM_3_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
    }
    
    
    /***************************************************************************
    * FUNCTION NAME:   PrISM_3_SetPulse1Mode
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
    void PrISM_3_SetPulse1Mode(uint8 pulse1Type) 
    {
        if(pulse1Type == PrISM_3_GREATERTHAN_OR_EQUAL)
        {
            PrISM_3_CONTROL_REG |= PrISM_3_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PrISM_3_CONTROL_REG &= (uint8)~PrISM_3_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
    }

#endif /* End PrISM_3_PULSE_TYPE_HARDCODED == 0 */


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_ReadSeed
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
uint8 PrISM_3_ReadSeed(void) 
{
    return( CY_GET_REG8(PrISM_3_SEED_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_WriteSeed
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
void PrISM_3_WriteSeed(uint8 seed) 
{
    if(seed != 0u)
    {
        CY_SET_REG8(PrISM_3_SEED_COPY_PTR, seed);
        CY_SET_REG8(PrISM_3_SEED_PTR, seed);
    }
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_ReadPolynomial
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
uint8 PrISM_3_ReadPolynomial(void) 
{
    return( CY_GET_REG8(PrISM_3_POLYNOM_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_WritePolynomial
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
void PrISM_3_WritePolynomial(uint8 polynomial) \
                                                                
{
    CY_SET_REG8(PrISM_3_POLYNOM_PTR, polynomial);
    
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_ReadPulse0
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
uint8 PrISM_3_ReadPulse0(void) 
{
    return( CY_GET_REG8(PrISM_3_DENSITY0_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_WritePulse0
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
void PrISM_3_WritePulse0(uint8 pulseDensity0) \
                                                                
{
    CY_SET_REG8(PrISM_3_DENSITY0_PTR, pulseDensity0);
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_ReadPulse1
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
uint8 PrISM_3_ReadPulse1(void) 
{
    return( CY_GET_REG8(PrISM_3_DENSITY1_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PrISM_3_WritePulse1
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
void PrISM_3_WritePulse1(uint8 pulseDensity1) \
                                                                    
{
    CY_SET_REG8(PrISM_3_DENSITY1_PTR, pulseDensity1);
}


/* [] END OF FILE */
