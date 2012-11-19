/*******************************************************************************
* File Name: USBUART_Host_drv.c
* Version 2.30
*
* Description:
*  Endpoint 0 Driver for the USBFS Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBUART_Host.h"


/***************************************
* Forward references for the EP0 ISR
***************************************/

void  USBUART_Host_HandleSetup(void) ;
void  USBUART_Host_HandleIN(void) ;
void  USBUART_Host_HandleOUT(void) ;
uint8 USBUART_Host_InitControlRead(void) ;
void  USBUART_Host_ControlReadDataStage(void) ;
void  USBUART_Host_ControlReadStatusStage(void) ;
void  USBUART_Host_ControlReadPrematureStatus(void)
                                                ;
uint8 USBUART_Host_InitControlWrite(void) ;
uint8 USBUART_Host_InitZeroLengthControlTransfer(void)
                                                ;
void  USBUART_Host_ControlWriteDataStage(void) ;
void  USBUART_Host_ControlWriteStatusStage(void) ;
void  USBUART_Host_ControlWritePrematureStatus(void)
                                                ;
uint8 USBUART_Host_InitNoDataControlTransfer(void) ;
void  USBUART_Host_NoDataControlStatusStage(void) ;
void  USBUART_Host_InitializeStatusBlock(void) ;
void  USBUART_Host_UpdateStatusBlock(uint8 completionCode) ;


/***************************************
* Request Handlers
***************************************/

uint8 USBUART_Host_HandleStandardRqst(void) ;
uint8 USBUART_Host_DispatchClassRqst(void) ;
uint8 USBUART_Host_HandleVendorRqst(void) ;


/***************************************
* External data references
***************************************/


/***************************************
* Global data allocation
***************************************/

volatile T_USBUART_Host_EP_CTL_BLOCK USBUART_Host_EP[USBUART_Host_MAX_EP];
volatile uint8 USBUART_Host_configuration;
volatile uint8 USBUART_Host_configurationChanged;
volatile uint8 USBUART_Host_deviceAddress;
volatile uint8 USBUART_Host_deviceStatus;
volatile uint8 USBUART_Host_interfaceSetting[USBUART_Host_MAX_INTERFACES_NUMBER];
volatile uint8 USBUART_Host_interfaceSetting_last[USBUART_Host_MAX_INTERFACES_NUMBER];
volatile uint8 USBUART_Host_interfaceStatus[USBUART_Host_MAX_INTERFACES_NUMBER];
volatile uint8 USBUART_Host_device;
const uint8 CYCODE *USBUART_Host_interfaceClass;


/***************************************
* Local data allocation
***************************************/

volatile uint8 USBUART_Host_ep0Toggle;
volatile uint8 USBUART_Host_lastPacketSize;
volatile uint8 USBUART_Host_transferState;
volatile T_USBUART_Host_TD USBUART_Host_currentTD;
volatile uint8 USBUART_Host_ep0Mode;
volatile uint8 USBUART_Host_ep0Count;
volatile uint16 USBUART_Host_transferByteCount;


