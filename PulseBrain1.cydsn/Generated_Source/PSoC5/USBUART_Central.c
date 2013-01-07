/*******************************************************************************
* File Name: USBUART_Central.c
* Version 2.50
*
* Description:
*  API for USBFS Component.
*
* Note:
*  Many of the functions use endpoint number.  RAM arrays are sized with 9
*  elements so they are indexed directly by epNumber.  The SIE and ARB
*  registers are indexed by variations of epNumber - 1.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <CyDmac.h>
#include "USBUART_Central.h"
#include "USBUART_Central_hid.h"
#if(USBUART_Central_DMA1_REMOVE == 0u)
    #include "USBUART_Central_ep1_dma.h"
#endif   /* End USBUART_Central_DMA1_REMOVE */
#if(USBUART_Central_DMA2_REMOVE == 0u)
    #include "USBUART_Central_ep2_dma.h"
#endif   /* End USBUART_Central_DMA2_REMOVE */
#if(USBUART_Central_DMA3_REMOVE == 0u)
    #include "USBUART_Central_ep3_dma.h"
#endif   /* End USBUART_Central_DMA3_REMOVE */
#if(USBUART_Central_DMA4_REMOVE == 0u)
    #include "USBUART_Central_ep4_dma.h"
#endif   /* End USBUART_Central_DMA4_REMOVE */
#if(USBUART_Central_DMA5_REMOVE == 0u)
    #include "USBUART_Central_ep5_dma.h"
#endif   /* End USBUART_Central_DMA5_REMOVE */
#if(USBUART_Central_DMA6_REMOVE == 0u)
    #include "USBUART_Central_ep6_dma.h"
#endif   /* End USBUART_Central_DMA6_REMOVE */
#if(USBUART_Central_DMA7_REMOVE == 0u)
    #include "USBUART_Central_ep7_dma.h"
#endif   /* End USBUART_Central_DMA7_REMOVE */
#if(USBUART_Central_DMA8_REMOVE == 0u)
    #include "USBUART_Central_ep8_dma.h"
#endif   /* End USBUART_Central_DMA8_REMOVE */


/***************************************
* External data references
***************************************/

extern volatile uint8 USBUART_Central_configuration;
extern volatile uint8 USBUART_Central_interfaceNumber;
extern volatile uint8 USBUART_Central_configurationChanged;
extern volatile uint8 USBUART_Central_interfaceSetting[];
extern volatile uint8 USBUART_Central_interfaceSetting_last[];
extern volatile uint8 USBUART_Central_deviceAddress;
extern volatile uint8 USBUART_Central_deviceStatus;
extern volatile uint8 USBUART_Central_device;
extern volatile uint8 USBUART_Central_transferState;
extern volatile uint8 USBUART_Central_lastPacketSize;

extern volatile T_USBUART_Central_EP_CTL_BLOCK USBUART_Central_EP[];

#if defined(USBUART_Central_ENABLE_HID_CLASS)
    extern volatile uint8 USBUART_Central_hidProtocol[];
#endif /* USBUART_Central_ENABLE_HID_CLASS */


/***************************************
* External function references
***************************************/

CY_ISR_PROTO(USBUART_Central_EP_0_ISR);
#if(USBUART_Central_EP1_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Central_EP_1_ISR);
#endif /* End USBUART_Central_EP1_ISR_REMOVE */
#if(USBUART_Central_EP2_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Central_EP_2_ISR);
#endif /* End USBUART_Central_EP2_ISR_REMOVE */
#if(USBUART_Central_EP3_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Central_EP_3_ISR);
#endif /* End USBUART_Central_EP3_ISR_REMOVE */
#if(USBUART_Central_EP4_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Central_EP_4_ISR);
#endif /* End USBUART_Central_EP4_ISR_REMOVE */
#if(USBUART_Central_EP5_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Central_EP_5_ISR);
#endif /* End USBUART_Central_EP5_ISR_REMOVE */
#if(USBUART_Central_EP6_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Central_EP_6_ISR);
#endif /* End USBUART_Central_EP6_ISR_REMOVE */
#if(USBUART_Central_EP7_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Central_EP_7_ISR);
#endif /* End USBUART_Central_EP7_ISR_REMOVE */
#if(USBUART_Central_EP8_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Central_EP_8_ISR);
#endif /* End USBUART_Central_EP8_ISR_REMOVE */
CY_ISR_PROTO(USBUART_Central_BUS_RESET_ISR);
#if(USBUART_Central_SOF_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Central_SOF_ISR);
#endif /* End USBUART_Central_SOF_ISR_REMOVE */
#if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)
    CY_ISR_PROTO(USBUART_Central_ARB_ISR);
#endif /* End USBUART_Central_EP_MM */


/***************************************
* Global data allocation
***************************************/

uint8 USBUART_Central_initVar = 0u;
#if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)
    uint8 USBUART_Central_DmaChan[USBUART_Central_MAX_EP];
    uint8 USBUART_Central_DmaTd[USBUART_Central_MAX_EP];
#endif /* End USBUART_Central_EP_MM */


