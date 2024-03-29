/*******************************************************************************
* File Name: Pin_Debjug_LED.c  
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
#include "Pin_Debjug_LED.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_Debjug_LED__PORT == 15 && (Pin_Debjug_LED__MASK & 0xC0))

/*******************************************************************************
* Function Name: Pin_Debjug_LED_Write
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
void Pin_Debjug_LED_Write(uint8 value) 
{
    uint8 staticBits = Pin_Debjug_LED_DR & ~Pin_Debjug_LED_MASK;
    Pin_Debjug_LED_DR = staticBits | ((value << Pin_Debjug_LED_SHIFT) & Pin_Debjug_LED_MASK);
}


/*******************************************************************************
* Function Name: Pin_Debjug_LED_SetDriveMode
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
void Pin_Debjug_LED_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_Debjug_LED_0, mode);
}


/*******************************************************************************
* Function Name: Pin_Debjug_LED_Read
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
*  Macro Pin_Debjug_LED_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_Debjug_LED_Read(void) 
{
    return (Pin_Debjug_LED_PS & Pin_Debjug_LED_MASK) >> Pin_Debjug_LED_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_Debjug_LED_ReadDataReg
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
uint8 Pin_Debjug_LED_ReadDataReg(void) 
{
    return (Pin_Debjug_LED_DR & Pin_Debjug_LED_MASK) >> Pin_Debjug_LED_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_Debjug_LED_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_Debjug_LED_ClearInterrupt
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
    uint8 Pin_Debjug_LED_ClearInterrupt(void) 
    {
        return (Pin_Debjug_LED_INTSTAT & Pin_Debjug_LED_MASK) >> Pin_Debjug_LED_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif
/* [] END OF FILE */ 
