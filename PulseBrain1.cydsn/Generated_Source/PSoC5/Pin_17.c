/*******************************************************************************
* File Name: Pin_17.c  
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
#include "Pin_17.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_17__PORT == 15 && (Pin_17__MASK & 0xC0))

/*******************************************************************************
* Function Name: Pin_17_Write
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
void Pin_17_Write(uint8 value) 
{
    uint8 staticBits = Pin_17_DR & ~Pin_17_MASK;
    Pin_17_DR = staticBits | ((value << Pin_17_SHIFT) & Pin_17_MASK);
}


/*******************************************************************************
* Function Name: Pin_17_SetDriveMode
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
void Pin_17_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_17_0, mode);
}


/*******************************************************************************
* Function Name: Pin_17_Read
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
*  Macro Pin_17_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_17_Read(void) 
{
    return (Pin_17_PS & Pin_17_MASK) >> Pin_17_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_17_ReadDataReg
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
uint8 Pin_17_ReadDataReg(void) 
{
    return (Pin_17_DR & Pin_17_MASK) >> Pin_17_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_17_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_17_ClearInterrupt
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
    uint8 Pin_17_ClearInterrupt(void) 
    {
        return (Pin_17_INTSTAT & Pin_17_MASK) >> Pin_17_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif
/* [] END OF FILE */ 
