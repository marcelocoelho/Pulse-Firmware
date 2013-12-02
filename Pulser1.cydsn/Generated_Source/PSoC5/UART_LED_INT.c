/*******************************************************************************
* File Name: UART_LED_INT.c
* Version 2.10
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "UART_LED.h"
#include "CyLib.h"


/***************************************
* Custom Declratations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if( (UART_LED_RX_ENABLED || UART_LED_HD_ENABLED) && \
     (UART_LED_RXBUFFERSIZE > UART_LED_FIFO_LENGTH))

    extern volatile uint8 UART_LED_rxBuffer[];
    extern volatile uint8 UART_LED_rxBufferRead;
    extern volatile uint8 UART_LED_rxBufferWrite;
    extern volatile uint8 UART_LED_rxBufferLoopDetect;
    extern volatile uint8 UART_LED_rxBufferOverflow;
    #if (UART_LED_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 UART_LED_rxAddressMode;
        extern volatile uint8 UART_LED_rxAddressDetected;
    #endif /* End EnableHWAddress */    

    /*******************************************************************************
    * Function Name: UART_LED_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_LED_rxBuffer - RAM buffer pointer for save received data.
    *  UART_LED_rxBufferWrite - cyclic index for write to rxBuffer, 
    *     increments after each byte saved to buffer.
    *  UART_LED_rxBufferRead - cyclic index for read from rxBuffer, 
    *     checked to detect overflow condition.
    *  UART_LED_rxBufferOverflow - software overflow flag. Set to one
    *     when UART_LED_rxBufferWrite index overtakes 
    *     UART_LED_rxBufferRead index.
    *  UART_LED_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when UART_LED_rxBufferWrite is equal to 
    *    UART_LED_rxBufferRead
    *  UART_LED_rxAddressMode - this variable contains the Address mode, 
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  UART_LED_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(UART_LED_RXISR)
    {
        uint8 readData;
        uint8 increment_pointer = 0;

        /* User code required at start of ISR */
        /* `#START UART_LED_RXISR_START` */

        /* `#END` */

        readData = UART_LED_RXSTATUS_REG;

        if((readData & (UART_LED_RX_STS_BREAK | UART_LED_RX_STS_PAR_ERROR |
                        UART_LED_RX_STS_STOP_ERROR | UART_LED_RX_STS_OVERRUN)) != 0)
        {
            /* ERROR handling. */
            /* `#START UART_LED_RXISR_ERROR` */

            /* `#END` */
        }

        while(readData & UART_LED_RX_STS_FIFO_NOTEMPTY)
        {
            
            #if (UART_LED_RXHW_ADDRESS_ENABLED)
                if(UART_LED_rxAddressMode == UART_LED__B_UART__AM_SW_DETECT_TO_BUFFER) 
                {
                    if((readData & UART_LED_RX_STS_MRKSPC) != 0u )
                    {  
                        if ((readData & UART_LED_RX_STS_ADDR_MATCH) != 0)
                        {
                            UART_LED_rxAddressDetected = 1u;
                        }
                        else
                        {
                            UART_LED_rxAddressDetected = 0u;
                        }
                    }

                    readData = UART_LED_RXDATA_REG;
                    if(UART_LED_rxAddressDetected != 0u)
                    {   /* store only addressed data */
                        UART_LED_rxBuffer[UART_LED_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* without software addressing */
                {
                    UART_LED_rxBuffer[UART_LED_rxBufferWrite] = UART_LED_RXDATA_REG;
                    increment_pointer = 1u;
                }
            #else  /* without addressing */
                UART_LED_rxBuffer[UART_LED_rxBufferWrite] = UART_LED_RXDATA_REG;
                increment_pointer = 1u;
            #endif /* End SW_DETECT_TO_BUFFER */
            
            /* do not increment buffer pointer when skip not adderessed data */
            if( increment_pointer != 0u )
            {
                if(UART_LED_rxBufferLoopDetect)
                {   /* Set Software Buffer status Overflow */
                    UART_LED_rxBufferOverflow = 1u;
                }
                /* Set next pointer. */
                UART_LED_rxBufferWrite++;

                /* Check pointer for a loop condition */
                if(UART_LED_rxBufferWrite >= UART_LED_RXBUFFERSIZE)
                {
                    UART_LED_rxBufferWrite = 0u;
                }
                /* Detect pre-overload condition and set flag */
                if(UART_LED_rxBufferWrite == UART_LED_rxBufferRead)
                {
                    UART_LED_rxBufferLoopDetect = 1u;
                    /* When Hardware Flow Control selected */
                    #if(UART_LED_FLOW_CONTROL != 0u)
                    /* Disable RX interrupt mask, it will be enabled when user read data from the buffer using APIs */
                        UART_LED_RXSTATUS_MASK_REG  &= ~UART_LED_RX_STS_FIFO_NOTEMPTY;
                        CyIntClearPending(UART_LED_RX_VECT_NUM); 
                        break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                    #endif /* End UART_LED_FLOW_CONTROL != 0 */    
                }
            }

            /* Check again if there is data. */
            readData = UART_LED_RXSTATUS_REG;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UART_LED_RXISR_END` */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (UART_LED_RXInternalInterrupt__ES2_PATCH))
            UART_LED_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif /* End UART_LED_RX_ENABLED && (UART_LED_RXBUFFERSIZE > UART_LED_FIFO_LENGTH) */


#if(UART_LED_TX_ENABLED && (UART_LED_TXBUFFERSIZE > UART_LED_FIFO_LENGTH))

    extern volatile uint8 UART_LED_txBuffer[];
    extern volatile uint8 UART_LED_txBufferRead;
    extern uint8 UART_LED_txBufferWrite;


    /*******************************************************************************
    * Function Name: UART_LED_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_LED_txBuffer - RAM buffer pointer for transmit data from.
    *  UART_LED_txBufferRead - cyclic index for read and transmit data 
    *     from txBuffer, increments after each transmited byte.
    *  UART_LED_rxBufferWrite - cyclic index for write to txBuffer, 
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(UART_LED_TXISR)
    {

        /* User code required at start of ISR */
        /* `#START UART_LED_TXISR_START` */

        /* `#END` */

        while((UART_LED_txBufferRead != UART_LED_txBufferWrite) && \
             !(UART_LED_TXSTATUS_REG & UART_LED_TX_STS_FIFO_FULL))
        {
            /* Check pointer. */
            if(UART_LED_txBufferRead >= UART_LED_TXBUFFERSIZE)
            {
                UART_LED_txBufferRead = 0u;
            }

            UART_LED_TXDATA_REG = UART_LED_txBuffer[UART_LED_txBufferRead];

            /* Set next pointer. */
            UART_LED_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UART_LED_TXISR_END` */

        /* `#END` */
        
        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(CY_PSOC3_ES2 && (UART_LED_TXInternalInterrupt__ES2_PATCH))
            UART_LED_ISR_PATCH();
        #endif /* End CY_PSOC3_ES2*/
    }

#endif /* End UART_LED_TX_ENABLED && (UART_LED_TXBUFFERSIZE > UART_LED_FIFO_LENGTH) */


/* [] END OF FILE */
