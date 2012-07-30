/*******************************************************************************
* File Name: USBUART_1_cdc.h
* Version 2.12
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

#if !defined(USBUART_1_cdc_H)
#define USBUART_1_cdc_H

#include "cytypes.h"


/***************************************
*  Memory Union Definitions
***************************************/
/*Pointer type conversions are restricted to avoid conflicts with compiler
optimizations. */
typedef union _USBUART_1_ConvertDword
{
    volatile uint8 bytes[4];
    uint32 dword;
} USBUART_1_CONVERT_DWORD;


/***************************************
* Prototypes of the $INSTANCE_NAME` API.
***************************************/

#if (USBUART_1_ENABLE_CDC_CLASS_API != 0u)
    void USBUART_1_CDC_Init(void) ;
    void USBUART_1_PutData(uint8* pData, uint16 length) ;
    void USBUART_1_PutString(char8* string) ;
    void USBUART_1_PutChar(char8 txDataByte) ;
    void USBUART_1_PutCRLF(void) ;
    uint16 USBUART_1_GetCount(void) ;
    uint8 USBUART_1_CDCIsReady(void) ;
    uint8 USBUART_1_DataIsReady(void) ;
    uint16 USBUART_1_GetData(uint8* pData, uint16 length) ;
    uint16 USBUART_1_GetAll(uint8* pData) ;
    uint8 USBUART_1_GetChar(void) ;
    uint8 USBUART_1_IsLineChanged(void) ;
    uint32 USBUART_1_GetDTERate(void) ;
    uint8 USBUART_1_GetCharFormat(void) ;
    uint8 USBUART_1_GetParityType(void) ;
    uint8 USBUART_1_GetDataBits(void) ;
    uint16 USBUART_1_GetLineControl(void) ;
#endif  /* End USBUART_1_ENABLE_CDC_CLASS_API*/


/***************************************
*  Constants for $INSTANCE_NAME` API.
***************************************/

/* CDC Class-Specific Request Codes (CDC ver 1.2 Table 19) */
#define USBUART_1_CDC_SET_LINE_CODING        (0x20u)
#define USBUART_1_CDC_GET_LINE_CODING        (0x21u)
#define USBUART_1_CDC_SET_CONTROL_LINE_STATE (0x22u)

#define USBUART_1_LINE_CODING_CHANGED        (0x01u)
#define USBUART_1_LINE_CONTROL_CHANGED       (0x02u)

#define USBUART_1_1_STOPBIT                  (0x00u)
#define USBUART_1_1_5_STOPBITS               (0x01u)
#define USBUART_1_2_STOPBITS                 (0x02u)

#define USBUART_1_PARITY_NONE                (0x00u)
#define USBUART_1_PARITY_ODD                 (0x01u)
#define USBUART_1_PARITY_EVEN                (0x02u)
#define USBUART_1_PARITY_MARK                (0x03u)
#define USBUART_1_PARITY_SPACE               (0x04u)

#define USBUART_1_LINE_CODING_SIZE           (0x07u)
#define USBUART_1_LINE_CODING_RATE           (0x00u)
#define USBUART_1_LINE_CODING_STOP_BITS      (0x04u)
#define USBUART_1_LINE_CODING_PARITY         (0x05u)
#define USBUART_1_LINE_CODING_DATA_BITS      (0x06u)

#define USBUART_1_LINE_CONTROL_DTR           (0x01u)
#define USBUART_1_LINE_CONTROL_RTS           (0x02u)

#endif /* End USBUART_1_cdc_H */


/* [] END OF FILE */
