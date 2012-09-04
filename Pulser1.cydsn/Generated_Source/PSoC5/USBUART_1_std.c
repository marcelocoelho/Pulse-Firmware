/*******************************************************************************
* File Name: USBUART_1_std.c
* Version 2.30
*
* Description:
*  USB Standard request handler.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBUART_1.h"


/***************************************
* External references
***************************************/

extern const uint8 CYCODE USBUART_1_DEVICE0_DESCR[];
extern const uint8 CYCODE USBUART_1_DEVICE0_CONFIGURATION0_DESCR[];
extern const uint8 CYCODE USBUART_1_STRING_DESCRIPTORS[];
extern const uint8 CYCODE USBUART_1_MSOS_DESCRIPTOR[];
extern const uint8 CYCODE USBUART_1_SN_STRING_DESCRIPTOR[];

extern volatile uint8 USBUART_1_device;
extern volatile uint8 USBUART_1_configuration;
extern volatile uint8 USBUART_1_configurationChanged;
extern volatile uint8 USBUART_1_interfaceSetting[];
extern volatile uint8 USBUART_1_interfaceSetting_last[];
extern volatile uint8 USBUART_1_deviceAddress;
extern volatile uint8 USBUART_1_deviceStatus;
extern volatile uint8 USBUART_1_interfaceStatus[];
extern uint8 CYCODE *USBUART_1_interfaceClass;
extern const T_USBUART_1_LUT CYCODE USBUART_1_TABLE[];
extern volatile T_USBUART_1_EP_CTL_BLOCK USBUART_1_EP[];
extern volatile T_USBUART_1_TD USBUART_1_currentTD;
#if defined(USBUART_1_ENABLE_CDC_CLASS)
    extern volatile uint8 USBUART_1_cdc_data_in_ep;
    extern volatile uint8 USBUART_1_cdc_data_out_ep;
#endif  /* End USBUART_1_ENABLE_CDC_CLASS*/
#if defined(USBUART_1_ENABLE_MIDI_STREAMING)
    extern volatile uint8 USBUART_1_midi_in_ep;
    extern volatile uint8 USBUART_1_midi_out_ep;
#endif /* End USBUART_1_ENABLE_MIDI_STREAMING */


/***************************************
*         Forward references
***************************************/

uint8 USBUART_1_InitControlRead(void) ;
uint8 USBUART_1_InitControlWrite(void) ;
uint8 USBUART_1_InitNoDataControlTransfer(void) ;
uint8 USBUART_1_DispatchClassRqst(void) ;

void USBUART_1_Config(uint8 clearAltSetting) ;
void USBUART_1_ConfigAltChanged(void) ;
T_USBUART_1_LUT *USBUART_1_GetConfigTablePtr(uint8 c)
                                                            ;
T_USBUART_1_LUT *USBUART_1_GetDeviceTablePtr(void)
                                                            ;
uint8 USBUART_1_ClearEndpointHalt(void) ;
uint8 USBUART_1_SetEndpointHalt(void) ;
uint8 USBUART_1_ValidateAlternateSetting(void) ;

/*DIE ID string descriptor for 8 bytes ID*/
#if defined(USBUART_1_ENABLE_IDSN_STRING)
    void USBUART_1_ReadDieID(uint8 *descr) ;
    uint8 USBUART_1_idSerialNumberStringDescriptor[0x22u]={0x22u, USBUART_1_DESCR_STRING};
#endif /* USBUART_1_ENABLE_IDSN_STRING */

#if(USBUART_1_EP_MM != USBUART_1__EP_MANUAL)
    void USBUART_1_Stop_DMA(uint8 epNumber) ;
#endif   /* End USBUART_1_EP_MM != USBUART_1__EP_MANUAL */
uint8 CYCODE *USBUART_1_GetInterfaceClassTablePtr(void)
                                                    ;


/***************************************
* Global data allocation
***************************************/

volatile uint8 USBUART_1_tBuffer[USBUART_1_EP_STATUS_LENGTH > USBUART_1_DEVICE_STATUS_LENGTH ? \
                               USBUART_1_EP_STATUS_LENGTH : USBUART_1_DEVICE_STATUS_LENGTH];
volatile uint8 *USBUART_1_fwSerialNumberStringDescriptor;
volatile uint8 USBUART_1_snStringConfirm = USBUART_1_FALSE;


/*******************************************************************************
* Function Name: USBUART_1_SerialNumString
********************************************************************************
*
* Summary:
*  Application firmware may supply the source of the USB device descriptors
*  serial number string during runtime.
*
* Parameters:
*  snString:  pointer to string.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void  USBUART_1_SerialNumString(uint8 *snString) 
{
    #if defined(USBUART_1_ENABLE_FWSN_STRING)
        USBUART_1_snStringConfirm = USBUART_1_FALSE;
        if(snString != NULL)
        {
            USBUART_1_fwSerialNumberStringDescriptor = snString;
            /* check descriptor validation */
            if( (USBUART_1_fwSerialNumberStringDescriptor[0u] > 1u ) &&  \
                (USBUART_1_fwSerialNumberStringDescriptor[1u] == USBUART_1_DESCR_STRING) )
            {
                USBUART_1_snStringConfirm = USBUART_1_TRUE;
            }
        }
    #else
        snString = snString;
    #endif  /* USBUART_1_ENABLE_FWSN_STRING */
}


