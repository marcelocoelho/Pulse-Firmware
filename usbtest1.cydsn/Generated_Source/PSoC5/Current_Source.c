/*******************************************************************************
* File Name: Current_Source.c  
* Version 1.80
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Current_Source.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Current_Source__PORT == 15 && ((Current_Source__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Current_Source_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void Current_Source_Write(uint8 value) 
{
    uint8 staticBits = (Current_Source_DR & (uint8)(~Current_Source_MASK));
    Current_Source_DR = staticBits | ((uint8)(value << Current_Source_SHIFT) & Current_Source_MASK);
}


/*******************************************************************************
* Function Name: Current_Source_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void Current_Source_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Current_Source_0, mode);
}


/*******************************************************************************
* Function Name: Current_Source_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro Current_Source_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Current_Source_Read(void) 
{
    return (Current_Source_PS & Current_Source_MASK) >> Current_Source_SHIFT;
}


/*******************************************************************************
* Function Name: Current_Source_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 Current_Source_ReadDataReg(void) 
{
    return (Current_Source_DR & Current_Source_MASK) >> Current_Source_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Current_Source_INTSTAT) 

    /*******************************************************************************
    * Function Name: Current_Source_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 Current_Source_ClearInterrupt(void) 
    {
        return (Current_Source_INTSTAT & Current_Source_MASK) >> Current_Source_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
