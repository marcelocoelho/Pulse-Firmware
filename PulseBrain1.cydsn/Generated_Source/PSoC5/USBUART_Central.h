/*******************************************************************************
* File Name: USBUART_Central.h
* Version 2.50
*
* Description:
*  Header File for the USFS component. Contains prototypes and constant values.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_USBFS_USBUART_Central_H)
#define CY_USBFS_USBUART_Central_H

#include "cytypes.h"
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component USBFS_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*  Memory Type Definitions
***************************************/

/* Renamed Type Definitions for backward compatibility.
*  Should not be used in new designs.
*/
#define USBUART_Central_CODE CYCODE
#define USBUART_Central_FAR CYFAR
#if defined(__C51__) || defined(__CX51__)
    #define USBUART_Central_DATA data
    #define USBUART_Central_XDATA xdata
#else
    #define USBUART_Central_DATA
    #define USBUART_Central_XDATA
#endif /* End __C51__ */
#define USBUART_Central_NULL       NULL


/***************************************
* Enumerated Types and Parameters
***************************************/

#define USBUART_Central__EP_MANUAL 0
#define USBUART_Central__EP_DMAMANUAL 1
#define USBUART_Central__EP_DMAAUTO 2

#define USBUART_Central__MA_STATIC 0
#define USBUART_Central__MA_DYNAMIC 1



/***************************************
*    Initial Parameter Constants
***************************************/

#define USBUART_Central_NUM_DEVICES   1
#define USBUART_Central_ENABLE_CDC_CLASS   
#define USBUART_Central_ENABLE_DESCRIPTOR_STRINGS   
#define USBUART_Central_ENABLE_MSOS_STRING   
#define USBUART_Central_ENABLE_SN_STRING   
#define USBUART_Central_ENABLE_IDSN_STRING   
#define USBUART_Central_ENABLE_STRINGS   
#define USBUART_Central_ENABLE_HID_CLASS   
#define USBUART_Central_HID_RPT_1_SIZE_LSB   0x32u
#define USBUART_Central_HID_RPT_1_SIZE_MSB   0x00u
#define USBUART_Central_MAX_REPORTID_NUMBER   0

#define USBUART_Central_MON_VBUS                       (0u)
#define USBUART_Central_EXTERN_VND                     (0u)
#define USBUART_Central_EXTERN_CLS                     (0u)
#define USBUART_Central_MAX_INTERFACES_NUMBER          (2u)
#define USBUART_Central_EP0_ISR_REMOVE                 (0u)
#define USBUART_Central_EP1_ISR_REMOVE                 (0u)
#define USBUART_Central_EP2_ISR_REMOVE                 (0u)
#define USBUART_Central_EP3_ISR_REMOVE                 (0u)
#define USBUART_Central_EP4_ISR_REMOVE                 (1u)
#define USBUART_Central_EP5_ISR_REMOVE                 (1u)
#define USBUART_Central_EP6_ISR_REMOVE                 (1u)
#define USBUART_Central_EP7_ISR_REMOVE                 (1u)
#define USBUART_Central_EP8_ISR_REMOVE                 (1u)
#define USBUART_Central_EP_MM                          (0u)
#define USBUART_Central_EP_MA                          (0u)
#define USBUART_Central_DMA1_REMOVE                    (1u)
#define USBUART_Central_DMA2_REMOVE                    (1u)
#define USBUART_Central_DMA3_REMOVE                    (1u)
#define USBUART_Central_DMA4_REMOVE                    (1u)
#define USBUART_Central_DMA5_REMOVE                    (1u)
#define USBUART_Central_DMA6_REMOVE                    (1u)
#define USBUART_Central_DMA7_REMOVE                    (1u)
#define USBUART_Central_DMA8_REMOVE                    (1u)
#define USBUART_Central_SOF_ISR_REMOVE                 (0u)
#define USBUART_Central_ARB_ISR_REMOVE                 (0u)
#define USBUART_Central_DP_ISR_REMOVE                  (0u)
#define USBUART_Central_ENABLE_CDC_CLASS_API           (1u)
#define USBUART_Central_ENABLE_MIDI_API                (0u)
#define USBUART_Central_MIDI_EXT_MODE                  (0u)


/***************************************
*    Data Struct Definition
***************************************/

typedef struct _USBUART_Central_EpCtlBlock
{
    uint8  attrib;
    uint8  apiEpState;
    uint8  hwEpState;
    uint8  epToggle;
    uint8  addr;
    uint8  epMode;
    uint16 buffOffset;
    uint16 bufferSize;
    uint8  interface;
} T_USBUART_Central_EP_CTL_BLOCK;

typedef struct _USBUART_Central_EpSettingsBlock
{
    uint8  interface;
    uint8  altSetting;
    uint8  addr;
    uint8  attributes;
    uint16 bufferSize;
    uint8  bMisc;
} T_USBUART_Central_EP_SETTINGS_BLOCK;

typedef struct _USBUART_Central_XferStatusBlock
{
    uint8  status;
    uint16 length;
} T_USBUART_Central_XFER_STATUS_BLOCK;

typedef struct _USBUART_Central_Td
{
    uint16  count;
    volatile uint8 *pData;
    T_USBUART_Central_XFER_STATUS_BLOCK *pStatusBlock;
} T_USBUART_Central_TD;

typedef struct _USBUART_Central_Lut
{
    uint8   c;
    void    *p_list;
} T_USBUART_Central_LUT;

/* Resume/Suspend API Support */
typedef struct _USBUART_Central_BackupStruct
{
    uint8 enableState;
    uint8 mode;
} USBUART_Central_BACKUP_STRUCT;


/* Renamed struct fields for backward compatibility.
*  Should not be used in new designs.
*/
#define wBuffOffset         buffOffset
#define wBufferSize         bufferSize
#define bStatus             status
#define wLength             length
#define wCount              count

/* Renamed global variable for backward compatibility.
*  Should not be used in new designs.
*/
#define CurrentTD           USBUART_Central_currentTD


/***************************************
*       Function Prototypes
***************************************/

void   USBUART_Central_Start(uint8 device, uint8 mode) ;
void   USBUART_Central_Init(void) ;
void   USBUART_Central_InitComponent(uint8 device, uint8 mode) ;
void   USBUART_Central_Stop(void) ;
uint8  USBUART_Central_CheckActivity(void) ;
uint8  USBUART_Central_GetConfiguration(void) ;
uint8  USBUART_Central_IsConfigurationChanged(void) ;
uint8  USBUART_Central_GetInterfaceSetting(uint8 interfaceNumber)
                                                        ;
uint8  USBUART_Central_GetEPState(uint8 epNumber) ;
uint16 USBUART_Central_GetEPCount(uint8 epNumber) ;
void   USBUART_Central_LoadInEP(uint8 epNumber, uint8 *pData, uint16 length)
                                                                    ;
uint16 USBUART_Central_ReadOutEP(uint8 epNumber, uint8 *pData, uint16 length)
                                                                    ;
void   USBUART_Central_EnableOutEP(uint8 epNumber) ;
void   USBUART_Central_DisableOutEP(uint8 epNumber) ;
void   USBUART_Central_Force(uint8 bState) ;
uint8  USBUART_Central_GetEPAckState(uint8 epNumber) ;
void   USBUART_Central_SetPowerStatus(uint8 powerStatus) ;
uint8  USBUART_Central_RWUEnabled(void) ;
void   USBUART_Central_SerialNumString(uint8 *snString) ;
void   USBUART_Central_TerminateEP(uint8 ep) ;

void   USBUART_Central_Suspend(void) ;
void   USBUART_Central_Resume(void) ;
#if(CY_PSOC5A)
    uint8 USBUART_Central_Resume_Condition(void);
#endif /* CY_PSOC5A */    

#if (USBUART_Central_MON_VBUS == 1u)
    uint8  USBUART_Central_VBusPresent(void) ;
#endif /* End USBUART_Central_MON_VBUS */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USBUART_Central) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

    void USBUART_Central_CyBtldrCommStart(void) ;
    void USBUART_Central_CyBtldrCommStop(void) ;
    void USBUART_Central_CyBtldrCommReset(void) ;
    cystatus USBUART_Central_CyBtldrCommWrite(uint8 *pData, uint16 size, uint16 *count, uint8 timeOut) CYSMALL
                                                        ;
    cystatus USBUART_Central_CyBtldrCommRead( uint8 *pData, uint16 size, uint16 *count, uint8 timeOut) CYSMALL
                                                        ;

    #define USBUART_Central_BTLDR_SIZEOF_WRITE_BUFFER      (64u)    /* EP 1 OUT */
    #define USBUART_Central_BTLDR_SIZEOF_READ_BUFFER       (64u)    /* EP 2 IN */
    #define USBUART_Central_BTLDR_MAX_PACKET_SIZE          USBUART_Central_BTLDR_SIZEOF_WRITE_BUFFER

    /* These defines active if used USBFS interface as an
    *  IO Component for bootloading. When Custom_Interface selected
    *  in Bootloder configuration as the IO Component, user must
    *  provide these functions
    */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USBUART_Central)
        #define CyBtldrCommStart        USBUART_Central_CyBtldrCommStart
        #define CyBtldrCommStop         USBUART_Central_CyBtldrCommStop
        #define CyBtldrCommReset        USBUART_Central_CyBtldrCommReset
        #define CyBtldrCommWrite        USBUART_Central_CyBtldrCommWrite
        #define CyBtldrCommRead         USBUART_Central_CyBtldrCommRead
    #endif  /*End   CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USBUART_Central */

#endif /* End CYDEV_BOOTLOADER_IO_COMP  */

#if(USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL)
    void USBUART_Central_InitEP_DMA(uint8 epNumber, uint8 *pData) ;
    void USBUART_Central_Stop_DMA(uint8 epNumber) ;
#endif /* End USBUART_Central_EP_MM != USBUART_Central__EP_MANUAL) */

