/*******************************************************************************
* File Name: USBUART_Central_std.c
* Version 2.50
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

#include "USBUART_Central.h"


/***************************************
* External references
***************************************/

extern const uint8 CYCODE USBUART_Central_DEVICE0_DESCR[];
extern const uint8 CYCODE USBUART_Central_DEVICE0_CONFIGURATION0_DESCR[];
extern const uint8 CYCODE USBUART_Central_STRING_DESCRIPTORS[];
extern const uint8 CYCODE USBUART_Central_MSOS_DESCRIPTOR[];
extern const uint8 CYCODE USBUART_Central_SN_STRING_DESCRIPTOR[];

extern volatile uint8 USBUART_Central_device;
extern volatile uint8 USBUART_Central_configuration;
extern volatile uint8 USBUART_Central_configurationChanged;
extern volatile uint8 USBUART_Central_interfaceNumber;
extern volatile uint8 USBUART_Central_interfaceSetting[];
extern volatile uint8 USBUART_Central_interfaceSetting_last[];
extern volatile uint8 USBUART_Central_deviceAddress;
extern volatile uint8 USBUART_Central_deviceStatus;
extern volatile uint8 USBUART_Central_interfaceStatus[];
extern uint8 CYCODE *USBUART_Central_interfaceClass;
extern const T_USBUART_Central_LUT CYCODE USBUART_Central_TABLE[];
extern volatile T_USBUART_Central_EP_CTL_BLOCK USBUART_Central_EP[];
extern volatile T_USBUART_Central_TD USBUART_Central_currentTD;
#if defined(USBUART_Central_ENABLE_CDC_CLASS)
    extern volatile uint8 USBUART_Central_cdc_data_in_ep;
    extern volatile uint8 USBUART_Central_cdc_data_out_ep;
#endif  /* End USBUART_Central_ENABLE_CDC_CLASS*/
#if defined(USBUART_Central_ENABLE_MIDI_STREAMING)
    extern volatile uint8 USBUART_Central_midi_in_ep;
    extern volatile uint8 USBUART_Central_midi_out_ep;
#endif /* End USBUART_Central_ENABLE_MIDI_STREAMING */


/***************************************
*         Forward references
***************************************/

uint8 USBUART_Central_InitControlRead(void) ;
uint8 USBUART_Central_InitControlWrite(void) ;
uint8 USBUART_Central_InitNoDataControlTransfer(void) ;
uint8 USBUART_Central_DispatchClassRqst(void) ;

void USBUART_Central_Config(uint8 clearAltSetting) ;
void USBUART_Central_ConfigAltChanged(void) ;
T_USBUART_Central_LUT *USBUART_Central_GetConfigTablePtr(uint8 c)
                                                            ;
T_USBUART_Central_LUT *USBUART_Central_GetDeviceTablePtr(void)
                                                            ;
uint8 USBUART_Central_ClearEndpointHalt(void) ;
uint8 USBUART_Central_SetEndpointHalt(void) ;
uint8 USBUART_Central_ValidateAlternateSetting(void) ;

/*DIE ID string descriptor for 8 bytes ID*/
#if defined(USBUART_Central_ENABLE_IDSN_STRING)
    void USBUART_Central_ReadDieID(uint8 *descr) ;
    uint8 USBUART_Central_idSerialNumberStringDescriptor[0x22u]={0x22u, USBUART_Central_DESCR_STRING};
#endif /* USBUART_Central_ENABLE_IDSN_STRING */

#if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)
    void USBUART_Central_Stop_DMA(uint8 epNumber) ;
#endif   /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL */
uint8 CYCODE *USBUART_Central_GetInterfaceClassTablePtr(void)
                                                    ;


/***************************************
* Global data allocation
***************************************/

volatile uint8 USBUART_Central_tBuffer[USBUART_Central_EP_STATUS_LENGTH > USBUART_Central_DEVICE_STATUS_LENGTH ? \
                               USBUART_Central_EP_STATUS_LENGTH : USBUART_Central_DEVICE_STATUS_LENGTH];
volatile uint8 *USBUART_Central_fwSerialNumberStringDescriptor;
volatile uint8 USBUART_Central_snStringConfirm = USBUART_Central_FALSE;


/*******************************************************************************
* Function Name: USBUART_Central_SerialNumString
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
void  USBUART_Central_SerialNumString(uint8 *snString) 
{
    #if defined(USBUART_Central_ENABLE_FWSN_STRING)
        USBUART_Central_snStringConfirm = USBUART_Central_FALSE;
        if(snString != NULL)
        {
            USBUART_Central_fwSerialNumberStringDescriptor = snString;
            /* check descriptor validation */
            if( (USBUART_Central_fwSerialNumberStringDescriptor[0u] > 1u ) &&  \
                (USBUART_Central_fwSerialNumberStringDescriptor[1u] == USBUART_Central_DESCR_STRING) )
            {
                USBUART_Central_snStringConfirm = USBUART_Central_TRUE;
            }
        }
    #else
        snString = snString;
    #endif  /* USBUART_Central_ENABLE_FWSN_STRING */
}


