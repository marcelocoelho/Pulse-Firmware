/*******************************************************************************
* File Name: USBUART_Host.c
* Version 2.30
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
#include "USBUART_Host.h"
#include "USBUART_Host_hid.h"
#if(USBUART_Host_DMA1_REMOVE == 0u)
    #include "USBUART_Host_ep1_dma.h"
#endif   /* End USBUART_Host_DMA1_REMOVE */
#if(USBUART_Host_DMA2_REMOVE == 0u)
    #include "USBUART_Host_ep2_dma.h"
#endif   /* End USBUART_Host_DMA2_REMOVE */
#if(USBUART_Host_DMA3_REMOVE == 0u)
    #include "USBUART_Host_ep3_dma.h"
#endif   /* End USBUART_Host_DMA3_REMOVE */
#if(USBUART_Host_DMA4_REMOVE == 0u)
    #include "USBUART_Host_ep4_dma.h"
#endif   /* End USBUART_Host_DMA4_REMOVE */
#if(USBUART_Host_DMA5_REMOVE == 0u)
    #include "USBUART_Host_ep5_dma.h"
#endif   /* End USBUART_Host_DMA5_REMOVE */
#if(USBUART_Host_DMA6_REMOVE == 0u)
    #include "USBUART_Host_ep6_dma.h"
#endif   /* End USBUART_Host_DMA6_REMOVE */
#if(USBUART_Host_DMA7_REMOVE == 0u)
    #include "USBUART_Host_ep7_dma.h"
#endif   /* End USBUART_Host_DMA7_REMOVE */
#if(USBUART_Host_DMA8_REMOVE == 0u)
    #include "USBUART_Host_ep8_dma.h"
#endif   /* End USBUART_Host_DMA8_REMOVE */


/***************************************
* External data references
***************************************/

extern volatile uint8 USBUART_Host_configuration;
extern volatile uint8 USBUART_Host_configurationChanged;
extern volatile uint8 USBUART_Host_interfaceSetting[];
extern volatile uint8 USBUART_Host_interfaceSetting_last[];
extern volatile uint8 USBUART_Host_deviceAddress;
extern volatile uint8 USBUART_Host_deviceStatus;
extern volatile uint8 USBUART_Host_device;
extern volatile uint8 USBUART_Host_transferState;
extern volatile uint8 USBUART_Host_lastPacketSize;

extern volatile T_USBUART_Host_EP_CTL_BLOCK USBUART_Host_EP[];

#if defined(USBUART_Host_ENABLE_HID_CLASS)
    extern volatile uint8 USBUART_Host_hidProtocol[];
#endif /* USBUART_Host_ENABLE_HID_CLASS */


/***************************************
* External function references
***************************************/

CY_ISR_PROTO(USBUART_Host_EP_0_ISR);
#if(USBUART_Host_EP1_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Host_EP_1_ISR);
#endif /* End USBUART_Host_EP1_ISR_REMOVE */
#if(USBUART_Host_EP2_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Host_EP_2_ISR);
#endif /* End USBUART_Host_EP2_ISR_REMOVE */
#if(USBUART_Host_EP3_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Host_EP_3_ISR);
#endif /* End USBUART_Host_EP3_ISR_REMOVE */
#if(USBUART_Host_EP4_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Host_EP_4_ISR);
#endif /* End USBUART_Host_EP4_ISR_REMOVE */
#if(USBUART_Host_EP5_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Host_EP_5_ISR);
#endif /* End USBUART_Host_EP5_ISR_REMOVE */
#if(USBUART_Host_EP6_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Host_EP_6_ISR);
#endif /* End USBUART_Host_EP6_ISR_REMOVE */
#if(USBUART_Host_EP7_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Host_EP_7_ISR);
#endif /* End USBUART_Host_EP7_ISR_REMOVE */
#if(USBUART_Host_EP8_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Host_EP_8_ISR);
#endif /* End USBUART_Host_EP8_ISR_REMOVE */
CY_ISR_PROTO(USBUART_Host_BUS_RESET_ISR);
#if(USBUART_Host_SOF_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_Host_SOF_ISR);
#endif /* End USBUART_Host_SOF_ISR_REMOVE */
#if(USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL)
    CY_ISR_PROTO(USBUART_Host_ARB_ISR);
#endif /* End USBUART_Host_EP_MM */


/***************************************
* Global data allocation
***************************************/

uint8 USBUART_Host_initVar = 0u;
#if(USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL)
    uint8 USBUART_Host_DmaChan[USBUART_Host_MAX_EP];
    uint8 USBUART_Host_DmaTd[USBUART_Host_MAX_EP];
#endif /* End USBUART_Host_EP_MM */