/*******************************************************************************
* Function Name: USBUART_Central_Start
********************************************************************************
*
* Summary:
*  This function initialize the USB SIE, arbiter and the
*  endpoint APIs, including setting the D+ Pullup
*
* Parameters:
*  device: Contains the device number of the desired device descriptor.
*          The device number can be found in the Device Descriptor Tab of 
*          "Configure" dialog, under the settings of desired Device Descriptor,
*          in the "Device Number" field.
*  mode: The operating voltage. This determines whether the voltage regulator
*        is enabled for 5V operation or if pass through mode is used for 3.3V
*        operation. Symbolic names and their associated values are given in the
*        following table.
*       USBUART_Central_3V_OPERATION - Disable voltage regulator and pass-thru
*                                       Vcc for pull-up
*       USBUART_Central_5V_OPERATION - Enable voltage regulator and use
*                                       regulator for pull-up
*       USBUART_Central_DWR_VDDD_OPERATION - Enable or Disable voltage
*                         regulator depend on Vddd Voltage configuration in DWR.
*
* Return:
*   None.
*
* Global variables:
*  The USBUART_Central_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time USBUART_Central_Start() is called.
*  This allows for component Re-Start without unnecessary re-initialization
*  in all subsequent calls to the USBUART_Central_Start() routine.
*  If re-initialization of the component is required the variable should be set
*  to zero before call of UART_Start() routine, or the user may call
*  USBUART_Central_Init() and USBUART_Central_InitComponent() as done
*  in the USBUART_Central_Start() routine.
*
* Side Effects:
*   This function will reset all communication states to default.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Central_Start(uint8 device, uint8 mode) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(USBUART_Central_initVar == 0u)
    {
        USBUART_Central_Init();
        USBUART_Central_initVar = 1u;
    }
    USBUART_Central_InitComponent(device, mode);
}


/*******************************************************************************
* Function Name: USBUART_Central_Init
********************************************************************************
*
* Summary:
*  Initialize component's hardware. Usually called in USBUART_Central_Start().
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
void USBUART_Central_Init(void) 
{
    uint8 enableInterrupts;
    #if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)
        uint16 i;
    #endif   /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL */

    enableInterrupts = CyEnterCriticalSection();

    /* Enable USB block  */
    USBUART_Central_PM_ACT_CFG_REG |= USBUART_Central_PM_ACT_EN_FSUSB;
    /* Enable USB block for Standby Power Mode */
    USBUART_Central_PM_STBY_CFG_REG |= USBUART_Central_PM_STBY_EN_FSUSB;

    #if(CY_PSOC5A)
        /* Disable USBIO for TO3 */
        USBUART_Central_PM_AVAIL_CR_REG &= ~USBUART_Central_PM_AVAIL_EN_FSUSBIO;
    #endif /* End CY_PSOC5A */

    /* Enable core clock */
    USBUART_Central_USB_CLK_EN_REG = USBUART_Central_USB_CLK_ENABLE;

    USBUART_Central_CR1_REG = USBUART_Central_CR1_ENABLE_LOCK;

    #if(CY_PSOC5A)
        /* Enable USBIO for TO3 */
        USBUART_Central_PM_AVAIL_CR_REG |= USBUART_Central_PM_AVAIL_EN_FSUSBIO;
        /* Bus Reset Length, It has correct default value in ES3 */
        USBUART_Central_BUS_RST_CNT_REG = USBUART_Central_BUS_RST_COUNT;
    #endif /* End CY_PSOC5A */

    /* ENABLING USBIO PADS IN USB MODE FROM I/O MODE */
    #if(!CY_PSOC5A)
        /* Ensure USB transmit enable is low (USB_USBIO_CR0.ten). - Manual Transmission - Disabled */
        USBUART_Central_USBIO_CR0_REG &= ~USBUART_Central_USBIO_CR0_TEN;
        CyDelayUs(0);  /*~50ns delay */
        /* Disable the USBIO by asserting PM.USB_CR0.fsusbio_pd_n(Inverted)
        *  high. This will have been set low by the power manger out of reset.
        *  Also confirm USBIO pull-up disabled
        */
        USBUART_Central_PM_USB_CR0_REG &= ~(USBUART_Central_PM_USB_CR0_PD_N |USBUART_Central_PM_USB_CR0_PD_PULLUP_N);

        /* Select iomode to USB mode*/
        USBUART_Central_USBIO_CR1_REG &= ~ USBUART_Central_USBIO_CR1_IOMODE;

        /* Enable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
        USBUART_Central_PM_USB_CR0_REG |= USBUART_Central_PM_USB_CR0_REF_EN;
        /* The reference will be available 1 us after the regulator is enabled */
        CyDelayUs(1);
        /* OR 40us after power restored */
        CyDelayUs(40);
        /* Ensure the single ended disable bits are low (PRT15.INP_DIS[7:6])(input receiver enabled). */
        USBUART_Central_DM_INP_DIS_REG &= ~USBUART_Central_DM_MASK;
        USBUART_Central_DP_INP_DIS_REG &= ~USBUART_Central_DP_MASK;

        /* Enable USBIO */
        USBUART_Central_PM_USB_CR0_REG |= USBUART_Central_PM_USB_CR0_PD_N;
        CyDelayUs(2);
        /* Set the USBIO pull-up enable */
        USBUART_Central_PM_USB_CR0_REG |= USBUART_Central_PM_USB_CR0_PD_PULLUP_N;

    #endif /* End !CY_PSOC5A */

    /* Write WAx */
    CY_SET_REG8(&USBUART_Central_ARB_RW1_WA_PTR[0u],     0u);
    CY_SET_REG8(&USBUART_Central_ARB_RW1_WA_MSB_PTR[0u], 0u);

    #if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)
        /* Init transfer descriptor. This will be used to detect the DMA state - initialized or not. */
        for (i = 0u; i < USBUART_Central_MAX_EP; i++)
        {
            USBUART_Central_DmaTd[i] = DMA_INVALID_TD;
        }
    #endif   /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL */

    CyExitCriticalSection(enableInterrupts);


    /* Set the bus reset Interrupt. */
    CyIntSetVector(USBUART_Central_BUS_RESET_VECT_NUM,   USBUART_Central_BUS_RESET_ISR);
    CyIntSetPriority(USBUART_Central_BUS_RESET_VECT_NUM, USBUART_Central_BUS_RESET_PRIOR);

    /* Set the SOF Interrupt. */
    #if(USBUART_Central_SOF_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Central_SOF_VECT_NUM,   USBUART_Central_SOF_ISR);
        CyIntSetPriority(USBUART_Central_SOF_VECT_NUM, USBUART_Central_SOF_PRIOR);
    #endif   /* End USBUART_Central_SOF_ISR_REMOVE */

    /* Set the Control Endpoint Interrupt. */
    CyIntSetVector(USBUART_Central_EP_0_VECT_NUM,   USBUART_Central_EP_0_ISR);
    CyIntSetPriority(USBUART_Central_EP_0_VECT_NUM, USBUART_Central_EP_0_PRIOR);

    /* Set the Data Endpoint 1 Interrupt. */
    #if(USBUART_Central_EP1_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Central_EP_1_VECT_NUM,   USBUART_Central_EP_1_ISR);
        CyIntSetPriority(USBUART_Central_EP_1_VECT_NUM, USBUART_Central_EP_1_PRIOR);
    #endif   /* End USBUART_Central_EP1_ISR_REMOVE */

    /* Set the Data Endpoint 2 Interrupt. */
    #if(USBUART_Central_EP2_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Central_EP_2_VECT_NUM,   USBUART_Central_EP_2_ISR);
        CyIntSetPriority(USBUART_Central_EP_2_VECT_NUM, USBUART_Central_EP_2_PRIOR);
    #endif   /* End USBUART_Central_EP2_ISR_REMOVE */

    /* Set the Data Endpoint 3 Interrupt. */
    #if(USBUART_Central_EP3_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Central_EP_3_VECT_NUM,   USBUART_Central_EP_3_ISR);
        CyIntSetPriority(USBUART_Central_EP_3_VECT_NUM, USBUART_Central_EP_3_PRIOR);
    #endif   /* End USBUART_Central_EP3_ISR_REMOVE */

    /* Set the Data Endpoint 4 Interrupt. */
    #if(USBUART_Central_EP4_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Central_EP_4_VECT_NUM,   USBUART_Central_EP_4_ISR);
        CyIntSetPriority(USBUART_Central_EP_4_VECT_NUM, USBUART_Central_EP_4_PRIOR);
    #endif   /* End USBUART_Central_EP4_ISR_REMOVE */

    /* Set the Data Endpoint 5 Interrupt. */
    #if(USBUART_Central_EP5_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Central_EP_5_VECT_NUM,   USBUART_Central_EP_5_ISR);
        CyIntSetPriority(USBUART_Central_EP_5_VECT_NUM, USBUART_Central_EP_5_PRIOR);
    #endif   /* End USBUART_Central_EP5_ISR_REMOVE */

    /* Set the Data Endpoint 6 Interrupt. */
    #if(USBUART_Central_EP6_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Central_EP_6_VECT_NUM,   USBUART_Central_EP_6_ISR);
        CyIntSetPriority(USBUART_Central_EP_6_VECT_NUM, USBUART_Central_EP_6_PRIOR);
    #endif   /* End USBUART_Central_EP6_ISR_REMOVE */

     /* Set the Data Endpoint 7 Interrupt. */
    #if(USBUART_Central_EP7_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Central_EP_7_VECT_NUM,   USBUART_Central_EP_7_ISR);
        CyIntSetPriority(USBUART_Central_EP_7_VECT_NUM, USBUART_Central_EP_7_PRIOR);
    #endif   /* End USBUART_Central_EP7_ISR_REMOVE */

    /* Set the Data Endpoint 8 Interrupt. */
    #if(USBUART_Central_EP8_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Central_EP_8_VECT_NUM,   USBUART_Central_EP_8_ISR);
        CyIntSetPriority(USBUART_Central_EP_8_VECT_NUM, USBUART_Central_EP_8_PRIOR);
    #endif   /* End USBUART_Central_EP8_ISR_REMOVE */

    #if((USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL) && (USBUART_Central_ARB_ISR_REMOVE == 0u))
        /* Set the ARB Interrupt. */
        CyIntSetVector(USBUART_Central_ARB_VECT_NUM,   USBUART_Central_ARB_ISR);
        CyIntSetPriority(USBUART_Central_ARB_VECT_NUM, USBUART_Central_ARB_PRIOR);
    #endif   /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL */

}