/*******************************************************************************
* Function Name: USBUART_1_HandleStandardRqst
********************************************************************************
*
* Summary:
*  This Routine dispatches standard requests
*
* Parameters:
*  None.
*
* Return:
*  TRUE if request handled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_1_HandleStandardRqst(void) 
{
    uint8 requestHandled = USBUART_1_FALSE;
    #if defined(USBUART_1_ENABLE_STRINGS)
        volatile uint8 *pStr = 0u;
        #if defined(USBUART_1_ENABLE_DESCRIPTOR_STRINGS)
            uint8 nStr;
        #endif /* USBUART_1_ENABLE_DESCRIPTOR_STRINGS */
    #endif /* USBUART_1_ENABLE_STRINGS */
    uint16 count;

    T_USBUART_1_LUT *pTmp;
    USBUART_1_currentTD.count = 0u;

    if ((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_DIR_MASK) == USBUART_1_RQST_DIR_D2H)
    {
        /* Control Read */
        switch (CY_GET_REG8(USBUART_1_bRequest))
        {
            case USBUART_1_GET_DESCRIPTOR:
                if (CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_DESCR_DEVICE)
                {
                    pTmp = USBUART_1_GetDeviceTablePtr();
                    USBUART_1_currentTD.pData = pTmp->p_list;
                    USBUART_1_currentTD.count = USBUART_1_DEVICE_DESCR_LENGTH;
                    requestHandled  = USBUART_1_InitControlRead();
                }
                else if (CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_DESCR_CONFIG)
                {
                    pTmp = USBUART_1_GetConfigTablePtr(CY_GET_REG8(USBUART_1_wValueLo));
                    USBUART_1_currentTD.pData = pTmp->p_list;
                    count = ((uint16)(USBUART_1_currentTD.pData)[ \
                                      USBUART_1_CONFIG_DESCR_TOTAL_LENGTH_HI] << 8u) | \
                                     (USBUART_1_currentTD.pData)[USBUART_1_CONFIG_DESCR_TOTAL_LENGTH_LOW];
                    USBUART_1_currentTD.count = count;
                    requestHandled  = USBUART_1_InitControlRead();
                }
                #if defined(USBUART_1_ENABLE_STRINGS)
                else if (CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_DESCR_STRING)
                {
                    /* Descriptor Strings*/
                    #if defined(USBUART_1_ENABLE_DESCRIPTOR_STRINGS)
                        nStr = 0u;
                        pStr = (volatile uint8 *)&USBUART_1_STRING_DESCRIPTORS[0u];
                        while ( (CY_GET_REG8(USBUART_1_wValueLo) > nStr) && (*pStr != 0u ))
                        {
                            pStr += *pStr;
                            nStr++;
                        };
                    #endif /* End USBUART_1_ENABLE_DESCRIPTOR_STRINGS */
                    /* Microsoft OS String*/
                    #if defined(USBUART_1_ENABLE_MSOS_STRING)
                        if( CY_GET_REG8(USBUART_1_wValueLo) == USBUART_1_STRING_MSOS )
                        {
                            pStr = (volatile uint8 *)&USBUART_1_MSOS_DESCRIPTOR[0u];
                        }
                    #endif /* End USBUART_1_ENABLE_MSOS_STRING*/
                    /* SN string*/
                    #if defined(USBUART_1_ENABLE_SN_STRING)
                        if( (CY_GET_REG8(USBUART_1_wValueLo) != 0) && 
                            (CY_GET_REG8(USBUART_1_wValueLo) == 
                            USBUART_1_DEVICE0_DESCR[USBUART_1_DEVICE_DESCR_SN_SHIFT]) )
                        {
                            pStr = (volatile uint8 *)&USBUART_1_SN_STRING_DESCRIPTOR[0u];
                            if(USBUART_1_snStringConfirm != USBUART_1_FALSE)
                            {
                                pStr = USBUART_1_fwSerialNumberStringDescriptor;
                            }
                            #if defined(USBUART_1_ENABLE_IDSN_STRING)
                                /* Read DIE ID and genarete string descriptor in RAM*/
                                USBUART_1_ReadDieID(USBUART_1_idSerialNumberStringDescriptor);
                                pStr = USBUART_1_idSerialNumberStringDescriptor;
                            #endif    /* End USBUART_1_ENABLE_IDSN_STRING */
                        }
                    #endif    /* End USBUART_1_ENABLE_SN_STRING */
                    if (*pStr != 0u)
                    {
                        USBUART_1_currentTD.count = *pStr;
                        USBUART_1_currentTD.pData = pStr;
                        requestHandled  = USBUART_1_InitControlRead();
                    }
                }
                #endif /* End USBUART_1_ENABLE_STRINGS */
                else
                {
                    requestHandled = USBUART_1_DispatchClassRqst();
                }
                break;
            case USBUART_1_GET_STATUS:
                switch ((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_RCPT_MASK))
                {
                    case USBUART_1_RQST_RCPT_EP:
                        USBUART_1_currentTD.count = USBUART_1_EP_STATUS_LENGTH;
                        USBUART_1_tBuffer[0] = USBUART_1_EP[ \
                                        CY_GET_REG8(USBUART_1_wIndexLo) & USBUART_1_DIR_UNUSED].hwEpState;
                        USBUART_1_tBuffer[1] = 0u;
                        USBUART_1_currentTD.pData = &USBUART_1_tBuffer[0u];
                        requestHandled  = USBUART_1_InitControlRead();
                        break;
                    case USBUART_1_RQST_RCPT_DEV:
                        USBUART_1_currentTD.count = USBUART_1_DEVICE_STATUS_LENGTH;
                        USBUART_1_tBuffer[0u] = USBUART_1_deviceStatus;
                        USBUART_1_tBuffer[1u] = 0u;
                        USBUART_1_currentTD.pData = &USBUART_1_tBuffer[0u];
                        requestHandled  = USBUART_1_InitControlRead();
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            case USBUART_1_GET_CONFIGURATION:
                USBUART_1_currentTD.count = 1u;
                USBUART_1_currentTD.pData = (uint8 *)&USBUART_1_configuration;
                requestHandled  = USBUART_1_InitControlRead();
                break;
            case USBUART_1_GET_INTERFACE:
                USBUART_1_currentTD.count = 1u;
                USBUART_1_currentTD.pData = (uint8 *)&USBUART_1_interfaceSetting[ \
                                                                            CY_GET_REG8(USBUART_1_wIndexLo)];
                requestHandled  = USBUART_1_InitControlRead();
                break;
            default: /* requestHandled is initialezed as FALSE by default */
                break;
        }
    }
    else {
        /* Control Write */
        switch (CY_GET_REG8(USBUART_1_bRequest))
        {
            case USBUART_1_SET_ADDRESS:
                USBUART_1_deviceAddress = CY_GET_REG8(USBUART_1_wValueLo);
                requestHandled = USBUART_1_InitNoDataControlTransfer();
                break;
            case USBUART_1_SET_CONFIGURATION:
                USBUART_1_configuration = CY_GET_REG8(USBUART_1_wValueLo);
                USBUART_1_configurationChanged = USBUART_1_TRUE;
                USBUART_1_Config(USBUART_1_TRUE);
                requestHandled = USBUART_1_InitNoDataControlTransfer();
                break;
            case USBUART_1_SET_INTERFACE:
                if (USBUART_1_ValidateAlternateSetting())
                {
                    USBUART_1_configurationChanged = USBUART_1_TRUE;
                    #if ((USBUART_1_EP_MA == USBUART_1__MA_DYNAMIC) && \
                         (USBUART_1_EP_MM == USBUART_1__EP_MANUAL) )
                        USBUART_1_Config(USBUART_1_FALSE);
                    #else
                        USBUART_1_ConfigAltChanged();
                    #endif /* End (USBUART_1_EP_MA == USBUART_1__MA_DYNAMIC) */
                    /* Update handled Alt setting changes status */
                    USBUART_1_interfaceSetting_last[CY_GET_REG8(USBUART_1_wIndexLo)] = 
                         USBUART_1_interfaceSetting[CY_GET_REG8(USBUART_1_wIndexLo)];
                    requestHandled = USBUART_1_InitNoDataControlTransfer();
                }
                break;
            case USBUART_1_CLEAR_FEATURE:
                switch (CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_RCPT_MASK)
                {
                    case USBUART_1_RQST_RCPT_EP:
                        if (CY_GET_REG8(USBUART_1_wValueLo) == USBUART_1_ENDPOINT_HALT)
                        {
                            requestHandled = USBUART_1_ClearEndpointHalt();
                        }
                        break;
                    case USBUART_1_RQST_RCPT_DEV:
                        /* Clear device REMOTE_WAKEUP */
                        if (CY_GET_REG8(USBUART_1_wValueLo) == USBUART_1_DEVICE_REMOTE_WAKEUP)
                        {
                            USBUART_1_deviceStatus &= ~USBUART_1_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBUART_1_InitNoDataControlTransfer();
                        }
                        break;
                    case USBUART_1_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (CY_GET_REG8(USBUART_1_wIndexLo) < USBUART_1_MAX_INTERFACES_NUMBER)
                        {
                            USBUART_1_interfaceStatus[CY_GET_REG8(USBUART_1_wIndexLo)] &= \
                                                                ~(CY_GET_REG8(USBUART_1_wValueLo)) ;
                            requestHandled = USBUART_1_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            case USBUART_1_SET_FEATURE:
                switch (CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_RCPT_MASK)
                {
                    case USBUART_1_RQST_RCPT_EP:
                        if (CY_GET_REG8(USBUART_1_wValueLo) == USBUART_1_ENDPOINT_HALT)
                        {
                            requestHandled = USBUART_1_SetEndpointHalt();
                        }
                        break;
                    case USBUART_1_RQST_RCPT_DEV:
                        /* Set device REMOTE_WAKEUP */
                        if (CY_GET_REG8(USBUART_1_wValueLo) == USBUART_1_DEVICE_REMOTE_WAKEUP)
                        {
                            USBUART_1_deviceStatus |= USBUART_1_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBUART_1_InitNoDataControlTransfer();
                        }
                        break;
                    case USBUART_1_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (CY_GET_REG8(USBUART_1_wIndexLo) < USBUART_1_MAX_INTERFACES_NUMBER)
                        {
                            USBUART_1_interfaceStatus[CY_GET_REG8(USBUART_1_wIndexLo)] &= \
                                                                ~(CY_GET_REG8(USBUART_1_wValueLo)) ;
                            requestHandled = USBUART_1_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            default:    /* requestHandled is initialezed as FALSE by default */
                break;
        }
    }
    return(requestHandled);
}


#if defined(USBUART_1_ENABLE_IDSN_STRING)

    /***************************************************************************
    * Function Name: USBUART_1_ReadDieID
    ****************************************************************************
    *
    * Summary:
    *  This routine read Die ID and genarete Serian Number string descriptor.
    *
    * Parameters:
    *  descr:  pointer on string descriptor.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    ***************************************************************************/
    void USBUART_1_ReadDieID(uint8 *descr) 
    {
        uint8 i,j;
        uint8 value;
        static char8 const hex[16u] = "0123456789ABCDEF";

        /* check descriptor validation */
        if( (descr[0u] > 1u ) && (descr[1u] == USBUART_1_DESCR_STRING) )
        {
            /* fill descriptor */
            for(j = 0u, i = 2u; i < descr[0u]; i += 2u)
            {
                value = CY_GET_XTND_REG8((void CYFAR *)(USBUART_1_DIE_ID + j++));
                descr[i] = (uint8)hex[value >> 4u];
                i += 2u;
                descr[i] = (uint8)hex[value & 0x0Fu];
            }
        }
    }

#endif /* End $INSTANCE_NAME`_ENABLE_IDSN_STRING*/


/*******************************************************************************
* Function Name: USBUART_1_ConfigReg
********************************************************************************
*
* Summary:
*  This routine configures hardware registers from the variables.
*  It is called from USBUART_1_Config() function and from RestoreConfig
*  after Wakeup.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void USBUART_1_ConfigReg(void) 
{
    uint8 ep;
    uint8 i;
    #if(USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO)
        uint8 ep_type = 0u;
    #endif /* End USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO */

    /* Set the endpoint buffer addresses */
    ep = USBUART_1_EP1;
    for (i = 0u; i < 0x80u; i+= 0x10u)
    {
        CY_SET_REG8(&USBUART_1_ARB_EP1_CFG_PTR[i], USBUART_1_ARB_EPX_CFG_CRC_BYPASS |
                                                          USBUART_1_ARB_EPX_CFG_RESET);

        #if(USBUART_1_EP_MM != USBUART_1__EP_MANUAL)
            /* Enable all Arbiter EP Interrupts : err, buf under, buf over, dma gnt, in buf full */
            USBUART_1_ARB_EP1_INT_EN_PTR[i] = USBUART_1_ARB_EPX_INT_MASK;
        #endif   /* End USBUART_1_EP_MM != USBUART_1__EP_MANUAL */

        if(USBUART_1_EP[ep].epMode != USBUART_1_MODE_DISABLE)
        {
            if((USBUART_1_EP[ep].addr & USBUART_1_DIR_IN) != 0u )
            {
                CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[i], USBUART_1_MODE_NAK_IN);
            }
            else
            {
                CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[i], USBUART_1_MODE_NAK_OUT);
                /* Prepare EP type mask for automatic memory allocation */
                #if(USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO)
                    ep_type |= 0x01u << (ep - USBUART_1_EP1);
                #endif /* End USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO */
            }
        }
        else
        {
            CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[i], USBUART_1_MODE_STALL_DATA_EP);
        }

        #if(USBUART_1_EP_MM != USBUART_1__EP_DMAAUTO)
            CY_SET_REG8(&USBUART_1_SIE_EP1_CNT0_PTR[i],   USBUART_1_EP[ep].bufferSize >> 8u);
            CY_SET_REG8(&USBUART_1_SIE_EP1_CNT1_PTR[i],   USBUART_1_EP[ep].bufferSize & 0xFFu);

            CY_SET_REG8(&USBUART_1_ARB_RW1_RA_PTR[i],     USBUART_1_EP[ep].buffOffset & 0xFFu);
            CY_SET_REG8(&USBUART_1_ARB_RW1_RA_MSB_PTR[i], USBUART_1_EP[ep].buffOffset >> 8u);
            CY_SET_REG8(&USBUART_1_ARB_RW1_WA_PTR[i],     USBUART_1_EP[ep].buffOffset & 0xFFu);
            CY_SET_REG8(&USBUART_1_ARB_RW1_WA_MSB_PTR[i], USBUART_1_EP[ep].buffOffset >> 8u);
        #endif /* End USBUART_1_EP_MM != USBUART_1__EP_DMAAUTO */

        ep++;
    }

    #if(USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO)
         /* BUF_SIZE depend on DMA_THRESS value: 55-32 bytes  44-16 bytes 33-8 bytes 22-4 bytes 11-2 bytes */
        USBUART_1_BUF_SIZE_REG = USBUART_1_DMA_BUF_SIZE;
        USBUART_1_DMA_THRES_REG = USBUART_1_DMA_BYTES_PER_BURST;   /* DMA burst threshold */
        USBUART_1_DMA_THRES_MSB_REG = 0u;
        USBUART_1_EP_ACTIVE_REG = USBUART_1_ARB_INT_MASK;
        USBUART_1_EP_TYPE_REG = ep_type;
        /* Cfg_cmp bit set to 1 once configuration is complete. */
        USBUART_1_ARB_CFG_REG = USBUART_1_ARB_CFG_AUTO_DMA | USBUART_1_ARB_CFG_AUTO_MEM |
                                       USBUART_1_ARB_CFG_CFG_CPM;
        /* Cfg_cmp bit set to 0 during configuration of PFSUSB Registers. */
        USBUART_1_ARB_CFG_REG = USBUART_1_ARB_CFG_AUTO_DMA | USBUART_1_ARB_CFG_AUTO_MEM;
    #endif /* End USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO */

    CY_SET_REG8(USBUART_1_SIE_EP_INT_EN_PTR, 0xFFu);
}


