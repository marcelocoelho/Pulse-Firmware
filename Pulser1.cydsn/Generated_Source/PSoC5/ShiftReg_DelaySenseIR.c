/*******************************************************************************
* File Name: ShiftReg_DelaySenseIR.c
* Version 2.0
*
* Description:
*  This file provides the API source code for the Shift Register component.
*
* Note: none
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/


#include "ShiftReg_DelaySenseIR.h"
#include "CyLib.h"


uint8 ShiftReg_DelaySenseIR_initVar = 0u;


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_Start
********************************************************************************
*
* Summary:
*  Starts the Shift Register.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ShiftReg_DelaySenseIR_initVar - used to check initial configuration, modified on 
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_Start(void)
{
    if (ShiftReg_DelaySenseIR_initVar == 0u)
    {
        ShiftReg_DelaySenseIR_Init();
        /* variable _initVar is set to indicate the fact of initialization */
        ShiftReg_DelaySenseIR_initVar = 1u; 
    }

    ShiftReg_DelaySenseIR_Enable();
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_Enable
********************************************************************************
*
* Summary:
*  Enables the Shift Register.
*
* Parameters:
*  void.
*
* Return:
*  void.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_Enable(void) 
{
    /* changing address in Datapath Control Store
       from NOP to component state machine commands space */
    ShiftReg_DelaySenseIR_SR_CONTROL |= ShiftReg_DelaySenseIR_CLK_EN; 
    
    ShiftReg_DelaySenseIR_EnableInt();
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_Init
********************************************************************************
*
* Summary:
*  Initializes Tx and/or Rx interrupt sources with initial values.
*
* Parameters:
*  void.
*
* Return:
*  void.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_Init(void) 
{
    ShiftReg_DelaySenseIR_SetIntMode(ShiftReg_DelaySenseIR_INT_SRC);
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_Stop
********************************************************************************
*
* Summary:
*  Disables the Shift Register
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_Stop(void) 
{
    /*changing Datapath Control Store address to NOP space*/
    ShiftReg_DelaySenseIR_SR_CONTROL &= ~ShiftReg_DelaySenseIR_CLK_EN; 
    ShiftReg_DelaySenseIR_DisableInt();
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_EnableInt
********************************************************************************
*
* Summary:
*  Enables the Shift Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_EnableInt(void) 
{
    uint8 interruptState = CyEnterCriticalSection();
    ShiftReg_DelaySenseIR_SR_AUX_CONTROL |= ShiftReg_DelaySenseIR_INTERRUPTS_ENABLE;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_DisableInt
********************************************************************************
*
* Summary:
*  Disables the Shift Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_DisableInt(void) 
{
    uint8 interruptState = CyEnterCriticalSection();
    ShiftReg_DelaySenseIR_SR_AUX_CONTROL &= ~ShiftReg_DelaySenseIR_INTERRUPTS_ENABLE;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_GetFIFOStatus
********************************************************************************
*
* Summary:
*  Returns current status of input or output FIFO.
*
* Parameters:
*  fifoId.
*
* Return:
*  FIFO status.
*
*******************************************************************************/
uint8 ShiftReg_DelaySenseIR_GetFIFOStatus(uint8 fifoId) 
{
    uint8 result;

    result = ShiftReg_DelaySenseIR_RET_FIFO_NOT_DEFINED; /*default status value*/

    #if (ShiftReg_DelaySenseIR_USE_INPUT_FIFO == 1u)
        
        if(fifoId == ShiftReg_DelaySenseIR_IN_FIFO)
        {    
            switch((ShiftReg_DelaySenseIR_SR_STATUS & ShiftReg_DelaySenseIR_IN_FIFO_MASK) >> 
                    ShiftReg_DelaySenseIR_IN_FIFO_SHFT_MASK)
            {
                case ShiftReg_DelaySenseIR_IN_FIFO_FULL :
                    result = ShiftReg_DelaySenseIR_RET_FIFO_FULL;
                    break;

                case ShiftReg_DelaySenseIR_IN_FIFO_EMPTY :
                    result = ShiftReg_DelaySenseIR_RET_FIFO_EMPTY;
                    break;

                case ShiftReg_DelaySenseIR_IN_FIFO_NOT_EMPTY :
                    result = ShiftReg_DelaySenseIR_RET_FIFO_NOT_EMPTY;
                    break;
                default:
                    result = ShiftReg_DelaySenseIR_RET_FIFO_EMPTY;
            }
        }
    #endif/*(ShiftReg_DelaySenseIR_USE_INPUT_FIFO == 1u)*/
    
    if(fifoId == ShiftReg_DelaySenseIR_OUT_FIFO)
    {
        switch((ShiftReg_DelaySenseIR_SR_STATUS & ShiftReg_DelaySenseIR_OUT_FIFO_MASK) >> 
                ShiftReg_DelaySenseIR_OUT_FIFO_SHFT_MASK)
        {
            case ShiftReg_DelaySenseIR_OUT_FIFO_FULL :
                result = ShiftReg_DelaySenseIR_RET_FIFO_FULL;
                break;

            case ShiftReg_DelaySenseIR_OUT_FIFO_EMPTY :
                result = ShiftReg_DelaySenseIR_RET_FIFO_EMPTY;
                break;

            case ShiftReg_DelaySenseIR_OUT_FIFO_NOT_EMPTY :
                result = ShiftReg_DelaySenseIR_RET_FIFO_NOT_EMPTY;
                break;

            default:
                result = ShiftReg_DelaySenseIR_RET_FIFO_FULL;
        }
    }

    return (result);
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_SetIntMode
********************************************************************************
*
* Summary:
*  Sets the Interrupt Source for the Shift Register interrupt. Multiple
*  sources may be ORed together
*
* Parameters:
*  interruptSource: Byte containing the constant for the selected interrupt
*  source/s.
*
* Return:
*  None.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_SetIntMode(uint8 interruptSource) 
{
    ShiftReg_DelaySenseIR_SR_STATUS_MASK = (ShiftReg_DelaySenseIR_SR_STATUS_MASK & ~ShiftReg_DelaySenseIR_INTS_EN_MASK) | \
                                      (interruptSource & ShiftReg_DelaySenseIR_INTS_EN_MASK);
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_GetIntStatus
********************************************************************************
*
* Summary:
*  Gets the Shift Register Interrupt status.
*
* Parameters:
*  None.
*
* Return:
*  Byte containing the constant for the selected interrupt source/s.
*
*******************************************************************************/
uint8 ShiftReg_DelaySenseIR_GetIntStatus(void) 
{
    return(ShiftReg_DelaySenseIR_SR_STATUS & ShiftReg_DelaySenseIR_INTS_EN_MASK);
}


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_WriteRegValue
********************************************************************************
*
* Summary:
*  Send state directly to shift register
*
* Parameters:
*  shiftData: containing shift register state.
*
* Return:
*  None.
*
*******************************************************************************/
void ShiftReg_DelaySenseIR_WriteRegValue(uint32 shiftData) \
                
{
    CY_SET_REG32(ShiftReg_DelaySenseIR_SHIFT_REG_LSB_PTR, shiftData);
}


#if (ShiftReg_DelaySenseIR_USE_INPUT_FIFO == 1u) /* if input FIFO is used */

    /*******************************************************************************
    * Function Name: ShiftReg_DelaySenseIR_WriteData
    ********************************************************************************
    *
    * Summary:
    *  Send state to FIFO for later transfer to shift register based on the Load
    *  input
    *
    * Parameters:
    *  shiftData: containing shift register state.
    *
    * Return:
    *  Indicates: successful execution of function
    *  when FIFO is empty; and error when
    *  FIFO is full.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 ShiftReg_DelaySenseIR_WriteData(uint32 shiftData) \
                    
    {
        uint8 result;
    
        result = CYRET_INVALID_STATE;/*default result state*/
    
        /*write data to input FIFO if FIFO is not full*/
        if((ShiftReg_DelaySenseIR_GetFIFOStatus(ShiftReg_DelaySenseIR_IN_FIFO)) != ShiftReg_DelaySenseIR_RET_FIFO_FULL)
        {
            CY_SET_REG32(ShiftReg_DelaySenseIR_IN_FIFO_VAL_LSB_PTR, shiftData);
            result = CYRET_SUCCESS; /*return 'success' status*/
        }
        return(result);
    }


#endif/*(ShiftReg_DelaySenseIR_USE_INPUT_FIFO == 1u)*/

#if (ShiftReg_DelaySenseIR_USE_OUTPUT_FIFO == 1u)/*if output FIFO is used*/

    /*******************************************************************************
    * Function Name: ShiftReg_DelaySenseIR_ReadData
    ********************************************************************************
    *
    * Summary:
    *  Returns state in FIFO due to Store input.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Shift Register state
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint32 ShiftReg_DelaySenseIR_ReadData(void) \
            
    {
        return(CY_GET_REG32(ShiftReg_DelaySenseIR_OUT_FIFO_VAL_LSB_PTR));
    }

#endif/*(ShiftReg_DelaySenseIR_USE_OUTPUT_FIFO == 1u)*/


/*******************************************************************************
* Function Name: ShiftReg_DelaySenseIR_ReadRegValue
********************************************************************************
*
* Summary:
*  Directly returns current state in shift register, not data in FIFO due
*  to Store input.
*
* Parameters:
*  None.
*
* Return:
*  Shift Register state
*
*  Clears output FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint32 ShiftReg_DelaySenseIR_ReadRegValue(void) 
{
    uint32 result;

    /* Clear FIFO before software capture */

    while(ShiftReg_DelaySenseIR_GetFIFOStatus(ShiftReg_DelaySenseIR_OUT_FIFO) != ShiftReg_DelaySenseIR_RET_FIFO_EMPTY)
    {
        result = CY_GET_REG32(ShiftReg_DelaySenseIR_OUT_FIFO_VAL_LSB_PTR);
    }
    
    /* Capture A1 to output FIFO */
    result = CY_GET_REG8(ShiftReg_DelaySenseIR_SHIFT_REG_VALUE_LSB_PTR);
    
    /* Read output FIFO */
    result = CY_GET_REG32(ShiftReg_DelaySenseIR_OUT_FIFO_VAL_LSB_PTR);
    result = result & ShiftReg_DelaySenseIR_SR_MASK;

    return(result);
}

/* [] END OF FILE */