/*******************************************************************************
* Function Name: USBUART_Central_InitComponent
********************************************************************************
*
* Summary:
*  Initialize the component, except for the HW which is done one time in
*  the Start function.  This function pulls up D+.
*
* Parameters:
*  device: Contains the device number of the desired device descriptor.
*          The device number can be found in the Device Descriptor Tab of 
*          "Configure" dialog, under the settings of desired Device Descriptor,
*          in the "Device Number" field.
*  mode: The operating voltage. This determines whether the voltage regulator
*        is enabled for 5V operation or if pass through mode is used for 3.3V
*        operation. Symbolic names and their associated values are given in the
*        following table.
*       USBUART_Central_3V_OPERATION - Disable voltage regulator and pass-thru
*                                       Vcc for pull-up
*       USBUART_Central_5V_OPERATION - Enable voltage regulator and use
*                                       regulator for pull-up
*       USBUART_Central_DWR_VDDD_OPERATION - Enable or Disable voltage
*                         regulator depend on Vddd Voltage configuration in DWR.
*
* Return:
*   None.
*
* Global variables:
*   USBUART_Central_device: Contains the device number of the desired device
*       descriptor. The device number can be found in the Device Descriptor Tab 
*       of "Configure" dialog, under the settings of desired Device Descriptor,
*       in the "Device Number" field.
*   USBUART_Central_transferState: This variable used by the communication
*       functions to handle current transfer state. Initialized to
*       TRANS_STATE_IDLE in this API.
*   USBUART_Central_configuration: Contains current configuration number
*       which is set by the Host using SET_CONFIGURATION request.
*       Initialized to zero in this API.
*   USBUART_Central_deviceAddress: Contains current device address. This
*       variable is initialized to zero in this API. Host starts to communicate
*      to device with address 0 and then set it to whatever value using
*      SET_ADDRESS request.
*   USBUART_Central_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit.
*   USBUART_Central_lastPacketSize initialized to 0;
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Central_InitComponent(uint8 device, uint8 mode) 
{
    /* Initialize _hidProtocol variable to comply with
    *  HID 7.2.6 Set_Protocol Request:
    *  "When initialized, all devices default to report protocol."
    */
    #if defined(USBUART_Central_ENABLE_HID_CLASS)
        uint8 interface;

        for (interface = 0u; interface < USBUART_Central_MAX_INTERFACES_NUMBER; interface++)
        {
            USBUART_Central_hidProtocol[interface] = USBUART_Central_PROTOCOL_REPORT;
        }
    #endif /* USBUART_Central_ENABLE_HID_CLASS */

    /* Enable Interrupts. */
    CyIntEnable(USBUART_Central_BUS_RESET_VECT_NUM);
    CyIntEnable(USBUART_Central_EP_0_VECT_NUM);
    #if(USBUART_Central_EP1_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Central_EP_1_VECT_NUM);
    #endif   /* End USBUART_Central_EP1_ISR_REMOVE */
    #if(USBUART_Central_EP2_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Central_EP_2_VECT_NUM);
    #endif   /* End USBUART_Central_EP2_ISR_REMOVE */
    #if(USBUART_Central_EP3_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Central_EP_3_VECT_NUM);
    #endif   /* End USBUART_Central_EP3_ISR_REMOVE */
    #if(USBUART_Central_EP4_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Central_EP_4_VECT_NUM);
    #endif   /* End USBUART_Central_EP4_ISR_REMOVE */
    #if(USBUART_Central_EP5_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Central_EP_5_VECT_NUM);
    #endif   /* End USBUART_Central_EP5_ISR_REMOVE */
    #if(USBUART_Central_EP6_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Central_EP_6_VECT_NUM);
    #endif   /* End USBUART_Central_EP6_ISR_REMOVE */
    #if(USBUART_Central_EP7_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Central_EP_7_VECT_NUM);
    #endif   /* End USBUART_Central_EP7_ISR_REMOVE */
    #if(USBUART_Central_EP8_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Central_EP_8_VECT_NUM);
    #endif   /* End USBUART_Central_EP8_ISR_REMOVE */
    #if((USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL) && (USBUART_Central_ARB_ISR_REMOVE == 0u))
        /* usb arb interrupt enable */
        USBUART_Central_ARB_INT_EN_REG = USBUART_Central_ARB_INT_MASK;
        CyIntEnable(USBUART_Central_ARB_VECT_NUM);
    #endif   /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL */

    /* Arbiter congiguration for DMA transfers */
    #if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)

        #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAMANUAL)
            USBUART_Central_ARB_CFG_REG = USBUART_Central_ARB_CFG_MANUAL_DMA;
        #endif   /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAMANUAL */
        #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO)
            /*Set cfg cmplt this rises DMA request when the full configuration is done */
            USBUART_Central_ARB_CFG_REG = USBUART_Central_ARB_CFG_AUTO_DMA | USBUART_Central_ARB_CFG_AUTO_MEM;
        #endif   /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */
    #endif   /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL */

    USBUART_Central_transferState = USBUART_Central_TRANS_STATE_IDLE;

    /* USB Locking: Enabled, VRegulator: depend on mode or DWR Voltage configuration*/
    switch(mode)
    {
        case USBUART_Central_3V_OPERATION:
            USBUART_Central_CR1_REG = USBUART_Central_CR1_ENABLE_LOCK;
            break;
        case USBUART_Central_5V_OPERATION:
            USBUART_Central_CR1_REG = USBUART_Central_CR1_ENABLE_LOCK | USBUART_Central_CR1_REG_ENABLE;
            break;
        default:   /*USBUART_Central_DWR_VDDD_OPERATION */
            #if(USBUART_Central_VDDD_MV < USBUART_Central_3500MV)
                USBUART_Central_CR1_REG = USBUART_Central_CR1_ENABLE_LOCK;
            #else
                USBUART_Central_CR1_REG = USBUART_Central_CR1_ENABLE_LOCK | USBUART_Central_CR1_REG_ENABLE;
            #endif /* End USBUART_Central_VDDD_MV < USBUART_Central_3500MV */
            break;
    }

    /* Record the descriptor selection */
    USBUART_Central_device = device;

    /* Clear all of the component data */
    USBUART_Central_configuration = 0u;
    USBUART_Central_interfaceNumber = 0u;
    USBUART_Central_configurationChanged = 0u;
    USBUART_Central_deviceAddress  = 0u;
    USBUART_Central_deviceStatus = 0u;

    USBUART_Central_lastPacketSize = 0u;

    /*  ACK Setup, Stall IN/OUT */
    CY_SET_REG8(USBUART_Central_EP0_CR_PTR, USBUART_Central_MODE_STALL_IN_OUT);

    /* Enable the SIE with an address 0 */
    CY_SET_REG8(USBUART_Central_CR0_PTR, USBUART_Central_CR0_ENABLE);

    /* Workarond for PSOC5LP */
    CyDelayCycles(1);
    
    /* Finally, Enable d+ pullup and select iomode to USB mode*/
    CY_SET_REG8(USBUART_Central_USBIO_CR1_PTR, USBUART_Central_USBIO_CR1_USBPUEN);
}