/*******************************************************************************
* Function Name: USBUART_1_Config
********************************************************************************
*
* Summary:
*  This routine configures endpoints for the entire configuration by scanning
*  the configuration descriptor.
*
* Parameters:
*  clearAltSetting: It configures the bAlternateSetting 0 for each interface.
*
* Return:
*  None.
*
* USBUART_1_interfaceClass - Initialized class array for each interface.
*   It is used for hangling Class specific requests depend on interface class.
*   Different classes in multiple Alternate settings does not supported.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_Config(uint8 clearAltSetting) 
{
    uint8 ep;
    uint8 cur_ep;
    uint8 i;
    uint8 iso;
    uint8 *pDescr;
    #if(USBUART_1_EP_MM != USBUART_1__EP_DMAAUTO)
        uint16 count = 0u;
    #endif /* End USBUART_1_EP_MM != USBUART_1__EP_DMAAUTO */
    
    T_USBUART_1_LUT *pTmp;
    T_USBUART_1_EP_SETTINGS_BLOCK *pEP;

    /* Clear all of the endpoints */
    for (ep = 0u; ep < USBUART_1_MAX_EP; ep++)
    {
        USBUART_1_EP[ep].attrib = 0u;
        USBUART_1_EP[ep].hwEpState = 0u;
        USBUART_1_EP[ep].apiEpState = USBUART_1_NO_EVENT_PENDING;
        USBUART_1_EP[ep].epToggle = 0u;
        USBUART_1_EP[ep].epMode = USBUART_1_MODE_DISABLE;
        USBUART_1_EP[ep].bufferSize = 0u;
        USBUART_1_EP[ep].interface = 0u;

    }

    /* Clear Alternate settings for all interfaces */
    if(clearAltSetting != 0u)
    {
        for (i = 0u; i < USBUART_1_MAX_INTERFACES_NUMBER; i++)
        {
            USBUART_1_interfaceSetting[i] = 0x00u;
            USBUART_1_interfaceSetting_last[i] = 0x00u;
        }
    }

    /* Init Endpoints and Device Status if configured */
    if(USBUART_1_configuration > 0u)
    {
        pTmp = USBUART_1_GetConfigTablePtr(USBUART_1_configuration - 1u);
        /* Set Power status for current configuration */
        pDescr = (uint8 *)pTmp->p_list;
        if((pDescr[USBUART_1_CONFIG_DESCR_ATTRIB] & USBUART_1_CONFIG_DESCR_ATTRIB_SELF_POWERED) != 0u)
        {
            USBUART_1_deviceStatus |=  USBUART_1_DEVICE_STATUS_SELF_POWERED;
        }
        else
        {
            USBUART_1_deviceStatus &=  ~USBUART_1_DEVICE_STATUS_SELF_POWERED;
        }
        pTmp++;
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        #if ((USBUART_1_EP_MA == USBUART_1__MA_DYNAMIC) && \
             (USBUART_1_EP_MM == USBUART_1__EP_MANUAL) )
            /* Config for dynamic EP memory allocation */
            /* p_list points the endpoint setting table. */
            pEP = (T_USBUART_1_EP_SETTINGS_BLOCK *) pTmp->p_list;
            for (i = 0u; i < ep; i++, pEP++)
            {
                /* compate current Alternate setting with EP Alt*/
                if(USBUART_1_interfaceSetting[pEP->interface] == pEP->altSetting)
                {
                    cur_ep = pEP->addr & USBUART_1_DIR_UNUSED;
                    iso  = ((pEP->attributes & USBUART_1_EP_TYPE_MASK) == USBUART_1_EP_TYPE_ISOC);
                    if (pEP->addr & USBUART_1_DIR_IN)
                    {
                        /* IN Endpoint */
                        USBUART_1_EP[cur_ep].apiEpState = USBUART_1_EVENT_PENDING;
                        USBUART_1_EP[cur_ep].epMode = \
                                                (iso ? USBUART_1_MODE_ISO_IN : USBUART_1_MODE_ACK_IN);
                        #if defined(USBUART_1_ENABLE_CDC_CLASS)
                            if(((pEP->bMisc == USBUART_1_CLASS_CDC_DATA) ||
                                (pEP->bMisc == USBUART_1_CLASS_CDC)) &&
                                ((pEP->attributes & USBUART_1_EP_TYPE_MASK) != USBUART_1_EP_TYPE_INT))
                            {
                                USBUART_1_cdc_data_in_ep = cur_ep;
                            }
                        #endif  /* End USBUART_1_ENABLE_CDC_CLASS*/
                        #if defined(USBUART_1_ENABLE_MIDI_STREAMING)
                            if((pEP->bMisc == USBUART_1_CLASS_AUDIO) &&
                               ((pEP->attributes & USBUART_1_EP_TYPE_MASK) == USBUART_1_EP_TYPE_BULK))
                            {
                                USBUART_1_midi_in_ep = cur_ep;
                            }
                        #endif  /* End USBUART_1_ENABLE_MIDI_STREAMING*/
                    }
                    else
                    {
                        /* OUT Endpoint */
                        USBUART_1_EP[cur_ep].apiEpState = USBUART_1_NO_EVENT_PENDING;
                        USBUART_1_EP[cur_ep].epMode = \
                                                (iso ? USBUART_1_MODE_ISO_OUT : USBUART_1_MODE_ACK_OUT);
                        #if defined(USBUART_1_ENABLE_CDC_CLASS)
                            if(((pEP->bMisc == USBUART_1_CLASS_CDC_DATA) ||
                                (pEP->bMisc == USBUART_1_CLASS_CDC)) &&
                                ((pEP->attributes & USBUART_1_EP_TYPE_MASK) != USBUART_1_EP_TYPE_INT))
                            {
                                USBUART_1_cdc_data_out_ep = cur_ep;
                            }
                        #endif  /* End USBUART_1_ENABLE_CDC_CLASS*/
                        #if defined(USBUART_1_ENABLE_MIDI_STREAMING)
                            if((pEP->bMisc == USBUART_1_CLASS_AUDIO) &&
                               ((pEP->attributes & USBUART_1_EP_TYPE_MASK) == USBUART_1_EP_TYPE_BULK))
                            {
                                USBUART_1_midi_out_ep = cur_ep;
                            }
                        #endif  /* End USBUART_1_ENABLE_MIDI_STREAMING*/
                    }
                    USBUART_1_EP[cur_ep].bufferSize = pEP->bufferSize;
                    USBUART_1_EP[cur_ep].addr = pEP->addr;
                    USBUART_1_EP[cur_ep].attrib = pEP->attributes;
                }
            }
        #else /* Config for static EP memory allocation  */
            for (i = USBUART_1_EP1; i < USBUART_1_MAX_EP; i++)
            {
                /* and p_list points the endpoint setting table. */
                pEP = (T_USBUART_1_EP_SETTINGS_BLOCK *) pTmp->p_list;
                /* find max length for each EP and select it (length could be different in different Alt settings) */
                /* but other settings should be correct with regards to Interface alt Setting */
                for (cur_ep = 0u; cur_ep < ep; cur_ep++, pEP++)
                {
                    /* EP count is equal to EP # in table and we found larger EP length than have before*/
                    if(i == (pEP->addr & USBUART_1_DIR_UNUSED))
                    {
                        if(USBUART_1_EP[i].bufferSize < pEP->bufferSize)
                        {
                            USBUART_1_EP[i].bufferSize = pEP->bufferSize;
                        }
                        /* compate current Alternate setting with EP Alt*/
                        if(USBUART_1_interfaceSetting[pEP->interface] == pEP->altSetting)
                        {
                            iso  = ((pEP->attributes & USBUART_1_EP_TYPE_MASK) == USBUART_1_EP_TYPE_ISOC);
                            if (pEP->addr & USBUART_1_DIR_IN)
                            {
                                /* IN Endpoint */
                                USBUART_1_EP[i].apiEpState = USBUART_1_EVENT_PENDING;
                                USBUART_1_EP[i].epMode =
                                                    (iso ? USBUART_1_MODE_ISO_IN : USBUART_1_MODE_ACK_IN);
                                /* Find and init CDC IN endpoint number */
                                #if defined(USBUART_1_ENABLE_CDC_CLASS)
                                    if(((pEP->bMisc == USBUART_1_CLASS_CDC_DATA) ||
                                        (pEP->bMisc == USBUART_1_CLASS_CDC)) &&
                                        ((pEP->attributes & USBUART_1_EP_TYPE_MASK) !=
                                                                                        USBUART_1_EP_TYPE_INT))
                                    {
                                        USBUART_1_cdc_data_in_ep = i;
                                    }
                                #endif  /* End USBUART_1_ENABLE_CDC_CLASS*/
                                #if defined(USBUART_1_ENABLE_MIDI_STREAMING)
                                    if((pEP->bMisc == USBUART_1_CLASS_AUDIO) &&
                                       ((pEP->attributes & USBUART_1_EP_TYPE_MASK) ==
                                                                                        USBUART_1_EP_TYPE_BULK))
                                    {
                                        USBUART_1_midi_in_ep = i;
                                    }
                                #endif  /* End USBUART_1_ENABLE_MIDI_STREAMING*/
                            }
                            else
                            {
                                /* OUT Endpoint */
                                USBUART_1_EP[i].apiEpState = USBUART_1_NO_EVENT_PENDING;
                                USBUART_1_EP[i].epMode =
                                                (iso ? USBUART_1_MODE_ISO_OUT : USBUART_1_MODE_ACK_OUT);
                                /* Find and init CDC IN endpoint number */
                                #if defined(USBUART_1_ENABLE_CDC_CLASS)
                                    if(((pEP->bMisc == USBUART_1_CLASS_CDC_DATA) ||
                                        (pEP->bMisc == USBUART_1_CLASS_CDC)) &&
                                        ((pEP->attributes & USBUART_1_EP_TYPE_MASK) !=
                                                                                    USBUART_1_EP_TYPE_INT))
                                    {
                                        USBUART_1_cdc_data_out_ep = i;
                                    }
                                #endif  /* End USBUART_1_ENABLE_CDC_CLASS*/
                                #if defined(USBUART_1_ENABLE_MIDI_STREAMING)
                                    if((pEP->bMisc == USBUART_1_CLASS_AUDIO) &&
                                       ((pEP->attributes & USBUART_1_EP_TYPE_MASK) ==
                                                                                        USBUART_1_EP_TYPE_BULK))
                                    {
                                        USBUART_1_midi_out_ep = i;
                                    }
                                #endif  /* End USBUART_1_ENABLE_MIDI_STREAMING*/
                            }
                            USBUART_1_EP[i].addr = pEP->addr;
                            USBUART_1_EP[i].attrib = pEP->attributes;

                            #if(USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO)
                                break;      /* use first EP setting in Auto memory managment */
                            #endif /* End USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO */
                        }
                    }
                }
            }
        #endif /* End (USBUART_1_EP_MA == USBUART_1__MA_DYNAMIC) */

        /* Init class array for each interface and interface number for each EP.
        *  It is used for hangling Class specific requests directed to either an
        *  interface or the endpoint.
        */
        /* p_list points the endpoint setting table. */
        pEP = (T_USBUART_1_EP_SETTINGS_BLOCK *) pTmp->p_list;
        for (i = 0u; i < ep; i++, pEP++)
        {
            /* config interface number for each EP*/
            USBUART_1_EP[pEP->addr & USBUART_1_DIR_UNUSED].interface = pEP->interface;
        }
        /* init pointer on interface class table*/
        USBUART_1_interfaceClass = USBUART_1_GetInterfaceClassTablePtr();
        /* Set the endpoint buffer addresses */
        
        #if(USBUART_1_EP_MM != USBUART_1__EP_DMAAUTO)
            for (ep = USBUART_1_EP1; ep < USBUART_1_MAX_EP; ep++)
            {
                USBUART_1_EP[ep].buffOffset = count;
                 count += USBUART_1_EP[ep].bufferSize;
            }
        #endif /* End USBUART_1_EP_MM != USBUART_1__EP_DMAAUTO */

        /* Congigure hardware registers */
        USBUART_1_ConfigReg();
    } /* USBUART_1_configuration > 0 */
}