#if defined(USBUART_Central_ENABLE_MIDI_STREAMING) && (USBUART_Central_ENABLE_MIDI_API != 0u)
    void USBUART_Central_MIDI_EP_Init(void) ;
    void USBUART_Central_MIDI_IN_Service(void) ;
    uint8 USBUART_Central_PutUsbMidiIn(uint8 ic, uint8* midiMsg, uint8 cable)
                                                            ;
    void USBUART_Central_MIDI_OUT_EP_Service(void) ;
#endif /* End USBUART_Central_ENABLE_MIDI_API != 0u */

/* Renamed Functions for backward compatibility.
*  Should not be used in new designs.
*/

#define USBUART_Central_bCheckActivity             USBUART_Central_CheckActivity
#define USBUART_Central_bGetConfiguration          USBUART_Central_GetConfiguration
#define USBUART_Central_bGetInterfaceSetting       USBUART_Central_GetInterfaceSetting
#define USBUART_Central_bGetEPState                USBUART_Central_GetEPState
#define USBUART_Central_wGetEPCount                USBUART_Central_GetEPCount
#define USBUART_Central_bGetEPAckState             USBUART_Central_GetEPAckState
#define USBUART_Central_bRWUEnabled                USBUART_Central_RWUEnabled
#define USBUART_Central_bVBusPresent               USBUART_Central_VBusPresent

#define USBUART_Central_bConfiguration             USBUART_Central_configuration
#define USBUART_Central_bInterfaceSetting          USBUART_Central_interfaceSetting
#define USBUART_Central_bDeviceAddress             USBUART_Central_deviceAddress
#define USBUART_Central_bDeviceStatus              USBUART_Central_deviceStatus
#define USBUART_Central_bDevice                    USBUART_Central_device
#define USBUART_Central_bTransferState             USBUART_Central_transferState
#define USBUART_Central_bLastPacketSize            USBUART_Central_lastPacketSize

#define USBUART_Central_LoadEP                     USBUART_Central_LoadInEP
#define USBUART_Central_LoadInISOCEP               USBUART_Central_LoadInEP
#define USBUART_Central_EnableOutISOCEP(e)         USBUART_Central_EnableOutEP(e)

#define USBUART_Central_SetVector                  CyIntSetVector
#define USBUART_Central_SetPriority                CyIntSetPriority
#define USBUART_Central_EnableInt(a, b)            CyIntEnable(a)


/***************************************
*          API Constants
***************************************/

#define USBUART_Central_EP0                        (0u)
#define USBUART_Central_EP1                        (1u)
#define USBUART_Central_EP2                        (2u)
#define USBUART_Central_EP3                        (3u)
#define USBUART_Central_EP4                        (4u)
#define USBUART_Central_EP5                        (5u)
#define USBUART_Central_EP6                        (6u)
#define USBUART_Central_EP7                        (7u)
#define USBUART_Central_EP8                        (8u)
#define USBUART_Central_MAX_EP                     (9u)

#define USBUART_Central_TRUE                       (1u)
#define USBUART_Central_FALSE                      (0u)

#define USBUART_Central_NO_EVENT_ALLOWED           (2u)
#define USBUART_Central_EVENT_PENDING              (1u)
#define USBUART_Central_NO_EVENT_PENDING           (0u)

#define USBUART_Central_IN_BUFFER_FULL             USBUART_Central_NO_EVENT_PENDING
#define USBUART_Central_IN_BUFFER_EMPTY            USBUART_Central_EVENT_PENDING
#define USBUART_Central_OUT_BUFFER_FULL            USBUART_Central_EVENT_PENDING
#define USBUART_Central_OUT_BUFFER_EMPTY           USBUART_Central_NO_EVENT_PENDING

#define USBUART_Central_FORCE_J                    (0xA0u)
#define USBUART_Central_FORCE_K                    (0x80u)
#define USBUART_Central_FORCE_SE0                  (0xC0u)
#define USBUART_Central_FORCE_NONE                 (0x00u)

#define USBUART_Central_IDLE_TIMER_RUNNING         (0x02u)
#define USBUART_Central_IDLE_TIMER_EXPIRED         (0x01u)
#define USBUART_Central_IDLE_TIMER_INDEFINITE      (0x00u)

#define USBUART_Central_DEVICE_STATUS_BUS_POWERED  (0x00u)
#define USBUART_Central_DEVICE_STATUS_SELF_POWERED (0x01u)

#define USBUART_Central_3V_OPERATION               (0x00u)
#define USBUART_Central_5V_OPERATION               (0x01u)
#define USBUART_Central_DWR_VDDD_OPERATION         (0x02u)

#define USBUART_Central_MODE_DISABLE               (0x00u)
#define USBUART_Central_MODE_NAK_IN_OUT            (0x01u)
#define USBUART_Central_MODE_STATUS_OUT_ONLY       (0x02u)
#define USBUART_Central_MODE_STALL_IN_OUT          (0x03u)
#define USBUART_Central_MODE_RESERVED_0100         (0x04u)
#define USBUART_Central_MODE_ISO_OUT               (0x05u)
#define USBUART_Central_MODE_STATUS_IN_ONLY        (0x06u)
#define USBUART_Central_MODE_ISO_IN                (0x07u)
#define USBUART_Central_MODE_NAK_OUT               (0x08u)
#define USBUART_Central_MODE_ACK_OUT               (0x09u)
#define USBUART_Central_MODE_RESERVED_1010         (0x0Au)
#define USBUART_Central_MODE_ACK_OUT_STATUS_IN     (0x0Bu)
#define USBUART_Central_MODE_NAK_IN                (0x0Cu)
#define USBUART_Central_MODE_ACK_IN                (0x0Du)
#define USBUART_Central_MODE_RESERVED_1110         (0x0Eu)
#define USBUART_Central_MODE_ACK_IN_STATUS_OUT     (0x0Fu)
#define USBUART_Central_MODE_MASK                  (0x0Fu)
#define USBUART_Central_MODE_STALL_DATA_EP         (0x80u)

#define USBUART_Central_MODE_ACKD                  (0x10u)
#define USBUART_Central_MODE_OUT_RCVD              (0x20u)
#define USBUART_Central_MODE_IN_RCVD               (0x40u)
#define USBUART_Central_MODE_SETUP_RCVD            (0x80u)

#define USBUART_Central_RQST_TYPE_MASK             (0x60u)
#define USBUART_Central_RQST_TYPE_STD              (0x00u)
#define USBUART_Central_RQST_TYPE_CLS              (0x20u)
#define USBUART_Central_RQST_TYPE_VND              (0x40u)
#define USBUART_Central_RQST_DIR_MASK              (0x80u)
#define USBUART_Central_RQST_DIR_D2H               (0x80u)
#define USBUART_Central_RQST_DIR_H2D               (0x00u)
#define USBUART_Central_RQST_RCPT_MASK             (0x03u)
#define USBUART_Central_RQST_RCPT_DEV              (0x00u)
#define USBUART_Central_RQST_RCPT_IFC              (0x01u)
#define USBUART_Central_RQST_RCPT_EP               (0x02u)
#define USBUART_Central_RQST_RCPT_OTHER            (0x03u)

/* USB Class Codes */
#define USBUART_Central_CLASS_DEVICE               (0x00u)     /* Use class code info from Interface Descriptors */
#define USBUART_Central_CLASS_AUDIO                (0x01u)     /* Audio device */
#define USBUART_Central_CLASS_CDC                  (0x02u)     /* Communication device class */
#define USBUART_Central_CLASS_HID                  (0x03u)     /* Human Interface Device */
#define USBUART_Central_CLASS_PDC                  (0x05u)     /* Physical device class */
#define USBUART_Central_CLASS_IMAGE                (0x06u)     /* Still Imaging device */
#define USBUART_Central_CLASS_PRINTER              (0x07u)     /* Printer device  */
#define USBUART_Central_CLASS_MSD                  (0x08u)     /* Mass Storage device  */
#define USBUART_Central_CLASS_HUB                  (0x09u)     /* Full/Hi speed Hub */
#define USBUART_Central_CLASS_CDC_DATA             (0x0Au)     /* CDC data device */
#define USBUART_Central_CLASS_SMART_CARD           (0x0Bu)     /* Smart Card device */
#define USBUART_Central_CLASS_CSD                  (0x0Du)     /* Content Security device */
#define USBUART_Central_CLASS_VIDEO                (0x0Eu)     /* Video device */
#define USBUART_Central_CLASS_PHD                  (0x0Fu)     /* Personal Healthcare device */
#define USBUART_Central_CLASS_WIRELESSD            (0xDCu)     /* Wireless Controller */
#define USBUART_Central_CLASS_MIS                  (0xE0u)     /* Miscellaneous */
#define USBUART_Central_CLASS_APP                  (0xEFu)     /* Application Specific */
#define USBUART_Central_CLASS_VENDOR               (0xFFu)     /* Vendor specific */


/* Standard Request Types (Table 9-4) */
#define USBUART_Central_GET_STATUS                 (0x00u)
#define USBUART_Central_CLEAR_FEATURE              (0x01u)
#define USBUART_Central_SET_FEATURE                (0x03u)
#define USBUART_Central_SET_ADDRESS                (0x05u)
#define USBUART_Central_GET_DESCRIPTOR             (0x06u)
#define USBUART_Central_SET_DESCRIPTOR             (0x07u)
#define USBUART_Central_GET_CONFIGURATION          (0x08u)
#define USBUART_Central_SET_CONFIGURATION          (0x09u)
#define USBUART_Central_GET_INTERFACE              (0x0Au)
#define USBUART_Central_SET_INTERFACE              (0x0Bu)
#define USBUART_Central_SYNCH_FRAME                (0x0Cu)

/* Vendor Specific Request Types */
/* Request for Microsoft OS String Descriptor*/
#define USBUART_Central_GET_EXTENDED_CONFIG_DESCRIPTOR (0x01u)