/*******************************************************************************
* Function Name: USBUART_Central_ReInitComponent
********************************************************************************
*
* Summary:
*  This function reinitialize the component configuration and is
*  intend to be called from the Reset interrupt.
*
* Parameters:
*  None.
*
* Return:
*   None.
*
* Global variables:
*   USBUART_Central_device: Contains the device number of the desired device 
*		descriptor. The device number can be found in the Device Descriptor Tab 
*       of "Configure" dialog, under the settings of desired Device Descriptor,
*       in the "Device Number" field.
*   USBUART_Central_transferState: This variable used by the communication
*       functions to handle current transfer state. Initialized to
*       TRANS_STATE_IDLE in this API.
*   USBUART_Central_configuration: Contains current configuration number
*       which is set by the Host using SET_CONFIGURATION request.
*       Initialized to zero in this API.
*   USBUART_Central_deviceAddress: Contains current device address. This
*       variable is initialized to zero in this API. Host starts to communicate
*      to device with address 0 and then set it to whatever value using
*      SET_ADDRESS request.
*   USBUART_Central_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit.
*   USBUART_Central_lastPacketSize initialized to 0;
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Central_ReInitComponent() 
{
    /* Initialize _hidProtocol variable to comply with HID 7.2.6 Set_Protocol
    *  Request: "When initialized, all devices default to report protocol."
    */
    #if defined(USBUART_Central_ENABLE_HID_CLASS)
        uint8 interface;

        for (interface = 0u; interface < USBUART_Central_MAX_INTERFACES_NUMBER; interface++)
        {
            USBUART_Central_hidProtocol[interface] = USBUART_Central_PROTOCOL_REPORT;
        }
    #endif /* USBUART_Central_ENABLE_HID_CLASS */

    USBUART_Central_transferState = USBUART_Central_TRANS_STATE_IDLE;

    /* Clear all of the component data */
    USBUART_Central_configuration = 0u;
    USBUART_Central_interfaceNumber = 0u;
    USBUART_Central_configurationChanged = 0u;
    USBUART_Central_deviceAddress  = 0u;
    USBUART_Central_deviceStatus = 0u;

    USBUART_Central_lastPacketSize = 0u;


    /*  ACK Setup, Stall IN/OUT */
    CY_SET_REG8(USBUART_Central_EP0_CR_PTR, USBUART_Central_MODE_STALL_IN_OUT);

    /* Enable the SIE with an address 0 */
    CY_SET_REG8(USBUART_Central_CR0_PTR, USBUART_Central_CR0_ENABLE);

}


/*******************************************************************************
* Function Name: USBUART_Central_Stop
********************************************************************************
*
* Summary:
*  This function shuts down the USB function including to release
*  the D+ Pullup and disabling the SIE.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*   USBUART_Central_configuration: Contains current configuration number
*       which is set by the Host using SET_CONFIGURATION request.
*       Initialized to zero in this API.
*   USBUART_Central_deviceAddress: Contains current device address. This
*       variable is initialized to zero in this API. Host starts to communicate
*      to device with address 0 and then set it to whatever value using
*      SET_ADDRESS request.
*   USBUART_Central_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit.
*   USBUART_Central_configurationChanged: This variable is set to one after
*       SET_CONFIGURATION request and cleared in this function.
*   USBUART_Central_intiVar variable is set to zero
*
*******************************************************************************/
void USBUART_Central_Stop(void) 
{

    #if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)
        USBUART_Central_Stop_DMA(USBUART_Central_MAX_EP);     /* Stop all DMAs */
    #endif   /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL */

    /* Disable the SIE */
    USBUART_Central_CR0_REG &= ~USBUART_Central_CR0_ENABLE;
    /* Disable the d+ pullup */
    USBUART_Central_USBIO_CR1_REG &= ~USBUART_Central_USBIO_CR1_USBPUEN;
    #if(CY_PSOC5A)
        /* Disable USBIO block*/
        USBUART_Central_PM_AVAIL_CR_REG &= ~USBUART_Central_PM_AVAIL_EN_FSUSBIO;
    #endif /* End CY_PSOC5A */    
    /* Disable USB in ACT PM */
    USBUART_Central_PM_ACT_CFG_REG &= ~USBUART_Central_PM_ACT_EN_FSUSB;
    /* Disable USB block for Standby Power Mode */
    USBUART_Central_PM_STBY_CFG_REG &= ~USBUART_Central_PM_STBY_EN_FSUSB;

    /* Disable the reset and EP interrupts */
    CyIntDisable(USBUART_Central_BUS_RESET_VECT_NUM);
    CyIntDisable(USBUART_Central_EP_0_VECT_NUM);
    #if(USBUART_Central_EP1_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Central_EP_1_VECT_NUM);
    #endif   /* End USBUART_Central_EP1_ISR_REMOVE */
    #if(USBUART_Central_EP2_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Central_EP_2_VECT_NUM);
    #endif   /* End USBUART_Central_EP2_ISR_REMOVE */
    #if(USBUART_Central_EP3_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Central_EP_3_VECT_NUM);
    #endif   /* End USBUART_Central_EP3_ISR_REMOVE */
    #if(USBUART_Central_EP4_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Central_EP_4_VECT_NUM);
    #endif   /* End USBUART_Central_EP4_ISR_REMOVE */
    #if(USBUART_Central_EP5_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Central_EP_5_VECT_NUM);
    #endif   /* End USBUART_Central_EP5_ISR_REMOVE */
    #if(USBUART_Central_EP6_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Central_EP_6_VECT_NUM);
    #endif   /* End USBUART_Central_EP6_ISR_REMOVE */
    #if(USBUART_Central_EP7_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Central_EP_7_VECT_NUM);
    #endif   /* End USBUART_Central_EP7_ISR_REMOVE */
    #if(USBUART_Central_EP8_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Central_EP_8_VECT_NUM);
    #endif   /* End USBUART_Central_EP8_ISR_REMOVE */

    /* Clear all of the component data */
    USBUART_Central_configuration = 0u;
    USBUART_Central_interfaceNumber = 0u;
    USBUART_Central_configurationChanged = 0u;
    USBUART_Central_deviceAddress  = 0u;
    USBUART_Central_deviceStatus = 0u;
    USBUART_Central_initVar = 0u;

}