/*******************************************************************************
* Function Name: USBUART_1_ConfigAltChanged
********************************************************************************
*
* Summary:
*  This routine undate configuration for the required endpoints only.
*  It is called after SET_INTERFACE request when Static memory allocation used.
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
void USBUART_1_ConfigAltChanged() 
{
    uint8 ep,cur_ep,i;
    uint8 iso;
    uint8 ri;

    T_USBUART_1_LUT *pTmp;
    T_USBUART_1_EP_SETTINGS_BLOCK *pEP;


    /* Init Endpoints and Device Status if configured */
    if(USBUART_1_configuration > 0u)
    {
        pTmp = USBUART_1_GetConfigTablePtr(USBUART_1_configuration - 1u);
        pTmp++;
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        /* Do not touch EP wich doesn't need reconfiguration */
        /* When Alt setting chaged, the only required endpoints need to be reconfigured */
        /* p_list points the endpoint setting table. */
        pEP = (T_USBUART_1_EP_SETTINGS_BLOCK *) pTmp->p_list;
        for (i = 0u; i < ep; i++, pEP++)
        {
            /*If Alt setting changed and new is same with EP Alt */
            if((USBUART_1_interfaceSetting[pEP->interface] !=
                USBUART_1_interfaceSetting_last[pEP->interface] ) &&
               (USBUART_1_interfaceSetting[pEP->interface] == pEP->altSetting) &&
                pEP->interface == CY_GET_REG8(USBUART_1_wIndexLo))
            {
                cur_ep = pEP->addr & USBUART_1_DIR_UNUSED;
                ri = ((cur_ep - USBUART_1_EP1) << USBUART_1_EPX_CNTX_ADDR_SHIFT);
                iso  = ((pEP->attributes & USBUART_1_EP_TYPE_MASK) == USBUART_1_EP_TYPE_ISOC);
                if (pEP->addr & USBUART_1_DIR_IN)
                {
                    /* IN Endpoint */
                    USBUART_1_EP[cur_ep].apiEpState = USBUART_1_EVENT_PENDING;
                    USBUART_1_EP[cur_ep].epMode =
                                            (iso ? USBUART_1_MODE_ISO_IN : USBUART_1_MODE_ACK_IN);
                }
                else
                {
                    /* OUT Endpoint */
                    USBUART_1_EP[cur_ep].apiEpState = USBUART_1_NO_EVENT_PENDING;
                    USBUART_1_EP[cur_ep].epMode =
                                            (iso ? USBUART_1_MODE_ISO_OUT : USBUART_1_MODE_ACK_OUT);
                }
                 /* Change the SIE mode for the selected EP to NAK ALL */
                 CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[ri], USBUART_1_MODE_NAK_IN_OUT);
                USBUART_1_EP[cur_ep].bufferSize = pEP->bufferSize;
                USBUART_1_EP[cur_ep].addr = pEP->addr;
                USBUART_1_EP[cur_ep].attrib = pEP->attributes;

                /* Clear the data toggle */
                USBUART_1_EP[cur_ep].epToggle = 0u;

                /* Dynamic reconfiguration for mode 3 transfer */
                #if((USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO) && (!CY_PSOC5A) )
                    /* In_data_rdy for selected EP should be set to 0 */
                    USBUART_1_ARB_EP1_CFG_PTR[ri] &= ~USBUART_1_ARB_EPX_CFG_IN_DATA_RDY;

                    /* write the EP number for which reconfiguration is required */
                    USBUART_1_DYN_RECONFIG_REG = (cur_ep << USBUART_1_DYN_RECONFIG_EP_SHIFT);
                    /* Set the dyn_config_en bit in dynamic reconfiguration register */
                    USBUART_1_DYN_RECONFIG_REG |= USBUART_1_DYN_RECONFIG_ENABLE;
                    /* wait for the dyn_config_rdy bit to set by the block,
                    *  this bit will be set to 1 when block is ready for reconfiguration.
                    */
                    while((USBUART_1_DYN_RECONFIG_REG & USBUART_1_DYN_RECONFIG_RDY_STS) == 0u);
                    /* Once dyn_config_rdy bit is set, FW can change the EP configuration. */
                    /* Change EP Type with new direction */
                    if((pEP->addr & USBUART_1_DIR_IN) == 0)
                    {
                        USBUART_1_EP_TYPE_REG |= 0x01u << (cur_ep - USBUART_1_EP1);
                    }
                    else
                    {
                        USBUART_1_EP_TYPE_REG &= ~(0x01u << (cur_ep - USBUART_1_EP1));
                    }
                    /* dynamic reconfiguration enable bit cleared, pointers and control/status
                    *  signals for the selected EP is cleared/re-initialized on negedge
                    *  of dynamic reconfiguration enable bit).
                    */
                    USBUART_1_DYN_RECONFIG_REG &= ~USBUART_1_DYN_RECONFIG_ENABLE;

                    /* Cfg_cmp bit set to 1 once configuration is complete. */
                    USBUART_1_ARB_CFG_REG = USBUART_1_ARB_CFG_AUTO_DMA |
                                                USBUART_1_ARB_CFG_AUTO_MEM | USBUART_1_ARB_CFG_CFG_CPM;
                    /* Cfg_cmp bit set to 0 during configuration of PFSUSB Registers. */
                    USBUART_1_ARB_CFG_REG = USBUART_1_ARB_CFG_AUTO_DMA |
                                                   USBUART_1_ARB_CFG_AUTO_MEM;
                    /* The main loop will reenable DMA and OUT endpoint*/
                #else
                CY_SET_REG8(&USBUART_1_SIE_EP1_CNT0_PTR[ri],   USBUART_1_EP[cur_ep].bufferSize >> 8u);
                CY_SET_REG8(&USBUART_1_SIE_EP1_CNT1_PTR[ri],   USBUART_1_EP[cur_ep].bufferSize & 0xFFu);

                CY_SET_REG8(&USBUART_1_ARB_RW1_RA_PTR[ri],     USBUART_1_EP[cur_ep].buffOffset & 0xFFu);
                CY_SET_REG8(&USBUART_1_ARB_RW1_RA_MSB_PTR[ri], USBUART_1_EP[cur_ep].buffOffset >> 8u);
                CY_SET_REG8(&USBUART_1_ARB_RW1_WA_PTR[ri],     USBUART_1_EP[cur_ep].buffOffset & 0xFFu);
                CY_SET_REG8(&USBUART_1_ARB_RW1_WA_MSB_PTR[ri], USBUART_1_EP[cur_ep].buffOffset >> 8u);
                #endif /* End USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO */
            }
        }
    } /* USBUART_1_configuration > 0 */
    CY_SET_REG8(USBUART_1_SIE_EP_INT_EN_PTR, 0xFFu);
}