/* Descriptor Types (Table 9-5) */
#define USBUART_Central_DESCR_DEVICE                   (1u)
#define USBUART_Central_DESCR_CONFIG                   (2u)
#define USBUART_Central_DESCR_STRING                   (3u)
#define USBUART_Central_DESCR_INTERFACE                (4u)
#define USBUART_Central_DESCR_ENDPOINT                 (5u)
#define USBUART_Central_DESCR_DEVICE_QUALIFIER         (6u)
#define USBUART_Central_DESCR_OTHER_SPEED              (7u)
#define USBUART_Central_DESCR_INTERFACE_POWER          (8u)

/* Device Descriptor Defines */
#define USBUART_Central_DEVICE_DESCR_LENGTH            (18u)
#define USBUART_Central_DEVICE_DESCR_SN_SHIFT          (16u)

/* Config Descriptor Shifts and Masks */
#define USBUART_Central_CONFIG_DESCR_LENGTH                (0u)
#define USBUART_Central_CONFIG_DESCR_TYPE                  (1u)
#define USBUART_Central_CONFIG_DESCR_TOTAL_LENGTH_LOW      (2u)
#define USBUART_Central_CONFIG_DESCR_TOTAL_LENGTH_HI       (3u)
#define USBUART_Central_CONFIG_DESCR_NUM_INTERFACES        (4u)
#define USBUART_Central_CONFIG_DESCR_CONFIG_VALUE          (5u)
#define USBUART_Central_CONFIG_DESCR_CONFIGURATION         (6u)
#define USBUART_Central_CONFIG_DESCR_ATTRIB                (7u)
#define USBUART_Central_CONFIG_DESCR_ATTRIB_SELF_POWERED   (0x40u)
#define USBUART_Central_CONFIG_DESCR_ATTRIB_RWU_EN         (0x20u)

/* Feature Selectors (Table 9-6) */
#define USBUART_Central_DEVICE_REMOTE_WAKEUP           (0x01u)
#define USBUART_Central_ENDPOINT_HALT                  (0x00u)
#define USBUART_Central_TEST_MODE                      (0x02u)

/* USB Device Status (Figure 9-4) */
#define USBUART_Central_DEVICE_STATUS_BUS_POWERED      (0x00u)
#define USBUART_Central_DEVICE_STATUS_SELF_POWERED     (0x01u)
#define USBUART_Central_DEVICE_STATUS_REMOTE_WAKEUP    (0x02u)

/* USB Endpoint Status (Figure 9-4) */
#define USBUART_Central_ENDPOINT_STATUS_HALT           (0x01u)

/* USB Endpoint Directions */
#define USBUART_Central_DIR_IN                         (0x80u)
#define USBUART_Central_DIR_OUT                        (0x00u)
#define USBUART_Central_DIR_UNUSED                     (0x7Fu)

/* USB Endpoint Attributes */
#define USBUART_Central_EP_TYPE_CTRL                   (0x00u)
#define USBUART_Central_EP_TYPE_ISOC                   (0x01u)
#define USBUART_Central_EP_TYPE_BULK                   (0x02u)
#define USBUART_Central_EP_TYPE_INT                    (0x03u)
#define USBUART_Central_EP_TYPE_MASK                   (0x03u)

#define USBUART_Central_EP_SYNC_TYPE_NO_SYNC           (0x00u)
#define USBUART_Central_EP_SYNC_TYPE_ASYNC             (0x04u)
#define USBUART_Central_EP_SYNC_TYPE_ADAPTIVE          (0x08u)
#define USBUART_Central_EP_SYNC_TYPE_SYNCHRONOUS       (0x0Cu)
#define USBUART_Central_EP_SYNC_TYPE_MASK              (0x0Cu)

#define USBUART_Central_EP_USAGE_TYPE_DATA             (0x00u)
#define USBUART_Central_EP_USAGE_TYPE_FEEDBACK         (0x10u)
#define USBUART_Central_EP_USAGE_TYPE_IMPLICIT         (0x20u)
#define USBUART_Central_EP_USAGE_TYPE_RESERVED         (0x30u)
#define USBUART_Central_EP_USAGE_TYPE_MASK             (0x30u)

/* Endpoint Status defines */
#define USBUART_Central_EP_STATUS_LENGTH               (0x02u)

/* Endpoint Device defines */
#define USBUART_Central_DEVICE_STATUS_LENGTH           (0x02u)

/* Transfer Completion Notification */
#define USBUART_Central_XFER_IDLE                      (0x00u)
#define USBUART_Central_XFER_STATUS_ACK                (0x01u)
#define USBUART_Central_XFER_PREMATURE                 (0x02u)
#define USBUART_Central_XFER_ERROR                     (0x03u)

/* Driver State defines */
#define USBUART_Central_TRANS_STATE_IDLE               (0x00u)
#define USBUART_Central_TRANS_STATE_CONTROL_READ       (0x02u)
#define USBUART_Central_TRANS_STATE_CONTROL_WRITE      (0x04u)
#define USBUART_Central_TRANS_STATE_NO_DATA_CONTROL    (0x06u)

/* String Descriptor defines */
#define USBUART_Central_STRING_MSOS                    (0xEEu)

#if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAMANUAL)
    /* DMA manual mode defines */
    #define USBUART_Central_DMA_BYTES_PER_BURST            (0u)
    #define USBUART_Central_DMA_REQUEST_PER_BURST          (0u)
#endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAMANUAL */
#if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO)
    /* DMA automatic mode defines */
    #define USBUART_Central_DMA_BYTES_PER_BURST            (32u)
    /* BUF_SIZE-BYTES_PER_BURST examples: 55-32 bytes  44-16 bytes 33-8 bytes 22-4 bytes 11-2 bytes */
    #define USBUART_Central_DMA_BUF_SIZE                   (0x55u)
    #define USBUART_Central_DMA_REQUEST_PER_BURST          (1u)
#endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */


/***************************************
*              Registers
***************************************/

#define USBUART_Central_ARB_CFG_PTR        (  (reg8 *) USBUART_Central_USB__ARB_CFG)
#define USBUART_Central_ARB_CFG_REG        (* (reg8 *) USBUART_Central_USB__ARB_CFG)

#define USBUART_Central_ARB_EP1_CFG_PTR    (  (reg8 *) USBUART_Central_USB__ARB_EP1_CFG)
#define USBUART_Central_ARB_EP1_CFG_REG    (* (reg8 *) USBUART_Central_USB__ARB_EP1_CFG)
#define USBUART_Central_ARB_EP1_INT_EN_PTR (  (reg8 *) USBUART_Central_USB__ARB_EP1_INT_EN)
#define USBUART_Central_ARB_EP1_INT_EN_REG (* (reg8 *) USBUART_Central_USB__ARB_EP1_INT_EN)
#define USBUART_Central_ARB_EP1_SR_PTR     (  (reg8 *) USBUART_Central_USB__ARB_EP1_SR)
#define USBUART_Central_ARB_EP1_SR_REG     (* (reg8 *) USBUART_Central_USB__ARB_EP1_SR)

#define USBUART_Central_ARB_EP2_CFG_PTR    (  (reg8 *) USBUART_Central_USB__ARB_EP2_CFG)
#define USBUART_Central_ARB_EP2_CFG_REG    (* (reg8 *) USBUART_Central_USB__ARB_EP2_CFG)
#define USBUART_Central_ARB_EP2_INT_EN_PTR (  (reg8 *) USBUART_Central_USB__ARB_EP2_INT_EN)
#define USBUART_Central_ARB_EP2_INT_EN_REG (* (reg8 *) USBUART_Central_USB__ARB_EP2_INT_EN)
#define USBUART_Central_ARB_EP2_SR_PTR     (  (reg8 *) USBUART_Central_USB__ARB_EP2_SR)
#define USBUART_Central_ARB_EP2_SR_REG     (* (reg8 *) USBUART_Central_USB__ARB_EP2_SR)

#define USBUART_Central_ARB_EP3_CFG_PTR    (  (reg8 *) USBUART_Central_USB__ARB_EP3_CFG)
#define USBUART_Central_ARB_EP3_CFG_REG    (* (reg8 *) USBUART_Central_USB__ARB_EP3_CFG)
#define USBUART_Central_ARB_EP3_INT_EN_PTR (  (reg8 *) USBUART_Central_USB__ARB_EP3_INT_EN)
#define USBUART_Central_ARB_EP3_INT_EN_REG (* (reg8 *) USBUART_Central_USB__ARB_EP3_INT_EN)
#define USBUART_Central_ARB_EP3_SR_PTR     (  (reg8 *) USBUART_Central_USB__ARB_EP3_SR)
#define USBUART_Central_ARB_EP3_SR_REG     (* (reg8 *) USBUART_Central_USB__ARB_EP3_SR)

#define USBUART_Central_ARB_EP4_CFG_PTR    (  (reg8 *) USBUART_Central_USB__ARB_EP4_CFG)
#define USBUART_Central_ARB_EP4_CFG_REG    (* (reg8 *) USBUART_Central_USB__ARB_EP4_CFG)
#define USBUART_Central_ARB_EP4_INT_EN_PTR (  (reg8 *) USBUART_Central_USB__ARB_EP4_INT_EN)
#define USBUART_Central_ARB_EP4_INT_EN_REG (* (reg8 *) USBUART_Central_USB__ARB_EP4_INT_EN)
#define USBUART_Central_ARB_EP4_SR_PTR     (  (reg8 *) USBUART_Central_USB__ARB_EP4_SR)
#define USBUART_Central_ARB_EP4_SR_REG     (* (reg8 *) USBUART_Central_USB__ARB_EP4_SR)

#define USBUART_Central_ARB_EP5_CFG_PTR    (  (reg8 *) USBUART_Central_USB__ARB_EP5_CFG)
#define USBUART_Central_ARB_EP5_CFG_REG    (* (reg8 *) USBUART_Central_USB__ARB_EP5_CFG)
#define USBUART_Central_ARB_EP5_INT_EN_PTR (  (reg8 *) USBUART_Central_USB__ARB_EP5_INT_EN)
#define USBUART_Central_ARB_EP5_INT_EN_REG (* (reg8 *) USBUART_Central_USB__ARB_EP5_INT_EN)
#define USBUART_Central_ARB_EP5_SR_PTR     (  (reg8 *) USBUART_Central_USB__ARB_EP5_SR)
#define USBUART_Central_ARB_EP5_SR_REG     (* (reg8 *) USBUART_Central_USB__ARB_EP5_SR)