/*******************************************************************************
* Function Name: USBUART_Host_ep_0_Interrupt
********************************************************************************
*
* Summary:
*  This Interrupt Service Routine handles Endpoint 0 (Control Pipe) traffic.
*  It dispactches setup requests and handles the data and status stages.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USBUART_Host_EP_0_ISR)
{
    uint8 bRegTemp;
    uint8 modifyReg;


    bRegTemp = CY_GET_REG8(USBUART_Host_EP0_CR_PTR);
    if (bRegTemp & USBUART_Host_MODE_ACKD)
    {
        modifyReg = 1u;
        if (bRegTemp & USBUART_Host_MODE_SETUP_RCVD)
        {
            if((bRegTemp & USBUART_Host_MODE_MASK) != USBUART_Host_MODE_NAK_IN_OUT)
            {
                modifyReg = 0u;                                     /* When mode not NAK_IN_OUT => invalid setup */
            }
            else
            {
                USBUART_Host_HandleSetup();
                if(USBUART_Host_ep0Mode & USBUART_Host_MODE_SETUP_RCVD)
                {
                    modifyReg = 0u;                         /* if SETUP bit set -> exit without modifying the mode */
                }

            }
        }
        else if (bRegTemp & USBUART_Host_MODE_IN_RCVD)
        {
            USBUART_Host_HandleIN();
        }
        else if (bRegTemp & USBUART_Host_MODE_OUT_RCVD)
        {
            USBUART_Host_HandleOUT();
        }
        else
        {
            modifyReg = 0u;
        }
        if(modifyReg != 0u)
        {
            bRegTemp = CY_GET_REG8(USBUART_Host_EP0_CR_PTR);    /* unlock registers */
            if((bRegTemp & USBUART_Host_MODE_SETUP_RCVD) == 0)  /* Check if SETUP bit is not set, otherwise exit */
            {
                /* Update the count register */
                bRegTemp = USBUART_Host_ep0Toggle | USBUART_Host_ep0Count;
                CY_SET_REG8(USBUART_Host_EP0_CNT_PTR, bRegTemp);
                if(bRegTemp == CY_GET_REG8(USBUART_Host_EP0_CNT_PTR))   /* continue if writing was succesful */
                {
                    do
                    {
                        modifyReg = USBUART_Host_ep0Mode;       /* init temporary variable */
                        /* unlock registers */
                        bRegTemp = CY_GET_REG8(USBUART_Host_EP0_CR_PTR) & USBUART_Host_MODE_SETUP_RCVD;
                        if(bRegTemp == 0)                           /* Check if SETUP bit is not set */
                        {
                            /* Set the Mode Register  */
                            CY_SET_REG8(USBUART_Host_EP0_CR_PTR, USBUART_Host_ep0Mode);
                            /* writing check */
                            modifyReg = CY_GET_REG8(USBUART_Host_EP0_CR_PTR) & USBUART_Host_MODE_MASK;
                        }
                    }while(modifyReg != USBUART_Host_ep0Mode);  /* repeat if writing was not successful */
                }
            }
        }
    }
}


/*******************************************************************************
* Function Name: USBUART_Host_HandleSetup
********************************************************************************
*
* Summary:
*  This Routine dispatches requests for the four USB request types
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_HandleSetup(void) 
{
    uint8 requestHandled;

    requestHandled = CY_GET_REG8(USBUART_Host_EP0_CR_PTR);      /* unlock registers */
    CY_SET_REG8(USBUART_Host_EP0_CR_PTR, requestHandled);       /* clear setup bit */
    requestHandled = CY_GET_REG8(USBUART_Host_EP0_CR_PTR);      /* reread register */
    if(requestHandled & USBUART_Host_MODE_SETUP_RCVD)
    {
        USBUART_Host_ep0Mode = requestHandled;        /* if SETUP bit set -> exit without modifying the mode */
    }
    else
    {
        /* In case the previous transfer did not complete, close it out */
        USBUART_Host_UpdateStatusBlock(USBUART_Host_XFER_PREMATURE);

        switch (CY_GET_REG8(USBUART_Host_bmRequestType) & USBUART_Host_RQST_TYPE_MASK)
        {
            case USBUART_Host_RQST_TYPE_STD:
                requestHandled = USBUART_Host_HandleStandardRqst();
                break;
            case USBUART_Host_RQST_TYPE_CLS:
                requestHandled = USBUART_Host_DispatchClassRqst();
                break;
            case USBUART_Host_RQST_TYPE_VND:
                requestHandled = USBUART_Host_HandleVendorRqst();
                break;
            default:
                requestHandled = USBUART_Host_FALSE;
                break;
        }
        if (requestHandled == USBUART_Host_FALSE)
        {
            USBUART_Host_ep0Mode = USBUART_Host_MODE_STALL_IN_OUT;
        }
    }
}