/*******************************************************************************
* Function Name: USBUART_Central_CheckActivity
********************************************************************************
*
* Summary:
*  Returns the activity status of the bus.  Clears the status hardware to
*  provide fresh activity status on the next call of this routine.
*
* Parameters:
*  None.
*
* Return:
*  1 - If bus activity was detected since the last call to this function
*  0 - If bus activity not was detected since the last call to this function
*
*******************************************************************************/
uint8 USBUART_Central_CheckActivity(void) 
{
    uint8 r;

    r = CY_GET_REG8(USBUART_Central_CR1_PTR);
    CY_SET_REG8(USBUART_Central_CR1_PTR, (r & ~USBUART_Central_CR1_BUS_ACTIVITY));

    return((r & USBUART_Central_CR1_BUS_ACTIVITY) >> USBUART_Central_CR1_BUS_ACTIVITY_SHIFT);
}


/*******************************************************************************
* Function Name: USBUART_Central_GetConfiguration
********************************************************************************
*
* Summary:
*  Returns the current configuration setting
*
* Parameters:
*  None.
*
* Return:
*  configuration.
*
*******************************************************************************/
uint8 USBUART_Central_GetConfiguration(void) 
{
    return(USBUART_Central_configuration);
}


/*******************************************************************************
* Function Name: USBUART_Central_IsConfigurationChanged
********************************************************************************
*
* Summary:
*  Returns the clear on read configuration state. It is usefull when PC send
*  double SET_CONFIGURATION request with same configuration number.
*
* Parameters:
*  None.
*
* Return:
*  Not zero value when new configuration has been changed, otherwise zero is
*  returned.
*
* Global variables:
*   USBUART_Central_configurationChanged: This variable is set to one after
*       SET_CONFIGURATION request and cleared in this function.
*
*******************************************************************************/
uint8 USBUART_Central_IsConfigurationChanged(void) 
{
    uint8 res = 0u;

    if(USBUART_Central_configurationChanged != 0u)
    {
        res = USBUART_Central_configurationChanged;
        USBUART_Central_configurationChanged = 0u;
    }

    return(res);
}


/*******************************************************************************
* Function Name: USBUART_Central_GetInterfaceSetting
********************************************************************************
*
* Summary:
*  Returns the alternate setting from current interface
*
* Parameters:
*  uint8 interfaceNumber, interface number
*
* Return:
*  Alternate setting.
*
*******************************************************************************/
uint8  USBUART_Central_GetInterfaceSetting(uint8 interfaceNumber)
                                                    
{
    return(USBUART_Central_interfaceSetting[interfaceNumber]);
}


/*******************************************************************************
* Function Name: USBUART_Central_GetEPState
********************************************************************************
*
* Summary:
*  Returned the state of the requested endpoint.
*
* Parameters:
*  epNumber: Endpoint Number
*
* Return:
*  State of the requested endpoint.
*
*******************************************************************************/
uint8 USBUART_Central_GetEPState(uint8 epNumber) 
{
    return(USBUART_Central_EP[epNumber].apiEpState);
}


