/*******************************************************************************
* File Name: Pin_12.c  
* Version 1.70
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "Pin_12.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_12__PORT == 15 && (Pin_12__MASK & 0xC0))

/*******************************************************************************
* Function Name: Pin_12_Write
********************************************************************************
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  void 
*  
*******************************************************************************/
void Pin_12_Write(uint8 value) 
{
    uint8 staticBits = Pin_12_DR & ~Pin_12_MASK;
    Pin_12_DR = staticBits | ((value << Pin_12_SHIFT) & Pin_12_MASK);
}


/*******************************************************************************
* Function Name: Pin_12_SetDriveMode
********************************************************************************
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  void
*
*******************************************************************************/
void Pin_12_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_12_0, mode);
}


/*******************************************************************************
* Function Name: Pin_12_Read
********************************************************************************
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro Pin_12_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_12_Read(void) 
{
    return (Pin_12_PS & Pin_12_MASK) >> Pin_12_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_12_ReadDataReg
********************************************************************************
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 Pin_12_ReadDataReg(void) 
{
    return (Pin_12_DR & Pin_12_MASK) >> Pin_12_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_12_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_12_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  void 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 Pin_12_ClearInterrupt(void) 
    {
        return (Pin_12_INTSTAT & Pin_12_MASK) >> Pin_12_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif
/* [] END OF FILE */ 
