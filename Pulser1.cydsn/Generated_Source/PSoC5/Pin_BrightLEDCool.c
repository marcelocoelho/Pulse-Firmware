/*******************************************************************************
* File Name: Pin_BrightLEDCool.c  
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
#include "Pin_BrightLEDCool.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Pin_BrightLEDCool__PORT == 15 && ((Pin_BrightLEDCool__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Pin_BrightLEDCool_Write
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
void Pin_BrightLEDCool_Write(uint8 value) 
{
    uint8 staticBits = (Pin_BrightLEDCool_DR & (uint8)(~Pin_BrightLEDCool_MASK));
    Pin_BrightLEDCool_DR = staticBits | ((uint8)(value << Pin_BrightLEDCool_SHIFT) & Pin_BrightLEDCool_MASK);
}


/*******************************************************************************
* Function Name: Pin_BrightLEDCool_SetDriveMode
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
void Pin_BrightLEDCool_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_BrightLEDCool_0, mode);
}


/*******************************************************************************
* Function Name: Pin_BrightLEDCool_Read
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
*  Macro Pin_BrightLEDCool_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_BrightLEDCool_Read(void) 
{
    return (Pin_BrightLEDCool_PS & Pin_BrightLEDCool_MASK) >> Pin_BrightLEDCool_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_BrightLEDCool_ReadDataReg
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
uint8 Pin_BrightLEDCool_ReadDataReg(void) 
{
    return (Pin_BrightLEDCool_DR & Pin_BrightLEDCool_MASK) >> Pin_BrightLEDCool_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_BrightLEDCool_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_BrightLEDCool_ClearInterrupt
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
    uint8 Pin_BrightLEDCool_ClearInterrupt(void) 
    {
        return (Pin_BrightLEDCool_INTSTAT & Pin_BrightLEDCool_MASK) >> Pin_BrightLEDCool_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