/*******************************************************************************
* Function Name: USBUART_1_GetConfigTablePtr
********************************************************************************
*
* Summary:
*  This routine returns a pointer a configuration table entry
*
* Parameters:
*  c:  Configuration Index
*
* Return:
*  Device Descriptor pointer.
*
*******************************************************************************/
T_USBUART_1_LUT *USBUART_1_GetConfigTablePtr(uint8 c) \
                                                        
{
    /* Device Table */
    T_USBUART_1_LUT *pTmp;

    pTmp = USBUART_1_GetDeviceTablePtr();

    /* The first entry points to the Device Descriptor,
       the the configuration entries  */
    return(pTmp[c + 1u].p_list);
}


/*******************************************************************************
* Function Name: USBUART_1_GetDeviceTablePtr
********************************************************************************
*
* Summary:
*  This routine returns a pointer to the Device table
*
* Parameters:
*  None.
*
* Return:
*  Device Table pointer
*
*******************************************************************************/
T_USBUART_1_LUT *USBUART_1_GetDeviceTablePtr(void) 
{
    /* Device Table */
    return(USBUART_1_TABLE[USBUART_1_device].p_list);
}


/*******************************************************************************
* Function Name: USB_GetInterfaceClassTablePtr
********************************************************************************
*
* Summary:
*  This routine returns Interface Class table pointer, which contains
*  the relation between interface number and interface class.
*
* Parameters:
*  None.
*
* Return:
*  Interface Class table pointer.
*
*******************************************************************************/
uint8 CYCODE *USBUART_1_GetInterfaceClassTablePtr(void)
                                                        
