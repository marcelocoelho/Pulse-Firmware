/*******************************************************************************
* File Name: USBUART_Host_audio.c
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

#include "USBUART_Host.h"

#if defined(USBUART_Host_ENABLE_AUDIO_CLASS)

#include "USBUART_Host_audio.h"
#include "USBUART_Host_midi.h"


/***************************************
*    AUDIO Variables
***************************************/

#if defined(USBUART_Host_ENABLE_AUDIO_STREAMING)
    volatile uint8 USBUART_Host_currentSampleFrequency[USBUART_Host_MAX_EP][USBUART_Host_SAMPLE_FREQ_LEN];
    volatile uint8 USBUART_Host_frequencyChanged;
    volatile uint8 USBUART_Host_currentMute;
    volatile uint8 USBUART_Host_currentVolume[USBUART_Host_VOLUME_LEN];
    volatile uint8 USBUART_Host_minimumVolume[] = {0x01, 0x80};
    volatile uint8 USBUART_Host_maximumVolume[] = {0xFF, 0x7F};
    volatile uint8 USBUART_Host_resolutionVolume[] = {0x01, 0x00};
#endif /* End USBUART_Host_ENABLE_AUDIO_STREAMING */


/***************************************
* Custom Declarations
***************************************/

/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
*    External references
***************************************/

uint8 USBUART_Host_InitControlRead(void) ;
uint8 USBUART_Host_InitControlWrite(void) ;
uint8 USBUART_Host_InitNoDataControlTransfer(void) ;
uint8 USBUART_Host_InitZeroLengthControlTransfer(void)
                                                ;

extern volatile T_USBUART_Host_EP_CTL_BLOCK USBUART_Host_EP[];
extern volatile T_USBUART_Host_TD USBUART_Host_currentTD;


/***************************************
*    Private function prototypes
***************************************/


