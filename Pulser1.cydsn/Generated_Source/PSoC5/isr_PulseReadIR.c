/*******************************************************************************
* File Name: isr_PulseReadIR.c  
* Version 1.50
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/


#include <CYDEVICE.H>
#include <CYDEVICE_TRM.H>
#include <CYLIB.H>
#include <isr_PulseReadIR.H>


/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START isr_PulseReadIR_intc` */

/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE	16
#endif
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);

/*******************************************************************************
* Function Name: isr_PulseReadIR_Start
********************************************************************************
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   void.
*
*
* Return:
*   void.
*
*******************************************************************************/
void isr_PulseReadIR_Start(void)
{
    /* For all we know the interrupt is active. */
    isr_PulseReadIR_Disable();

    /* Set the ISR to point to the isr_PulseReadIR Interrupt. */
    isr_PulseReadIR_SetVector(isr_PulseReadIR_Interrupt);

    /* Set the priority. */
    isr_PulseReadIR_SetPriority(isr_PulseReadIR_INTC_PRIOR_NUMBER);

    /* Enable it. */
    isr_PulseReadIR_Enable();
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_StartEx
********************************************************************************
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
*
* Return:
*   void.
*
*******************************************************************************/
void isr_PulseReadIR_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    isr_PulseReadIR_Disable();

    /* Set the ISR to point to the isr_PulseReadIR Interrupt. */
    isr_PulseReadIR_SetVector(address);

    /* Set the priority. */
    isr_PulseReadIR_SetPriority(isr_PulseReadIR_INTC_PRIOR_NUMBER);

    /* Enable it. */
    isr_PulseReadIR_Enable();
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_Stop
********************************************************************************
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*
*
* Return:
*   void.
*
*******************************************************************************/
void isr_PulseReadIR_Stop(void) 
{
    /* Disable this interrupt. */
    isr_PulseReadIR_Disable();

    /* Set the ISR to point to the passive one. */
    isr_PulseReadIR_SetVector(IntDefaultHandler);
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_Interrupt
********************************************************************************
* Summary:
*   The default Interrupt Service Routine for isr_PulseReadIR.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
*
*
* Parameters:  
*
*
* Return:
*   void.
*
*******************************************************************************/
CY_ISR(isr_PulseReadIR_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START isr_PulseReadIR_Interrupt` */
	extern void pulse_read_IR_handler();
	pulse_read_IR_handler();
    /* `#END` */
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_SetVector
********************************************************************************
* Summary:
*   Change the ISR vector for the Interrupt. Note calling isr_PulseReadIR_Start
*   will override any effect this method would have had. To set the vector before
*   the component has been started use isr_PulseReadIR_StartEx instead.
*
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
*
* Return:
*   void.
*
*
*******************************************************************************/
void isr_PulseReadIR_SetVector(cyisraddress address) 
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + isr_PulseReadIR__INTC_NUMBER] = address;
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_GetVector
********************************************************************************
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
*
* Parameters:
*   void.
*
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*
*******************************************************************************/
cyisraddress isr_PulseReadIR_GetVector(void) 
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + isr_PulseReadIR__INTC_NUMBER];
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_SetPriority
********************************************************************************
* Summary:
*   Sets the Priority of the Interrupt. Note calling isr_PulseReadIR_Start
*   or isr_PulseReadIR_StartEx will override any effect this method would have had. 
*	This method should only be called after isr_PulseReadIR_Start or 
*	isr_PulseReadIR_StartEx has been called. To set the initial
*	priority for the component use the cydwr file in the tool.
*
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
*
* Return:
*   void.
*
*
*******************************************************************************/
void isr_PulseReadIR_SetPriority(uint8 priority) 
{
    *isr_PulseReadIR_INTC_PRIOR = priority << 5;
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_GetPriority
********************************************************************************
* Summary:
*   Gets the Priority of the Interrupt.
*
*
* Parameters:
*   void.
*
*
* Return:
*   Priority of the interrupt. 0 - 7, 0 being the highest.
*
*
*******************************************************************************/
uint8 isr_PulseReadIR_GetPriority(void) 
{
    uint8 priority;


    priority = *isr_PulseReadIR_INTC_PRIOR >> 5;

    return priority;
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_Enable
********************************************************************************
* Summary:
*   Enables the interrupt.
*
*
* Parameters:
*   void.
*
*
* Return:
*   void.
*
*
*******************************************************************************/
void isr_PulseReadIR_Enable(void) 
{
    /* Enable the general interrupt. */
    *isr_PulseReadIR_INTC_SET_EN = isr_PulseReadIR__INTC_MASK;
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_GetState
********************************************************************************
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
*
* Parameters:
*   void.
*
*
* Return:
*   1 if enabled, 0 if disabled.
*
*
*******************************************************************************/
uint8 isr_PulseReadIR_GetState(void) 
{
    /* Get the state of the general interrupt. */
    return (*isr_PulseReadIR_INTC_SET_EN & isr_PulseReadIR__INTC_MASK) ? 1:0;
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_Disable
********************************************************************************
* Summary:
*   Disables the Interrupt.
*
*
* Parameters:
*   void.
*
*
* Return:
*   void.
*
*
*******************************************************************************/
void isr_PulseReadIR_Disable(void) 
{
    /* Disable the general interrupt. */
    *isr_PulseReadIR_INTC_CLR_EN = isr_PulseReadIR__INTC_MASK;
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_SetPending
********************************************************************************
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
*
* Parameters:
*   void.
*
*
* Return:
*   void.
*
*
*******************************************************************************/
void isr_PulseReadIR_SetPending(void) 
{
    *isr_PulseReadIR_INTC_SET_PD = isr_PulseReadIR__INTC_MASK;
}

/*******************************************************************************
* Function Name: isr_PulseReadIR_ClearPending
********************************************************************************
* Summary:
*   Clears a pending interrupt.
*
* Parameters:
*   void.
*
*
* Return:
*   void.
*
*
*******************************************************************************/
void isr_PulseReadIR_ClearPending(void) 
{
    *isr_PulseReadIR_INTC_CLR_PD = isr_PulseReadIR__INTC_MASK;
}