/*******************************************************************************
* Function Name: USBUART_Host_HandleIN
********************************************************************************
*
* Summary:
*  This routine handles EP0 IN transfers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_HandleIN(void) 
{
    switch (USBUART_Host_transferState)
    {
        case USBUART_Host_TRANS_STATE_IDLE:
            break;
        case USBUART_Host_TRANS_STATE_CONTROL_READ:
            USBUART_Host_ControlReadDataStage();
            break;
        case USBUART_Host_TRANS_STATE_CONTROL_WRITE:
            USBUART_Host_ControlWriteStatusStage();
            break;
        case USBUART_Host_TRANS_STATE_NO_DATA_CONTROL:
            USBUART_Host_NoDataControlStatusStage();
            break;
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBUART_Host_HandleOUT
********************************************************************************
*
* Summary:
*  This routine handles EP0 OUT transfers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_HandleOUT(void) 
{
    switch (USBUART_Host_transferState)
    {
        case USBUART_Host_TRANS_STATE_IDLE:
            break;
        case USBUART_Host_TRANS_STATE_CONTROL_READ:
            USBUART_Host_ControlReadStatusStage();
            break;
        case USBUART_Host_TRANS_STATE_CONTROL_WRITE:
            USBUART_Host_ControlWriteDataStage();
            break;
        case USBUART_Host_TRANS_STATE_NO_DATA_CONTROL:
            /* Update the completion block */
            USBUART_Host_UpdateStatusBlock(USBUART_Host_XFER_ERROR);
            /* We expect no more data, so stall INs and OUTs */
            USBUART_Host_ep0Mode = USBUART_Host_MODE_STALL_IN_OUT;
            break;
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBUART_Host_LoadEP0
********************************************************************************
*
* Summary:
*  This routine loads the EP0 data registers for OUT transfers.  It uses the
*  currentTD (previously initialized by the _InitControlWrite function and
*  updated for each OUT transfer, and the bLastPacketSize) to determine how
*  many uint8s to transfer on the current OUT.
*
*  If the number of uint8s remaining is zero and the last transfer was full,
*  we need to send a zero length packet.  Otherwise we send the minimum
*  of the control endpoint size (8) or remaining number of uint8s for the
*  transaction.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_Host_transferByteCount - Update the transfer byte count from the
*     last transaction.
*  USBUART_Host_ep0Count - counts the data loaded to the SIE memory in
*     current packet.
*  USBUART_Host_lastPacketSize - remebers the USBFS_ep0Count value for the
*     next packet.
*  USBUART_Host_transferByteCount - suma of the previos bytes transferred
*     on previous packets(suma of USBFS_lastPacketSize)
*  USBUART_Host_ep0Toggle - inverted
*  USBUART_Host_ep0Mode  - prepare for mode register content.
*  USBUART_Host_transferState - set to TRANS_STATE_CONTROL_READ
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_LoadEP0(void) 
{
    /* Update the transfer byte count from the last transaction */
    USBUART_Host_transferByteCount += USBUART_Host_lastPacketSize;
    /* Now load the next transaction */
    USBUART_Host_ep0Count = 0u;
    while ((USBUART_Host_currentTD.count > 0u) && (USBUART_Host_ep0Count < 8u))
    {
        USBUART_Host_EP0_DR0_PTR[USBUART_Host_ep0Count] = *USBUART_Host_currentTD.pData++;
        USBUART_Host_ep0Count++;
        USBUART_Host_currentTD.count--;
    }
    /* Support zero-length packet*/
    if( (USBUART_Host_ep0Count > 0u) || (USBUART_Host_lastPacketSize == 8u) )
    {
        /* Update the data toggle */
        USBUART_Host_ep0Toggle ^= USBUART_Host_EP0_CNT_DATA_TOGGLE;
        /* Set the Mode Register  */
        USBUART_Host_ep0Mode = USBUART_Host_MODE_ACK_IN_STATUS_OUT;
        /* Update the state (or stay the same) */
        USBUART_Host_transferState = USBUART_Host_TRANS_STATE_CONTROL_READ;
    }
    else
    {
        /* Expect Status Stage Out */
        USBUART_Host_ep0Mode = USBUART_Host_MODE_STATUS_OUT_ONLY;
        /* Update the state (or stay the same) */
        USBUART_Host_transferState = USBUART_Host_TRANS_STATE_CONTROL_READ;
    }

    /* Save the packet size for next time */
    USBUART_Host_lastPacketSize = USBUART_Host_ep0Count;
}