/*******************************************************************************
* Function Name: USBUART_Host_DispatchAUDIOClassRqst
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
*   USBUART_Host_currentSampleFrequency: Contains the current audio Sample
*       Frequency. It is set by the Host using SET_CUR request to the endpoint.
*   USBUART_Host_frequencyChanged: This variable is used as a flag for the
*       user code, to be aware that Host has been sent request for changing
*       Sample Frequency. Sample frequency will be sent on the next OUT
*       transaction. It is contains endpoint address when set. The following
*       code is recommended for detecting new Sample Frequency in main code:
*       if((USBUART_Host_frequencyChanged != 0) &&
*       (USBUART_Host_transferState == USBUART_Host_TRANS_STATE_IDLE))
*       {
*          USBUART_Host_frequencyChanged = 0;
*       }
*       USBUART_Host_transferState variable is checked to be sure that
*             transfer completes.
*   USBUART_Host_currentMute: Contains mute configuration set by Host.
*   USBUART_Host_currentVolume: Contains volume level set by Host.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_Host_DispatchAUDIOClassRqst() 
{
    uint8 requestHandled = USBUART_Host_FALSE;
    
    #if defined(USBUART_Host_ENABLE_AUDIO_STREAMING)
        uint8 epNumber;
        epNumber = CY_GET_REG8(USBUART_Host_wIndexLo) & USBUART_Host_DIR_UNUSED;
    #endif /* End USBUART_Host_ENABLE_AUDIO_STREAMING */

    if ((CY_GET_REG8(USBUART_Host_bmRequestType) & USBUART_Host_RQST_DIR_MASK) == USBUART_Host_RQST_DIR_D2H)
    {
        /* Control Read */
        if((CY_GET_REG8(USBUART_Host_bmRequestType) & USBUART_Host_RQST_RCPT_MASK) == \
                                                                                    USBUART_Host_RQST_RCPT_EP)
        {
            /* Endpoint */
            switch (CY_GET_REG8(USBUART_Host_bRequest))
            {
                case USBUART_Host_GET_CUR:
                #if defined(USBUART_Host_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_Host_wValueHi) == USBUART_Host_SAMPLING_FREQ_CONTROL)
                    {
                         /* Endpoint Control Selector is Sampling Frequency */
                        USBUART_Host_currentTD.wCount = USBUART_Host_SAMPLE_FREQ_LEN;
                        USBUART_Host_currentTD.pData  = USBUART_Host_currentSampleFrequency[epNumber];
                        requestHandled   = USBUART_Host_InitControlRead();
                    }
                #endif /* End USBUART_Host_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_READ_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else if((CY_GET_REG8(USBUART_Host_bmRequestType) & USBUART_Host_RQST_RCPT_MASK) == \
                                                                                    USBUART_Host_RQST_RCPT_IFC)
        {
            /* Interface or Entity ID */
            switch (CY_GET_REG8(USBUART_Host_bRequest))
            {
                case USBUART_Host_GET_CUR:
                #if defined(USBUART_Host_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_Host_wValueHi) == USBUART_Host_MUTE_CONTROL)
                    {
                         /* Entity ID Control Selector is MUTE */
                        USBUART_Host_currentTD.wCount = 1;
                        USBUART_Host_currentTD.pData  = &USBUART_Host_currentMute;
                        requestHandled   = USBUART_Host_InitControlRead();
                    }
                    else if(CY_GET_REG8(USBUART_Host_wValueHi) == USBUART_Host_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_GET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                         /* Entity ID Control Selector is VOLUME, */
                        USBUART_Host_currentTD.wCount = USBUART_Host_VOLUME_LEN;
                        USBUART_Host_currentTD.pData  = USBUART_Host_currentVolume;
                        requestHandled   = USBUART_Host_InitControlRead();
                    }
                    else
                    {
                    }
                    break;
                case USBUART_Host_GET_MIN:    /* GET_MIN */
                    if(CY_GET_REG8(USBUART_Host_wValueHi) == USBUART_Host_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USBUART_Host_currentTD.wCount = USBUART_Host_VOLUME_LEN;
                        USBUART_Host_currentTD.pData  = &USBUART_Host_minimumVolume[0];
                        requestHandled   = USBUART_Host_InitControlRead();
                    }
                    break;
                case USBUART_Host_GET_MAX:    /* GET_MAX */
                    if(CY_GET_REG8(USBUART_Host_wValueHi) == USBUART_Host_VOLUME_CONTROL)
                    {
                             /* Entity ID Control Selector is VOLUME, */
                        USBUART_Host_currentTD.wCount = USBUART_Host_VOLUME_LEN;
                        USBUART_Host_currentTD.pData  = &USBUART_Host_maximumVolume[0];
                        requestHandled   = USBUART_Host_InitControlRead();
                    }
                    break;
                case USBUART_Host_GET_RES:    /* GET_RES */
                    if(CY_GET_REG8(USBUART_Host_wValueHi) == USBUART_Host_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USBUART_Host_currentTD.wCount = USBUART_Host_VOLUME_LEN;
                        USBUART_Host_currentTD.pData  = &USBUART_Host_resolutionVolume[0];
                        requestHandled   = USBUART_Host_InitControlRead();
                    }
                    break;
                /* The contents of the status message is reserved for future use.
                *  For the time being, a null packet should be returned in the data stage of the 
                *  control transfer, and the received null packet should be ACKed.
                */
                case USBUART_Host_GET_STAT:
                        USBUART_Host_currentTD.wCount = 0;
                        requestHandled   = USBUART_Host_InitControlWrite();

                #endif /* End USBUART_Host_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_WRITE_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else
        {   /* USBUART_Host_RQST_RCPT_OTHER */
        }
    }
    else if ((CY_GET_REG8(USBUART_Host_bmRequestType) & USBUART_Host_RQST_DIR_MASK) == \
                                                                                    USBUART_Host_RQST_DIR_H2D)
    {
        /* Control Write */
        if((CY_GET_REG8(USBUART_Host_bmRequestType) & USBUART_Host_RQST_RCPT_MASK) == \
                                                                                    USBUART_Host_RQST_RCPT_EP)
        {
            /* Endpoint */
            switch (CY_GET_REG8(USBUART_Host_bRequest))
            {
                case USBUART_Host_SET_CUR:
                #if defined(USBUART_Host_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_Host_wValueHi) == USBUART_Host_SAMPLING_FREQ_CONTROL)
                    {
                         /* Endpoint Control Selector is Sampling Frequency */
                        USBUART_Host_currentTD.wCount = USBUART_Host_SAMPLE_FREQ_LEN;
                        USBUART_Host_currentTD.pData  = USBUART_Host_currentSampleFrequency[epNumber];
                        requestHandled   = USBUART_Host_InitControlWrite();
                        USBUART_Host_frequencyChanged = epNumber;
                    }
                #endif /* End USBUART_Host_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_SAMPLING_FREQ_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else if((CY_GET_REG8(USBUART_Host_bmRequestType) & USBUART_Host_RQST_RCPT_MASK) == \
                                                                                    USBUART_Host_RQST_RCPT_IFC)
        {
            /* Interface or Entity ID */
            switch (CY_GET_REG8(USBUART_Host_bRequest))
            {
                case USBUART_Host_SET_CUR:
                #if defined(USBUART_Host_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_Host_wValueHi) == USBUART_Host_MUTE_CONTROL)
                    {
                         /* Entity ID Control Selector is MUTE */
                        USBUART_Host_currentTD.wCount = 1;
                        USBUART_Host_currentTD.pData  = &USBUART_Host_currentMute;
                        requestHandled   = USBUART_Host_InitControlWrite();
                    }
                    else if(CY_GET_REG8(USBUART_Host_wValueHi) == USBUART_Host_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_SET_REQUEST` Place multi-channel handler here */

                        /* `#END` */
                    
                         /* Entity ID Control Selector is VOLUME */
                        USBUART_Host_currentTD.wCount = USBUART_Host_VOLUME_LEN;
                        USBUART_Host_currentTD.pData  = USBUART_Host_currentVolume;
                        requestHandled   = USBUART_Host_InitControlWrite();
                    }
                #endif /* End USBUART_Host_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_CONTROL_SEL_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else
        {   /* USBUART_Host_RQST_RCPT_OTHER */
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

#endif  /* End USBUART_Host_ENABLE_AUDIO_CLASS*/


/* [] END OF FILE */
