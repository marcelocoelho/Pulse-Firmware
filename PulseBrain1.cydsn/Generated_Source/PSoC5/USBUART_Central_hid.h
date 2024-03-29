/*******************************************************************************
* File Name: USBUART_Central_hid.h
* Version 2.50
*
* Description:
*  Header File for the USFS component. Contains prototypes and constant values.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(USBUART_Central_hid_H)
#define USBUART_Central_hid_H

#include "cytypes.h"


/***************************************
* Prototypes of the $INSTANCE_NAME` API.
***************************************/

uint8 USBUART_Central_UpdateHIDTimer(uint8 interface) ;
uint8 USBUART_Central_GetProtocol(uint8 interface) ;


/***************************************
*Renamed Functions for backward compatible
***************************************/

#define USBUART_Central_bGetProtocol               USBUART_Central_GetProtocol


/***************************************
*  Constants for $INSTANCE_NAME` API.
***************************************/

#define USBUART_Central_PROTOCOL_BOOT              (0x00u)
#define USBUART_Central_PROTOCOL_REPORT            (0x01u)

/* Request Types (HID Chapter 7.2) */
#define USBUART_Central_HID_GET_REPORT             (0x01u)
#define USBUART_Central_HID_GET_IDLE               (0x02u)
#define USBUART_Central_HID_GET_PROTOCOL           (0x03u)
#define USBUART_Central_HID_SET_REPORT             (0x09u)
#define USBUART_Central_HID_SET_IDLE               (0x0Au)
#define USBUART_Central_HID_SET_PROTOCOL           (0x0Bu)

/* Descriptor Types (HID Chapter 7.1) */
#define USBUART_Central_DESCR_HID_CLASS            (0x21u)
#define USBUART_Central_DESCR_HID_REPORT           (0x22u)
#define USBUART_Central_DESCR_HID_PHYSICAL         (0x23u)

/* Report Request Types (HID Chapter 7.2.1) */
#define USBUART_Central_HID_GET_REPORT_INPUT       (0x01u)
#define USBUART_Central_HID_GET_REPORT_OUTPUT      (0x02u)
#define USBUART_Central_HID_GET_REPORT_FEATURE     (0x03u)

#endif /* End USBUART_Central_hid_H */


/* [] END OF FILE */