{
    T_USBUART_1_LUT *pTmp;
    uint8 currentInterfacesNum;

    pTmp = USBUART_1_GetConfigTablePtr(USBUART_1_configuration - 1u);
    currentInterfacesNum  = ((uint8 *) pTmp->p_list)[USBUART_1_CONFIG_DESCR_NUM_INTERFACES];
    /* Third entry in the LUT starts the Interface Table pointers */
    pTmp += 2;
    /* The INTERFACE_CLASS table is located after all interfaces*/
    pTmp += currentInterfacesNum;
    return(pTmp->p_list);
}


/*******************************************************************************
* Function Name: USBUART_1_TerminateEP
********************************************************************************
*
* Summary:
*  This routine handles set endpoint halt.
*
* Parameters:
*  Endpoint number.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_TerminateEP(uint8 ep) 
{
    uint8 ri;

    ep &= USBUART_1_DIR_UNUSED;
    ri = ((ep - USBUART_1_EP1) << USBUART_1_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBUART_1_EP0) && (ep < USBUART_1_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBUART_1_EP[ep].hwEpState |= (USBUART_1_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART_1_EP[ep].epToggle = 0u;
        USBUART_1_EP[ep].apiEpState = USBUART_1_NO_EVENT_ALLOWED;

        if (USBUART_1_EP[ep].addr & USBUART_1_DIR_IN)
        {
            /* IN Endpoint */
            CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[ri], USBUART_1_MODE_NAK_IN);
        }
        else
        {
            /* OUT Endpoint */
            CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[ri], USBUART_1_MODE_NAK_OUT);
        }
    }
}