#define USBUART_Central_ARB_EP6_CFG_PTR    (  (reg8 *) USBUART_Central_USB__ARB_EP6_CFG)
#define USBUART_Central_ARB_EP6_CFG_REG    (* (reg8 *) USBUART_Central_USB__ARB_EP6_CFG)
#define USBUART_Central_ARB_EP6_INT_EN_PTR (  (reg8 *) USBUART_Central_USB__ARB_EP6_INT_EN)
#define USBUART_Central_ARB_EP6_INT_EN_REG (* (reg8 *) USBUART_Central_USB__ARB_EP6_INT_EN)
#define USBUART_Central_ARB_EP6_SR_PTR     (  (reg8 *) USBUART_Central_USB__ARB_EP6_SR)
#define USBUART_Central_ARB_EP6_SR_REG     (* (reg8 *) USBUART_Central_USB__ARB_EP6_SR)

#define USBUART_Central_ARB_EP7_CFG_PTR    (  (reg8 *) USBUART_Central_USB__ARB_EP7_CFG)
#define USBUART_Central_ARB_EP7_CFG_REG    (* (reg8 *) USBUART_Central_USB__ARB_EP7_CFG)
#define USBUART_Central_ARB_EP7_INT_EN_PTR (  (reg8 *) USBUART_Central_USB__ARB_EP7_INT_EN)
#define USBUART_Central_ARB_EP7_INT_EN_REG (* (reg8 *) USBUART_Central_USB__ARB_EP7_INT_EN)
#define USBUART_Central_ARB_EP7_SR_PTR     (  (reg8 *) USBUART_Central_USB__ARB_EP7_SR)
#define USBUART_Central_ARB_EP7_SR_REG     (* (reg8 *) USBUART_Central_USB__ARB_EP7_SR)

#define USBUART_Central_ARB_EP8_CFG_PTR    (  (reg8 *) USBUART_Central_USB__ARB_EP8_CFG)
#define USBUART_Central_ARB_EP8_CFG_REG    (* (reg8 *) USBUART_Central_USB__ARB_EP8_CFG)
#define USBUART_Central_ARB_EP8_INT_EN_PTR (  (reg8 *) USBUART_Central_USB__ARB_EP8_INT_EN)
#define USBUART_Central_ARB_EP8_INT_EN_REG (* (reg8 *) USBUART_Central_USB__ARB_EP8_INT_EN)
#define USBUART_Central_ARB_EP8_SR_PTR     (  (reg8 *) USBUART_Central_USB__ARB_EP8_SR)
#define USBUART_Central_ARB_EP8_SR_REG     (* (reg8 *) USBUART_Central_USB__ARB_EP8_SR)

#define USBUART_Central_ARB_INT_EN_PTR     (  (reg8 *) USBUART_Central_USB__ARB_INT_EN)
#define USBUART_Central_ARB_INT_EN_REG     (* (reg8 *) USBUART_Central_USB__ARB_INT_EN)
#define USBUART_Central_ARB_INT_SR_PTR     (  (reg8 *) USBUART_Central_USB__ARB_INT_SR)
#define USBUART_Central_ARB_INT_SR_REG     (* (reg8 *) USBUART_Central_USB__ARB_INT_SR)

#define USBUART_Central_ARB_RW1_DR_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW1_DR)
#define USBUART_Central_ARB_RW1_RA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW1_RA)
#define USBUART_Central_ARB_RW1_RA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW1_RA_MSB)
#define USBUART_Central_ARB_RW1_WA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW1_WA)
#define USBUART_Central_ARB_RW1_WA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW1_WA_MSB)

#define USBUART_Central_ARB_RW2_DR_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW2_DR)
#define USBUART_Central_ARB_RW2_RA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW2_RA)
#define USBUART_Central_ARB_RW2_RA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW2_RA_MSB)
#define USBUART_Central_ARB_RW2_WA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW2_WA)
#define USBUART_Central_ARB_RW2_WA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW2_WA_MSB)

#define USBUART_Central_ARB_RW3_DR_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW3_DR)
#define USBUART_Central_ARB_RW3_RA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW3_RA)
#define USBUART_Central_ARB_RW3_RA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW3_RA_MSB)
#define USBUART_Central_ARB_RW3_WA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW3_WA)
#define USBUART_Central_ARB_RW3_WA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW3_WA_MSB)

#define USBUART_Central_ARB_RW4_DR_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW4_DR)
#define USBUART_Central_ARB_RW4_RA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW4_RA)
#define USBUART_Central_ARB_RW4_RA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW4_RA_MSB)
#define USBUART_Central_ARB_RW4_WA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW4_WA)
#define USBUART_Central_ARB_RW4_WA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW4_WA_MSB)

#define USBUART_Central_ARB_RW5_DR_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW5_DR)
#define USBUART_Central_ARB_RW5_RA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW5_RA)
#define USBUART_Central_ARB_RW5_RA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW5_RA_MSB)
#define USBUART_Central_ARB_RW5_WA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW5_WA)
#define USBUART_Central_ARB_RW5_WA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW5_WA_MSB)

#define USBUART_Central_ARB_RW6_DR_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW6_DR)
#define USBUART_Central_ARB_RW6_RA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW6_RA)
#define USBUART_Central_ARB_RW6_RA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW6_RA_MSB)
#define USBUART_Central_ARB_RW6_WA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW6_WA)
#define USBUART_Central_ARB_RW6_WA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW6_WA_MSB)

#define USBUART_Central_ARB_RW7_DR_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW7_DR)
#define USBUART_Central_ARB_RW7_RA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW7_RA)
#define USBUART_Central_ARB_RW7_RA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW7_RA_MSB)
#define USBUART_Central_ARB_RW7_WA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW7_WA)
#define USBUART_Central_ARB_RW7_WA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW7_WA_MSB)

#define USBUART_Central_ARB_RW8_DR_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW8_DR)
#define USBUART_Central_ARB_RW8_RA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW8_RA)
#define USBUART_Central_ARB_RW8_RA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW8_RA_MSB)
#define USBUART_Central_ARB_RW8_WA_PTR     ((reg8 *) USBUART_Central_USB__ARB_RW8_WA)
#define USBUART_Central_ARB_RW8_WA_MSB_PTR ((reg8 *) USBUART_Central_USB__ARB_RW8_WA_MSB)

#define USBUART_Central_BUF_SIZE_PTR       (  (reg8 *) USBUART_Central_USB__BUF_SIZE)
#define USBUART_Central_BUF_SIZE_REG       (* (reg8 *) USBUART_Central_USB__BUF_SIZE)
#define USBUART_Central_BUS_RST_CNT_PTR    (  (reg8 *) USBUART_Central_USB__BUS_RST_CNT)
#define USBUART_Central_BUS_RST_CNT_REG    (* (reg8 *) USBUART_Central_USB__BUS_RST_CNT)
#define USBUART_Central_CWA_PTR            (  (reg8 *) USBUART_Central_USB__CWA)
#define USBUART_Central_CWA_REG            (* (reg8 *) USBUART_Central_USB__CWA)
#define USBUART_Central_CWA_MSB_PTR        (  (reg8 *) USBUART_Central_USB__CWA_MSB)
#define USBUART_Central_CWA_MSB_REG        (* (reg8 *) USBUART_Central_USB__CWA_MSB)
#define USBUART_Central_CR0_PTR            (  (reg8 *) USBUART_Central_USB__CR0)
#define USBUART_Central_CR0_REG            (* (reg8 *) USBUART_Central_USB__CR0)
#define USBUART_Central_CR1_PTR            (  (reg8 *) USBUART_Central_USB__CR1)
#define USBUART_Central_CR1_REG            (* (reg8 *) USBUART_Central_USB__CR1)

#define USBUART_Central_DMA_THRES_PTR      (  (reg8 *) USBUART_Central_USB__DMA_THRES)
#define USBUART_Central_DMA_THRES_REG      (* (reg8 *) USBUART_Central_USB__DMA_THRES)
#define USBUART_Central_DMA_THRES_MSB_PTR  (  (reg8 *) USBUART_Central_USB__DMA_THRES_MSB)
#define USBUART_Central_DMA_THRES_MSB_REG  (* (reg8 *) USBUART_Central_USB__DMA_THRES_MSB)

#define USBUART_Central_EP_ACTIVE_PTR      (  (reg8 *) USBUART_Central_USB__EP_ACTIVE)
#define USBUART_Central_EP_ACTIVE_REG      (* (reg8 *) USBUART_Central_USB__EP_ACTIVE)
#define USBUART_Central_EP_TYPE_PTR        (  (reg8 *) USBUART_Central_USB__EP_TYPE)
#define USBUART_Central_EP_TYPE_REG        (* (reg8 *) USBUART_Central_USB__EP_TYPE)