/*******************************************************************************
* Function Name: USBUART_Host_Start
********************************************************************************
*
* Summary:
*  This function initialize the USB SIE, arbiter and the
*  endpoint APIs, including setting the D+ Pullup
*
* Parameters:
*  device: Contains the device number from the desired device descriptor set
*          entered with the USBFS customizer.
*  mode: The operating voltage. This determines whether the voltage regulator
*        is enabled for 5V operation or if pass through mode is used for 3.3V
*        operation. Symbolic names and their associated values are given in the
*        following table.
*       USBUART_Host_3V_OPERATION - Disable voltage regulator and pass-thru
*                                       Vcc for pull-up
*       USBUART_Host_5V_OPERATION - Enable voltage regulator and use
*                                       regulator for pull-up
*       USBUART_Host_DWR_VDDD_OPERATION - Enable or Disable voltage
*                         regulator depend on Vddd Voltage configuration in DWR.
*
* Return:
*   None.
*
* Global variables:
*  The USBUART_Host_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time USBUART_Host_Start() is called.
*  This allows for component Re-Start without unnecessary re-initialization
*  in all subsequent calls to the USBUART_Host_Start() routine.
*  If re-initialization of the component is required the variable should be set
*  to zero before call of UART_Start() routine, or the user may call
*  USBUART_Host_Init() and USBUART_Host_InitComponent() as done
*  in the USBUART_Host_Start() routine.
*
* Side Effects:
*   This function will reset all communication states to default.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_Start(uint8 device, uint8 mode) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(USBUART_Host_initVar == 0u)
    {
        USBUART_Host_Init();
        USBUART_Host_initVar = 1u;
    }
    USBUART_Host_InitComponent(device, mode);
}


/*******************************************************************************
* Function Name: USBUART_Host_Init
********************************************************************************
*
* Summary:
*  Initialize component's hardware. Usually called in USBUART_Host_Start().
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
void USBUART_Host_Init(void) 
{
    uint8 enableInterrupts;
    #if(USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL)
        uint16 i;
    #endif   /* End USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL */

    enableInterrupts = CyEnterCriticalSection();

    /* Enable USB block  */
    USBUART_Host_PM_ACT_CFG_REG |= USBUART_Host_PM_ACT_EN_FSUSB;
    /* Enable USB block for Standby Power Mode */
    USBUART_Host_PM_STBY_CFG_REG |= USBUART_Host_PM_STBY_EN_FSUSB;

    #if(CY_PSOC5A)
        /* Disable USBIO for TO3 */
        USBUART_Host_PM_AVAIL_CR_REG &= ~USBUART_Host_PM_AVAIL_EN_FSUSBIO;
    #endif /* End CY_PSOC5A */

    /* Enable core clock */
    USBUART_Host_USB_CLK_EN_REG = USBUART_Host_USB_CLK_ENABLE;

    USBUART_Host_CR1_REG = USBUART_Host_CR1_ENABLE_LOCK;

    #if(CY_PSOC5A)
        /* Enable USBIO for TO3 */
        USBUART_Host_PM_AVAIL_CR_REG |= USBUART_Host_PM_AVAIL_EN_FSUSBIO;
        /* Bus Reset Length, It has correct default value in ES3 */
        USBUART_Host_BUS_RST_CNT_REG = USBUART_Host_BUS_RST_COUNT;
    #endif /* End CY_PSOC5A */

    /* ENABLING USBIO PADS IN USB MODE FROM I/O MODE */
    #if(!CY_PSOC5A)
        /* Ensure USB transmit enable is low (USB_USBIO_CR0.ten). - Manual Transmission - Disabled */
        USBUART_Host_USBIO_CR0_REG &= ~USBUART_Host_USBIO_CR0_TEN;
        CyDelayUs(0);  /*~50ns delay */
        /* Disable the USBIO by asserting PM.USB_CR0.fsusbio_pd_n(Inverted)
        *  high. This will have been set low by the power manger out of reset.
        *  Also confirm USBIO pull-up disabled
        */
        USBUART_Host_PM_USB_CR0_REG &= ~(USBUART_Host_PM_USB_CR0_PD_N |USBUART_Host_PM_USB_CR0_PD_PULLUP_N);
        /* Enable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
        USBUART_Host_PM_USB_CR0_REG |= USBUART_Host_PM_USB_CR0_REF_EN;
        /* The reference will be available 1 us after the regulator is enabled */
        CyDelayUs(1);
        /* OR 40us after power restored */
        CyDelayUs(40);
        /* Ensure the single ended disable bits are low (PRT15.INP_DIS[7:6])(input receiver enabled). */
        USBUART_Host_DM_INP_DIS_REG &= ~USBUART_Host_DM_MASK;
        USBUART_Host_DP_INP_DIS_REG &= ~USBUART_Host_DP_MASK;

        /* Enable USBIO */
        USBUART_Host_PM_USB_CR0_REG |= USBUART_Host_PM_USB_CR0_PD_N;
        CyDelayUs(2);
        /* Set the USBIO pull-up enable */
        USBUART_Host_PM_USB_CR0_REG |= USBUART_Host_PM_USB_CR0_PD_PULLUP_N;

    #endif /* End !CY_PSOC5A */

    /* Write WAx */
    CY_SET_REG8(&USBUART_Host_ARB_RW1_WA_PTR[0u],     0u);
    CY_SET_REG8(&USBUART_Host_ARB_RW1_WA_MSB_PTR[0u], 0u);

    #if(USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL)
        /* Init transfer descriptor. This will be used to detect the DMA state - initialized or not. */
        for (i = 0u; i < USBUART_Host_MAX_EP; i++)
        {
            USBUART_Host_DmaTd[i] = DMA_INVALID_TD;
        }
    #endif   /* End USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL */

    CyExitCriticalSection(enableInterrupts);


    /* Set the bus reset Interrupt. */
    CyIntSetVector(USBUART_Host_BUS_RESET_VECT_NUM,   USBUART_Host_BUS_RESET_ISR);
    CyIntSetPriority(USBUART_Host_BUS_RESET_VECT_NUM, USBUART_Host_BUS_RESET_PRIOR);

    /* Set the SOF Interrupt. */
    #if(USBUART_Host_SOF_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Host_SOF_VECT_NUM,   USBUART_Host_SOF_ISR);
        CyIntSetPriority(USBUART_Host_SOF_VECT_NUM, USBUART_Host_SOF_PRIOR);
    #endif   /* End USBUART_Host_SOF_ISR_REMOVE */

    /* Set the Control Endpoint Interrupt. */
    CyIntSetVector(USBUART_Host_EP_0_VECT_NUM,   USBUART_Host_EP_0_ISR);
    CyIntSetPriority(USBUART_Host_EP_0_VECT_NUM, USBUART_Host_EP_0_PRIOR);

    /* Set the Data Endpoint 1 Interrupt. */
    #if(USBUART_Host_EP1_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Host_EP_1_VECT_NUM,   USBUART_Host_EP_1_ISR);
        CyIntSetPriority(USBUART_Host_EP_1_VECT_NUM, USBUART_Host_EP_1_PRIOR);
    #endif   /* End USBUART_Host_EP1_ISR_REMOVE */

    /* Set the Data Endpoint 2 Interrupt. */
    #if(USBUART_Host_EP2_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Host_EP_2_VECT_NUM,   USBUART_Host_EP_2_ISR);
        CyIntSetPriority(USBUART_Host_EP_2_VECT_NUM, USBUART_Host_EP_2_PRIOR);
    #endif   /* End USBUART_Host_EP2_ISR_REMOVE */

    /* Set the Data Endpoint 3 Interrupt. */
    #if(USBUART_Host_EP3_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Host_EP_3_VECT_NUM,   USBUART_Host_EP_3_ISR);
        CyIntSetPriority(USBUART_Host_EP_3_VECT_NUM, USBUART_Host_EP_3_PRIOR);
    #endif   /* End USBUART_Host_EP3_ISR_REMOVE */

    /* Set the Data Endpoint 4 Interrupt. */
    #if(USBUART_Host_EP4_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Host_EP_4_VECT_NUM,   USBUART_Host_EP_4_ISR);
        CyIntSetPriority(USBUART_Host_EP_4_VECT_NUM, USBUART_Host_EP_4_PRIOR);
    #endif   /* End USBUART_Host_EP4_ISR_REMOVE */

    /* Set the Data Endpoint 5 Interrupt. */
    #if(USBUART_Host_EP5_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Host_EP_5_VECT_NUM,   USBUART_Host_EP_5_ISR);
        CyIntSetPriority(USBUART_Host_EP_5_VECT_NUM, USBUART_Host_EP_5_PRIOR);
    #endif   /* End USBUART_Host_EP5_ISR_REMOVE */

    /* Set the Data Endpoint 6 Interrupt. */
    #if(USBUART_Host_EP6_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Host_EP_6_VECT_NUM,   USBUART_Host_EP_6_ISR);
        CyIntSetPriority(USBUART_Host_EP_6_VECT_NUM, USBUART_Host_EP_6_PRIOR);
    #endif   /* End USBUART_Host_EP6_ISR_REMOVE */

     /* Set the Data Endpoint 7 Interrupt. */
    #if(USBUART_Host_EP7_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Host_EP_7_VECT_NUM,   USBUART_Host_EP_7_ISR);
        CyIntSetPriority(USBUART_Host_EP_7_VECT_NUM, USBUART_Host_EP_7_PRIOR);
    #endif   /* End USBUART_Host_EP7_ISR_REMOVE */

    /* Set the Data Endpoint 8 Interrupt. */
    #if(USBUART_Host_EP8_ISR_REMOVE == 0u)
        CyIntSetVector(USBUART_Host_EP_8_VECT_NUM,   USBUART_Host_EP_8_ISR);
        CyIntSetPriority(USBUART_Host_EP_8_VECT_NUM, USBUART_Host_EP_8_PRIOR);
    #endif   /* End USBUART_Host_EP8_ISR_REMOVE */

    #if((USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL) && (USBUART_Host_ARB_ISR_REMOVE == 0u))
        /* Set the ARB Interrupt. */
        CyIntSetVector(USBUART_Host_ARB_VECT_NUM,   USBUART_Host_ARB_ISR);
        CyIntSetPriority(USBUART_Host_ARB_VECT_NUM, USBUART_Host_ARB_PRIOR);
    #endif   /* End USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL */

}


