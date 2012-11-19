/*******************************************************************************
* File Name: USBUART_Central_audio.c
* Version 2.30
*
* Description:
*  USB AUDIO Class request handler.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBUART_Central.h"

#if defined(USBUART_Central_ENABLE_AUDIO_CLASS)

#include "USBUART_Central_audio.h"
#include "USBUART_Central_midi.h"


/***************************************
*    AUDIO Variables
***************************************/

#if defined(USBUART_Central_ENABLE_AUDIO_STREAMING)
    volatile uint8 USBUART_Central_currentSampleFrequency[USBUART_Central_MAX_EP][USBUART_Central_SAMPLE_FREQ_LEN];
    volatile uint8 USBUART_Central_frequencyChanged;
    volatile uint8 USBUART_Central_currentMute;
    volatile uint8 USBUART_Central_currentVolume[USBUART_Central_VOLUME_LEN];
    volatile uint8 USBUART_Central_minimumVolume[] = {0x01, 0x80};
    volatile uint8 USBUART_Central_maximumVolume[] = {0xFF, 0x7F};
    volatile uint8 USBUART_Central_resolutionVolume[] = {0x01, 0x00};
#endif /* End USBUART_Central_ENABLE_AUDIO_STREAMING */


/***************************************
* Custom Declarations
***************************************/

/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
*    External references
***************************************/

uint8 USBUART_Central_InitControlRead(void) ;
uint8 USBUART_Central_InitControlWrite(void) ;
uint8 USBUART_Central_InitNoDataControlTransfer(void) ;
uint8 USBUART_Central_InitZeroLengthControlTransfer(void)
                                                ;

extern volatile T_USBUART_Central_EP_CTL_BLOCK USBUART_Central_EP[];
extern volatile T_USBUART_Central_TD USBUART_Central_currentTD;


/***************************************
*    Private function prototypes
***************************************/