#define USBUART_Central_EP0_CNT_PTR        (  (reg8 *) USBUART_Central_USB__EP0_CNT)
#define USBUART_Central_EP0_CNT_REG        (* (reg8 *) USBUART_Central_USB__EP0_CNT)
#define USBUART_Central_EP0_CR_PTR         (  (reg8 *) USBUART_Central_USB__EP0_CR)
#define USBUART_Central_EP0_CR_REG         (* (reg8 *) USBUART_Central_USB__EP0_CR)
#define USBUART_Central_EP0_DR0_PTR        (  (reg8 *) USBUART_Central_USB__EP0_DR0)
#define USBUART_Central_EP0_DR0_REG        (* (reg8 *) USBUART_Central_USB__EP0_DR0)
#define USBUART_Central_EP0_DR1_PTR        (  (reg8 *) USBUART_Central_USB__EP0_DR1)
#define USBUART_Central_EP0_DR1_REG        (* (reg8 *) USBUART_Central_USB__EP0_DR1)
#define USBUART_Central_EP0_DR2_PTR        (  (reg8 *) USBUART_Central_USB__EP0_DR2)
#define USBUART_Central_EP0_DR2_REG        (* (reg8 *) USBUART_Central_USB__EP0_DR2)
#define USBUART_Central_EP0_DR3_PTR        (  (reg8 *) USBUART_Central_USB__EP0_DR3)
#define USBUART_Central_EP0_DR3_REG        (* (reg8 *) USBUART_Central_USB__EP0_DR3)
#define USBUART_Central_EP0_DR4_PTR        (  (reg8 *) USBUART_Central_USB__EP0_DR4)
#define USBUART_Central_EP0_DR4_REG        (* (reg8 *) USBUART_Central_USB__EP0_DR4)
#define USBUART_Central_EP0_DR5_PTR        (  (reg8 *) USBUART_Central_USB__EP0_DR5)
#define USBUART_Central_EP0_DR5_REG        (* (reg8 *) USBUART_Central_USB__EP0_DR5)
#define USBUART_Central_EP0_DR6_PTR        (  (reg8 *) USBUART_Central_USB__EP0_DR6)
#define USBUART_Central_EP0_DR6_REG        (* (reg8 *) USBUART_Central_USB__EP0_DR6)
#define USBUART_Central_EP0_DR7_PTR        (  (reg8 *) USBUART_Central_USB__EP0_DR7)
#define USBUART_Central_EP0_DR7_REG        (* (reg8 *) USBUART_Central_USB__EP0_DR7)

#define USBUART_Central_OSCLK_DR0_PTR      (  (reg8 *) USBUART_Central_USB__OSCLK_DR0)
#define USBUART_Central_OSCLK_DR0_REG      (* (reg8 *) USBUART_Central_USB__OSCLK_DR0)
#define USBUART_Central_OSCLK_DR1_PTR      (  (reg8 *) USBUART_Central_USB__OSCLK_DR1)
#define USBUART_Central_OSCLK_DR1_REG      (* (reg8 *) USBUART_Central_USB__OSCLK_DR1)

#define USBUART_Central_PM_ACT_CFG_PTR     (  (reg8 *) USBUART_Central_USB__PM_ACT_CFG)
#define USBUART_Central_PM_ACT_CFG_REG     (* (reg8 *) USBUART_Central_USB__PM_ACT_CFG)
#define USBUART_Central_PM_STBY_CFG_PTR    (  (reg8 *) USBUART_Central_USB__PM_STBY_CFG)
#define USBUART_Central_PM_STBY_CFG_REG    (* (reg8 *) USBUART_Central_USB__PM_STBY_CFG)

#define USBUART_Central_SIE_EP_INT_EN_PTR  (  (reg8 *) USBUART_Central_USB__SIE_EP_INT_EN)
#define USBUART_Central_SIE_EP_INT_EN_REG  (* (reg8 *) USBUART_Central_USB__SIE_EP_INT_EN)
#define USBUART_Central_SIE_EP_INT_SR_PTR  (  (reg8 *) USBUART_Central_USB__SIE_EP_INT_SR)
#define USBUART_Central_SIE_EP_INT_SR_REG  (* (reg8 *) USBUART_Central_USB__SIE_EP_INT_SR)

#define USBUART_Central_SIE_EP1_CNT0_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP1_CNT0)
#define USBUART_Central_SIE_EP1_CNT0_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP1_CNT0)
#define USBUART_Central_SIE_EP1_CNT1_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP1_CNT1)
#define USBUART_Central_SIE_EP1_CNT1_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP1_CNT1)
#define USBUART_Central_SIE_EP1_CR0_PTR    (  (reg8 *) USBUART_Central_USB__SIE_EP1_CR0)
#define USBUART_Central_SIE_EP1_CR0_REG    (* (reg8 *) USBUART_Central_USB__SIE_EP1_CR0)

#define USBUART_Central_SIE_EP2_CNT0_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP2_CNT0)
#define USBUART_Central_SIE_EP2_CNT0_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP2_CNT0)
#define USBUART_Central_SIE_EP2_CNT1_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP2_CNT1)
#define USBUART_Central_SIE_EP2_CNT1_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP2_CNT1)
#define USBUART_Central_SIE_EP2_CR0_PTR    (  (reg8 *) USBUART_Central_USB__SIE_EP2_CR0)
#define USBUART_Central_SIE_EP2_CR0_REG    (* (reg8 *) USBUART_Central_USB__SIE_EP2_CR0)

#define USBUART_Central_SIE_EP3_CNT0_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP3_CNT0)
#define USBUART_Central_SIE_EP3_CNT0_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP3_CNT0)
#define USBUART_Central_SIE_EP3_CNT1_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP3_CNT1)
#define USBUART_Central_SIE_EP3_CNT1_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP3_CNT1)
#define USBUART_Central_SIE_EP3_CR0_PTR    (  (reg8 *) USBUART_Central_USB__SIE_EP3_CR0)
#define USBUART_Central_SIE_EP3_CR0_REG    (* (reg8 *) USBUART_Central_USB__SIE_EP3_CR0)

#define USBUART_Central_SIE_EP4_CNT0_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP4_CNT0)
#define USBUART_Central_SIE_EP4_CNT0_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP4_CNT0)
#define USBUART_Central_SIE_EP4_CNT1_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP4_CNT1)
#define USBUART_Central_SIE_EP4_CNT1_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP4_CNT1)
#define USBUART_Central_SIE_EP4_CR0_PTR    (  (reg8 *) USBUART_Central_USB__SIE_EP4_CR0)
#define USBUART_Central_SIE_EP4_CR0_REG    (* (reg8 *) USBUART_Central_USB__SIE_EP4_CR0)

#define USBUART_Central_SIE_EP5_CNT0_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP5_CNT0)
#define USBUART_Central_SIE_EP5_CNT0_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP5_CNT0)
#define USBUART_Central_SIE_EP5_CNT1_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP5_CNT1)
#define USBUART_Central_SIE_EP5_CNT1_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP5_CNT1)
#define USBUART_Central_SIE_EP5_CR0_PTR    (  (reg8 *) USBUART_Central_USB__SIE_EP5_CR0)
#define USBUART_Central_SIE_EP5_CR0_REG    (* (reg8 *) USBUART_Central_USB__SIE_EP5_CR0)

#define USBUART_Central_SIE_EP6_CNT0_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP6_CNT0)
#define USBUART_Central_SIE_EP6_CNT0_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP6_CNT0)
#define USBUART_Central_SIE_EP6_CNT1_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP6_CNT1)
#define USBUART_Central_SIE_EP6_CNT1_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP6_CNT1)
#define USBUART_Central_SIE_EP6_CR0_PTR    (  (reg8 *) USBUART_Central_USB__SIE_EP6_CR0)
#define USBUART_Central_SIE_EP6_CR0_REG    (* (reg8 *) USBUART_Central_USB__SIE_EP6_CR0)

#define USBUART_Central_SIE_EP7_CNT0_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP7_CNT0)
#define USBUART_Central_SIE_EP7_CNT0_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP7_CNT0)
#define USBUART_Central_SIE_EP7_CNT1_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP7_CNT1)
#define USBUART_Central_SIE_EP7_CNT1_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP7_CNT1)
#define USBUART_Central_SIE_EP7_CR0_PTR    (  (reg8 *) USBUART_Central_USB__SIE_EP7_CR0)
#define USBUART_Central_SIE_EP7_CR0_REG    (* (reg8 *) USBUART_Central_USB__SIE_EP7_CR0)

#define USBUART_Central_SIE_EP8_CNT0_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP8_CNT0)
#define USBUART_Central_SIE_EP8_CNT0_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP8_CNT0)
#define USBUART_Central_SIE_EP8_CNT1_PTR   (  (reg8 *) USBUART_Central_USB__SIE_EP8_CNT1)
#define USBUART_Central_SIE_EP8_CNT1_REG   (* (reg8 *) USBUART_Central_USB__SIE_EP8_CNT1)
#define USBUART_Central_SIE_EP8_CR0_PTR    (  (reg8 *) USBUART_Central_USB__SIE_EP8_CR0)
#define USBUART_Central_SIE_EP8_CR0_REG    (* (reg8 *) USBUART_Central_USB__SIE_EP8_CR0)

#define USBUART_Central_SOF0_PTR           (  (reg8 *) USBUART_Central_USB__SOF0)
#define USBUART_Central_SOF0_REG           (* (reg8 *) USBUART_Central_USB__SOF0)
#define USBUART_Central_SOF1_PTR           (  (reg8 *) USBUART_Central_USB__SOF1)
#define USBUART_Central_SOF1_REG           (* (reg8 *) USBUART_Central_USB__SOF1)

#define USBUART_Central_USB_CLK_EN_PTR     (  (reg8 *) USBUART_Central_USB__USB_CLK_EN)
#define USBUART_Central_USB_CLK_EN_REG     (* (reg8 *) USBUART_Central_USB__USB_CLK_EN)

#define USBUART_Central_USBIO_CR0_PTR      (  (reg8 *) USBUART_Central_USB__USBIO_CR0)
#define USBUART_Central_USBIO_CR0_REG      (* (reg8 *) USBUART_Central_USB__USBIO_CR0)
#define USBUART_Central_USBIO_CR1_PTR      (  (reg8 *) USBUART_Central_USB__USBIO_CR1)
#define USBUART_Central_USBIO_CR1_REG      (* (reg8 *) USBUART_Central_USB__USBIO_CR1)
#if(!CY_PSOC5LP)
    #define USBUART_Central_USBIO_CR2_PTR      (  (reg8 *) USBUART_Central_USB__USBIO_CR2)
    #define USBUART_Central_USBIO_CR2_REG      (* (reg8 *) USBUART_Central_USB__USBIO_CR2)
#endif /* End CY_PSOC5LP */

#define USBUART_Central_DIE_ID             CYDEV_FLSHID_CUST_TABLES_BASE