/*******************************************************************************
* Function Name: USBUART_Host_InitComponent
********************************************************************************
*
* Summary:
*  Initialize the component, except for the HW which is done one time in
*  the Start function.  This function pulls up D+.
*
* Parameters:
*  device: Contains the device number from the desired device descriptor set
*          entered with the USBFS customizer.
*  mode: The operating voltage. This determines whether the voltage regulator
*        is enabled for 5V operation or if pass through mode is used for 3.3V
*        operation. Symbolic names and their associated values are given in the
*        following table.
*       USBUART_Host_3V_OPERATION - Disable voltage regulator and pass-thru
*                                       Vcc for pull-up
*       USBUART_Host_5V_OPERATION - Enable voltage regulator and use
*                                       regulator for pull-up
*       USBUART_Host_DWR_VDDD_OPERATION - Enable or Disable voltage
*                         regulator depend on Vddd Voltage configuration in DWR.
*
* Return:
*   None.
*
* Global variables:
*   USBUART_Host_device: Contains the started device number from the
*       desired device descriptor set entered with the USBFS customizer.
*   USBUART_Host_transferState: This variable used by the communication
*       functions to handle current transfer state. Initialized to
*       TRANS_STATE_IDLE in this API.
*   USBUART_Host_configuration: Contains current configuration number
*       which is set by the Host using SET_CONFIGURATION request.
*       Initialized to zero in this API.
*   USBUART_Host_deviceAddress: Contains current device address. This
*       variable is initialized to zero in this API. Host starts to communicate
*      to device with address 0 and then set it to whatever value using
*      SET_ADDRESS request.
*   USBUART_Host_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit.
*   USBUART_Host_lastPacketSize initialized to 0;
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_InitComponent(uint8 device, uint8 mode) 
{
    /* Initialize _hidProtocol variable to comply with
    *  HID 7.2.6 Set_Protocol Request:
    *  "When initialized, all devices default to report protocol."
    */
    #if defined(USBUART_Host_ENABLE_HID_CLASS)
        uint8 interface;

        for (interface = 0u; interface < USBUART_Host_MAX_INTERFACES_NUMBER; interface++)
        {
            USBUART_Host_hidProtocol[interface] = USBUART_Host_PROTOCOL_REPORT;
        }
    #endif /* USBUART_Host_ENABLE_HID_CLASS */

    /* Enable Interrupts. */
    CyIntEnable(USBUART_Host_BUS_RESET_VECT_NUM);
    CyIntEnable(USBUART_Host_EP_0_VECT_NUM);
    #if(USBUART_Host_EP1_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Host_EP_1_VECT_NUM);
    #endif   /* End USBUART_Host_EP1_ISR_REMOVE */
    #if(USBUART_Host_EP2_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Host_EP_2_VECT_NUM);
    #endif   /* End USBUART_Host_EP2_ISR_REMOVE */
    #if(USBUART_Host_EP3_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Host_EP_3_VECT_NUM);
    #endif   /* End USBUART_Host_EP3_ISR_REMOVE */
    #if(USBUART_Host_EP4_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Host_EP_4_VECT_NUM);
    #endif   /* End USBUART_Host_EP4_ISR_REMOVE */
    #if(USBUART_Host_EP5_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Host_EP_5_VECT_NUM);
    #endif   /* End USBUART_Host_EP5_ISR_REMOVE */
    #if(USBUART_Host_EP6_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Host_EP_6_VECT_NUM);
    #endif   /* End USBUART_Host_EP6_ISR_REMOVE */
    #if(USBUART_Host_EP7_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Host_EP_7_VECT_NUM);
    #endif   /* End USBUART_Host_EP7_ISR_REMOVE */
    #if(USBUART_Host_EP8_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_Host_EP_8_VECT_NUM);
    #endif   /* End USBUART_Host_EP8_ISR_REMOVE */
    #if((USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL) && (USBUART_Host_ARB_ISR_REMOVE == 0u))
        /* usb arb interrupt enable */
        USBUART_Host_ARB_INT_EN_REG = USBUART_Host_ARB_INT_MASK;
        CyIntEnable(USBUART_Host_ARB_VECT_NUM);
    #endif   /* End USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL */

    /* Arbiter congiguration for DMA transfers */
    #if(USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL)

        #if(USBUART_Host_EP_MM == USBUART_Host__EP_DMAMANUAL)
            USBUART_Host_ARB_CFG_REG = USBUART_Host_ARB_CFG_MANUAL_DMA;
        #endif   /* End USBUART_Host_EP_MM == USBUART_Host__EP_DMAMANUAL */
        #if(USBUART_Host_EP_MM == USBUART_Host__EP_DMAAUTO)
            /*Set cfg cmplt this rises DMA request when the full configuration is done */
            USBUART_Host_ARB_CFG_REG = USBUART_Host_ARB_CFG_AUTO_DMA | USBUART_Host_ARB_CFG_AUTO_MEM;
        #endif   /* End USBUART_Host_EP_MM == USBUART_Host__EP_DMAAUTO */
    #endif   /* End USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL */

    USBUART_Host_transferState = USBUART_Host_TRANS_STATE_IDLE;

    /* USB Locking: Enabled, VRegulator: depend on mode or DWR Voltage configuration*/
    switch(mode)
    {
        case USBUART_Host_3V_OPERATION:
            USBUART_Host_CR1_REG = USBUART_Host_CR1_ENABLE_LOCK;
            break;
        case USBUART_Host_5V_OPERATION:
            USBUART_Host_CR1_REG = USBUART_Host_CR1_ENABLE_LOCK | USBUART_Host_CR1_REG_ENABLE;
            break;
        default:   /*USBUART_Host_DWR_VDDD_OPERATION */
            #if(USBUART_Host_VDDD_MV < USBUART_Host_3500MV)
                USBUART_Host_CR1_REG = USBUART_Host_CR1_ENABLE_LOCK;
            #else
                USBUART_Host_CR1_REG = USBUART_Host_CR1_ENABLE_LOCK | USBUART_Host_CR1_REG_ENABLE;
            #endif /* End USBUART_Host_VDDD_MV < USBUART_Host_3500MV */
            break;
    }

    /* Record the descriptor selection */
    USBUART_Host_device = device;

    /* Clear all of the component data */
    USBUART_Host_configuration = 0u;
    USBUART_Host_configurationChanged = 0u;
    USBUART_Host_deviceAddress  = 0u;
    USBUART_Host_deviceStatus = 0u;

    USBUART_Host_lastPacketSize = 0u;

    /*  ACK Setup, Stall IN/OUT */
    CY_SET_REG8(USBUART_Host_EP0_CR_PTR, USBUART_Host_MODE_STALL_IN_OUT);

    /* Enable the SIE with an address 0 */
    CY_SET_REG8(USBUART_Host_CR0_PTR, USBUART_Host_CR0_ENABLE);

    /* Finally, Enable d+ pullup and select iomode to USB mode*/
    CY_SET_REG8(USBUART_Host_USBIO_CR1_PTR, USBUART_Host_USBIO_CR1_USBPUEN);
}