/*******************************************************************************
* Function Name: USBUART_Host_InitControlRead
********************************************************************************
*
* Summary:
*  Initialize a control read transaction, usable to send data to the host.
*  The following global variables shold be initialized before this function
*  called. To send zero length packet use InitZeroLengthControlTransfer
*  function.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USBUART_Host_currentTD.count - counts of data to be sent.
*  USBUART_Host_currentTD.pData - data pointer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_Host_InitControlRead(void) 
{
    uint16 xferCount;
    if(USBUART_Host_currentTD.count == 0)
    {
        USBUART_Host_InitZeroLengthControlTransfer();
    }
    else
    {
        /* Set up the state machine */
        USBUART_Host_transferState = USBUART_Host_TRANS_STATE_CONTROL_READ;
        /* Set the toggle, it gets updated in LoadEP */
        USBUART_Host_ep0Toggle = 0u;
        /* Initialize the Status Block */
        USBUART_Host_InitializeStatusBlock();
        xferCount = (((uint16)CY_GET_REG8(USBUART_Host_lengthHi) << 8u) | (CY_GET_REG8(USBUART_Host_lengthLo)));

        if (USBUART_Host_currentTD.count > xferCount)
        {
            USBUART_Host_currentTD.count = xferCount;
        }
        USBUART_Host_LoadEP0();
    }

    return(USBUART_Host_TRUE);
}


/*******************************************************************************
* Function Name: USBUART_Host_InitZeroLengthControlTransfer
********************************************************************************
*
* Summary:
*  Initialize a zero length data IN transfer.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USBUART_Host_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBUART_Host_ep0Mode  - prepare for mode register content.
*  USBUART_Host_transferState - set to TRANS_STATE_CONTROL_READ
*  USBUART_Host_ep0Count - cleared, means the zero-length packet.
*  USBUART_Host_lastPacketSize - cleared.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_Host_InitZeroLengthControlTransfer(void)
                                                
{
    /* Update the state */
    USBUART_Host_transferState = USBUART_Host_TRANS_STATE_CONTROL_READ;
    /* Set the data toggle */
    USBUART_Host_ep0Toggle = USBUART_Host_EP0_CNT_DATA_TOGGLE;
    /* Set the Mode Register  */
    USBUART_Host_ep0Mode = USBUART_Host_MODE_ACK_IN_STATUS_OUT;
    /* Save the packet size for next time */
    USBUART_Host_lastPacketSize = 0u;
    USBUART_Host_ep0Count = 0u;

    return(USBUART_Host_TRUE);
}