#if(CY_PSOC5A)
    #define USBUART_Central_PM_AVAIL_CR_PTR    (  (reg8 *) CYREG_PM_AVAIL_CR6)
    #define USBUART_Central_PM_AVAIL_CR_REG    (* (reg8 *) CYREG_PM_AVAIL_CR6)
#else
    #define USBUART_Central_PM_USB_CR0_PTR     (  (reg8 *) CYREG_PM_USB_CR0)
    #define USBUART_Central_PM_USB_CR0_REG     (* (reg8 *) CYREG_PM_USB_CR0)
    #define USBUART_Central_DYN_RECONFIG_PTR   (  (reg8 *) USBUART_Central_USB__DYN_RECONFIG)
    #define USBUART_Central_DYN_RECONFIG_REG   (* (reg8 *) USBUART_Central_USB__DYN_RECONFIG)
#endif /* End CY_PSOC5A */

#define USBUART_Central_DM_INP_DIS_PTR     (  (reg8 *) USBUART_Central_Dm__INP_DIS)
#define USBUART_Central_DM_INP_DIS_REG     (* (reg8 *) USBUART_Central_Dm__INP_DIS)
#define USBUART_Central_DP_INP_DIS_PTR     (  (reg8 *) USBUART_Central_Dp__INP_DIS)
#define USBUART_Central_DP_INP_DIS_REG     (* (reg8 *) USBUART_Central_Dp__INP_DIS)
#define USBUART_Central_DP_INTSTAT_PTR     (  (reg8 *) USBUART_Central_Dp__INTSTAT)
#define USBUART_Central_DP_INTSTAT_REG     (* (reg8 *) USBUART_Central_Dp__INTSTAT)

#if (USBUART_Central_MON_VBUS == 1u)
    #define USBUART_Central_VBUS_DR_PTR        (  (reg8 *) USBUART_Central_VBUS__DR)
    #define USBUART_Central_VBUS_DR_REG        (* (reg8 *) USBUART_Central_VBUS__DR)
    #define USBUART_Central_VBUS_PS_PTR        (  (reg8 *) USBUART_Central_VBUS__PS)
    #define USBUART_Central_VBUS_PS_REG        (* (reg8 *) USBUART_Central_VBUS__PS)
    #define USBUART_Central_VBUS_MASK          USBUART_Central_VBUS__MASK
#endif /* End USBUART_Central_MON_VBUS */

/* Renamed Registers for backward compatibility.
*  Should not be used in new designs.
*/
#define USBUART_Central_ARB_CFG        USBUART_Central_ARB_CFG_PTR

#define USBUART_Central_ARB_EP1_CFG    USBUART_Central_ARB_EP1_CFG_PTR
#define USBUART_Central_ARB_EP1_INT_EN USBUART_Central_ARB_EP1_INT_EN_PTR
#define USBUART_Central_ARB_EP1_SR     USBUART_Central_ARB_EP1_SR_PTR

#define USBUART_Central_ARB_EP2_CFG    USBUART_Central_ARB_EP2_CFG_PTR
#define USBUART_Central_ARB_EP2_INT_EN USBUART_Central_ARB_EP2_INT_EN_PTR
#define USBUART_Central_ARB_EP2_SR     USBUART_Central_ARB_EP2_SR_PTR

#define USBUART_Central_ARB_EP3_CFG    USBUART_Central_ARB_EP3_CFG_PTR
#define USBUART_Central_ARB_EP3_INT_EN USBUART_Central_ARB_EP3_INT_EN_PTR
#define USBUART_Central_ARB_EP3_SR     USBUART_Central_ARB_EP3_SR_PTR

#define USBUART_Central_ARB_EP4_CFG    USBUART_Central_ARB_EP4_CFG_PTR
#define USBUART_Central_ARB_EP4_INT_EN USBUART_Central_ARB_EP4_INT_EN_PTR
#define USBUART_Central_ARB_EP4_SR     USBUART_Central_ARB_EP4_SR_PTR

#define USBUART_Central_ARB_EP5_CFG    USBUART_Central_ARB_EP5_CFG_PTR
#define USBUART_Central_ARB_EP5_INT_EN USBUART_Central_ARB_EP5_INT_EN_PTR
#define USBUART_Central_ARB_EP5_SR     USBUART_Central_ARB_EP5_SR_PTR

#define USBUART_Central_ARB_EP6_CFG    USBUART_Central_ARB_EP6_CFG_PTR
#define USBUART_Central_ARB_EP6_INT_EN USBUART_Central_ARB_EP6_INT_EN_PTR
#define USBUART_Central_ARB_EP6_SR     USBUART_Central_ARB_EP6_SR_PTR

#define USBUART_Central_ARB_EP7_CFG    USBUART_Central_ARB_EP7_CFG_PTR
#define USBUART_Central_ARB_EP7_INT_EN USBUART_Central_ARB_EP7_INT_EN_PTR
#define USBUART_Central_ARB_EP7_SR     USBUART_Central_ARB_EP7_SR_PTR

#define USBUART_Central_ARB_EP8_CFG    USBUART_Central_ARB_EP8_CFG_PTR
#define USBUART_Central_ARB_EP8_INT_EN USBUART_Central_ARB_EP8_INT_EN_PTR
#define USBUART_Central_ARB_EP8_SR     USBUART_Central_ARB_EP8_SR_PTR

#define USBUART_Central_ARB_INT_EN     USBUART_Central_ARB_INT_EN_PTR
#define USBUART_Central_ARB_INT_SR     USBUART_Central_ARB_INT_SR_PTR

#define USBUART_Central_ARB_RW1_DR     USBUART_Central_ARB_RW1_DR_PTR
#define USBUART_Central_ARB_RW1_RA     USBUART_Central_ARB_RW1_RA_PTR
#define USBUART_Central_ARB_RW1_RA_MSB USBUART_Central_ARB_RW1_RA_MSB_PTR
#define USBUART_Central_ARB_RW1_WA     USBUART_Central_ARB_RW1_WA_PTR
#define USBUART_Central_ARB_RW1_WA_MSB USBUART_Central_ARB_RW1_WA_MSB_PTR

#define USBUART_Central_ARB_RW2_DR     USBUART_Central_ARB_RW2_DR_PTR
#define USBUART_Central_ARB_RW2_RA     USBUART_Central_ARB_RW2_RA_PTR
#define USBUART_Central_ARB_RW2_RA_MSB USBUART_Central_ARB_RW2_RA_MSB_PTR
#define USBUART_Central_ARB_RW2_WA     USBUART_Central_ARB_RW2_WA_PTR
#define USBUART_Central_ARB_RW2_WA_MSB USBUART_Central_ARB_RW2_WA_MSB_PTR

#define USBUART_Central_ARB_RW3_DR     USBUART_Central_ARB_RW3_DR_PTR
#define USBUART_Central_ARB_RW3_RA     USBUART_Central_ARB_RW3_RA_PTR
#define USBUART_Central_ARB_RW3_RA_MSB USBUART_Central_ARB_RW3_RA_MSB_PTR
#define USBUART_Central_ARB_RW3_WA     USBUART_Central_ARB_RW3_WA_PTR
#define USBUART_Central_ARB_RW3_WA_MSB USBUART_Central_ARB_RW3_WA_MSB_PTR

#define USBUART_Central_ARB_RW4_DR     USBUART_Central_ARB_RW4_DR_PTR
#define USBUART_Central_ARB_RW4_RA     USBUART_Central_ARB_RW4_RA_PTR
#define USBUART_Central_ARB_RW4_RA_MSB USBUART_Central_ARB_RW4_RA_MSB_PTR
#define USBUART_Central_ARB_RW4_WA     USBUART_Central_ARB_RW4_WA_PTR
#define USBUART_Central_ARB_RW4_WA_MSB USBUART_Central_ARB_RW4_WA_MSB_PTR

#define USBUART_Central_ARB_RW5_DR     USBUART_Central_ARB_RW5_DR_PTR
#define USBUART_Central_ARB_RW5_RA     USBUART_Central_ARB_RW5_RA_PTR
#define USBUART_Central_ARB_RW5_RA_MSB USBUART_Central_ARB_RW5_RA_MSB_PTR
#define USBUART_Central_ARB_RW5_WA     USBUART_Central_ARB_RW5_WA_PTR
#define USBUART_Central_ARB_RW5_WA_MSB USBUART_Central_ARB_RW5_WA_MSB_PTR

#define USBUART_Central_ARB_RW6_DR     USBUART_Central_ARB_RW6_DR_PTR
#define USBUART_Central_ARB_RW6_RA     USBUART_Central_ARB_RW6_RA_PTR
#define USBUART_Central_ARB_RW6_RA_MSB USBUART_Central_ARB_RW6_RA_MSB_PTR
#define USBUART_Central_ARB_RW6_WA     USBUART_Central_ARB_RW6_WA_PTR
#define USBUART_Central_ARB_RW6_WA_MSB USBUART_Central_ARB_RW6_WA_MSB_PTR

#define USBUART_Central_ARB_RW7_DR     USBUART_Central_ARB_RW7_DR_PTR
#define USBUART_Central_ARB_RW7_RA     USBUART_Central_ARB_RW7_RA_PTR
#define USBUART_Central_ARB_RW7_RA_MSB USBUART_Central_ARB_RW7_RA_MSB_PTR
#define USBUART_Central_ARB_RW7_WA     USBUART_Central_ARB_RW7_WA_PTR
#define USBUART_Central_ARB_RW7_WA_MSB USBUART_Central_ARB_RW7_WA_MSB_PTR

#define USBUART_Central_ARB_RW8_DR     USBUART_Central_ARB_RW8_DR_PTR
#define USBUART_Central_ARB_RW8_RA     USBUART_Central_ARB_RW8_RA_PTR
#define USBUART_Central_ARB_RW8_RA_MSB USBUART_Central_ARB_RW8_RA_MSB_PTR
#define USBUART_Central_ARB_RW8_WA     USBUART_Central_ARB_RW8_WA_PTR
#define USBUART_Central_ARB_RW8_WA_MSB USBUART_Central_ARB_RW8_WA_MSB_PTR