/*******************************************************************************
* Function Name: USBUART_Central_GetEPCount
********************************************************************************
*
* Summary:
*  This function supports Data Endpoints only(EP1-EP8).
*  Returns the transfer count for the requested endpoint.  The value from
*  the count registers includes 2 counts for the two byte checksum of the
*  packet.  This function subtracts the two counts.
*
* Parameters:
*  epNumber: Data Endpoint Number.
*            Valid values are between 1 and 8.
*
* Return:
*  Returns the current byte count from the specified endpoin or 0 for an
*  invalid endpoint.
*
*******************************************************************************/
uint16 USBUART_Central_GetEPCount(uint8 epNumber) 
{
    uint8 ri;
    uint16 count = 0u;

    if((epNumber > USBUART_Central_EP0) && (epNumber < USBUART_Central_MAX_EP))
    {
        ri = ((epNumber - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);

        count = ((uint16)((CY_GET_REG8(&USBUART_Central_SIE_EP1_CNT0_PTR[ri]) & USBUART_Central_EPX_CNT0_MASK) << 8u)
                         | CY_GET_REG8(&USBUART_Central_SIE_EP1_CNT1_PTR[ri]))
                         - USBUART_Central_EPX_CNTX_CRC_COUNT;
    }
    return(count);
}


#if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)


    /*******************************************************************************
    * Function Name: USBUART_Central_InitEP_DMA
    ********************************************************************************
    *
    * Summary: This function allocate and initializes a EP DMA chanel to be used
    *     by the USBUART_Central_LoadInEP() or USBUART_Central_ReadOutEP() APIs.
    *
    * Parameters:
    *  epNumber: Contains the data endpoint number.
    *            Valid values are between 1 and 8.
    *  *pData: A pointer to a data array which will be related to the EP transfers
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_Central_InitEP_DMA(uint8 epNumber, uint8 *pData) 
    {
        uint16 src;
        uint16 dst;
        #if (defined(__C51__))          /* PSoC 3 - Source is Flash */
            src = HI16(CYDEV_SRAM_BASE);
            dst = HI16(CYDEV_PERIPH_BASE);
            pData = pData;
        #else                           /* PSoC 5 */
            if((USBUART_Central_EP[epNumber].addr & USBUART_Central_DIR_IN) != 0u )
            {   /* for the IN EP source is the flash memory buffer */
                src = HI16(pData);
                dst = HI16(CYDEV_PERIPH_BASE);
            }
            else
            {   /* for the OUT EP source is the SIE register */
                src = HI16(CYDEV_PERIPH_BASE);
                dst = HI16(pData);
            }
        #endif  /* End C51 */
        switch(epNumber)
        {
            case USBUART_Central_EP1:
                #if(USBUART_Central_DMA1_REMOVE == 0u)
                    USBUART_Central_DmaChan[epNumber] = USBUART_Central_ep1_DmaInitialize(
                        USBUART_Central_DMA_BYTES_PER_BURST, USBUART_Central_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Central_DMA1_REMOVE */
                break;
            case USBUART_Central_EP2:
                #if(USBUART_Central_DMA2_REMOVE == 0u)
                    USBUART_Central_DmaChan[epNumber] = USBUART_Central_ep2_DmaInitialize(
                        USBUART_Central_DMA_BYTES_PER_BURST, USBUART_Central_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Central_DMA2_REMOVE */
                break;
            case USBUART_Central_EP3:
                #if(USBUART_Central_DMA3_REMOVE == 0u)
                    USBUART_Central_DmaChan[epNumber] = USBUART_Central_ep3_DmaInitialize(
                        USBUART_Central_DMA_BYTES_PER_BURST, USBUART_Central_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Central_DMA3_REMOVE */
                break;
            case USBUART_Central_EP4:
                #if(USBUART_Central_DMA4_REMOVE == 0u)
                    USBUART_Central_DmaChan[epNumber] = USBUART_Central_ep4_DmaInitialize(
                        USBUART_Central_DMA_BYTES_PER_BURST, USBUART_Central_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Central_DMA4_REMOVE */
                break;
            case USBUART_Central_EP5:
                #if(USBUART_Central_DMA5_REMOVE == 0u)
                    USBUART_Central_DmaChan[epNumber] = USBUART_Central_ep5_DmaInitialize(
                        USBUART_Central_DMA_BYTES_PER_BURST, USBUART_Central_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Central_DMA5_REMOVE */
                break;
            case USBUART_Central_EP6:
                #if(USBUART_Central_DMA6_REMOVE == 0u)
                    USBUART_Central_DmaChan[epNumber] = USBUART_Central_ep6_DmaInitialize(
                        USBUART_Central_DMA_BYTES_PER_BURST, USBUART_Central_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Central_DMA6_REMOVE */
                break;
            case USBUART_Central_EP7:
                #if(USBUART_Central_DMA7_REMOVE == 0u)
                    USBUART_Central_DmaChan[epNumber] = USBUART_Central_ep7_DmaInitialize(
                        USBUART_Central_DMA_BYTES_PER_BURST, USBUART_Central_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Central_DMA7_REMOVE */
                break;
            case USBUART_Central_EP8:
                #if(USBUART_Central_DMA8_REMOVE == 0u)
                    USBUART_Central_DmaChan[epNumber] = USBUART_Central_ep8_DmaInitialize(
                        USBUART_Central_DMA_BYTES_PER_BURST, USBUART_Central_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Central_DMA8_REMOVE */
                break;
            default:
                /* Do not support EP0 DMA transfers */
                break;
        }
        if((epNumber > USBUART_Central_EP0) && (epNumber < USBUART_Central_MAX_EP))
        {
            USBUART_Central_DmaTd[epNumber] = CyDmaTdAllocate();
        }
    }


    /*******************************************************************************
    * Function Name: USBUART_Central_Stop_DMA
    ********************************************************************************
    *
    * Summary: Stops and free DMA
    *
    * Parameters:
    *  epNumber: Contains the data endpoint number or
    *           USBUART_Central_MAX_EP to stop all DMAs
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_Central_Stop_DMA(uint8 epNumber) 
    {
        uint8 i;
        i = (epNumber < USBUART_Central_MAX_EP) ? epNumber : USBUART_Central_EP1;
        do
        {
            if(USBUART_Central_DmaTd[i] != DMA_INVALID_TD)
            {
                CyDmaChDisable(USBUART_Central_DmaChan[i]);
                CyDmaTdFree(USBUART_Central_DmaTd[i]);
                USBUART_Central_DmaTd[i] = DMA_INVALID_TD;
            }
            i++;
        }while((i < USBUART_Central_MAX_EP) && (epNumber == USBUART_Central_MAX_EP));
    }

#endif /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL */


/*******************************************************************************
* Function Name: USBUART_Central_LoadInEP
********************************************************************************
*
* Summary:
*  Loads and enables the specified USB data endpoint for an IN interrupt or bulk
*  transfer.
*
* Parameters:
*  epNumber: Contains the data endpoint number.
*            Valid values are between 1 and 8.
*  *pData: A pointer to a data array from which the data for the endpoint space
*          is loaded.
*  length: The number of bytes to transfer from the array and then send as a
*          result of an IN request. Valid values are between 0 and 512.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Central_LoadInEP(uint8 epNumber, uint8 *pData, uint16 length)
                                                                        
{
    uint8 ri;
    uint8 *p;
    #if(USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL)
        uint16 i;
    #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL */

    if((epNumber > USBUART_Central_EP0) && (epNumber < USBUART_Central_MAX_EP))
    {
        ri = ((epNumber - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);
        p = (uint8 *)&USBUART_Central_ARB_RW1_DR_PTR[ri];

        #if(USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO)
            /* Limits length to available buffer space, auto MM could send packets up to 1024 bytes */
            if(length > USBUART_Central_EPX_DATA_BUF_MAX - USBUART_Central_EP[epNumber].buffOffset)
            {
                length = USBUART_Central_EPX_DATA_BUF_MAX - USBUART_Central_EP[epNumber].buffOffset;
            }
        #endif /* End USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO */

        /* Set the count and data toggle */
        CY_SET_REG8(&USBUART_Central_SIE_EP1_CNT0_PTR[ri], (length >> 8u) | (USBUART_Central_EP[epNumber].epToggle));
        CY_SET_REG8(&USBUART_Central_SIE_EP1_CNT1_PTR[ri],  length & 0xFFu);

        #if(USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL)
            if(pData != NULL)
            {
                /* Copy the data using the arbiter data register */
                for (i = 0u; i < length; i++)
                {
                    CY_SET_REG8(p, pData[i]);
                }
            }
            USBUART_Central_EP[epNumber].apiEpState = USBUART_Central_NO_EVENT_PENDING;
            /* Write the Mode register */
            USBUART_Central_SIE_EP1_CR0_PTR[ri] = USBUART_Central_EP[epNumber].epMode;
        #else
            /*Init DMA if it was not initialized */
            if(USBUART_Central_DmaTd[epNumber] == DMA_INVALID_TD)
            {
                USBUART_Central_InitEP_DMA(epNumber, pData);
            }
        #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL */

        #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAMANUAL)
            if((pData != NULL) && (length > 0))
            {

                /* Enable DMA in mode2 for transfering data */
                CyDmaChDisable(USBUART_Central_DmaChan[epNumber]);
                CyDmaTdSetConfiguration(USBUART_Central_DmaTd[epNumber], length, DMA_INVALID_TD,\
                                                                                    TD_TERMIN_EN | TD_INC_SRC_ADR);
                CyDmaTdSetAddress(USBUART_Central_DmaTd[epNumber],  LO16((uint32)pData), LO16((uint32)p));
                /* Enable the DMA */
                CyDmaChSetInitialTd(USBUART_Central_DmaChan[epNumber], USBUART_Central_DmaTd[epNumber]);
                CyDmaChEnable(USBUART_Central_DmaChan[epNumber], 1);
                /* Generate DMA request */
                USBUART_Central_ARB_EP1_CFG_PTR[ri] |= USBUART_Central_ARB_EPX_CFG_DMA_REQ;
                USBUART_Central_ARB_EP1_CFG_PTR[ri] &= ~USBUART_Central_ARB_EPX_CFG_DMA_REQ;
                /* Mode register will be writen in arb ISR after DMA transfer copmlete */
            }
            else
            {
                /* When zero-length packet - write the Mode register directly */
                USBUART_Central_SIE_EP1_CR0_PTR[ri] = USBUART_Central_EP[epNumber].epMode;
            }
        #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAMANUAL */

        #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO)
            if(pData != NULL)
            {
                /* Enable DMA in mode3 for transfering data */
                CyDmaChDisable(USBUART_Central_DmaChan[epNumber]);
                CyDmaTdSetConfiguration(USBUART_Central_DmaTd[epNumber], length, USBUART_Central_DmaTd[epNumber],\
                                                                                    TD_TERMIN_EN | TD_INC_SRC_ADR);
                CyDmaTdSetAddress(USBUART_Central_DmaTd[epNumber],  LO16((uint32)pData), LO16((uint32)p));
                /* Clear Any potential pending DMA requests before starting the DMA channel to transfer data */
                CyDmaClearPendingDrq(USBUART_Central_DmaChan[epNumber]);
                /* Enable the DMA */
                CyDmaChSetInitialTd(USBUART_Central_DmaChan[epNumber], USBUART_Central_DmaTd[epNumber]);
                CyDmaChEnable(USBUART_Central_DmaChan[epNumber], 1);
            }
            else
            {
                USBUART_Central_EP[epNumber].apiEpState = USBUART_Central_NO_EVENT_PENDING;
                if(length > 0)
                {
                    /* Set Data ready status, This will generate DMA request */
                    USBUART_Central_ARB_EP1_CFG_PTR[ri] |= USBUART_Central_ARB_EPX_CFG_IN_DATA_RDY;
                    /* Mode register will be writen in arb ISR(In Buffer Full) after first DMA transfer copmlete */
                }
                else
                {
                    /* When zero-length packet - write the Mode register directly */
                    USBUART_Central_SIE_EP1_CR0_PTR[ri] = USBUART_Central_EP[epNumber].epMode;
                }
            }
        #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */

    }
}


/*******************************************************************************
* Function Name: USBUART_Central_ReadOutEP
********************************************************************************
*
* Summary:
*  Read data from an endpoint.  The application must call
*  USBUART_Central_GetEPState to see if an event is pending.
*
* Parameters:
*  epNumber: Contains the data endpoint number.
*            Valid values are between 1 and 8.
*  pData: A pointer to a data array from which the data for the endpoint space
*         is loaded.
*  length: The number of bytes to transfer from the USB Out enpoint and loads
*          it into data array. Valid values are between 0 and 1023. The function
*          moves fewer than the requested number of bytes if the host sends
*          fewer bytes than requested.
*
* Returns:
*  Number of bytes received, 0 for an invalid endpoint.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint16 USBUART_Central_ReadOutEP(uint8 epNumber, uint8 *pData, uint16 length)
                                                                        
{
    uint8 ri;
    uint8 *p;
    #if(USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL)
        uint16 i;
    #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL */
    #if(USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO)
        uint16 xferCount;
    #endif /* End USBUART_Central_EP_MM != $INSTANCE_NAME`__EP_DMAAUTO */

    if((epNumber > USBUART_Central_EP0) && (epNumber < USBUART_Central_MAX_EP) && (pData != NULL))
    {
        ri = ((epNumber - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);
        p = (uint8 *)&USBUART_Central_ARB_RW1_DR_PTR[ri];

        #if(USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO)
            /* Determine which is smaller the requested data or the available data */
            xferCount = USBUART_Central_GetEPCount(epNumber);
            if (length > xferCount)
            {
                length = xferCount;
            }
        #endif /* End USBUART_Central_EP_MM != USBUART_Central__EP_DMAAUTO */

        #if(USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL)
            /* Copy the data using the arbiter data register */
            for (i = 0u; i < length; i++)
            {
                pData[i] = CY_GET_REG8(p);
            }

            /* (re)arming of OUT endpoint */
            USBUART_Central_EnableOutEP(epNumber);
        #else
            /*Init DMA if it was not initialized */
            if(USBUART_Central_DmaTd[epNumber] == DMA_INVALID_TD)
            {
                USBUART_Central_InitEP_DMA(epNumber, pData);
            }
        #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_MANUAL */

        #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAMANUAL)
            /* Enable DMA in mode2 for transfering data */
            CyDmaChDisable(USBUART_Central_DmaChan[epNumber]);
            CyDmaTdSetConfiguration(USBUART_Central_DmaTd[epNumber], length, DMA_INVALID_TD,
                                                                                TD_TERMIN_EN | TD_INC_DST_ADR);
            CyDmaTdSetAddress(USBUART_Central_DmaTd[epNumber],  LO16((uint32)p), LO16((uint32)pData));
            /* Enable the DMA */
            CyDmaChSetInitialTd(USBUART_Central_DmaChan[epNumber], USBUART_Central_DmaTd[epNumber]);
            CyDmaChEnable(USBUART_Central_DmaChan[epNumber], 1);

            /* Generate DMA request */
            USBUART_Central_ARB_EP1_CFG_PTR[ri] |= USBUART_Central_ARB_EPX_CFG_DMA_REQ;
            USBUART_Central_ARB_EP1_CFG_PTR[ri] &= ~USBUART_Central_ARB_EPX_CFG_DMA_REQ;
            /* Out EP will be (re)armed in arb ISR after transfer complete */
        #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAMANUAL */

        #if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO)
            /* Enable DMA in mode3 for transfering data */
            CyDmaChDisable(USBUART_Central_DmaChan[epNumber]);
            CyDmaTdSetConfiguration(USBUART_Central_DmaTd[epNumber], length, USBUART_Central_DmaTd[epNumber],
                                                                                TD_TERMIN_EN | TD_INC_DST_ADR);
            CyDmaTdSetAddress(USBUART_Central_DmaTd[epNumber],  LO16((uint32)p), LO16((uint32)pData));

            /* Clear Any potential pending DMA requests before starting the DMA channel to transfer data */
            CyDmaClearPendingDrq(USBUART_Central_DmaChan[epNumber]);
            /* Enable the DMA */
            CyDmaChSetInitialTd(USBUART_Central_DmaChan[epNumber], USBUART_Central_DmaTd[epNumber]);
            CyDmaChEnable(USBUART_Central_DmaChan[epNumber], 1);
            /* Out EP will be (re)armed in arb ISR after transfer complete */
        #endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */

    }
    else
    {
        length = 0;
    }

    return(length);
}


/*******************************************************************************
* Function Name: USBUART_Central_EnableOutEP
********************************************************************************
*
* Summary:
*  This function enables an OUT endpoint.  It should not be
*  called for an IN endpoint.
*
* Parameters:
*  epNumber: Endpoint Number
*            Valid values are between 1 and 8.
*
* Return:
*   None.
*
* Global variables:
*  USBUART_Central_EP[epNumber].apiEpState - set to NO_EVENT_PENDING
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Central_EnableOutEP(uint8 epNumber) 
{
    uint8 ri;

    if((epNumber > USBUART_Central_EP0) && (epNumber < USBUART_Central_MAX_EP))
    {
        ri = ((epNumber - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);
        USBUART_Central_EP[epNumber].apiEpState = USBUART_Central_NO_EVENT_PENDING;
        /* Write the Mode register */
        CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_EP[epNumber].epMode);
    }
}


/*******************************************************************************
* Function Name: USBUART_Central_DisableOutEP
********************************************************************************
*
* Summary:
*  This function disables an OUT endpoint.  It should not be
*  called for an IN endpoint.
*
* Parameters:
*  epNumber: Endpoint Number
*            Valid values are between 1 and 8.
*
* Return:
*  None.
*
*******************************************************************************/
void USBUART_Central_DisableOutEP(uint8 epNumber) 
{
    uint8 ri ;

    if((epNumber > USBUART_Central_EP0) && (epNumber < USBUART_Central_MAX_EP))
    {
        ri = ((epNumber - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);
        /* Write the Mode register */
        CY_SET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri], USBUART_Central_MODE_NAK_OUT);
    }
}


/*******************************************************************************
* Function Name: USBUART_Central_Force
********************************************************************************
*
* Summary:
*  Forces the bus state
*
* Parameters:
*  bState
*    USBUART_Central_FORCE_J
*    USBUART_Central_FORCE_K
*    USBUART_Central_FORCE_SE0
*    USBUART_Central_FORCE_NONE
*
* Return:
*  None.
*
*******************************************************************************/
void USBUART_Central_Force(uint8 bState) 
{
    CY_SET_REG8(USBUART_Central_USBIO_CR0_PTR, bState);
}


/*******************************************************************************
* Function Name: USBUART_Central_GetEPAckState
********************************************************************************
*
* Summary:
*  Returns the ACK of the CR0 Register (ACKD)
*
* Parameters:
*  epNumber: Endpoint Number
*            Valid values are between 1 and 8.
*
* Returns
*  0 if nothing has been ACKD, non-=zero something has been ACKD
*
*******************************************************************************/
uint8 USBUART_Central_GetEPAckState(uint8 epNumber) 
{
    uint8 ri;
    uint8 cr = 0;

    if((epNumber > USBUART_Central_EP0) && (epNumber < USBUART_Central_MAX_EP))
    {
        ri = ((epNumber - USBUART_Central_EP1) << USBUART_Central_EPX_CNTX_ADDR_SHIFT);
        cr = CY_GET_REG8(&USBUART_Central_SIE_EP1_CR0_PTR[ri]) & USBUART_Central_MODE_ACKD;
    }

    return(cr);
}


/*******************************************************************************
* Function Name: USBUART_Central_SetPowerStatus
********************************************************************************
*
* Summary:
*  Sets the device power status for reporting in the Get Device Status
*  request
*
* Parameters:
*  powerStatus: USBUART_Central_DEVICE_STATUS_BUS_POWERED(0) - Bus Powered,
*               USBUART_Central_DEVICE_STATUS_SELF_POWERED(1) - Self Powered
*
* Return:
*   None.
*
* Global variables:
*  USBUART_Central_deviceStatus - set power status
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Central_SetPowerStatus(uint8 powerStatus) 
{
    if (powerStatus != USBUART_Central_DEVICE_STATUS_BUS_POWERED)
    {
        USBUART_Central_deviceStatus |=  USBUART_Central_DEVICE_STATUS_SELF_POWERED;
    }
    else
    {
        USBUART_Central_deviceStatus &=  ~USBUART_Central_DEVICE_STATUS_SELF_POWERED;
    }
}


#if (USBUART_Central_MON_VBUS == 1u)

    /*******************************************************************************
    * Function Name: USBUART_Central_VBusPresent
    ********************************************************************************
    *
    * Summary:
    *  Determines VBUS presence for Self Powered Devices.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  1 if VBUS is present, otherwise 0.
    *
    *******************************************************************************/
    uint8 USBUART_Central_VBusPresent(void) 
    {
        return((CY_GET_REG8(USBUART_Central_VBUS_PS_PTR) & USBUART_Central_VBUS_MASK) ? 1u : 0u);
    }

#endif /* USBUART_Central_MON_VBUS */


/*******************************************************************************
* Function Name: USBUART_Central_RWUEnabled
********************************************************************************
*
* Summary:
*  Returns TRUE if Remote Wake Up is enabled, otherwise FALSE
*
* Parameters:
*   None.
*
* Return:
*  TRUE -  Remote Wake Up Enabled
*  FALSE - Remote Wake Up Disabled
*
* Global variables:
*  USBUART_Central_deviceStatus - checked to determine remote status
*
*******************************************************************************/
uint8 USBUART_Central_RWUEnabled(void) 
{
    uint8 result = USBUART_Central_FALSE;
    if((USBUART_Central_deviceStatus & USBUART_Central_DEVICE_STATUS_REMOTE_WAKEUP) != 0u)
    {
        result = USBUART_Central_TRUE;
    }

    return(result);
}


/* [] END OF FILE */