/*******************************************************************************
* Function Name: USBUART_Host_ControlReadDataStage
********************************************************************************
*
* Summary:
*  Handle the Data Stage of a control read transfer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_ControlReadDataStage(void) 

{
    USBUART_Host_LoadEP0();
}


/*******************************************************************************
* Function Name: USBUART_Host_ControlReadStatusStage
********************************************************************************
*
* Summary:
*  Handle the Status Stage of a control read transfer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_Host_USBFS_transferByteCount - updated with last packet size.
*  USBUART_Host_transferState - set to TRANS_STATE_IDLE.
*  USBUART_Host_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_ControlReadStatusStage(void) 
{
    /* Update the transfer byte count */
    USBUART_Host_transferByteCount += USBUART_Host_lastPacketSize;
    /* Go Idle */
    USBUART_Host_transferState = USBUART_Host_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBUART_Host_UpdateStatusBlock(USBUART_Host_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USBUART_Host_ep0Mode =  USBUART_Host_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBUART_Host_InitControlWrite
********************************************************************************
*
* Summary:
*  Initialize a control write transaction
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USBUART_Host_USBFS_transferState - set to TRANS_STATE_CONTROL_WRITE
*  USBUART_Host_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBUART_Host_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_Host_InitControlWrite(void) 
{
    uint16 xferCount;

    /* Set up the state machine */
    USBUART_Host_transferState = USBUART_Host_TRANS_STATE_CONTROL_WRITE;
    /* This migh not be necessary */
    USBUART_Host_ep0Toggle = USBUART_Host_EP0_CNT_DATA_TOGGLE;
    /* Initialize the Status Block */
    USBUART_Host_InitializeStatusBlock();

    xferCount = (((uint16)CY_GET_REG8(USBUART_Host_lengthHi) << 8u) | (CY_GET_REG8(USBUART_Host_lengthLo)));

    if (USBUART_Host_currentTD.count > xferCount)
    {
        USBUART_Host_currentTD.count = xferCount;
    }

    /* Expect Data or Status Stage */
    USBUART_Host_ep0Mode = USBUART_Host_MODE_ACK_OUT_STATUS_IN;

    return(USBUART_Host_TRUE);
}


/*******************************************************************************
* Function Name: USBUART_Host_ControlWriteDataStage
********************************************************************************
*
* Summary:
*  Handle the Data Stage of a control write transfer
*       1. Get the data (We assume the destination was validated previously)
*       2. Update the count and data toggle
*       3. Update the mode register for the next transaction
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_Host_transferByteCount - Update the transfer byte count from the
*    last transaction.
*  USBUART_Host_ep0Count - counts the data loaded from the SIE memory
*    in current packet.
*  USBUART_Host_transferByteCount - suma of the previos bytes transferred
*    on previous packets(suma of USBFS_lastPacketSize)
*  USBUART_Host_ep0Toggle - inverted
*  USBUART_Host_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_ControlWriteDataStage(void) 
{
    uint8 *pReg = (uint8 *)USBUART_Host_EP0_DR0_PTR;

    USBUART_Host_ep0Count = (CY_GET_REG8(USBUART_Host_EP0_CNT_PTR ) & 0x0Fu) - 2u;

    USBUART_Host_transferByteCount += USBUART_Host_ep0Count;

    while ((USBUART_Host_currentTD.count > 0u) && (USBUART_Host_ep0Count > 0u))
    {
        *USBUART_Host_currentTD.pData++ = CY_GET_REG8(pReg++);
        USBUART_Host_ep0Count--;
        USBUART_Host_currentTD.count--;
    }
    /* Update the data toggle */
    USBUART_Host_ep0Toggle ^= USBUART_Host_EP0_CNT_DATA_TOGGLE;
    /* Expect Data or Status Stage */
    USBUART_Host_ep0Mode = USBUART_Host_MODE_ACK_OUT_STATUS_IN;
}


/*******************************************************************************
* Function Name: USBUART_Host_ControlWriteStatusStage
********************************************************************************
*
* Summary:
*  Handle the Status Stage of a control write transfer
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_Host_transferState - set to TRANS_STATE_IDLE.
*  USBUART_Host_USBFS_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_ControlWriteStatusStage(void) 
{
    /* Go Idle */
    USBUART_Host_transferState = USBUART_Host_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBUART_Host_UpdateStatusBlock(USBUART_Host_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USBUART_Host_ep0Mode = USBUART_Host_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBUART_Host_InitNoDataControlTransfer
********************************************************************************
*
* Summary:
*  Initialize a no data control transfer
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USBUART_Host_transferState - set to TRANS_STATE_NO_DATA_CONTROL.
*  USBUART_Host_ep0Mode  - set to MODE_STATUS_IN_ONLY.
*  USBUART_Host_ep0Count - cleared.
*  USBUART_Host_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_Host_InitNoDataControlTransfer(void) 
{
    USBUART_Host_transferState = USBUART_Host_TRANS_STATE_NO_DATA_CONTROL;
    USBUART_Host_ep0Mode = USBUART_Host_MODE_STATUS_IN_ONLY;
    USBUART_Host_ep0Toggle = USBUART_Host_EP0_CNT_DATA_TOGGLE;
    USBUART_Host_ep0Count = 0u;

    return(USBUART_Host_TRUE);
}


/*******************************************************************************
* Function Name: USBUART_Host_NoDataControlStatusStage
********************************************************************************
* Summary:
*  Handle the Status Stage of a no data control transfer.
*
*  SET_ADDRESS is special, since we need to receive the status stage with
*  the old address.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_Host_transferState - set to TRANS_STATE_IDLE.
*  USBUART_Host_ep0Mode  - set to MODE_STALL_IN_OUT.
*  USBUART_Host_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBUART_Host_deviceAddress - used to set new address and cleared
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_NoDataControlStatusStage(void) 
{
    /* Change the USB address register if we got a SET_ADDRESS. */
    if (USBUART_Host_deviceAddress != 0u)
    {
        CY_SET_REG8(USBUART_Host_CR0_PTR, USBUART_Host_deviceAddress | USBUART_Host_CR0_ENABLE);
        USBUART_Host_deviceAddress = 0u;
    }
    /* Go Idle */
    USBUART_Host_transferState = USBUART_Host_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBUART_Host_UpdateStatusBlock(USBUART_Host_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USBUART_Host_ep0Mode = USBUART_Host_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBUART_Host_UpdateStatusBlock
********************************************************************************
*
* Summary:
*  Update the Completion Status Block for a Request.  The block is updated
*  with the completion code the USBUART_Host_transferByteCount.  The
*  StatusBlock Pointer is set to NULL.
*
* Parameters:
*  completionCode - status.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_Host_currentTD.pStatusBlock->status - updated by the 
*    completionCode parameter.
*  USBUART_Host_currentTD.pStatusBlock->length - updated.
*  USBUART_Host_currentTD.pStatusBlock - cleared.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_UpdateStatusBlock(uint8 completionCode) 
{
    if (USBUART_Host_currentTD.pStatusBlock != NULL)
    {
        USBUART_Host_currentTD.pStatusBlock->status = completionCode;
        USBUART_Host_currentTD.pStatusBlock->length = USBUART_Host_transferByteCount;
        USBUART_Host_currentTD.pStatusBlock = NULL;
    }
}


/*******************************************************************************
* Function Name: USBUART_Host_InitializeStatusBlock
********************************************************************************
*
* Summary:
*  Initialize the Completion Status Block for a Request.  The completion
*  code is set to USB_XFER_IDLE.
*
*  Also, initializes USBUART_Host_transferByteCount.  Save some space,
*  this is the only consumer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_Host_currentTD.pStatusBlock->status - set to XFER_IDLE.
*  USBUART_Host_currentTD.pStatusBlock->length - cleared.
*  USBUART_Host_transferByteCount - cleared.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_InitializeStatusBlock(void) 
{
    USBUART_Host_transferByteCount = 0u;
    if(USBUART_Host_currentTD.pStatusBlock != NULL)
    {
        USBUART_Host_currentTD.pStatusBlock->status = USBUART_Host_XFER_IDLE;
        USBUART_Host_currentTD.pStatusBlock->length = 0u;
    }
}


/* [] END OF FILE */