#define USBUART_Central_BUF_SIZE       USBUART_Central_BUF_SIZE_PTR
#define USBUART_Central_BUS_RST_CNT    USBUART_Central_BUS_RST_CNT_PTR
#define USBUART_Central_CR0            USBUART_Central_CR0_PTR
#define USBUART_Central_CR1            USBUART_Central_CR1_PTR
#define USBUART_Central_CWA            USBUART_Central_CWA_PTR
#define USBUART_Central_CWA_MSB        USBUART_Central_CWA_MSB_PTR

#define USBUART_Central_DMA_THRES      USBUART_Central_DMA_THRES_PTR
#define USBUART_Central_DMA_THRES_MSB  USBUART_Central_DMA_THRES_MSB_PTR

#define USBUART_Central_EP_ACTIVE      USBUART_Central_EP_ACTIVE_PTR
#define USBUART_Central_EP_TYPE        USBUART_Central_EP_TYPE_PTR

#define USBUART_Central_EP0_CNT        USBUART_Central_EP0_CNT_PTR
#define USBUART_Central_EP0_CR         USBUART_Central_EP0_CR_PTR
#define USBUART_Central_EP0_DR0        USBUART_Central_EP0_DR0_PTR
#define USBUART_Central_EP0_DR1        USBUART_Central_EP0_DR1_PTR
#define USBUART_Central_EP0_DR2        USBUART_Central_EP0_DR2_PTR
#define USBUART_Central_EP0_DR3        USBUART_Central_EP0_DR3_PTR
#define USBUART_Central_EP0_DR4        USBUART_Central_EP0_DR4_PTR
#define USBUART_Central_EP0_DR5        USBUART_Central_EP0_DR5_PTR
#define USBUART_Central_EP0_DR6        USBUART_Central_EP0_DR6_PTR
#define USBUART_Central_EP0_DR7        USBUART_Central_EP0_DR7_PTR

#define USBUART_Central_OSCLK_DR0      USBUART_Central_OSCLK_DR0_PTR
#define USBUART_Central_OSCLK_DR1      USBUART_Central_OSCLK_DR1_PTR

#define USBUART_Central_PM_ACT_CFG     USBUART_Central_PM_ACT_CFG_PTR
#define USBUART_Central_PM_STBY_CFG    USBUART_Central_PM_STBY_CFG_PTR
#if(CY_PSOC5A)
    #define USBUART_Central_PM_AVAIL_CR    USBUART_Central_PM_AVAIL_CR_PTR
#endif /* End CY_PSOC5A */
#define USBUART_Central_SIE_EP_INT_EN  USBUART_Central_SIE_EP_INT_EN_PTR
#define USBUART_Central_SIE_EP_INT_SR  USBUART_Central_SIE_EP_INT_SR_PTR

#define USBUART_Central_SIE_EP1_CNT0   USBUART_Central_SIE_EP1_CNT0_PTR
#define USBUART_Central_SIE_EP1_CNT1   USBUART_Central_SIE_EP1_CNT1_PTR
#define USBUART_Central_SIE_EP1_CR0    USBUART_Central_SIE_EP1_CR0_PTR

#define USBUART_Central_SIE_EP2_CNT0   USBUART_Central_SIE_EP2_CNT0_PTR
#define USBUART_Central_SIE_EP2_CNT1   USBUART_Central_SIE_EP2_CNT1_PTR
#define USBUART_Central_SIE_EP2_CR0    USBUART_Central_SIE_EP2_CR0_PTR

#define USBUART_Central_SIE_EP3_CNT0   USBUART_Central_SIE_EP3_CNT0_PTR
#define USBUART_Central_SIE_EP3_CNT1   USBUART_Central_SIE_EP3_CNT1_PTR
#define USBUART_Central_SIE_EP3_CR0    USBUART_Central_SIE_EP3_CR0_PTR

#define USBUART_Central_SIE_EP4_CNT0   USBUART_Central_SIE_EP4_CNT0_PTR
#define USBUART_Central_SIE_EP4_CNT1   USBUART_Central_SIE_EP4_CNT1_PTR
#define USBUART_Central_SIE_EP4_CR0    USBUART_Central_SIE_EP4_CR0_PTR

#define USBUART_Central_SIE_EP5_CNT0   USBUART_Central_SIE_EP5_CNT0_PTR
#define USBUART_Central_SIE_EP5_CNT1   USBUART_Central_SIE_EP5_CNT1_PTR
#define USBUART_Central_SIE_EP5_CR0    USBUART_Central_SIE_EP5_CR0_PTR

#define USBUART_Central_SIE_EP6_CNT0   USBUART_Central_SIE_EP6_CNT0_PTR
#define USBUART_Central_SIE_EP6_CNT1   USBUART_Central_SIE_EP6_CNT1_PTR
#define USBUART_Central_SIE_EP6_CR0    USBUART_Central_SIE_EP6_CR0_PTR

#define USBUART_Central_SIE_EP7_CNT0   USBUART_Central_SIE_EP7_CNT0_PTR
#define USBUART_Central_SIE_EP7_CNT1   USBUART_Central_SIE_EP7_CNT1_PTR
#define USBUART_Central_SIE_EP7_CR0    USBUART_Central_SIE_EP7_CR0_PTR

#define USBUART_Central_SIE_EP8_CNT0   USBUART_Central_SIE_EP8_CNT0_PTR
#define USBUART_Central_SIE_EP8_CNT1   USBUART_Central_SIE_EP8_CNT1_PTR
#define USBUART_Central_SIE_EP8_CR0    USBUART_Central_SIE_EP8_CR0_PTR

#define USBUART_Central_SOF0           USBUART_Central_SOF0_PTR
#define USBUART_Central_SOF1           USBUART_Central_SOF1_PTR

#define USBUART_Central_USB_CLK_EN     USBUART_Central_USB_CLK_EN_PTR

#define USBUART_Central_USBIO_CR0      USBUART_Central_USBIO_CR0_PTR
#define USBUART_Central_USBIO_CR1      USBUART_Central_USBIO_CR1_PTR
#define USBUART_Central_USBIO_CR2      USBUART_Central_USBIO_CR2_PTR

#define USBUART_Central_USB_MEM        ((reg8 *) CYDEV_USB_MEM_BASE)

#if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
    /* PSoC3 interrupt registers*/
    #define USBUART_Central_USB_ISR_PRIOR  ((reg8 *) CYDEV_INTC_PRIOR0)
    #define USBUART_Central_USB_ISR_SET_EN ((reg8 *) CYDEV_INTC_SET_EN0)
    #define USBUART_Central_USB_ISR_CLR_EN ((reg8 *) CYDEV_INTC_CLR_EN0)
    #define USBUART_Central_USB_ISR_VECT   ((cyisraddress *) CYDEV_INTC_VECT_MBASE)
#elif(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_PANTHER)
    /* PSoC5 interrupt registers*/
    #define USBUART_Central_USB_ISR_PRIOR  ((reg8 *) CYDEV_NVIC_PRI_0)
    #define USBUART_Central_USB_ISR_SET_EN ((reg8 *) CYDEV_NVIC_SETENA0)
    #define USBUART_Central_USB_ISR_CLR_EN ((reg8 *) CYDEV_NVIC_CLRENA0)
    #define USBUART_Central_USB_ISR_VECT   ((cyisraddress *) CYDEV_NVIC_VECT_OFFSET)
#endif /* End CYDEV_CHIP_DIE_EXPECT */


/***************************************
* Interrupt vectors, masks and priorities
***************************************/

#define USBUART_Central_BUS_RESET_PRIOR    USBUART_Central_bus_reset__INTC_PRIOR_NUM
#define USBUART_Central_BUS_RESET_MASK     USBUART_Central_bus_reset__INTC_MASK
#define USBUART_Central_BUS_RESET_VECT_NUM USBUART_Central_bus_reset__INTC_NUMBER

#define USBUART_Central_SOF_PRIOR          USBUART_Central_sof_int__INTC_PRIOR_NUM
#define USBUART_Central_SOF_MASK           USBUART_Central_sof_int__INTC_MASK
#define USBUART_Central_SOF_VECT_NUM       USBUART_Central_sof_int__INTC_NUMBER

#define USBUART_Central_EP_0_PRIOR         USBUART_Central_ep_0__INTC_PRIOR_NUM
#define USBUART_Central_EP_0_MASK          USBUART_Central_ep_0__INTC_MASK
#define USBUART_Central_EP_0_VECT_NUM      USBUART_Central_ep_0__INTC_NUMBER

#define USBUART_Central_EP_1_PRIOR         USBUART_Central_ep_1__INTC_PRIOR_NUM
#define USBUART_Central_EP_1_MASK          USBUART_Central_ep_1__INTC_MASK
#define USBUART_Central_EP_1_VECT_NUM      USBUART_Central_ep_1__INTC_NUMBER

#define USBUART_Central_EP_2_PRIOR         USBUART_Central_ep_2__INTC_PRIOR_NUM
#define USBUART_Central_EP_2_MASK          USBUART_Central_ep_2__INTC_MASK
#define USBUART_Central_EP_2_VECT_NUM      USBUART_Central_ep_2__INTC_NUMBER

#define USBUART_Central_EP_3_PRIOR         USBUART_Central_ep_3__INTC_PRIOR_NUM
#define USBUART_Central_EP_3_MASK          USBUART_Central_ep_3__INTC_MASK
#define USBUART_Central_EP_3_VECT_NUM      USBUART_Central_ep_3__INTC_NUMBER

#define USBUART_Central_EP_4_PRIOR         USBUART_Central_ep_4__INTC_PRIOR_NUM
#define USBUART_Central_EP_4_MASK          USBUART_Central_ep_4__INTC_MASK
#define USBUART_Central_EP_4_VECT_NUM      USBUART_Central_ep_4__INTC_NUMBER

#define USBUART_Central_EP_5_PRIOR         USBUART_Central_ep_5__INTC_PRIOR_NUM
#define USBUART_Central_EP_5_MASK          USBUART_Central_ep_5__INTC_MASK
#define USBUART_Central_EP_5_VECT_NUM      USBUART_Central_ep_5__INTC_NUMBER

