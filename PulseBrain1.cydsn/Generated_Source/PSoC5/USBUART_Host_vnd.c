/*******************************************************************************
* File Name: USBUART_Host_vnd.c
* Version 2.30
*
* Description:
*  USB vendor request handler.
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

#if(USBUART_Host_EXTERN_VND == USBUART_Host_FALSE)


/***************************************
* Vendor Specific Declarations
***************************************/

/* `#START VENDOR_SPECIFIC_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* External References
***************************************/

uint8 USBUART_Host_InitControlRead(void) ;
uint8 USBUART_Host_InitControlWrite(void) ;


extern uint8 CYCODE USBUART_Host_MSOS_CONFIGURATION_DESCR[];

extern volatile T_USBUART_Host_TD USBUART_Host_currentTD;


/*******************************************************************************
* Function Name: USBUART_Host_HandleVendorRqst
********************************************************************************
*
* Summary:
*  This routine provide users with a method to implement vendor specifc
*  requests.
*
*  To implement vendor specific requests, add your code in this function to
*  decode and disposition the request.  If the request is handled, your code
*  must set the variable "requestHandled" to TRUE, indicating that the
*  request has been handled.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_Host_HandleVendorRqst(void) 
{
    uint8 requestHandled = USBUART_Host_FALSE;

    if ((CY_GET_REG8(USBUART_Host_bmRequestType) & USBUART_Host_RQST_DIR_MASK) == USBUART_Host_RQST_DIR_D2H)
    {
        /* Control Read */
        switch (CY_GET_REG8(USBUART_Host_bRequest))
        {
            case USBUART_Host_GET_EXTENDED_CONFIG_DESCRIPTOR:
                #if defined(USBUART_Host_ENABLE_MSOS_STRING)
                    USBUART_Host_currentTD.pData = &USBUART_Host_MSOS_CONFIGURATION_DESCR[0u];
                    USBUART_Host_currentTD.count = USBUART_Host_MSOS_CONFIGURATION_DESCR[0u];
                    requestHandled  = USBUART_Host_InitControlRead();
                #endif /* End USBUART_Host_ENABLE_MSOS_STRING */
                break;
            default:
                break;
        }
    }

    /* `#START VENDOR_SPECIFIC_CODE` Place your vendor specific request here */

    /* `#END` */

    return(requestHandled);
}


/*******************************************************************************
* Additional user functions supporting Vendor Specific Requests
********************************************************************************/

/* `#START VENDOR_SPECIFIC_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif /* USBUART_Host_EXTERN_VND */


/* [] END OF FILE */