/*******************************************************************************
* Function Name: USBUART_Central_HandleStandardRqst
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
uint8 USBUART_Central_HandleStandardRqst(void) 
{
    uint8 requestHandled = USBUART_Central_FALSE;
    #if defined(USBUART_Central_ENABLE_STRINGS)
        volatile uint8 *pStr = 0u;
        #if defined(USBUART_Central_ENABLE_DESCRIPTOR_STRINGS)
            uint8 nStr;
        #endif /* USBUART_Central_ENABLE_DESCRIPTOR_STRINGS */
    #endif /* USBUART_Central_ENABLE_STRINGS */
    uint16 count;

    T_USBUART_Central_LUT *pTmp;
    USBUART_Central_currentTD.count = 0u;

    if ((CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_DIR_MASK) == USBUART_Central_RQST_DIR_D2H)
    {
        /* Control Read */
        switch (CY_GET_REG8(USBUART_Central_bRequest))
        {
            case USBUART_Central_GET_DESCRIPTOR:
                if (CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_DESCR_DEVICE)
                {
                    pTmp = USBUART_Central_GetDeviceTablePtr();
                    USBUART_Central_currentTD.pData = pTmp->p_list;
                    USBUART_Central_currentTD.count = USBUART_Central_DEVICE_DESCR_LENGTH;
                    requestHandled  = USBUART_Central_InitControlRead();
                }
                else if (CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_DESCR_CONFIG)
                {
                    pTmp = USBUART_Central_GetConfigTablePtr(CY_GET_REG8(USBUART_Central_wValueLo));
                    USBUART_Central_currentTD.pData = pTmp->p_list;
                    count = ((uint16)(USBUART_Central_currentTD.pData)[ \
                                      USBUART_Central_CONFIG_DESCR_TOTAL_LENGTH_HI] << 8u) | \
                                     (USBUART_Central_currentTD.pData)[USBUART_Central_CONFIG_DESCR_TOTAL_LENGTH_LOW];
                    USBUART_Central_currentTD.count = count;
                    requestHandled  = USBUART_Central_InitControlRead();
                }
                #if defined(USBUART_Central_ENABLE_STRINGS)
                else if (CY_GET_REG8(USBUART_Central_wValueHi) == USBUART_Central_DESCR_STRING)
                {
                    /* Descriptor Strings*/
                    #if defined(USBUART_Central_ENABLE_DESCRIPTOR_STRINGS)
                        nStr = 0u;
                        pStr = (volatile uint8 *)&USBUART_Central_STRING_DESCRIPTORS[0u];
                        while ( (CY_GET_REG8(USBUART_Central_wValueLo) > nStr) && (*pStr != 0u ))
                        {
                            pStr += *pStr;
                            nStr++;
                        };
                    #endif /* End USBUART_Central_ENABLE_DESCRIPTOR_STRINGS */
                    /* Microsoft OS String*/
                    #if defined(USBUART_Central_ENABLE_MSOS_STRING)
                        if( CY_GET_REG8(USBUART_Central_wValueLo) == USBUART_Central_STRING_MSOS )
                        {
                            pStr = (volatile uint8 *)&USBUART_Central_MSOS_DESCRIPTOR[0u];
                        }
                    #endif /* End USBUART_Central_ENABLE_MSOS_STRING*/
                    /* SN string*/
                    #if defined(USBUART_Central_ENABLE_SN_STRING)
                        if( (CY_GET_REG8(USBUART_Central_wValueLo) != 0) && 
                            (CY_GET_REG8(USBUART_Central_wValueLo) == 
                            USBUART_Central_DEVICE0_DESCR[USBUART_Central_DEVICE_DESCR_SN_SHIFT]) )
                        {
                            pStr = (volatile uint8 *)&USBUART_Central_SN_STRING_DESCRIPTOR[0u];
                            if(USBUART_Central_snStringConfirm != USBUART_Central_FALSE)
                            {
                                pStr = USBUART_Central_fwSerialNumberStringDescriptor;
                            }
                            #if defined(USBUART_Central_ENABLE_IDSN_STRING)
                                /* Read DIE ID and genarete string descriptor in RAM*/
                                USBUART_Central_ReadDieID(USBUART_Central_idSerialNumberStringDescriptor);
                                pStr = USBUART_Central_idSerialNumberStringDescriptor;
                            #endif    /* End USBUART_Central_ENABLE_IDSN_STRING */
                        }
                    #endif    /* End USBUART_Central_ENABLE_SN_STRING */
                    if (*pStr != 0u)
                    {
                        USBUART_Central_currentTD.count = *pStr;
                        USBUART_Central_currentTD.pData = pStr;
                        requestHandled  = USBUART_Central_InitControlRead();
                    }
                }
                #endif /* End USBUART_Central_ENABLE_STRINGS */
                else
                {
                    requestHandled = USBUART_Central_DispatchClassRqst();
                }
                break;
            case USBUART_Central_GET_STATUS:
                switch ((CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_RCPT_MASK))
                {
                    case USBUART_Central_RQST_RCPT_EP:
                        USBUART_Central_currentTD.count = USBUART_Central_EP_STATUS_LENGTH;
                        USBUART_Central_tBuffer[0] = USBUART_Central_EP[ \
                                        CY_GET_REG8(USBUART_Central_wIndexLo) & USBUART_Central_DIR_UNUSED].hwEpState;
                        USBUART_Central_tBuffer[1] = 0u;
                        USBUART_Central_currentTD.pData = &USBUART_Central_tBuffer[0u];
                        requestHandled  = USBUART_Central_InitControlRead();
                        break;
                    case USBUART_Central_RQST_RCPT_DEV:
                        USBUART_Central_currentTD.count = USBUART_Central_DEVICE_STATUS_LENGTH;
                        USBUART_Central_tBuffer[0u] = USBUART_Central_deviceStatus;
                        USBUART_Central_tBuffer[1u] = 0u;
                        USBUART_Central_currentTD.pData = &USBUART_Central_tBuffer[0u];
                        requestHandled  = USBUART_Central_InitControlRead();
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            case USBUART_Central_GET_CONFIGURATION:
                USBUART_Central_currentTD.count = 1u;
                USBUART_Central_currentTD.pData = (uint8 *)&USBUART_Central_configuration;
                requestHandled  = USBUART_Central_InitControlRead();
                break;
            case USBUART_Central_GET_INTERFACE:
                USBUART_Central_currentTD.count = 1u;
                USBUART_Central_currentTD.pData = (uint8 *)&USBUART_Central_interfaceSetting[ \
                                                                            CY_GET_REG8(USBUART_Central_wIndexLo)];
                requestHandled  = USBUART_Central_InitControlRead();
                break;
            default: /* requestHandled is initialezed as FALSE by default */
                break;
        }
    }
    else {
        /* Control Write */
        switch (CY_GET_REG8(USBUART_Central_bRequest))
        {
            case USBUART_Central_SET_ADDRESS:
                USBUART_Central_deviceAddress = CY_GET_REG8(USBUART_Central_wValueLo);
                requestHandled = USBUART_Central_InitNoDataControlTransfer();
                break;
            case USBUART_Central_SET_CONFIGURATION:
                USBUART_Central_configuration = CY_GET_REG8(USBUART_Central_wValueLo);
                USBUART_Central_configurationChanged = USBUART_Central_TRUE;
                USBUART_Central_Config(USBUART_Central_TRUE);
                requestHandled = USBUART_Central_InitNoDataControlTransfer();
                break;
            case USBUART_Central_SET_INTERFACE:
                if (USBUART_Central_ValidateAlternateSetting())
                {
                    USBUART_Central_interfaceNumber = CY_GET_REG8(USBUART_Central_wIndexLo);
                    USBUART_Central_configurationChanged = USBUART_Central_TRUE;
                    #if ((USBUART_Central_EP_MA == USBUART_Central__MA_DYNAMIC) && \
                         (USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL) )
                        USBUART_Central_Config(USBUART_Central_FALSE);
                    #else
                        USBUART_Central_ConfigAltChanged();
                    #endif /* End (USBUART_Central_EP_MA == USBUART_Central__MA_DYNAMIC) */
                    /* Update handled Alt setting changes status */
                    USBUART_Central_interfaceSetting_last[USBUART_Central_interfaceNumber] = 
                         USBUART_Central_interfaceSetting[USBUART_Central_interfaceNumber];
                    requestHandled = USBUART_Central_InitNoDataControlTransfer();
                }
                break;
            case USBUART_Central_CLEAR_FEATURE:
                switch (CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_RCPT_MASK)
                {
                    case USBUART_Central_RQST_RCPT_EP:
                        if (CY_GET_REG8(USBUART_Central_wValueLo) == USBUART_Central_ENDPOINT_HALT)
                        {
                            requestHandled = USBUART_Central_ClearEndpointHalt();
                        }
                        break;
                    case USBUART_Central_RQST_RCPT_DEV:
                        /* Clear device REMOTE_WAKEUP */
                        if (CY_GET_REG8(USBUART_Central_wValueLo) == USBUART_Central_DEVICE_REMOTE_WAKEUP)
                        {
                            USBUART_Central_deviceStatus &= ~USBUART_Central_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBUART_Central_InitNoDataControlTransfer();
                        }
                        break;
                    case USBUART_Central_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (CY_GET_REG8(USBUART_Central_wIndexLo) < USBUART_Central_MAX_INTERFACES_NUMBER)
                        {
                            USBUART_Central_interfaceStatus[CY_GET_REG8(USBUART_Central_wIndexLo)] &= \
                                                                ~(CY_GET_REG8(USBUART_Central_wValueLo)) ;
                            requestHandled = USBUART_Central_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            case USBUART_Central_SET_FEATURE:
                switch (CY_GET_REG8(USBUART_Central_bmRequestType) & USBUART_Central_RQST_RCPT_MASK)
                {
                    case USBUART_Central_RQST_RCPT_EP:
                        if (CY_GET_REG8(USBUART_Central_wValueLo) == USBUART_Central_ENDPOINT_HALT)
                        {
                            requestHandled = USBUART_Central_SetEndpointHalt();
                        }
                        break;
                    case USBUART_Central_RQST_RCPT_DEV:
                        /* Set device REMOTE_WAKEUP */
                        if (CY_GET_REG8(USBUART_Central_wValueLo) == USBUART_Central_DEVICE_REMOTE_WAKEUP)
                        {
                            USBUART_Central_deviceStatus |= USBUART_Central_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBUART_Central_InitNoDataControlTransfer();
                        }
                        break;
                    case USBUART_Central_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (CY_GET_REG8(USBUART_Central_wIndexLo) < USBUART_Central_MAX_INTERFACES_NUMBER)
                        {
                            USBUART_Central_interfaceStatus[CY_GET_REG8(USBUART_Central_wIndexLo)] &= \
                                                                ~(CY_GET_REG8(USBUART_Central_wValueLo)) ;
                            requestHandled = USBUART_Central_InitNoDataControlTransfer();
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


#if defined(USBUART_Central_ENABLE_IDSN_STRING)

    /***************************************************************************
    * Function Name: USBUART_Central_ReadDieID
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
    void USBUART_Central_ReadDieID(uint8 *descr) 
    {
        uint8 i,j;
        uint8 value;
        static char8 const hex[16u] = "0123456789ABCDEF";

        /* check descriptor validation */
        if( (descr[0u] > 1u ) && (descr[1u] == USBUART_Central_DESCR_STRING) )
        {
            /* fill descriptor */
            for(j = 0u, i = 2u; i < descr[0u]; i += 2u)
            {
                value = CY_GET_XTND_REG8((void CYFAR *)(USBUART_Central_DIE_ID + j++));
                descr[i] = (uint8)hex[value >> 4u];
                i += 2u;
                descr[i] = (uint8)hex[value & 0x0Fu];
            }
        }
    }

#endif /* End $INSTANCE_NAME`_ENABLE_IDSN_STRING*/


/*******************************************************************************
* Function Name: USBUART_Central_ConfigReg
********************************************************************************
*
* Summary:
*  This routine configures hardware registers from the variables.
*  It is called from USBUART_Central_Config() function and from RestoreConfig
*  after Wakeup.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void USBUART_Central_ConfigReg(void) 
{
    uint8 ep;
    uint8 i;
    #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO)
        uint8 ep_type = 0u;
    #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */

    /* Set the endpoint buffer addresses */
    ep = USBUART_Central_EP1;
    for (i = 0u; i < 0x80u; i+= 0x10u)
    {
        CY_SET_REG8(&USBUART_Central_ARB_EP1_CFG_PTR[i], USBUART_Central_ARB_EPX_CFG_CRC_BYPASS |
                                                          USBUART_Central_ARB_EPX_CFG_RESET);

        #if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)
            /* Enable all Arbiter EP Interrupts : err, buf under, buf over, dma gnt(mode2 only), in buf full */
            USBUART_Central_ARB_EP1_INT_EN_PTR[i] = USBUART_Central_ARB_EPX_INT_MASK;
        #endif   /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL */

        if(USBUART_Central_EP[ep].epMode != USBUART_Central_MODE_DISABLE)
        {
            if((USBUART_Central_EP[ep].addr & USBUART_Central_DIR_IN) != 0u )
            {
                CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[i], USBUART_Central_MODE_NAK_IN);
            }
            else
            {
                CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[i], USBUART_Central_MODE_NAK_OUT);
                /* Prepare EP type mask for automatic memory allocation */
                #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO)
                    ep_type |= 0x01u << (ep - USBUART_Central_EP1);
                #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */
            }
        }
        else
        {
            CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[i], USBUART_Central_MODE_STALL_DATA_EP);
        }

        #if(USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO)
            CY_SET_REG8(&USBUART_Central_SIE_EP1_CNT0_PTR[i],   USBUART_Central_EP[ep].bufferSize >> 8u);
            CY_SET_REG8(&USBUART_Central_SIE_EP1_CNT1_PTR[i],   USBUART_Central_EP[ep].bufferSize & 0xFFu);

            CY_SET_REG8(&USBUART_Central_ARB_RW1_RA_PTR[i],     USBUART_Central_EP[ep].buffOffset & 0xFFu);
            CY_SET_REG8(&USBUART_Central_ARB_RW1_RA_MSB_PTR[i], USBUART_Central_EP[ep].buffOffset >> 8u);
            CY_SET_REG8(&USBUART_Central_ARB_RW1_WA_PTR[i],     USBUART_Central_EP[ep].buffOffset & 0xFFu);
            CY_SET_REG8(&USBUART_Central_ARB_RW1_WA_MSB_PTR[i], USBUART_Central_EP[ep].buffOffset >> 8u);
        #endif /* End USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO */

        ep++;
    }

    #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO)
         /* BUF_SIZE depend on DMA_THRESS value: 55-32 bytes  44-16 bytes 33-8 bytes 22-4 bytes 11-2 bytes */
        USBUART_Central_BUF_SIZE_REG = USBUART_Central_DMA_BUF_SIZE;
        USBUART_Central_DMA_THRES_REG = USBUART_Central_DMA_BYTES_PER_BURST;   /* DMA burst threshold */
        USBUART_Central_DMA_THRES_MSB_REG = 0u;
        USBUART_Central_EP_ACTIVE_REG = USBUART_Central_ARB_INT_MASK;
        USBUART_Central_EP_TYPE_REG = ep_type;
        /* Cfg_cmp bit set to 1 once configuration is complete. */
        USBUART_Central_ARB_CFG_REG = USBUART_Central_ARB_CFG_AUTO_DMA | USBUART_Central_ARB_CFG_AUTO_MEM |
                                       USBUART_Central_ARB_CFG_CFG_CPM;
        /* Cfg_cmp bit set to 0 during configuration of PFSUSB Registers. */
        USBUART_Central_ARB_CFG_REG = USBUART_Central_ARB_CFG_AUTO_DMA | USBUART_Central_ARB_CFG_AUTO_MEM;
    #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */

    CY_SET_REG8(USBUART_Central_SIE_EP_INT_EN_PTR, 0xFFu);
}


/*******************************************************************************
* Function Name: USBUART_Central_Config
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
* USBUART_Central_interfaceClass - Initialized class array for each interface.
*   It is used for hangling Class specific requests depend on interface class.
*   Different classes in multiple Alternate settings does not supported.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Central_Config(uint8 clearAltSetting) 
{
    uint8 ep;
    uint8 cur_ep;
    uint8 i;
    uint8 iso;
    uint8 *pDescr;
    #if(USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO)
        uint16 count = 0u;
    #endif /* End USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO */
    
    T_USBUART_Central_LUT *pTmp;
    T_USBUART_Central_EP_SETTINGS_BLOCK *pEP;

    /* Clear all of the endpoints */
    for (ep = 0u; ep < USBUART_Central_MAX_EP; ep++)
    {
        USBUART_Central_EP[ep].attrib = 0u;
        USBUART_Central_EP[ep].hwEpState = 0u;
        USBUART_Central_EP[ep].apiEpState = USBUART_Central_NO_EVENT_PENDING;
        USBUART_Central_EP[ep].epToggle = 0u;
        USBUART_Central_EP[ep].epMode = USBUART_Central_MODE_DISABLE;
        USBUART_Central_EP[ep].bufferSize = 0u;
        USBUART_Central_EP[ep].interface = 0u;

    }

    /* Clear Alternate settings for all interfaces */
    if(clearAltSetting != 0u)
    {
        for (i = 0u; i < USBUART_Central_MAX_INTERFACES_NUMBER; i++)
        {
            USBUART_Central_interfaceSetting[i] = 0x00u;
            USBUART_Central_interfaceSetting_last[i] = 0x00u;
        }
    }

    /* Init Endpoints and Device Status if configured */
    if(USBUART_Central_configuration > 0u)
    {
        pTmp = USBUART_Central_GetConfigTablePtr(USBUART_Central_configuration - 1u);
        /* Set Power status for current configuration */
        pDescr = (uint8 *)pTmp->p_list;
        if((pDescr[USBUART_Central_CONFIG_DESCR_ATTRIB] & USBUART_Central_CONFIG_DESCR_ATTRIB_SELF_POWERED) != 0u)
        {
            USBUART_Central_deviceStatus |=  USBUART_Central_DEVICE_STATUS_SELF_POWERED;
        }
        else
        {
            USBUART_Central_deviceStatus &=  ~USBUART_Central_DEVICE_STATUS_SELF_POWERED;
        }
        pTmp++;
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        #if ((USBUART_Central_EP_MA == USBUART_Central__MA_DYNAMIC) && \
             (USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL) )
            /* Config for dynamic EP memory allocation */
            /* p_list points the endpoint setting table. */
            pEP = (T_USBUART_Central_EP_SETTINGS_BLOCK *) pTmp->p_list;
            for (i = 0u; i < ep; i++, pEP++)
            {
                /* compate current Alternate setting with EP Alt*/
                if(USBUART_Central_interfaceSetting[pEP->interface] == pEP->altSetting)
                {
                    cur_ep = pEP->addr & USBUART_Central_DIR_UNUSED;
                    iso  = ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) == USBUART_Central_EP_TYPE_ISOC);
                    if (pEP->addr & USBUART_Central_DIR_IN)
                    {
                        /* IN Endpoint */
                        USBUART_Central_EP[cur_ep].apiEpState = USBUART_Central_EVENT_PENDING;
                        USBUART_Central_EP[cur_ep].epMode = \
                                                (iso ? USBUART_Central_MODE_ISO_IN : USBUART_Central_MODE_ACK_IN);
                        #if defined(USBUART_Central_ENABLE_CDC_CLASS)
                            if(((pEP->bMisc == USBUART_Central_CLASS_CDC_DATA) ||
                                (pEP->bMisc == USBUART_Central_CLASS_CDC)) &&
                                ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) != USBUART_Central_EP_TYPE_INT))
                            {
                                USBUART_Central_cdc_data_in_ep = cur_ep;
                            }
                        #endif  /* End USBUART_Central_ENABLE_CDC_CLASS*/
                        #if defined(USBUART_Central_ENABLE_MIDI_STREAMING)
                            if((pEP->bMisc == USBUART_Central_CLASS_AUDIO) &&
                               ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) == USBUART_Central_EP_TYPE_BULK))
                            {
                                USBUART_Central_midi_in_ep = cur_ep;
                            }
                        #endif  /* End USBUART_Central_ENABLE_MIDI_STREAMING*/
                    }
                    else
                    {
                        /* OUT Endpoint */
                        USBUART_Central_EP[cur_ep].apiEpState = USBUART_Central_NO_EVENT_PENDING;
                        USBUART_Central_EP[cur_ep].epMode = \
                                                (iso ? USBUART_Central_MODE_ISO_OUT : USBUART_Central_MODE_ACK_OUT);
                        #if defined(USBUART_Central_ENABLE_CDC_CLASS)
                            if(((pEP->bMisc == USBUART_Central_CLASS_CDC_DATA) ||
                                (pEP->bMisc == USBUART_Central_CLASS_CDC)) &&
                                ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) != USBUART_Central_EP_TYPE_INT))
                            {
                                USBUART_Central_cdc_data_out_ep = cur_ep;
                            }
                        #endif  /* End USBUART_Central_ENABLE_CDC_CLASS*/
                        #if defined(USBUART_Central_ENABLE_MIDI_STREAMING)
                            if((pEP->bMisc == USBUART_Central_CLASS_AUDIO) &&
                               ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) == USBUART_Central_EP_TYPE_BULK))
                            {
                                USBUART_Central_midi_out_ep = cur_ep;
                            }
                        #endif  /* End USBUART_Central_ENABLE_MIDI_STREAMING*/
                    }
                    USBUART_Central_EP[cur_ep].bufferSize = pEP->bufferSize;
                    USBUART_Central_EP[cur_ep].addr = pEP->addr;
                    USBUART_Central_EP[cur_ep].attrib = pEP->attributes;
                }
            }
        #else /* Config for static EP memory allocation  */
            for (i = USBUART_Central_EP1; i < USBUART_Central_MAX_EP; i++)
            {
                /* and p_list points the endpoint setting table. */
                pEP = (T_USBUART_Central_EP_SETTINGS_BLOCK *) pTmp->p_list;
                /* find max length for each EP and select it (length could be different in different Alt settings) */
                /* but other settings should be correct with regards to Interface alt Setting */
                for (cur_ep = 0u; cur_ep < ep; cur_ep++, pEP++)
                {
                    /* EP count is equal to EP # in table and we found larger EP length than have before*/
                    if(i == (pEP->addr & USBUART_Central_DIR_UNUSED))
                    {
                        if(USBUART_Central_EP[i].bufferSize < pEP->bufferSize)
                        {
                            USBUART_Central_EP[i].bufferSize = pEP->bufferSize;
                        }
                        /* compate current Alternate setting with EP Alt*/
                        if(USBUART_Central_interfaceSetting[pEP->interface] == pEP->altSetting)
                        {
                            iso  = ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) == USBUART_Central_EP_TYPE_ISOC);
                            if (pEP->addr & USBUART_Central_DIR_IN)
                            {
                                /* IN Endpoint */
                                USBUART_Central_EP[i].apiEpState = USBUART_Central_EVENT_PENDING;
                                USBUART_Central_EP[i].epMode =
                                                    (iso ? USBUART_Central_MODE_ISO_IN : USBUART_Central_MODE_ACK_IN);
                                /* Find and init CDC IN endpoint number */
                                #if defined(USBUART_Central_ENABLE_CDC_CLASS)
                                    if(((pEP->bMisc == USBUART_Central_CLASS_CDC_DATA) ||
                                        (pEP->bMisc == USBUART_Central_CLASS_CDC)) &&
                                        ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) !=
                                                                                        USBUART_Central_EP_TYPE_INT))
                                    {
                                        USBUART_Central_cdc_data_in_ep = i;
                                    }
                                #endif  /* End USBUART_Central_ENABLE_CDC_CLASS*/
                                #if defined(USBUART_Central_ENABLE_MIDI_STREAMING)
                                    if((pEP->bMisc == USBUART_Central_CLASS_AUDIO) &&
                                       ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) ==
                                                                                        USBUART_Central_EP_TYPE_BULK))
                                    {
                                        USBUART_Central_midi_in_ep = i;
                                    }
                                #endif  /* End USBUART_Central_ENABLE_MIDI_STREAMING*/
                            }
                            else
                            {
                                /* OUT Endpoint */
                                USBUART_Central_EP[i].apiEpState = USBUART_Central_NO_EVENT_PENDING;
                                USBUART_Central_EP[i].epMode =
                                                (iso ? USBUART_Central_MODE_ISO_OUT : USBUART_Central_MODE_ACK_OUT);
                                /* Find and init CDC IN endpoint number */
                                #if defined(USBUART_Central_ENABLE_CDC_CLASS)
                                    if(((pEP->bMisc == USBUART_Central_CLASS_CDC_DATA) ||
                                        (pEP->bMisc == USBUART_Central_CLASS_CDC)) &&
                                        ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) !=
                                                                                    USBUART_Central_EP_TYPE_INT))
                                    {
                                        USBUART_Central_cdc_data_out_ep = i;
                                    }
                                #endif  /* End USBUART_Central_ENABLE_CDC_CLASS*/
                                #if defined(USBUART_Central_ENABLE_MIDI_STREAMING)
                                    if((pEP->bMisc == USBUART_Central_CLASS_AUDIO) &&
                                       ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) ==
                                                                                        USBUART_Central_EP_TYPE_BULK))
                                    {
                                        USBUART_Central_midi_out_ep = i;
                                    }
                                #endif  /* End USBUART_Central_ENABLE_MIDI_STREAMING*/
                            }
                            USBUART_Central_EP[i].addr = pEP->addr;
                            USBUART_Central_EP[i].attrib = pEP->attributes;

                            #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO)
                                break;      /* use first EP setting in Auto memory managment */
                            #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */
                        }
                    }
                }
            }
        #endif /* End (USBUART_Central_EP_MA == USBUART_Central__MA_DYNAMIC) */

        /* Init class array for each interface and interface number for each EP.
        *  It is used for hangling Class specific requests directed to either an
        *  interface or the endpoint.
        */
        /* p_list points the endpoint setting table. */
        pEP = (T_USBUART_Central_EP_SETTINGS_BLOCK *) pTmp->p_list;
        for (i = 0u; i < ep; i++, pEP++)
        {
            /* config interface number for each EP*/
            USBUART_Central_EP[pEP->addr & USBUART_Central_DIR_UNUSED].interface = pEP->interface;
        }
        /* init pointer on interface class table*/
        USBUART_Central_interfaceClass = USBUART_Central_GetInterfaceClassTablePtr();
        /* Set the endpoint buffer addresses */
        
        #if(USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO)
            for (ep = USBUART_Central_EP1; ep < USBUART_Central_MAX_EP; ep++)
            {
                USBUART_Central_EP[ep].buffOffset = count;
                 count += USBUART_Central_EP[ep].bufferSize;
            }
        #endif /* End USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO */

        /* Congigure hardware registers */
        USBUART_Central_ConfigReg();
    } /* USBUART_Central_configuration > 0 */
}