/*******************************************************************************
* Function Name: USBUART_Host_ReInitComponent
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
*   USBUART_Host_device: Contains the started device number from the
*       desired device descriptor set entered with the USBFS customizer.
*   USBUART_Host_transferState: This variable used by the communication
*       functions to handle current transfer state. Initialized to
*       TRANS_STATE_IDLE in this API.
*   USBUART_Host_configuration: Contains current configuration number
*       which is set by the Host using SET_CONFIGURATION request.
*       Initialized to zero in this API.
*   USBUART_Host_deviceAddress: Contains current device address. This
*       variable is initialized to zero in this API. Host starts to communicate
*      to device with address 0 and then set it to whatever value using
*      SET_ADDRESS request.
*   USBUART_Host_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit.
*   USBUART_Host_lastPacketSize initialized to 0;
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_ReInitComponent() 
{
    /* Initialize _hidProtocol variable to comply with HID 7.2.6 Set_Protocol
    *  Request: "When initialized, all devices default to report protocol."
    */
    #if defined(USBUART_Host_ENABLE_HID_CLASS)
        uint8 interface;

        for (interface = 0u; interface < USBUART_Host_MAX_INTERFACES_NUMBER; interface++)
        {
            USBUART_Host_hidProtocol[interface] = USBUART_Host_PROTOCOL_REPORT;
        }
    #endif /* USBUART_Host_ENABLE_HID_CLASS */

    USBUART_Host_transferState = USBUART_Host_TRANS_STATE_IDLE;

    /* Clear all of the component data */
    USBUART_Host_configuration = 0u;
    USBUART_Host_configurationChanged = 0u;
    USBUART_Host_deviceAddress  = 0u;
    USBUART_Host_deviceStatus = 0u;

    USBUART_Host_lastPacketSize = 0u;


    /*  ACK Setup, Stall IN/OUT */
    CY_SET_REG8(USBUART_Host_EP0_CR_PTR, USBUART_Host_MODE_STALL_IN_OUT);

    /* Enable the SIE with an address 0 */
    CY_SET_REG8(USBUART_Host_CR0_PTR, USBUART_Host_CR0_ENABLE);

}


