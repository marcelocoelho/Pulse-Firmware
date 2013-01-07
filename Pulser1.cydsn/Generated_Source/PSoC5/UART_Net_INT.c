/*******************************************************************************
* File Name: UART_Net_INT.c
* Version 2.30
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART_Net.h"
#include "CyLib.h"


/***************************************
* Custom Declratations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if( (UART_Net_RX_ENABLED || UART_Net_HD_ENABLED) && \
     (UART_Net_RXBUFFERSIZE > UART_Net_FIFO_LENGTH))


    /*******************************************************************************
    * Function Name: UART_Net_RXISR
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
    *  UART_Net_rxBuffer - RAM buffer pointer for save received data.
    *  UART_Net_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  UART_Net_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  UART_Net_rxBufferOverflow - software overflow flag. Set to one
    *     when UART_Net_rxBufferWrite index overtakes
    *     UART_Net_rxBufferRead index.
    *  UART_Net_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when UART_Net_rxBufferWrite is equal to
    *    UART_Net_rxBufferRead
    *  UART_Net_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  UART_Net_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(UART_Net_RXISR)
    {
        uint8 readData;
        uint8 increment_pointer = 0u;
        #if(CY_PSOC3)
            uint8 int_en;
        #endif /* CY_PSOC3 */

        /* User code required at start of ISR */
        /* `#START UART_Net_RXISR_START` */

        /* `#END` */

        #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
            int_en = EA;
            CyGlobalIntEnable;
        #endif /* CY_PSOC3 */

        readData = UART_Net_RXSTATUS_REG;

        if((readData & (UART_Net_RX_STS_BREAK | UART_Net_RX_STS_PAR_ERROR |
                        UART_Net_RX_STS_STOP_ERROR | UART_Net_RX_STS_OVERRUN)) != 0u)
        {
            /* ERROR handling. */
            /* `#START UART_Net_RXISR_ERROR` */

            /* `#END` */
        }

        while((readData & UART_Net_RX_STS_FIFO_NOTEMPTY) != 0u)
        {

            #if (UART_Net_RXHW_ADDRESS_ENABLED)
                if(UART_Net_rxAddressMode == (uint8)UART_Net__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readData & UART_Net_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readData & UART_Net_RX_STS_ADDR_MATCH) != 0u)
                        {
                            UART_Net_rxAddressDetected = 1u;
                        }
                        else
                        {
                            UART_Net_rxAddressDetected = 0u;
                        }
                    }

                    readData = UART_Net_RXDATA_REG;
                    if(UART_Net_rxAddressDetected != 0u)
                    {   /* store only addressed data */
                        UART_Net_rxBuffer[UART_Net_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* without software addressing */
                {
                    UART_Net_rxBuffer[UART_Net_rxBufferWrite] = UART_Net_RXDATA_REG;
                    increment_pointer = 1u;
                }
            #else  /* without addressing */
                UART_Net_rxBuffer[UART_Net_rxBufferWrite] = UART_Net_RXDATA_REG;
                increment_pointer = 1u;
            #endif /* End SW_DETECT_TO_BUFFER */

            /* do not increment buffer pointer when skip not adderessed data */
            if( increment_pointer != 0u )
            {
                if(UART_Net_rxBufferLoopDetect != 0u)
                {   /* Set Software Buffer status Overflow */
                    UART_Net_rxBufferOverflow = 1u;
                }
                /* Set next pointer. */
                UART_Net_rxBufferWrite++;

                /* Check pointer for a loop condition */
                if(UART_Net_rxBufferWrite >= UART_Net_RXBUFFERSIZE)
                {
                    UART_Net_rxBufferWrite = 0u;
                }
                /* Detect pre-overload condition and set flag */
                if(UART_Net_rxBufferWrite == UART_Net_rxBufferRead)
                {
                    UART_Net_rxBufferLoopDetect = 1u;
                    /* When Hardware Flow Control selected */
                    #if(UART_Net_FLOW_CONTROL != 0u)
                    /* Disable RX interrupt mask, it will be enabled when user read data from the buffer using APIs */
                        UART_Net_RXSTATUS_MASK_REG  &= (uint8)~UART_Net_RX_STS_FIFO_NOTEMPTY;
                        CyIntClearPending(UART_Net_RX_VECT_NUM);
                        break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                    #endif /* End UART_Net_FLOW_CONTROL != 0 */
                }
            }

            /* Check again if there is data. */
            readData = UART_Net_RXSTATUS_REG;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UART_Net_RXISR_END` */

        /* `#END` */

        #if(CY_PSOC3)
            EA = int_en;
        #endif /* CY_PSOC3 */

    }

#endif /* End UART_Net_RX_ENABLED && (UART_Net_RXBUFFERSIZE > UART_Net_FIFO_LENGTH) */


#if(UART_Net_TX_ENABLED && (UART_Net_TXBUFFERSIZE > UART_Net_FIFO_LENGTH))


    /*******************************************************************************
    * Function Name: UART_Net_TXISR
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
    *  UART_Net_txBuffer - RAM buffer pointer for transmit data from.
    *  UART_Net_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmited byte.
    *  UART_Net_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(UART_Net_TXISR)
    {

        #if(CY_PSOC3)
            uint8 int_en;
        #endif /* CY_PSOC3 */

        /* User code required at start of ISR */
        /* `#START UART_Net_TXISR_START` */

        /* `#END` */

        #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
            int_en = EA;
            CyGlobalIntEnable;
        #endif /* CY_PSOC3 */

        while((UART_Net_txBufferRead != UART_Net_txBufferWrite) &&
             ((UART_Net_TXSTATUS_REG & UART_Net_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer. */
            if(UART_Net_txBufferRead >= UART_Net_TXBUFFERSIZE)
            {
                UART_Net_txBufferRead = 0u;
            }

            UART_Net_TXDATA_REG = UART_Net_txBuffer[UART_Net_txBufferRead];

            /* Set next pointer. */
            UART_Net_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UART_Net_TXISR_END` */

        /* `#END` */

        #if(CY_PSOC3)
            EA = int_en;
        #endif /* CY_PSOC3 */

    }

#endif /* End UART_Net_TX_ENABLED && (UART_Net_TXBUFFERSIZE > UART_Net_FIFO_LENGTH) */


/* [] END OF FILE */