/*******************************************************************************
* Function Name: USBUART_Central_ConfigAltChanged
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
void USBUART_Central_ConfigAltChanged() 
{
    uint8 ep,cur_ep,i;
    uint8 iso;
    uint8 ri;

    T_USBUART_Central_LUT *pTmp;
    T_USBUART_Central_EP_SETTINGS_BLOCK *pEP;


    /* Init Endpoints and Device Status if configured */
    if(USBUART_Central_configuration > 0u)
    {
        pTmp = USBUART_Central_GetConfigTablePtr(USBUART_Central_configuration - 1u);
        pTmp++;
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        /* Do not touch EP wich doesn't need reconfiguration */
        /* When Alt setting chaged, the only required endpoints need to be reconfigured */
        /* p_list points the endpoint setting table. */
        pEP = (T_USBUART_Central_EP_SETTINGS_BLOCK *) pTmp->p_list;
        for (i = 0u; i < ep; i++, pEP++)
        {
            /*If Alt setting changed and new is same with EP Alt */
            if((USBUART_Central_interfaceSetting[pEP->interface] !=
                USBUART_Central_interfaceSetting_last[pEP->interface] ) &&
               (USBUART_Central_interfaceSetting[pEP->interface] == pEP->altSetting) &&
                pEP->interface == CY_GET_REG8(USBUART_Central_wIndexLo))
            {
                cur_ep = pEP->addr & USBUART_Central_DIR_UNUSED;
                ri = ((cur_ep - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);
                iso  = ((pEP->attributes & USBUART_Central_EP_TYPE_MASK) == USBUART_Central_EP_TYPE_ISOC);
                if (pEP->addr & USBUART_Central_DIR_IN)
                {
                    /* IN Endpoint */
                    USBUART_Central_EP[cur_ep].apiEpState = USBUART_Central_EVENT_PENDING;
                    USBUART_Central_EP[cur_ep].epMode =
                                            (iso ? USBUART_Central_MODE_ISO_IN : USBUART_Central_MODE_ACK_IN);
                }
                else
                {
                    /* OUT Endpoint */
                    USBUART_Central_EP[cur_ep].apiEpState = USBUART_Central_NO_EVENT_PENDING;
                    USBUART_Central_EP[cur_ep].epMode =
                                            (iso ? USBUART_Central_MODE_ISO_OUT : USBUART_Central_MODE_ACK_OUT);
                }
                 /* Change the SIE mode for the selected EP to NAK ALL */
                 CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_NAK_IN_OUT);
                USBUART_Central_EP[cur_ep].bufferSize = pEP->bufferSize;
                USBUART_Central_EP[cur_ep].addr = pEP->addr;
                USBUART_Central_EP[cur_ep].attrib = pEP->attributes;

                /* Clear the data toggle */
                USBUART_Central_EP[cur_ep].epToggle = 0u;

                /* Dynamic reconfiguration for mode 3 transfer */
                #if((USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO) && (!CY_PSOC5A) )
                    /* In_data_rdy for selected EP should be set to 0 */
                    USBUART_Central_ARB_EP1_CFG_PTR[ri] &= ~USBUART_Central_ARB_EPX_CFG_IN_DATA_RDY;

                    /* write the EP number for which reconfiguration is required */
                    USBUART_Central_DYN_RECONFIG_REG = (cur_ep - USBUART_Central_EP1) << 
                                                        USBUART_Central_DYN_RECONFIG_EP_SHIFT;
                    /* Set the dyn_config_en bit in dynamic reconfiguration register */
                    USBUART_Central_DYN_RECONFIG_REG |= USBUART_Central_DYN_RECONFIG_ENABLE;
                    /* wait for the dyn_config_rdy bit to set by the block,
                    *  this bit will be set to 1 when block is ready for reconfiguration.
                    */
                    while((USBUART_Central_DYN_RECONFIG_REG & USBUART_Central_DYN_RECONFIG_RDY_STS) == 0u);
                    /* Once dyn_config_rdy bit is set, FW can change the EP configuration. */
                    /* Change EP Type with new direction */
                    if((pEP->addr & USBUART_Central_DIR_IN) == 0)
                    {
                        USBUART_Central_EP_TYPE_REG |= 0x01u << (cur_ep - USBUART_Central_EP1);
                    }
                    else
                    {
                        USBUART_Central_EP_TYPE_REG &= ~(0x01u << (cur_ep - USBUART_Central_EP1));
                    }
                    /* dynamic reconfiguration enable bit cleared, pointers and control/status
                    *  signals for the selected EP is cleared/re-initialized on negedge
                    *  of dynamic reconfiguration enable bit).
                    */
                    USBUART_Central_DYN_RECONFIG_REG &= ~USBUART_Central_DYN_RECONFIG_ENABLE;
                    /* The main loop has to reenable DMA and OUT endpoint*/
                #else
                CY_SET_REG8(&USBUART_Central_SIE_EP1_CNT0_PTR[ri],   USBUART_Central_EP[cur_ep].bufferSize >> 8u);
                CY_SET_REG8(&USBUART_Central_SIE_EP1_CNT1_PTR[ri],   USBUART_Central_EP[cur_ep].bufferSize & 0xFFu);

                CY_SET_REG8(&USBUART_Central_ARB_RW1_RA_PTR[ri],     USBUART_Central_EP[cur_ep].buffOffset & 0xFFu);
                CY_SET_REG8(&USBUART_Central_ARB_RW1_RA_MSB_PTR[ri], USBUART_Central_EP[cur_ep].buffOffset >> 8u);
                CY_SET_REG8(&USBUART_Central_ARB_RW1_WA_PTR[ri],     USBUART_Central_EP[cur_ep].buffOffset & 0xFFu);
                CY_SET_REG8(&USBUART_Central_ARB_RW1_WA_MSB_PTR[ri], USBUART_Central_EP[cur_ep].buffOffset >> 8u);
                #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */
            }
        }
    } /* USBUART_Central_configuration > 0 */
}