/*******************************************************************************
* Function Name: USBUART_1_SetEndpointHalt
********************************************************************************
*
* Summary:
*  This routine handles set endpoint halt.
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
uint8 USBUART_1_SetEndpointHalt(void) 
{
    uint8 ep, ri;
    uint8 requestHandled = USBUART_1_FALSE;

    /* Clear endpoint halt */
    ep = CY_GET_REG8(USBUART_1_wIndexLo) & USBUART_1_DIR_UNUSED;
    ri = ((ep - USBUART_1_EP1) << USBUART_1_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBUART_1_EP0) && (ep < USBUART_1_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBUART_1_EP[ep].hwEpState |= (USBUART_1_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART_1_EP[ep].epToggle = 0u;
        USBUART_1_EP[ep].apiEpState = USBUART_1_NO_EVENT_ALLOWED;

        if (USBUART_1_EP[ep].addr & USBUART_1_DIR_IN)
        {
            /* IN Endpoint */
            CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[ri], USBUART_1_MODE_STALL_DATA_EP |
                                                               USBUART_1_MODE_ACK_IN);
        }
        else
        {
            /* OUT Endpoint */
            CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[ri], USBUART_1_MODE_STALL_DATA_EP |
                                                               USBUART_1_MODE_ACK_OUT);
        }
        requestHandled = USBUART_1_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBUART_1_ClearEndpointHalt
