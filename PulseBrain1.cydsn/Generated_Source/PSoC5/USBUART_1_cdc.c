/*******************************************************************************
* File Name: USBUART_1_cdc.c
* Version 2.30
*
* Description:
*  USB HID Class request handler.
*
* Note:
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBUART_1.h"

#if defined(USBUART_1_ENABLE_CDC_CLASS)

#include <string.h>
#include "USBUART_1_cdc.h"


/***************************************
*    CDC Variables
***************************************/

volatile uint8 USBUART_1_lineCoding[USBUART_1_LINE_CODING_SIZE];
volatile uint8 USBUART_1_lineChanged;
volatile uint16 USBUART_1_lineControlBitmap;
volatile uint8 USBUART_1_cdc_data_in_ep;
volatile uint8 USBUART_1_cdc_data_out_ep;


/***************************************
* Custom Declarations
***************************************/

/* `#START CDC_CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* External data references
***************************************/

extern volatile T_USBUART_1_EP_CTL_BLOCK USBUART_1_EP[];
extern volatile T_USBUART_1_TD USBUART_1_currentTD;
extern volatile uint8 USBUART_1_transferState;


/***************************************
* External function references
***************************************/

uint8 USBUART_1_InitControlRead(void) ;
uint8 USBUART_1_InitControlWrite(void) ;
uint8 USBUART_1_InitNoDataControlTransfer(void) ;


/*******************************************************************************
* Function Name: USBUART_1_DispatchCDCClassRqst
********************************************************************************
*
* Summary:
*  This routine dispatches CDC class requests.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled
*
* Global variables:
*   USBUART_1_lineCoding: Contains the current line coding structure.
*     It is set by the Host using SET_LINE_CODING request and returned to the
*     user code by the USBFS_GetDTERate(), USBFS_GetCharFormat(),
*     USBFS_GetParityType(), USBFS_GetDataBits() APIs.
*   USBUART_1_lineControlBitmap: Contains the current control signal
*     bitmap. It is set by the Host using SET_CONTROL_LINE request and returned
*     to the user code by the USBFS_GetLineControl() API.
*   USBUART_1_lineChanged: This variable is used as a flag for the
*     USBFS_IsLineChanged() API, to be aware that Host has been sent request
*     for changing Line Coding or Control Bitmap.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_1_DispatchCDCClassRqst() 
{
    uint8 requestHandled = USBUART_1_FALSE;

    if ((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_DIR_MASK) == USBUART_1_RQST_DIR_D2H)
    {   /* Control Read */
        switch (CY_GET_REG8(USBUART_1_bRequest))
        {
            case USBUART_1_CDC_GET_LINE_CODING:
                USBUART_1_currentTD.count = USBUART_1_LINE_CODING_SIZE;
                USBUART_1_currentTD.pData = USBUART_1_lineCoding;
                requestHandled  = USBUART_1_InitControlRead();
                break;

            /* `#START CDC_READ_REQUESTS` Place other request handler here */

            /* `#END` */

            default:    /* requestHandled is initialezed as FALSE by default */
                break;
        }
    }
    else if ((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_DIR_MASK) == \
                                                                            USBUART_1_RQST_DIR_H2D)
    {   /* Control Write */
        switch (CY_GET_REG8(USBUART_1_bRequest))
        {
            case USBUART_1_CDC_SET_LINE_CODING:
                USBUART_1_currentTD.count = USBUART_1_LINE_CODING_SIZE;
                USBUART_1_currentTD.pData = USBUART_1_lineCoding;
                USBUART_1_lineChanged |= USBUART_1_LINE_CODING_CHANGED;
                requestHandled = USBUART_1_InitControlWrite();
                break;

            case USBUART_1_CDC_SET_CONTROL_LINE_STATE:
                USBUART_1_lineControlBitmap = CY_GET_REG8(USBUART_1_wValueLo);
                USBUART_1_lineChanged |= USBUART_1_LINE_CONTROL_CHANGED;
                requestHandled = USBUART_1_InitNoDataControlTransfer();
                break;

            /* `#START CDC_WRITE_REQUESTS` Place other request handler here */

            /* `#END` */

            default:    /* requestHandled is initialezed as FALSE by default */
                break;
        }
    }
    else
    {   /* requestHandled is initialezed as FALSE by default */
    }

    return(requestHandled);
}


