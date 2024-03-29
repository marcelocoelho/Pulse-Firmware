/*******************************************************************************
* File Name: Pin_6.c  
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
#include "Pin_6.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_6__PORT == 15 && (Pin_6__MASK & 0xC0))

/*******************************************************************************
* Function Name: Pin_6_Write
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
void Pin_6_Write(uint8 value) 
{
    uint8 staticBits = Pin_6_DR & ~Pin_6_MASK;
    Pin_6_DR = staticBits | ((value << Pin_6_SHIFT) & Pin_6_MASK);
}


/*******************************************************************************
* Function Name: Pin_6_SetDriveMode
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
void Pin_6_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_6_0, mode);
}


/*******************************************************************************
* Function Name: Pin_6_Read
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
*  Macro Pin_6_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_6_Read(void) 
{
    return (Pin_6_PS & Pin_6_MASK) >> Pin_6_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_6_ReadDataReg
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
uint8 Pin_6_ReadDataReg(void) 
{
    return (Pin_6_DR & Pin_6_MASK) >> Pin_6_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_6_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_6_ClearInterrupt
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
    uint8 Pin_6_ClearInterrupt(void) 
    {
        return (Pin_6_INTSTAT & Pin_6_MASK) >> Pin_6_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif
/* [] END OF FILE */ 