/*******************************************************************************
* Function Name: USBUART_Central_DispatchAUDIOClassRqst
********************************************************************************
*
* Summary:
*  This routine dispatches class requests
*
* Parameters:
*  None.
*
* Return:
*  requestHandled
*
* Global variables:
*   USBUART_Central_currentSampleFrequency: Contains the current audio Sample
*       Frequency. It is set by the Host using SET_CUR request to the endpoint.
*   USBUART_Central_frequencyChanged: This variable is used as a flag for the
*       user code, to be aware that Host has been sent request for changing
*       Sample Frequency. Sample frequency will be sent on the next OUT
*       transaction. It is contains endpoint address when set. The following
*       code is recommended for detecting new Sample Frequency in main code:
*       if((USBUART_Central_frequencyChanged != 0) &&
*       (USBUART_Central_transferState == USBUART_Central_TRANS_STATE_IDLE))
*       {
*          USBUART_Central_frequencyChanged = 0;
*       }
*       USBUART_Central_transferState variable is checked to be sure that
*             transfer completes.
*   USBUART_Central_currentMute: Contains mute configuration set by Host.
*   USBUART_Central_currentVolume: Contains volume level set by Host.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_Central_DispatchAUDIOClassRqst() 
{
    uint8 requestHandled = USBUART_Central_FALSE;
    
    #if defined(USBUART_Central_ENABLE_AUDIO_STREAMING)
        uint8 epNumber;
        epNumber = CY_GET_REG8(USBUART_Central_wIndexLo) & USBUART_Central_DIR_UNUSED;
    #endif /* End USBUART_Central_ENABLE_AUDIO_STREAMING */

    if ((CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_DIR_MASK) == USBUART_Central_RQST_DIR_D2H)
    {
        /* Control Read */
        if((CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_RCPT_MASK) == \
                                                                                    USBUART_Central_RQST_RCPT_EP)
        {
            /* Endpoint */
            switch (CY_GET_REG8(USBUART_Central_bRequest))
            {
                case USBUART_Central_GET_CUR:
                #if defined(USBUART_Central_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_SAMPLING_FREQ_CONTROL)
                    {
                         /* Endpoint Control Selector is Sampling Frequency */
                        USBUART_Central_currentTD.wCount = USBUART_Central_SAMPLE_FREQ_LEN;
                        USBUART_Central_currentTD.pData  = USBUART_Central_currentSampleFrequency[epNumber];
                        requestHandled   = USBUART_Central_InitControlRead();
                    }
                #endif /* End USBUART_Central_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_READ_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else if((CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_RCPT_MASK) == \
                                                                                    USBUART_Central_RQST_RCPT_IFC)
        {
            /* Interface or Entity ID */
            switch (CY_GET_REG8(USBUART_Central_bRequest))
            {
                case USBUART_Central_GET_CUR:
                #if defined(USBUART_Central_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_MUTE_CONTROL)
                    {
                         /* Entity ID Control Selector is MUTE */
                        USBUART_Central_currentTD.wCount = 1;
                        USBUART_Central_currentTD.pData  = &USBUART_Central_currentMute;
                        requestHandled   = USBUART_Central_InitControlRead();
                    }
                    else if(CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_GET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                         /* Entity ID Control Selector is VOLUME, */
                        USBUART_Central_currentTD.wCount = USBUART_Central_VOLUME_LEN;
                        USBUART_Central_currentTD.pData  = USBUART_Central_currentVolume;
                        requestHandled   = USBUART_Central_InitControlRead();
                    }
                    else
                    {
                    }
                    break;
                case USBUART_Central_GET_MIN:    /* GET_MIN */
                    if(CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USBUART_Central_currentTD.wCount = USBUART_Central_VOLUME_LEN;
                        USBUART_Central_currentTD.pData  = &USBUART_Central_minimumVolume[0];
                        requestHandled   = USBUART_Central_InitControlRead();
                    }
                    break;
                case USBUART_Central_GET_MAX:    /* GET_MAX */
                    if(CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_VOLUME_CONTROL)
                    {
                             /* Entity ID Control Selector is VOLUME, */
                        USBUART_Central_currentTD.wCount = USBUART_Central_VOLUME_LEN;
                        USBUART_Central_currentTD.pData  = &USBUART_Central_maximumVolume[0];
                        requestHandled   = USBUART_Central_InitControlRead();
                    }
                    break;
                case USBUART_Central_GET_RES:    /* GET_RES */
                    if(CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USBUART_Central_currentTD.wCount = USBUART_Central_VOLUME_LEN;
                        USBUART_Central_currentTD.pData  = &USBUART_Central_resolutionVolume[0];
                        requestHandled   = USBUART_Central_InitControlRead();
                    }
                    break;
                /* The contents of the status message is reserved for future use.
                *  For the time being, a null packet should be returned in the data stage of the 
                *  control transfer, and the received null packet should be ACKed.
                */
                case USBUART_Central_GET_STAT:
                        USBUART_Central_currentTD.wCount = 0;
                        requestHandled   = USBUART_Central_InitControlWrite();

                #endif /* End USBUART_Central_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_WRITE_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else
        {   /* USBUART_Central_RQST_RCPT_OTHER */
        }
    }
    else if ((CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_DIR_MASK) == \
                                                                                    USBUART_Central_RQST_DIR_H2D)
    {
        /* Control Write */
        if((CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_RCPT_MASK) == \
                                                                                    USBUART_Central_RQST_RCPT_EP)
        {
            /* Endpoint */
            switch (CY_GET_REG8(USBUART_Central_bRequest))
            {
                case USBUART_Central_SET_CUR:
                #if defined(USBUART_Central_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_SAMPLING_FREQ_CONTROL)
                    {
                         /* Endpoint Control Selector is Sampling Frequency */
                        USBUART_Central_currentTD.wCount = USBUART_Central_SAMPLE_FREQ_LEN;
                        USBUART_Central_currentTD.pData  = USBUART_Central_currentSampleFrequency[epNumber];
                        requestHandled   = USBUART_Central_InitControlWrite();
                        USBUART_Central_frequencyChanged = epNumber;
                    }
                #endif /* End USBUART_Central_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_SAMPLING_FREQ_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else if((CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_RCPT_MASK) == \
                                                                                    USBUART_Central_RQST_RCPT_IFC)
        {
            /* Interface or Entity ID */
            switch (CY_GET_REG8(USBUART_Central_bRequest))
            {
                case USBUART_Central_SET_CUR:
                #if defined(USBUART_Central_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_MUTE_CONTROL)
                    {
                         /* Entity ID Control Selector is MUTE */
                        USBUART_Central_currentTD.wCount = 1;
                        USBUART_Central_currentTD.pData  = &USBUART_Central_currentMute;
                        requestHandled   = USBUART_Central_InitControlWrite();
                    }
                    else if(CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_SET_REQUEST` Place multi-channel handler here */

                        /* `#END` */
                    
                         /* Entity ID Control Selector is VOLUME */
                        USBUART_Central_currentTD.wCount = USBUART_Central_VOLUME_LEN;
                        USBUART_Central_currentTD.pData  = USBUART_Central_currentVolume;
                        requestHandled   = USBUART_Central_InitControlWrite();
                    }
                #endif /* End USBUART_Central_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_CONTROL_SEL_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else
        {   /* USBUART_Central_RQST_RCPT_OTHER */
        }
    }
    else
    {   /* requestHandled is initialezed as FALSE by default */
    }

    return(requestHandled);
}


/*******************************************************************************
* Additional user functions supporting AUDIO Requests
********************************************************************************/

/* `#START AUDIO_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif  /* End USBUART_Central_ENABLE_AUDIO_CLASS*/


/* [] END OF FILE */