/***************************************
* Optional CDC APIs
***************************************/
#if (USBUART_1_ENABLE_CDC_CLASS_API != 0u)


    /*******************************************************************************
    * Function Name: USBUART_1_CDC_Init
    ********************************************************************************
    *
    * Summary:
    *  This function initialize the CDC interface to be ready for the receive data
    *  from the PC.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_1_lineChanged: Initialized to zero.
    *   USBUART_1_cdc_data_out_ep: Used as an OUT endpoint number.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_1_CDC_Init(void) 
    {
        USBUART_1_lineChanged = 0u;
        USBUART_1_EnableOutEP(USBUART_1_cdc_data_out_ep);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_PutData
    ********************************************************************************
    *
    * Summary:
    *  Sends a specified number of bytes from the location specified by a
    *  pointer to the PC.
    *
    * Parameters:
    *  pData: pointer to the buffer containing data to be sent.
    *  length: Specifies the number of bytes to send from the pData
    *  buffer. Maximum length will be limited by the maximum packet
    *  size for the endpoint.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_1_cdc_data_in_ep: CDC IN endpoint number used for sending
    *     data.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_1_PutData(uint8* pData, uint16 length) 
    {
        /* Limits length to maximim packet size for the EP */
        if(length > USBUART_1_EP[USBUART_1_cdc_data_in_ep].bufferSize)
        {
            length = USBUART_1_EP[USBUART_1_cdc_data_in_ep].bufferSize;
        }
        USBUART_1_LoadInEP(USBUART_1_cdc_data_in_ep, pData, length);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a null terminated string to the PC.
    *
    * Parameters:
    *  string: pointer to the string to be sent to the PC
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_1_cdc_data_in_ep: CDC IN endpoint number used for sending
    *     data.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  This function will block if there is not enough memory to place the whole
    *  string, it will block until the entire string has been written to the
    *  transmit buffer.
    *
    *******************************************************************************/
    void USBUART_1_PutString(char8* string) 
    {
        uint16 str_length;
        uint16 send_length;

        /* Get length of the null terminated string */
        str_length = strlen(string);
        do
        {
            /* Limits length to maximim packet size for the EP */
            send_length = (str_length > USBUART_1_EP[USBUART_1_cdc_data_in_ep].bufferSize) ?
                          USBUART_1_EP[USBUART_1_cdc_data_in_ep].bufferSize : str_length;
             /* Enable IN transfer */
            USBUART_1_LoadInEP(USBUART_1_cdc_data_in_ep, (uint8 *)string, send_length);
            str_length -= send_length;

            /* If more data are present to send */
            if(str_length > 0)
            {
                string += send_length;
                /* Wait for the Host to read it. */
                while(USBUART_1_EP[USBUART_1_cdc_data_in_ep].apiEpState ==
                                          USBUART_1_IN_BUFFER_FULL);
            }
        }while(str_length > 0);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Writes a single character to the PC.
    *
    * Parameters:
    *  txDataByte: Character to be sent to the PC.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_1_cdc_data_in_ep: CDC IN endpoint number used for sending
    *     data.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_1_PutChar(char8 txDataByte) 
    {
        USBUART_1_LoadInEP(USBUART_1_cdc_data_in_ep, (uint8 *)&txDataByte, 1u);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Sends a carriage return (0x0D) and line feed (0x0A) to the PC
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_1_cdc_data_in_ep: CDC IN endpoint number used for sending
    *     data.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_1_PutCRLF(void) 
    {
        const uint8 CYCODE txData[] = {0x0Du, 0x0Au};

        USBUART_1_LoadInEP(USBUART_1_cdc_data_in_ep, (uint8 *)txData, 2u);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_GetCount
    ********************************************************************************
    *
    * Summary:
    *  This function returns the number of bytes that were received from the PC.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns the number of received bytes.
    *
    * Global variables:
    *   USBUART_1_cdc_data_out_ep: CDC OUT endpoint number used.
    *
    *******************************************************************************/
    uint16 USBUART_1_GetCount(void) 
    {
        uint8 bytesCount = 0u;

        if (USBUART_1_EP[USBUART_1_cdc_data_out_ep].apiEpState == USBUART_1_OUT_BUFFER_FULL)
        {
            bytesCount = USBUART_1_GetEPCount(USBUART_1_cdc_data_out_ep);
        }

        return(bytesCount);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_DataIsReady
    ********************************************************************************
    *
    * Summary:
    *  Returns a nonzero value if the component received data or received
    *  zero-length packet. The GetAll() or GetData() API should be called to read
    *  data from the buffer and reinit OUT endpoint even when zero-length packet
    *  received.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  If the OUT packet received this function returns a nonzero value.
    *  Otherwise zero is returned.
    *
    * Global variables:
    *   USBUART_1_cdc_data_out_ep: CDC OUT endpoint number used.
    *
    *******************************************************************************/
    uint8 USBUART_1_DataIsReady(void) 
    {
        return(USBUART_1_EP[USBUART_1_cdc_data_out_ep].apiEpState);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_CDCIsReady
    ********************************************************************************
    *
    * Summary:
    *  Returns a nonzero value if the component is ready to send more data to the
    *  PC. Otherwise returns zero. Should be called before sending new data to
    *  ensure the previous data has finished sending.This function returns the
    *  number of bytes that were received from the PC.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  If the buffer can accept new data then this function returns a nonzero value.
    *  Otherwise zero is returned.
    *
    * Global variables:
    *   USBUART_1_cdc_data_in_ep: CDC IN endpoint number used.
    *
    *******************************************************************************/
    uint8 USBUART_1_CDCIsReady(void) 
    {
        return(USBUART_1_EP[USBUART_1_cdc_data_in_ep].apiEpState);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_GetData
    ********************************************************************************
    *
    * Summary:
    *  Gets a specified number of bytes from the input buffer and places it in a
    *  data array specified by the passed pointer.
    *  USBUART_1_DataIsReady() API should be called before, to be sure
    *  that data is received from the Host.
    *
    * Parameters:
    *  pData: Pointer to the data array where data will be placed.
    *  Length: Number of bytes to read into the data array from the RX buffer.
    *          Maximum length is limited by the the number of received bytes.
    *
    * Return:
    *  Number of bytes received.
    *
    * Global variables:
    *   USBUART_1_cdc_data_out_ep: CDC OUT endpoint number used.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 USBUART_1_GetData(uint8* pData, uint16 length) 
    {
        return(USBUART_1_ReadOutEP(USBUART_1_cdc_data_out_ep, pData, length));
    }


    /*******************************************************************************
    * Function Name: USBUART_1_GetAll
    ********************************************************************************
    *
    * Summary:
    *  Gets all bytes of received data from the input buffer and places it into a
    *  specified data array. USBUART_1_DataIsReady() API should be called
    *  before, to be sure that data is received from the Host.
    *
    * Parameters:
    *  pData: Pointer to the data array where data will be placed.
    *
    * Return:
    *  Number of bytes received.
    *
    * Global variables:
    *   USBUART_1_cdc_data_out_ep: CDC OUT endpoint number used.
    *   USBUART_1_EP[].bufferSize: EP max packet size is used as a length
    *     to read all data from the EP buffer.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 USBUART_1_GetAll(uint8* pData) 
    {
        return (USBUART_1_ReadOutEP(USBUART_1_cdc_data_out_ep, pData,
                                           USBUART_1_EP[USBUART_1_cdc_data_out_ep].bufferSize));
    }


    /*******************************************************************************
    * Function Name: USBUART_1_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Reads one byte of received data from the buffer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received one character.
    *
    * Global variables:
    *   USBUART_1_cdc_data_out_ep: CDC OUT endpoint number used.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 USBUART_1_GetChar(void) 
    {
         uint8 rxData;

        USBUART_1_ReadOutEP(USBUART_1_cdc_data_out_ep, &rxData, 1u);

        return(rxData);
    }

    /*******************************************************************************
    * Function Name: USBUART_1_IsLineChanged
    ********************************************************************************
    *
    * Summary:
    *  This function returns clear on read status of the line.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  If SET_LINE_CODING or CDC_SET_CONTROL_LINE_STATE request received then not
    *  zero value returned. Otherwise zero is returned.
    *
    * Global variables:
    *  USBUART_1_transferState - it is checked to be sure then OUT data
    *    phase has been compleate, and data written to the lineCoding or Control
    *    Bitmap buffer.
    *  USBUART_1_lineChanged: used as a flag to be aware that Host has been
    *    sent request for changing Line Coding or Control Bitmap.
    *
    *******************************************************************************/
    uint8 USBUART_1_IsLineChanged(void) 
    {
        uint8 state = 0u;
        /* transferState is checked to be sure then OUT data phase has been compleate */
        if((USBUART_1_lineChanged != 0u) &&
           (USBUART_1_transferState == USBUART_1_TRANS_STATE_IDLE))
        {
            state = USBUART_1_lineChanged;
            USBUART_1_lineChanged = 0u;
        }
        return(state);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_GetDTERate
    ********************************************************************************
    *
    * Summary:
    *  Returns the data terminal rate set for this port in bits per second.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns a uint32 value of the data rate in bits per second.
    *
    * Global variables:
    *  USBUART_1_lineCoding: First four bytes converted to uint32
    *    depend on compiler, and returned as a data rate.
    *
    *******************************************************************************/
    uint32 USBUART_1_GetDTERate(void) 
    {
        uint32 rate;
        /* Data terminal rate has little endian format. */
        #if defined(__C51__)
            /*   KEIL for the 8051 is a Big Endian compiler. This requires four bytes swapping. */
            ((uint8 *)&rate)[0u] = USBUART_1_lineCoding[USBUART_1_LINE_CODING_RATE + 3u];
            ((uint8 *)&rate)[1u] = USBUART_1_lineCoding[USBUART_1_LINE_CODING_RATE + 2u];
            ((uint8 *)&rate)[2u] = USBUART_1_lineCoding[USBUART_1_LINE_CODING_RATE + 1u];
            ((uint8 *)&rate)[3u] = USBUART_1_lineCoding[USBUART_1_LINE_CODING_RATE];
        #elif defined(__GNUC__) || defined(__ARMCC_VERSION)
            /* ARM compilers (GCC and RealView) are little-endian */
            USBUART_1_CONVERT_DWORD *convert =
                (USBUART_1_CONVERT_DWORD *)&USBUART_1_lineCoding[USBUART_1_LINE_CODING_RATE];
            rate = convert->dword;
        #endif /* End Compillers */
        return(rate);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_GetCharFormat
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of stop bits.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns the number of stop bits.
    *
    * Global variables:
    *  USBUART_1_lineCoding: used to get a parameter.
    *
    *******************************************************************************/
    uint8 USBUART_1_GetCharFormat(void) 
    {
        return(USBUART_1_lineCoding[USBUART_1_LINE_CODING_STOP_BITS]);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_GetParityType
    ********************************************************************************
    *
    * Summary:
    *  Returns the parity type for the CDC port.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns the parity type.
    *
    * Global variables:
    *  USBUART_1_lineCoding: used to get a parameter.
    *
    *******************************************************************************/
    uint8 USBUART_1_GetParityType(void) 
    {
        return(USBUART_1_lineCoding[USBUART_1_LINE_CODING_PARITY]);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_GetDataBits
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of data bits for the CDC port.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns the number of data bits.
    *  The number of data bits can be 5, 6, 7, 8 or 16.
    *
    * Global variables:
    *  USBUART_1_lineCoding: used to get a parameter.
    *
    *******************************************************************************/
    uint8 USBUART_1_GetDataBits(void) 
    {
        return(USBUART_1_lineCoding[USBUART_1_LINE_CODING_DATA_BITS]);
    }


    /*******************************************************************************
    * Function Name: USBUART_1_GetLineControl
    ********************************************************************************
    *
    * Summary:
    *  Returns Line control bitmap.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns Line control bitmap.
    *
    * Global variables:
    *  USBUART_1_lineControlBitmap: used to get a parameter.
    *
    *******************************************************************************/
    uint16 USBUART_1_GetLineControl(void) 
    {
        return(USBUART_1_lineControlBitmap);
    }

#endif  /* End USBUART_1_ENABLE_CDC_CLASS_API*/


/*******************************************************************************
* Additional user functions supporting CDC Requests
********************************************************************************/

/* `#START CDC_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif  /* End USBUART_1_ENABLE_CDC_CLASS*/


/* [] END OF FILE */