/*******************************************************************************
* Function Name: USBUART_Host_Stop
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
*   USBUART_Host_configuration: Contains current configuration number
*       which is set by the Host using SET_CONFIGURATION request.
*       Initialized to zero in this API.
*   USBUART_Host_deviceAddress: Contains current device address. This
*       variable is initialized to zero in this API. Host starts to communicate
*      to device with address 0 and then set it to whatever value using
*      SET_ADDRESS request.
*   USBUART_Host_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit.
*   USBUART_Host_configurationChanged: This variable is set to one after
*       SET_CONFIGURATION request and cleared in this function.
*   USBUART_Host_intiVar variable is set to zero
*
*******************************************************************************/
void USBUART_Host_Stop(void) 
{

    #if(USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL)
        USBUART_Host_Stop_DMA(USBUART_Host_MAX_EP);     /* Stop all DMAs */
    #endif   /* End USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL */

    /* Disable the SIE */
    USBUART_Host_CR0_REG &= ~USBUART_Host_CR0_ENABLE;
    /* Disable the d+ pullup */
    USBUART_Host_USBIO_CR1_REG &= ~USBUART_Host_USBIO_CR1_USBPUEN;
    #if(CY_PSOC5A)
        /* Disable USBIO block*/
        USBUART_Host_PM_AVAIL_CR_REG &= ~USBUART_Host_PM_AVAIL_EN_FSUSBIO;
    #endif /* End CY_PSOC5A */    
    /* Disable USB in ACT PM */
    USBUART_Host_PM_ACT_CFG_REG &= ~USBUART_Host_PM_ACT_EN_FSUSB;
    /* Disable USB block for Standby Power Mode */
    USBUART_Host_PM_STBY_CFG_REG &= ~USBUART_Host_PM_STBY_EN_FSUSB;

    /* Disable the reset and EP interrupts */
    CyIntDisable(USBUART_Host_BUS_RESET_VECT_NUM);
    CyIntDisable(USBUART_Host_EP_0_VECT_NUM);
    #if(USBUART_Host_EP1_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Host_EP_1_VECT_NUM);
    #endif   /* End USBUART_Host_EP1_ISR_REMOVE */
    #if(USBUART_Host_EP2_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Host_EP_2_VECT_NUM);
    #endif   /* End USBUART_Host_EP2_ISR_REMOVE */
    #if(USBUART_Host_EP3_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Host_EP_3_VECT_NUM);
    #endif   /* End USBUART_Host_EP3_ISR_REMOVE */
    #if(USBUART_Host_EP4_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Host_EP_4_VECT_NUM);
    #endif   /* End USBUART_Host_EP4_ISR_REMOVE */
    #if(USBUART_Host_EP5_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Host_EP_5_VECT_NUM);
    #endif   /* End USBUART_Host_EP5_ISR_REMOVE */
    #if(USBUART_Host_EP6_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Host_EP_6_VECT_NUM);
    #endif   /* End USBUART_Host_EP6_ISR_REMOVE */
    #if(USBUART_Host_EP7_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Host_EP_7_VECT_NUM);
    #endif   /* End USBUART_Host_EP7_ISR_REMOVE */
    #if(USBUART_Host_EP8_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_Host_EP_8_VECT_NUM);
    #endif   /* End USBUART_Host_EP8_ISR_REMOVE */

    /* Clear all of the component data */
    USBUART_Host_configuration = 0u;
    USBUART_Host_configurationChanged = 0u;
    USBUART_Host_deviceAddress  = 0u;
    USBUART_Host_deviceStatus = 0u;
    USBUART_Host_initVar = 0u;

}


/*******************************************************************************
* Function Name: USBUART_Host_CheckActivity
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
uint8 USBUART_Host_CheckActivity(void) 
{
    uint8 r;

    r = CY_GET_REG8(USBUART_Host_CR1_PTR);
    CY_SET_REG8(USBUART_Host_CR1_PTR, (r & ~USBUART_Host_CR1_BUS_ACTIVITY));

    return((r & USBUART_Host_CR1_BUS_ACTIVITY) >> USBUART_Host_CR1_BUS_ACTIVITY_SHIFT);
}


/*******************************************************************************
* Function Name: USBUART_Host_GetConfiguration
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
uint8 USBUART_Host_GetConfiguration(void) 
{
    return(USBUART_Host_configuration);
}


/*******************************************************************************
* Function Name: USBUART_Host_IsConfigurationChanged
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
*   USBUART_Host_configurationChanged: This variable is set to one after
*       SET_CONFIGURATION request and cleared in this function.
*
*******************************************************************************/
uint8 USBUART_Host_IsConfigurationChanged(void) 
{
    uint8 res = 0u;

    if(USBUART_Host_configurationChanged != 0u)
    {
        res = USBUART_Host_configurationChanged;
        USBUART_Host_configurationChanged = 0u;
    }

    return(res);
}