********************************************************************************
*
* Summary:
*  This routine handles clear endpoint halt.
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
uint8 USBUART_1_ClearEndpointHalt(void) 
{
    uint8 ep, ri;
    uint8 requestHandled = USBUART_1_FALSE;

    /* Clear endpoint halt */
    ep = CY_GET_REG8(USBUART_1_wIndexLo) & USBUART_1_DIR_UNUSED;
    ri = ((ep - USBUART_1_EP1) << USBUART_1_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBUART_1_EP0) && (ep < USBUART_1_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBUART_1_EP[ep].hwEpState &= ~(USBUART_1_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART_1_EP[ep].epToggle = 0u;

        if (USBUART_1_EP[ep].addr & USBUART_1_DIR_IN)
        {
            /* IN Endpoint */
            USBUART_1_EP[ep].apiEpState = USBUART_1_NO_EVENT_PENDING;
            CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[ri], USBUART_1_MODE_NAK_IN);
        }
        else
        {
            /* OUT Endpoint */
            USBUART_1_EP[ep].apiEpState = USBUART_1_EVENT_PENDING;
            CY_SET_REG8(&USBUART_1_SIE_EP1_CR0_PTR[ri], USBUART_1_MODE_ACK_OUT);
        }
        requestHandled = USBUART_1_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBUART_1_ValidateAlternateSetting
********************************************************************************
*
* Summary:
*  Validates (and records) a SET INTERFACE request.
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
uint8 USBUART_1_ValidateAlternateSetting(void) 
{
    uint8 requestHandled = USBUART_1_TRUE;
    uint8 interfaceNum;
    T_USBUART_1_LUT *pTmp;
    uint8 currentInterfacesNum;

    interfaceNum = CY_GET_REG8(USBUART_1_wIndexLo);
    /* Validate interface setting, stall if invalid. */
    pTmp = USBUART_1_GetConfigTablePtr(USBUART_1_configuration - 1u);
    currentInterfacesNum  = ((uint8 *) pTmp->p_list)[USBUART_1_CONFIG_DESCR_NUM_INTERFACES];

    if((interfaceNum >= currentInterfacesNum) || (interfaceNum >= USBUART_1_MAX_INTERFACES_NUMBER))
    {   /* wrong interface number */
        requestHandled = USBUART_1_FALSE;
    }
    else
    {
        /* save current Alt setting to find out the difference in Config() function */
        USBUART_1_interfaceSetting_last[interfaceNum] = USBUART_1_interfaceSetting[interfaceNum];
        USBUART_1_interfaceSetting[interfaceNum] = CY_GET_REG8(USBUART_1_wValueLo);
    }

    return (requestHandled);
}


/* [] END OF FILE */