/*******************************************************************************
* Function Name: USBUART_Central_GetConfigTablePtr
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
T_USBUART_Central_LUT *USBUART_Central_GetConfigTablePtr(uint8 c) \
                                                        
{
    /* Device Table */
    T_USBUART_Central_LUT *pTmp;

    pTmp = USBUART_Central_GetDeviceTablePtr();

    /* The first entry points to the Device Descriptor,
       the the configuration entries  */
    return(pTmp[c + 1u].p_list);
}


/*******************************************************************************
* Function Name: USBUART_Central_GetDeviceTablePtr
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
T_USBUART_Central_LUT *USBUART_Central_GetDeviceTablePtr(void) 
{
    /* Device Table */
    return(USBUART_Central_TABLE[USBUART_Central_device].p_list);
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
uint8 CYCODE *USBUART_Central_GetInterfaceClassTablePtr(void)
                                                        
{
    T_USBUART_Central_LUT *pTmp;
    uint8 currentInterfacesNum;

    pTmp = USBUART_Central_GetConfigTablePtr(USBUART_Central_configuration - 1u);
    currentInterfacesNum  = ((uint8 *) pTmp->p_list)[USBUART_Central_CONFIG_DESCR_NUM_INTERFACES];
    /* Third entry in the LUT starts the Interface Table pointers */
    pTmp += 2;
    /* The INTERFACE_CLASS table is located after all interfaces*/
    pTmp += currentInterfacesNum;
    return(pTmp->p_list);
}