/*******************************************************************************
* Function Name: USBUART_Host_GetInterfaceSetting
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
uint8  USBUART_Host_GetInterfaceSetting(uint8 interfaceNumber)
                                                    
{
    return(USBUART_Host_interfaceSetting[interfaceNumber]);
}


/*******************************************************************************
* Function Name: USBUART_Host_GetEPState
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
uint8 USBUART_Host_GetEPState(uint8 epNumber) 
{
    return(USBUART_Host_EP[epNumber].apiEpState);
}


/*******************************************************************************
* Function Name: USBUART_Host_GetEPCount
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
uint16 USBUART_Host_GetEPCount(uint8 epNumber) 
{
    uint8 ri;
    uint16 count = 0u;

    if((epNumber > USBUART_Host_EP0) && (epNumber < USBUART_Host_MAX_EP))
    {
        ri = ((epNumber - USBUART_Host_EP1) << USBUART_Host_EPX_CNTX_ADDR_SHIFT);

        count = ((uint16)((CY_GET_REG8(&USBUART_Host_SIE_EP1_CNT0_PTR[ri]) & USBUART_Host_EPX_CNT0_MASK) << 8u)
                         | CY_GET_REG8(&USBUART_Host_SIE_EP1_CNT1_PTR[ri]))
                         - USBUART_Host_EPX_CNTX_CRC_COUNT;
    }
    return(count);
}


#if(USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL)


    /*******************************************************************************
    * Function Name: USBUART_Host_InitEP_DMA
    ********************************************************************************
    *
    * Summary: This function allocate and initializes a EP DMA chanel to be used
    *     by the USBUART_Host_LoadInEP() or USBUART_Host_ReadOutEP() APIs.
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
    void USBUART_Host_InitEP_DMA(uint8 epNumber, uint8 *pData) 
    {
        uint16 src;
        uint16 dst;
        #if (defined(__C51__))          /* PSoC 3 - Source is Flash */
            src = HI16(CYDEV_SRAM_BASE);
            dst = HI16(CYDEV_PERIPH_BASE);
            pData = pData;
        #else                           /* PSoC 5 */
            if((USBUART_Host_EP[epNumber].addr & USBUART_Host_DIR_IN) != 0u )
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
            case USBUART_Host_EP1:
                #if(USBUART_Host_DMA1_REMOVE == 0u)
                    USBUART_Host_DmaChan[epNumber] = USBUART_Host_ep1_DmaInitialize(
                        USBUART_Host_DMA_BYTES_PER_BURST, USBUART_Host_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Host_DMA1_REMOVE */
                break;
            case USBUART_Host_EP2:
                #if(USBUART_Host_DMA2_REMOVE == 0u)
                    USBUART_Host_DmaChan[epNumber] = USBUART_Host_ep2_DmaInitialize(
                        USBUART_Host_DMA_BYTES_PER_BURST, USBUART_Host_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Host_DMA2_REMOVE */
                break;
            case USBUART_Host_EP3:
                #if(USBUART_Host_DMA3_REMOVE == 0u)
                    USBUART_Host_DmaChan[epNumber] = USBUART_Host_ep3_DmaInitialize(
                        USBUART_Host_DMA_BYTES_PER_BURST, USBUART_Host_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Host_DMA3_REMOVE */
                break;
            case USBUART_Host_EP4:
                #if(USBUART_Host_DMA4_REMOVE == 0u)
                    USBUART_Host_DmaChan[epNumber] = USBUART_Host_ep4_DmaInitialize(
                        USBUART_Host_DMA_BYTES_PER_BURST, USBUART_Host_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Host_DMA4_REMOVE */
                break;
            case USBUART_Host_EP5:
                #if(USBUART_Host_DMA5_REMOVE == 0u)
                    USBUART_Host_DmaChan[epNumber] = USBUART_Host_ep5_DmaInitialize(
                        USBUART_Host_DMA_BYTES_PER_BURST, USBUART_Host_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Host_DMA5_REMOVE */
                break;
            case USBUART_Host_EP6:
                #if(USBUART_Host_DMA6_REMOVE == 0u)
                    USBUART_Host_DmaChan[epNumber] = USBUART_Host_ep6_DmaInitialize(
                        USBUART_Host_DMA_BYTES_PER_BURST, USBUART_Host_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Host_DMA6_REMOVE */
                break;
            case USBUART_Host_EP7:
                #if(USBUART_Host_DMA7_REMOVE == 0u)
                    USBUART_Host_DmaChan[epNumber] = USBUART_Host_ep7_DmaInitialize(
                        USBUART_Host_DMA_BYTES_PER_BURST, USBUART_Host_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Host_DMA7_REMOVE */
                break;
            case USBUART_Host_EP8:
                #if(USBUART_Host_DMA8_REMOVE == 0u)
                    USBUART_Host_DmaChan[epNumber] = USBUART_Host_ep8_DmaInitialize(
                        USBUART_Host_DMA_BYTES_PER_BURST, USBUART_Host_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /* End USBUART_Host_DMA8_REMOVE */
                break;
            default:
                /* Do not support EP0 DMA transfers */
                break;
        }
        if((epNumber > USBUART_Host_EP0) && (epNumber < USBUART_Host_MAX_EP))
        {
            USBUART_Host_DmaTd[epNumber] = CyDmaTdAllocate();
        }
    }


    /*******************************************************************************
    * Function Name: USBUART_Host_Stop_DMA
    ********************************************************************************
    *
    * Summary: Stops and free DMA
    *
    * Parameters:
    *  epNumber: Contains the data endpoint number or
    *           USBUART_Host_MAX_EP to stop all DMAs
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_Host_Stop_DMA(uint8 epNumber) 
    {
        uint8 i;
        i = (epNumber < USBUART_Host_MAX_EP) ? epNumber : USBUART_Host_EP1;
        do
        {
            if(USBUART_Host_DmaTd[i] != DMA_INVALID_TD)
            {
                CyDmaChDisable(USBUART_Host_DmaChan[i]);
                CyDmaTdFree(USBUART_Host_DmaTd[i]);
                USBUART_Host_DmaTd[i] = DMA_INVALID_TD;
            }
            i++;
        }while((i < USBUART_Host_MAX_EP) && (epNumber == USBUART_Host_MAX_EP));
    }

#endif /* End USBUART_Host_EP_MM != USBUART_Host__EP_MANUAL */


/*******************************************************************************
* Function Name: USBUART_Host_LoadInEP
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
void USBUART_Host_LoadInEP(uint8 epNumber, uint8 *pData, uint16 length)
                                                                        
{
    uint8 ri;
    uint8 *p;
    #if(USBUART_Host_EP_MM == USBUART_Host__EP_MANUAL)
        uint16 i;
    #endif /* End USBUART_Host_EP_MM == USBUART_Host__EP_MANUAL */

    if((epNumber > USBUART_Host_EP0) && (epNumber < USBUART_Host_MAX_EP))
    {
        ri = ((epNumber - USBUART_Host_EP1) << USBUART_Host_EPX_CNTX_ADDR_SHIFT);
        p = (uint8 *)&USBUART_Host_ARB_RW1_DR_PTR[ri];

        #if(USBUART_Host_EP_MM != USBUART_Host__EP_DMAAUTO)
            /* Limits length to available buffer space, auto MM could send packets up to 1024 bytes */
            if(length > USBUART_Host_EPX_DATA_BUF_MAX - USBUART_Host_EP[epNumber].buffOffset)
            {
                length = USBUART_Host_EPX_DATA_BUF_MAX - USBUART_Host_EP[epNumber].buffOffset;
            }
        #endif /* End USBUART_Host_EP_MM != USBUART_Host__EP_DMAAUTO */

        /* Set the count and data toggle */
        CY_SET_REG8(&USBUART_Host_SIE_EP1_CNT0_PTR[ri], (length >> 8u) | (USBUART_Host_EP[epNumber].epToggle));
        CY_SET_REG8(&USBUART_Host_SIE_EP1_CNT1_PTR[ri],  length & 0xFFu);

        #if(USBUART_Host_EP_MM == USBUART_Host__EP_MANUAL)
            if(pData != NULL)
            {
                /* Copy the data using the arbiter data register */
                for (i = 0u; i < length; i++)
                {
                    CY_SET_REG8(p, pData[i]);
                }
            }
            USBUART_Host_EP[epNumber].apiEpState = USBUART_Host_NO_EVENT_PENDING;
            /* Write the Mode register */
            USBUART_Host_SIE_EP1_CR0_PTR[ri] = USBUART_Host_EP[epNumber].epMode;
        #else
            /*Init DMA if it was not initialized */
            if(USBUART_Host_DmaTd[epNumber] == DMA_INVALID_TD)
            {
                USBUART_Host_InitEP_DMA(epNumber, pData);
            }
        #endif /* End USBUART_Host_EP_MM == USBUART_Host__EP_MANUAL */

        #if(USBUART_Host_EP_MM == USBUART_Host__EP_DMAMANUAL)
            if((pData != NULL) && (length > 0))
            {

                /* Enable DMA in mode2 for transfering data */
                CyDmaChDisable(USBUART_Host_DmaChan[epNumber]);
                CyDmaTdSetConfiguration(USBUART_Host_DmaTd[epNumber], length, DMA_INVALID_TD,\
                                                                                    TD_TERMIN_EN | TD_INC_SRC_ADR);
                CyDmaTdSetAddress(USBUART_Host_DmaTd[epNumber],  LO16((uint32)pData), LO16((uint32)p));
                /* Enable the DMA */
                CyDmaChSetInitialTd(USBUART_Host_DmaChan[epNumber], USBUART_Host_DmaTd[epNumber]);
                CyDmaChEnable(USBUART_Host_DmaChan[epNumber], 1);
                /* Generate DMA request */
                USBUART_Host_ARB_EP1_CFG_PTR[ri] |= USBUART_Host_ARB_EPX_CFG_DMA_REQ;
                USBUART_Host_ARB_EP1_CFG_PTR[ri] &= ~USBUART_Host_ARB_EPX_CFG_DMA_REQ;
                /* Mode register will be writen in arb ISR after DMA transfer copmlete */
            }
            else
            {
                /* When zero-length packet - write the Mode register directly */
                USBUART_Host_SIE_EP1_CR0_PTR[ri] = USBUART_Host_EP[epNumber].epMode;
            }
        #endif /* End USBUART_Host_EP_MM == USBUART_Host__EP_DMAMANUAL */

        #if(USBUART_Host_EP_MM == USBUART_Host__EP_DMAAUTO)
            if(pData != NULL)
            {
                /* Enable DMA in mode3 for transfering data */
                CyDmaChDisable(USBUART_Host_DmaChan[epNumber]);
                CyDmaTdSetConfiguration(USBUART_Host_DmaTd[epNumber], length, USBUART_Host_DmaTd[epNumber],\
                                                                                    TD_TERMIN_EN | TD_INC_SRC_ADR);
                CyDmaTdSetAddress(USBUART_Host_DmaTd[epNumber],  LO16((uint32)pData), LO16((uint32)p));
                /* Clear Any potential pending DMA requests before starting the DMA channel to transfer data */
                CyDmaClearPendingDrq(USBUART_Host_DmaChan[epNumber]);
                /* Enable the DMA */
                CyDmaChSetInitialTd(USBUART_Host_DmaChan[epNumber], USBUART_Host_DmaTd[epNumber]);
                CyDmaChEnable(USBUART_Host_DmaChan[epNumber], 1);
            }
            else
            {
                USBUART_Host_EP[epNumber].apiEpState = USBUART_Host_NO_EVENT_PENDING;
                if(length > 0)
                {
                    /* Set Data ready status, This will generate DMA request */
                    USBUART_Host_ARB_EP1_CFG_PTR[ri] |= USBUART_Host_ARB_EPX_CFG_IN_DATA_RDY;
                    /* Mode register will be writen in arb ISR(In Buffer Full) after first DMA transfer copmlete */
                }
                else
                {
                    /* When zero-length packet - write the Mode register directly */
                    USBUART_Host_SIE_EP1_CR0_PTR[ri] = USBUART_Host_EP[epNumber].epMode;
                }
            }
        #endif /* End USBUART_Host_EP_MM == USBUART_Host__EP_DMAAUTO */

    }
}


/*******************************************************************************
* Function Name: USBUART_Host_ReadOutEP
********************************************************************************
*
* Summary:
*  Read data from an endpoint.  The application must call
*  USBUART_Host_GetEPState to see if an event is pending.
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
uint16 USBUART_Host_ReadOutEP(uint8 epNumber, uint8 *pData, uint16 length)
                                                                        
{
    uint8 ri;
    uint8 *p;
    #if(USBUART_Host_EP_MM == USBUART_Host__EP_MANUAL)
        uint16 i;
    #endif /* End USBUART_Host_EP_MM == USBUART_Host__EP_MANUAL */
    #if(USBUART_Host_EP_MM != USBUART_Host__EP_DMAAUTO)
        uint16 xferCount;
    #endif /* End USBUART_Host_EP_MM != $INSTANCE_NAME`__EP_DMAAUTO */

    if((epNumber > USBUART_Host_EP0) && (epNumber < USBUART_Host_MAX_EP) && (pData != NULL))
    {
        ri = ((epNumber - USBUART_Host_EP1) << USBUART_Host_EPX_CNTX_ADDR_SHIFT);
        p = (uint8 *)&USBUART_Host_ARB_RW1_DR_PTR[ri];

        #if(USBUART_Host_EP_MM != USBUART_Host__EP_DMAAUTO)
            /* Determine which is smaller the requested data or the available data */
            xferCount = USBUART_Host_GetEPCount(epNumber);
            if (length > xferCount)
            {
                length = xferCount;
            }
        #endif /* End USBUART_Host_EP_MM != USBUART_Host__EP_DMAAUTO */

        #if(USBUART_Host_EP_MM == USBUART_Host__EP_MANUAL)
            /* Copy the data using the arbiter data register */
            for (i = 0u; i < length; i++)
            {
                pData[i] = CY_GET_REG8(p);
            }

            /* (re)arming of OUT endpoint */
            USBUART_Host_EnableOutEP(epNumber);
        #else
            /*Init DMA if it was not initialized */
            if(USBUART_Host_DmaTd[epNumber] == DMA_INVALID_TD)
            {
                USBUART_Host_InitEP_DMA(epNumber, pData);
            }
        #endif /* End USBUART_Host_EP_MM == USBUART_Host__EP_MANUAL */

        #if(USBUART_Host_EP_MM == USBUART_Host__EP_DMAMANUAL)
            /* Enable DMA in mode2 for transfering data */
            CyDmaChDisable(USBUART_Host_DmaChan[epNumber]);
            CyDmaTdSetConfiguration(USBUART_Host_DmaTd[epNumber], length, DMA_INVALID_TD,
                                                                                TD_TERMIN_EN | TD_INC_DST_ADR);
            CyDmaTdSetAddress(USBUART_Host_DmaTd[epNumber],  LO16((uint32)p), LO16((uint32)pData));
            /* Enable the DMA */
            CyDmaChSetInitialTd(USBUART_Host_DmaChan[epNumber], USBUART_Host_DmaTd[epNumber]);
            CyDmaChEnable(USBUART_Host_DmaChan[epNumber], 1);

            /* Generate DMA request */
            USBUART_Host_ARB_EP1_CFG_PTR[ri] |= USBUART_Host_ARB_EPX_CFG_DMA_REQ;
            USBUART_Host_ARB_EP1_CFG_PTR[ri] &= ~USBUART_Host_ARB_EPX_CFG_DMA_REQ;
            /* Out EP will be (re)armed in arb ISR after transfer complete */
        #endif /* End USBUART_Host_EP_MM == USBUART_Host__EP_DMAMANUAL */

        #if(USBUART_Host_EP_MM == USBUART_Host__EP_DMAAUTO)
            /* Enable DMA in mode3 for transfering data */
            CyDmaChDisable(USBUART_Host_DmaChan[epNumber]);
            CyDmaTdSetConfiguration(USBUART_Host_DmaTd[epNumber], length, USBUART_Host_DmaTd[epNumber],
                                                                                TD_TERMIN_EN | TD_INC_DST_ADR);
            CyDmaTdSetAddress(USBUART_Host_DmaTd[epNumber],  LO16((uint32)p), LO16((uint32)pData));

            /* Clear Any potential pending DMA requests before starting the DMA channel to transfer data */
            CyDmaClearPendingDrq(USBUART_Host_DmaChan[epNumber]);
            /* Enable the DMA */
            CyDmaChSetInitialTd(USBUART_Host_DmaChan[epNumber], USBUART_Host_DmaTd[epNumber]);
            CyDmaChEnable(USBUART_Host_DmaChan[epNumber], 1);
            /* Out EP will be (re)armed in arb ISR after transfer complete */
        #endif /* End USBUART_Host_EP_MM == USBUART_Host__EP_DMAAUTO */

    }
    else
    {
        length = 0;
    }

    return(length);
}


/*******************************************************************************
* Function Name: USBUART_Host_EnableOutEP
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
*  USBUART_Host_EP[epNumber].apiEpState - set to NO_EVENT_PENDING
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_EnableOutEP(uint8 epNumber) 
{
    uint8 ri;

    if((epNumber > USBUART_Host_EP0) && (epNumber < USBUART_Host_MAX_EP))
    {
        ri = ((epNumber - USBUART_Host_EP1) << USBUART_Host_EPX_CNTX_ADDR_SHIFT);
        USBUART_Host_EP[epNumber].apiEpState = USBUART_Host_NO_EVENT_PENDING;
        /* Write the Mode register */
        CY_SET_REG8(&USBUART_Host_SIE_EP1_CR0_PTR[ri], USBUART_Host_EP[epNumber].epMode);
    }
}


/*******************************************************************************
* Function Name: USBUART_Host_DisableOutEP
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
void USBUART_Host_DisableOutEP(uint8 epNumber) 
{
    uint8 ri ;

    if((epNumber > USBUART_Host_EP0) && (epNumber < USBUART_Host_MAX_EP))
    {
        ri = ((epNumber - USBUART_Host_EP1) << USBUART_Host_EPX_CNTX_ADDR_SHIFT);
        /* Write the Mode register */
        CY_SET_REG8(&USBUART_Host_SIE_EP1_CR0_PTR[ri], USBUART_Host_MODE_NAK_OUT);
    }
}