#define USBUART_Central_EP_6_PRIOR         USBUART_Central_ep_6__INTC_PRIOR_NUM
#define USBUART_Central_EP_6_MASK          USBUART_Central_ep_6__INTC_MASK
#define USBUART_Central_EP_6_VECT_NUM      USBUART_Central_ep_6__INTC_NUMBER

#define USBUART_Central_EP_7_PRIOR         USBUART_Central_ep_7__INTC_PRIOR_NUM
#define USBUART_Central_EP_7_MASK          USBUART_Central_ep_7__INTC_MASK
#define USBUART_Central_EP_7_VECT_NUM      USBUART_Central_ep_7__INTC_NUMBER

#define USBUART_Central_EP_8_PRIOR         USBUART_Central_ep_8__INTC_PRIOR_NUM
#define USBUART_Central_EP_8_MASK          USBUART_Central_ep_8__INTC_MASK
#define USBUART_Central_EP_8_VECT_NUM      USBUART_Central_ep_8__INTC_NUMBER

#define USBUART_Central_DP_INTC_PRIOR      USBUART_Central_dp_int__INTC_PRIOR_NUM
#define USBUART_Central_DP_INTC_MASK       USBUART_Central_dp_int__INTC_MASK
#define USBUART_Central_DP_INTC_VECT_NUM   USBUART_Central_dp_int__INTC_NUMBER

/* ARB ISR should have higher priority from EP_X ISR, therefore it is defined to higest (0) */
#define USBUART_Central_ARB_PRIOR          (0u)
#define USBUART_Central_ARB_MASK           USBUART_Central_arb_int__INTC_MASK
#define USBUART_Central_ARB_VECT_NUM       USBUART_Central_arb_int__INTC_NUMBER

/***************************************
 *  Endpoint 0 offsets (Table 9-2)
 **************************************/

#define USBUART_Central_bmRequestType      USBUART_Central_EP0_DR0_PTR
#define USBUART_Central_bRequest           USBUART_Central_EP0_DR1_PTR
#define USBUART_Central_wValue             USBUART_Central_EP0_DR2_PTR
#define USBUART_Central_wValueHi           USBUART_Central_EP0_DR3_PTR
#define USBUART_Central_wValueLo           USBUART_Central_EP0_DR2_PTR
#define USBUART_Central_wIndex             USBUART_Central_EP0_DR4_PTR
#define USBUART_Central_wIndexHi           USBUART_Central_EP0_DR5_PTR
#define USBUART_Central_wIndexLo           USBUART_Central_EP0_DR4_PTR
#define USBUART_Central_length             USBUART_Central_EP0_DR6_PTR
#define USBUART_Central_lengthHi           USBUART_Central_EP0_DR7_PTR
#define USBUART_Central_lengthLo           USBUART_Central_EP0_DR6_PTR


/***************************************
*       Register Constants
***************************************/
#define USBUART_Central_VDDD_MV                    CYDEV_VDDD_MV
#define USBUART_Central_3500MV                     (3500u)

#define USBUART_Central_CR1_REG_ENABLE             (0x01u)
#define USBUART_Central_CR1_ENABLE_LOCK            (0x02u)
#define USBUART_Central_CR1_BUS_ACTIVITY_SHIFT     (0x02u)
#define USBUART_Central_CR1_BUS_ACTIVITY           (0x01u << USBUART_Central_CR1_BUS_ACTIVITY_SHIFT)
#if(!CY_PSOC5A)
    #define USBUART_Central_CR1_TRIM_MSB_EN            (0x08u)
#endif /* End !CY_PSOC5A */

#define USBUART_Central_EP0_CNT_DATA_TOGGLE        (0x80u)
#define USBUART_Central_EPX_CNT_DATA_TOGGLE        (0x80u)
#define USBUART_Central_EPX_CNT0_MASK              (0x07u)
#define USBUART_Central_EPX_CNTX_ADDR_SHIFT        (0x04u)
#define USBUART_Central_EPX_CNTX_ADDR_OFFSET       (0x10u)
#define USBUART_Central_EPX_CNTX_CRC_COUNT         (0x02u)
#define USBUART_Central_EPX_DATA_BUF_MAX           (512u)

#define USBUART_Central_CR0_ENABLE                 (0x80u)

#if(CY_PSOC5A)
    /* USB_BUS_RST_CNT register 2 bits wide only in PSOC5A, therefore we use max length */
    #define USBUART_Central_BUS_RST_COUNT              (0x03u)
    #define USBUART_Central_USBIO_CR1_IOMODE           (0x80u)
    #define USBUART_Central_USBIO_CR1_DRIVE_MODE       (0x40u)
    #define USBUART_Central_USBIO_CR1_DPI              (0x20u)
    #define USBUART_Central_USBIO_CR1_DMI              (0x10u)
    #define USBUART_Central_USBIO_CR1_P2PUEN           (0x08u)
#else
    /* In leopard, a 100 KHz clock is used. Recommended is to count 10 pulses */
    #define USBUART_Central_BUS_RST_COUNT              (0x0au)
    #define USBUART_Central_USBIO_CR1_IOMODE           (0x20u)
#endif /* End CY_PSOC5A */
#define USBUART_Central_USBIO_CR1_USBPUEN          (0x04u)
#define USBUART_Central_USBIO_CR1_DP0              (0x02u)
#define USBUART_Central_USBIO_CR1_DM0              (0x01u)

#define USBUART_Central_USBIO_CR0_TEN              (0x80u)
#define USBUART_Central_USBIO_CR0_TSE0             (0x40u)
#define USBUART_Central_USBIO_CR0_TD               (0x20u)
#define USBUART_Central_USBIO_CR0_RD               (0x01u)

#define USBUART_Central_FASTCLK_IMO_CR_USBCLK_ON   (0x40u)
#define USBUART_Central_FASTCLK_IMO_CR_XCLKEN      (0x20u)
#define USBUART_Central_FASTCLK_IMO_CR_FX2ON       (0x10u)

#define USBUART_Central_ARB_EPX_CFG_RESET          (0x08u)
#define USBUART_Central_ARB_EPX_CFG_CRC_BYPASS     (0x04u)
#define USBUART_Central_ARB_EPX_CFG_DMA_REQ        (0x02u)
#define USBUART_Central_ARB_EPX_CFG_IN_DATA_RDY    (0x01u)

#define USBUART_Central_ARB_EPX_SR_IN_BUF_FULL     (0x01u)
#define USBUART_Central_ARB_EPX_SR_DMA_GNT         (0x02u)
#define USBUART_Central_ARB_EPX_SR_BUF_OVER        (0x04u)
#define USBUART_Central_ARB_EPX_SR_BUF_UNDER       (0x08u)

#define USBUART_Central_ARB_CFG_AUTO_MEM           (0x10u)
#define USBUART_Central_ARB_CFG_MANUAL_DMA         (0x20u)
#define USBUART_Central_ARB_CFG_AUTO_DMA           (0x40u)
#define USBUART_Central_ARB_CFG_CFG_CPM            (0x80u)

#if(USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO)
    #define USBUART_Central_ARB_EPX_INT_MASK           (0x1Du)
#else    
    #define USBUART_Central_ARB_EPX_INT_MASK           (0x1Fu)
#endif /* End USBUART_Central_EP_MM == USBUART_Central__EP_DMAAUTO */
#define USBUART_Central_ARB_INT_MASK              ((USBUART_Central_DMA1_REMOVE ^ 1) | \
                                                    (USBUART_Central_DMA2_REMOVE ^ 1) << 1 | \
                                                    (USBUART_Central_DMA3_REMOVE ^ 1) << 2 | \
                                                    (USBUART_Central_DMA4_REMOVE ^ 1) << 3 | \
                                                    (USBUART_Central_DMA5_REMOVE ^ 1) << 4 | \
                                                    (USBUART_Central_DMA6_REMOVE ^ 1) << 5 | \
                                                    (USBUART_Central_DMA7_REMOVE ^ 1) << 6 | \
                                                    (USBUART_Central_DMA8_REMOVE ^ 1) << 7)

#define USBUART_Central_SIE_EP_INT_EP1_MASK        (0x01u)
#define USBUART_Central_SIE_EP_INT_EP2_MASK        (0x02u)
#define USBUART_Central_SIE_EP_INT_EP3_MASK        (0x04u)
#define USBUART_Central_SIE_EP_INT_EP4_MASK        (0x08u)
#define USBUART_Central_SIE_EP_INT_EP5_MASK        (0x10u)
#define USBUART_Central_SIE_EP_INT_EP6_MASK        (0x20u)
#define USBUART_Central_SIE_EP_INT_EP7_MASK        (0x40u)
#define USBUART_Central_SIE_EP_INT_EP8_MASK        (0x80u)

#define USBUART_Central_PM_ACT_EN_FSUSB            USBUART_Central_USB__PM_ACT_MSK
#define USBUART_Central_PM_STBY_EN_FSUSB           USBUART_Central_USB__PM_STBY_MSK
#define USBUART_Central_PM_AVAIL_EN_FSUSBIO        (0x10u)

#define USBUART_Central_PM_USB_CR0_REF_EN          (0x01u)
#define USBUART_Central_PM_USB_CR0_PD_N            (0x02u)
#define USBUART_Central_PM_USB_CR0_PD_PULLUP_N     (0x04u)

#define USBUART_Central_USB_CLK_ENABLE             (0x01u)

#define USBUART_Central_DM_MASK                    USBUART_Central_Dm__0__MASK
#define USBUART_Central_DP_MASK                    USBUART_Central_Dp__0__MASK

#define USBUART_Central_DYN_RECONFIG_ENABLE        (0x01u)
#define USBUART_Central_DYN_RECONFIG_EP_SHIFT      (0x01u)
#define USBUART_Central_DYN_RECONFIG_RDY_STS       (0x10u)


#endif /* End CY_USBFS_USBUART_Central_H */


/* [] END OF FILE */
