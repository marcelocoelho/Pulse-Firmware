/*******************************************************************************
* File Name: Pin_Rx_Net_Enable.c  
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
#include "Pin_Rx_Net_Enable.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_Rx_Net_Enable__PORT == 15 && (Pin_Rx_Net_Enable__MASK & 0xC0))

/*******************************************************************************
* Function Name: Pin_Rx_Net_Enable_Write
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
void Pin_Rx_Net_Enable_Write(uint8 value) 
{
    uint8 staticBits = Pin_Rx_Net_Enable_DR & ~Pin_Rx_Net_Enable_MASK;
    Pin_Rx_Net_Enable_DR = staticBits | ((value << Pin_Rx_Net_Enable_SHIFT) & Pin_Rx_Net_Enable_MASK);
}


/*******************************************************************************
* Function Name: Pin_Rx_Net_Enable_SetDriveMode
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
void Pin_Rx_Net_Enable_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_Rx_Net_Enable_0, mode);
}


/*******************************************************************************
* Function Name: Pin_Rx_Net_Enable_Read
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
*  Macro Pin_Rx_Net_Enable_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_Rx_Net_Enable_Read(void) 
{
    return (Pin_Rx_Net_Enable_PS & Pin_Rx_Net_Enable_MASK) >> Pin_Rx_Net_Enable_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_Rx_Net_Enable_ReadDataReg
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
uint8 Pin_Rx_Net_Enable_ReadDataReg(void) 
{
    return (Pin_Rx_Net_Enable_DR & Pin_Rx_Net_Enable_MASK) >> Pin_Rx_Net_Enable_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_Rx_Net_Enable_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_Rx_Net_Enable_ClearInterrupt
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
    uint8 Pin_Rx_Net_Enable_ClearInterrupt(void) 
    {
        return (Pin_Rx_Net_Enable_INTSTAT & Pin_Rx_Net_Enable_MASK) >> Pin_Rx_Net_Enable_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif
/* [] END OF FILE */ 