/*******************************************************************************
* Function Name: USBUART_Host_Force
********************************************************************************
*
* Summary:
*  Forces the bus state
*
* Parameters:
*  bState
*    USBUART_Host_FORCE_J
*    USBUART_Host_FORCE_K
*    USBUART_Host_FORCE_SE0
*    USBUART_Host_FORCE_NONE
*
* Return:
*  None.
*
*******************************************************************************/
void USBUART_Host_Force(uint8 bState) 
{
    CY_SET_REG8(USBUART_Host_USBIO_CR0_PTR, bState);
}


/*******************************************************************************
* Function Name: USBUART_Host_GetEPAckState
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
uint8 USBUART_Host_GetEPAckState(uint8 epNumber) 
{
    uint8 ri;
    uint8 cr = 0;

    if((epNumber > USBUART_Host_EP0) && (epNumber < USBUART_Host_MAX_EP))
    {
        ri = ((epNumber - USBUART_Host_EP1) << USBUART_Host_EPX_CNTX_ADDR_SHIFT);
        cr = CY_GET_REG8(&USBUART_Host_SIE_EP1_CR0_PTR[ri]) & USBUART_Host_MODE_ACKD;
    }

    return(cr);
}


/*******************************************************************************
* Function Name: USBUART_Host_SetPowerStatus
********************************************************************************
*
* Summary:
*  Sets the device power status for reporting in the Get Device Status
*  request
*
* Parameters:
*  powerStatus: USBUART_Host_DEVICE_STATUS_BUS_POWERED(0) - Bus Powered,
*               USBUART_Host_DEVICE_STATUS_SELF_POWERED(1) - Self Powered
*
* Return:
*   None.
*
* Global variables:
*  USBUART_Host_deviceStatus - set power status
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Host_SetPowerStatus(uint8 powerStatus) 
{
    if (powerStatus != USBUART_Host_DEVICE_STATUS_BUS_POWERED)
    {
        USBUART_Host_deviceStatus |=  USBUART_Host_DEVICE_STATUS_SELF_POWERED;
    }
    else
    {
        USBUART_Host_deviceStatus &=  ~USBUART_Host_DEVICE_STATUS_SELF_POWERED;
    }
}


#if (USBUART_Host_MON_VBUS == 1u)

    /*******************************************************************************
    * Function Name: USBUART_Host_VBusPresent
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
    uint8 USBUART_Host_VBusPresent(void) 
    {
        return((CY_GET_REG8(USBUART_Host_VBUS_PS_PTR) & USBUART_Host_VBUS_MASK) ? 1u : 0u);
    }

#endif /* USBUART_Host_MON_VBUS */


/*******************************************************************************
* Function Name: USBUART_Host_RWUEnabled
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
*  USBUART_Host_deviceStatus - checked to determine remote status
*
*******************************************************************************/
uint8 USBUART_Host_RWUEnabled(void) 
{
    uint8 result = USBUART_Host_FALSE;
    if((USBUART_Host_deviceStatus & USBUART_Host_DEVICE_STATUS_REMOTE_WAKEUP) != 0u)
    {
        result = USBUART_Host_TRUE;
    }

    return(result);
}


/* [] END OF FILE */
