/*******************************************************************************
* File Name: USBUART_Host_cdc.h
* Version 2.30
*
* Description:
*  Header File for the USFS component.
*  Contains CDC class prototypes and constant values.
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(USBUART_Host_cdc_H)
#define USBUART_Host_cdc_H

#include "cytypes.h"


/***************************************
*  Memory Union Definitions
***************************************/
/*Pointer type conversions are restricted to avoid conflicts with compiler
optimizations. */
typedef union _USBUART_Host_ConvertDword
{
    volatile uint8 bytes[4];
    uint32 dword;
} USBUART_Host_CONVERT_DWORD;


/***************************************
* Prototypes of the $INSTANCE_NAME` API.
***************************************/

#if (USBUART_Host_ENABLE_CDC_CLASS_API != 0u)
    void USBUART_Host_CDC_Init(void) ;
    void USBUART_Host_PutData(uint8* pData, uint16 length) ;
    void USBUART_Host_PutString(char8* string) ;
    void USBUART_Host_PutChar(char8 txDataByte) ;
    void USBUART_Host_PutCRLF(void) ;
    uint16 USBUART_Host_GetCount(void) ;
    uint8 USBUART_Host_CDCIsReady(void) ;
    uint8 USBUART_Host_DataIsReady(void) ;
    uint16 USBUART_Host_GetData(uint8* pData, uint16 length) ;
    uint16 USBUART_Host_GetAll(uint8* pData) ;
    uint8 USBUART_Host_GetChar(void) ;
    uint8 USBUART_Host_IsLineChanged(void) ;
    uint32 USBUART_Host_GetDTERate(void) ;
    uint8 USBUART_Host_GetCharFormat(void) ;
    uint8 USBUART_Host_GetParityType(void) ;
    uint8 USBUART_Host_GetDataBits(void) ;
    uint16 USBUART_Host_GetLineControl(void) ;
#endif  /* End USBUART_Host_ENABLE_CDC_CLASS_API*/


/***************************************
*  Constants for $INSTANCE_NAME` API.
***************************************/

/* CDC Class-Specific Request Codes (CDC ver 1.2 Table 19) */
#define USBUART_Host_CDC_SET_LINE_CODING        (0x20u)
#define USBUART_Host_CDC_GET_LINE_CODING        (0x21u)
#define USBUART_Host_CDC_SET_CONTROL_LINE_STATE (0x22u)

#define USBUART_Host_LINE_CODING_CHANGED        (0x01u)
#define USBUART_Host_LINE_CONTROL_CHANGED       (0x02u)

#define USBUART_Host_1_STOPBIT                  (0x00u)
#define USBUART_Host_1_5_STOPBITS               (0x01u)
#define USBUART_Host_2_STOPBITS                 (0x02u)

#define USBUART_Host_PARITY_NONE                (0x00u)
#define USBUART_Host_PARITY_ODD                 (0x01u)
#define USBUART_Host_PARITY_EVEN                (0x02u)
#define USBUART_Host_PARITY_MARK                (0x03u)
#define USBUART_Host_PARITY_SPACE               (0x04u)

#define USBUART_Host_LINE_CODING_SIZE           (0x07u)
#define USBUART_Host_LINE_CODING_RATE           (0x00u)
#define USBUART_Host_LINE_CODING_STOP_BITS      (0x04u)
#define USBUART_Host_LINE_CODING_PARITY         (0x05u)
#define USBUART_Host_LINE_CODING_DATA_BITS      (0x06u)

#define USBUART_Host_LINE_CONTROL_DTR           (0x01u)
#define USBUART_Host_LINE_CONTROL_RTS           (0x02u)

#endif /* End USBUART_Host_cdc_H */


/* [] END OF FILE */
