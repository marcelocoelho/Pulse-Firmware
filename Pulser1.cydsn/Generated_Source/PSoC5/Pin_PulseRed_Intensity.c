/*******************************************************************************
* File Name: Pin_PulseRed_Intensity.c  
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
#include "Pin_PulseRed_Intensity.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Pin_PulseRed_Intensity__PORT == 15 && ((Pin_PulseRed_Intensity__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Pin_PulseRed_Intensity_Write
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
void Pin_PulseRed_Intensity_Write(uint8 value) 
{
    uint8 staticBits = (Pin_PulseRed_Intensity_DR & (uint8)(~Pin_PulseRed_Intensity_MASK));
    Pin_PulseRed_Intensity_DR = staticBits | ((uint8)(value << Pin_PulseRed_Intensity_SHIFT) & Pin_PulseRed_Intensity_MASK);
}


/*******************************************************************************
* Function Name: Pin_PulseRed_Intensity_SetDriveMode
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
void Pin_PulseRed_Intensity_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_PulseRed_Intensity_0, mode);
}


/*******************************************************************************
* Function Name: Pin_PulseRed_Intensity_Read
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
*  Macro Pin_PulseRed_Intensity_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_PulseRed_Intensity_Read(void) 
{
    return (Pin_PulseRed_Intensity_PS & Pin_PulseRed_Intensity_MASK) >> Pin_PulseRed_Intensity_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_PulseRed_Intensity_ReadDataReg
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
uint8 Pin_PulseRed_Intensity_ReadDataReg(void) 
{
    return (Pin_PulseRed_Intensity_DR & Pin_PulseRed_Intensity_MASK) >> Pin_PulseRed_Intensity_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_PulseRed_Intensity_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_PulseRed_Intensity_ClearInterrupt
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
    uint8 Pin_PulseRed_Intensity_ClearInterrupt(void) 
    {
        return (Pin_PulseRed_Intensity_INTSTAT & Pin_PulseRed_Intensity_MASK) >> Pin_PulseRed_Intensity_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */