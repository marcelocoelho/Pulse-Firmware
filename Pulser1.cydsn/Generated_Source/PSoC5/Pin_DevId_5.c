/*******************************************************************************
* File Name: Pin_DevId_5.c  
* Version 1.60
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
#include "Pin_DevId_5.h"

/* APIs are not generated for P15[7:6] */
#if !(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 &&\
     CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_PRODUCTION &&\
	 Pin_DevId_5__PORT == 15 && (Pin_DevId_5__MASK & 0xC0))

/*******************************************************************************
* Function Name: Pin_DevId_5_Write
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
void Pin_DevId_5_Write(uint8 value) 
{
    uint8 staticBits = Pin_DevId_5_DR & ~Pin_DevId_5_MASK;
    Pin_DevId_5_DR = staticBits | ((value << Pin_DevId_5_SHIFT) & Pin_DevId_5_MASK);
}


/*******************************************************************************
* Function Name: Pin_DevId_5_SetDriveMode
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
void Pin_DevId_5_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_DevId_5_0, mode);
}


/*******************************************************************************
* Function Name: Pin_DevId_5_Read
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
*  Macro Pin_DevId_5_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_DevId_5_Read(void) 
{
    return (Pin_DevId_5_PS & Pin_DevId_5_MASK) >> Pin_DevId_5_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_DevId_5_ReadDataReg
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
uint8 Pin_DevId_5_ReadDataReg(void) 
{
    return (Pin_DevId_5_DR & Pin_DevId_5_MASK) >> Pin_DevId_5_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_DevId_5_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_DevId_5_ClearInterrupt
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
    uint8 Pin_DevId_5_ClearInterrupt(void) 
    {
        return (Pin_DevId_5_INTSTAT & Pin_DevId_5_MASK) >> Pin_DevId_5_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif
/* [] END OF FILE */ 