/*******************************************************************************
* Function Name: USBUART_Central_TerminateEP
********************************************************************************
*
* Summary:
*  This function terminates the specified USBFS endpoint. 
*  This function should be used before endpoint reconfiguration.
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
void USBUART_Central_TerminateEP(uint8 ep) 
{
    uint8 ri;

    ep &= USBUART_Central_DIR_UNUSED;
    ri = ((ep - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBUART_Central_EP0) && (ep < USBUART_Central_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBUART_Central_EP[ep].hwEpState |= (USBUART_Central_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART_Central_EP[ep].epToggle = 0u;
        USBUART_Central_EP[ep].apiEpState = USBUART_Central_NO_EVENT_ALLOWED;

        if (USBUART_Central_EP[ep].addr & USBUART_Central_DIR_IN)
        {
            /* IN Endpoint */
            CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_NAK_IN);
        }
        else
        {
            /* OUT Endpoint */
            CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_NAK_OUT);
        }
    }
}


/*******************************************************************************
* Function Name: USBUART_Central_SetEndpointHalt
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
uint8 USBUART_Central_SetEndpointHalt(void) 
{
    uint8 ep, ri;
    uint8 requestHandled = USBUART_Central_FALSE;

    /* Set endpoint halt */
    ep = CY_GET_REG8(USBUART_Central_wIndexLo) & USBUART_Central_DIR_UNUSED;
    ri = ((ep - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBUART_Central_EP0) && (ep < USBUART_Central_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBUART_Central_EP[ep].hwEpState |= (USBUART_Central_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART_Central_EP[ep].epToggle = 0u;
        USBUART_Central_EP[ep].apiEpState |= USBUART_Central_NO_EVENT_ALLOWED;

        if (USBUART_Central_EP[ep].addr & USBUART_Central_DIR_IN)
        {
            /* IN Endpoint */
            CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_STALL_DATA_EP |
                                                               USBUART_Central_MODE_ACK_IN);
        }
        else
        {
            /* OUT Endpoint */
            CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_STALL_DATA_EP |
                                                               USBUART_Central_MODE_ACK_OUT);
        }
        requestHandled = USBUART_Central_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBUART_Central_ClearEndpointHalt
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
uint8 USBUART_Central_ClearEndpointHalt(void) 
{
    uint8 ep, ri;
    uint8 requestHandled = USBUART_Central_FALSE;

    /* Clear endpoint halt */
    ep = CY_GET_REG8(USBUART_Central_wIndexLo) & USBUART_Central_DIR_UNUSED;
    ri = ((ep - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBUART_Central_EP0) && (ep < USBUART_Central_MAX_EP))
    {
        /* Clear the endpoint Halt */
        USBUART_Central_EP[ep].hwEpState &= ~(USBUART_Central_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART_Central_EP[ep].epToggle = 0u;
        /* Clear toggle bit for already armed packet */
        CY_SET_REG8(&USBUART_Central_SIE_EP1_CNT0_PTR[ri], 
                    CY_GET_REG8(&USBUART_Central_SIE_EP1_CNT0_PTR[ri]) & ~USBUART_Central_EPX_CNT_DATA_TOGGLE);
        /* Return api State as it was defined before */
        USBUART_Central_EP[ep].apiEpState &= ~USBUART_Central_NO_EVENT_ALLOWED;

        if (USBUART_Central_EP[ep].addr & USBUART_Central_DIR_IN)
        {
            /* IN Endpoint */
            if(USBUART_Central_EP[ep].apiEpState == USBUART_Central_IN_BUFFER_EMPTY)
            {       /* Wait for next packet from application */
                CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_NAK_IN);
            }
            else    /* Continue armed transfer */
            {
                CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_ACK_IN);
            }
        }
        else
        {
            /* OUT Endpoint */
            if(USBUART_Central_EP[ep].apiEpState == USBUART_Central_OUT_BUFFER_FULL)
            {       /* Allow application to read full buffer */
                CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_NAK_OUT);
            }
            else    /* Mark endpoint as empty, so it will be reloaded */
            {
                CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_ACK_OUT);
            }
        }
        requestHandled = USBUART_Central_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBUART_Central_ValidateAlternateSetting
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
uint8 USBUART_Central_ValidateAlternateSetting(void) 
{
    uint8 requestHandled = USBUART_Central_TRUE;
    uint8 interfaceNum;
    T_USBUART_Central_LUT *pTmp;
    uint8 currentInterfacesNum;

    interfaceNum = CY_GET_REG8(USBUART_Central_wIndexLo);
    /* Validate interface setting, stall if invalid. */
    pTmp = USBUART_Central_GetConfigTablePtr(USBUART_Central_configuration - 1u);
    currentInterfacesNum  = ((uint8 *) pTmp->p_list)[USBUART_Central_CONFIG_DESCR_NUM_INTERFACES];

    if((interfaceNum >= currentInterfacesNum) || (interfaceNum >= USBUART_Central_MAX_INTERFACES_NUMBER))
    {   /* wrong interface number */
        requestHandled = USBUART_Central_FALSE;
    }
    else
    {
        /* save current Alt setting to find out the difference in Config() function */
        USBUART_Central_interfaceSetting_last[interfaceNum] = USBUART_Central_interfaceSetting[interfaceNum];
        USBUART_Central_interfaceSetting[interfaceNum] = CY_GET_REG8(USBUART_Central_wValueLo);
    }

    return (requestHandled);
}


/* [] END OF FILE */
