/*******************************************************************************
* File Name: Filter_PulseInLow.h
* Version 2.0
*
* Description:
*  This header file contains definitions associated with the FILT component.
*
* Note:
* 
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(Filter_PulseInLow_H) /* FILT Header File */
#define Filter_PulseInLow_H

#include "cyfitter.h"
#include "CyLib.h"


/***************************************
*     Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct _Filter_PulseInLow_backupStruct 
{
    uint8 enableState;
    uint8 cr;
    uint8 sr;
    uint8 sema;
}   Filter_PulseInLow_BACKUP_STRUCT;


/***************************************
* FILT component function prototypes.
****************************************/

void   Filter_PulseInLow_Start(void);
void   Filter_PulseInLow_Stop(void) ;
uint8 Filter_PulseInLow_Read8(uint8 channel) ;
uint16 Filter_PulseInLow_Read16(uint8 channel) ;
uint32 Filter_PulseInLow_Read24(uint8 channel) ;
void Filter_PulseInLow_Write8(uint8 channel, uint8 sample) ;
void Filter_PulseInLow_Write16(uint8 channel, uint16 sample) ;
void Filter_PulseInLow_Write24(uint8 channel, uint32 sample) ;
void Filter_PulseInLow_Sleep(void);
void Filter_PulseInLow_Wakeup(void) ;
void Filter_PulseInLow_SaveConfig(void);
void Filter_PulseInLow_RestoreConfig(void) ;
void Filter_PulseInLow_Init(void) ;
void Filter_PulseInLow_Enable(void) ;
void Filter_PulseInLow_SetCoherency(uint8 channel, uint8 byteSelect) ;
void Filter_PulseInLow_RestoreACURam(void) ;


/*****************************************
* FILT component API Constants.
******************************************/

/* Channel Definitions */
#define Filter_PulseInLow_CHANNEL_A             (0u)
#define Filter_PulseInLow_CHANNEL_B             (1u)

#define Filter_PulseInLow_CHANNEL_A_INTR        (0x08u)
#define Filter_PulseInLow_CHANNEL_B_INTR        (0x10u)

#define Filter_PulseInLow_ALL_INTR              (0xf8u)

#define Filter_PulseInLow_SIGN_BIT              (0x00800000u)
#define Filter_PulseInLow_SIGN_BYTE             (0xFF000000u)

/* Component's enable/disable state */
#define Filter_PulseInLow_ENABLED               (0x01u)
#define Filter_PulseInLow_DISABLED              (0x00u)

/* SetCoherency API constants */
#define Filter_PulseInLow_KEY_LOW               (0x00u)
#define Filter_PulseInLow_KEY_MID               (0x01u)
#define Filter_PulseInLow_KEY_HIGH              (0x02u)


/*******************************************************************************
* FILT component macros.
*******************************************************************************/

#define Filter_PulseInLow_ClearInterruptSource() \
   (Filter_PulseInLow_SR_REG = Filter_PulseInLow_ALL_INTR )

#define Filter_PulseInLow_IsInterruptChannelA() \
    (Filter_PulseInLow_SR_REG & Filter_PulseInLow_CHANNEL_A_INTR)

#define Filter_PulseInLow_IsInterruptChannelB() \
    (Filter_PulseInLow_SR_REG & Filter_PulseInLow_CHANNEL_B_INTR)


/*******************************************************************************
* FILT component DFB registers.
*******************************************************************************/

/* DFB Status register */
#define Filter_PulseInLow_SR_REG             (* (reg8 *) Filter_PulseInLow_DFB__SR)
#define Filter_PulseInLow_SR_PTR             (  (reg8 *) Filter_PulseInLow_DFB__SR)

/* DFB Control register */
#define Filter_PulseInLow_CR_REG             (* (reg8 *) Filter_PulseInLow_DFB__CR)
#define Filter_PulseInLow_CR_PTR             (  (reg8 *) Filter_PulseInLow_DFB__CR)


/*******************************************************************************
* DFB.COHER bit field defines.
*******************************************************************************/

/* STAGEA key coherency mask */
#define Filter_PulseInLow_STAGEA_COHER_MASK    0x03u

/* HOLDA key coherency mask */
#define Filter_PulseInLow_HOLDA_COHER_MASK    (0x03u << 4)

/* STAGEB key coherency mask */
#define Filter_PulseInLow_STAGEB_COHER_MASK    0x0Cu

/* HOLDB key coherency mask */
#define Filter_PulseInLow_HOLDB_COHER_MASK    (0x0Cu << 4)

#endif /* End FILT Header File */


/* [] END OF FILE */
