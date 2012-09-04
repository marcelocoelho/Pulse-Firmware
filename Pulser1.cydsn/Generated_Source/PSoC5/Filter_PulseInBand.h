/*******************************************************************************
* File Name: Filter_PulseInBand.h
* Version 2.10
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

#if !defined(Filter_PulseInBand_H) /* FILT Header File */
#define Filter_PulseInBand_H

#include "cyfitter.h"
#include "CyLib.h"


/***************************************
*     Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct _Filter_PulseInBand_backupStruct 
{
    uint8 enableState;
    uint8 cr;
    uint8 sr;
    uint8 sema;
}   Filter_PulseInBand_BACKUP_STRUCT;


/***************************************
* FILT component function prototypes.
****************************************/

void   Filter_PulseInBand_Start(void) ;
void   Filter_PulseInBand_Stop(void) ;
uint8 Filter_PulseInBand_Read8(uint8 channel) ;
uint16 Filter_PulseInBand_Read16(uint8 channel) ;
uint32 Filter_PulseInBand_Read24(uint8 channel) ;
void Filter_PulseInBand_Write8(uint8 channel, uint8 sample) ;
void Filter_PulseInBand_Write16(uint8 channel, uint16 sample) ;
void Filter_PulseInBand_Write24(uint8 channel, uint32 sample) ;
void Filter_PulseInBand_Sleep(void) ;
void Filter_PulseInBand_Wakeup(void) ;
void Filter_PulseInBand_SaveConfig(void) ;
void Filter_PulseInBand_RestoreConfig(void) ;
void Filter_PulseInBand_Init(void) ;
void Filter_PulseInBand_Enable(void) ;
void Filter_PulseInBand_SetCoherency(uint8 channel, uint8 byteSelect) ;
void Filter_PulseInBand_RestoreACURam(void) ;


/*****************************************
* FILT component API Constants.
******************************************/

/* Channel Definitions */
#define Filter_PulseInBand_CHANNEL_A             (0u)
#define Filter_PulseInBand_CHANNEL_B             (1u)

#define Filter_PulseInBand_CHANNEL_A_INTR        (0x08u)
#define Filter_PulseInBand_CHANNEL_B_INTR        (0x10u)

#define Filter_PulseInBand_ALL_INTR              (0xf8u)

#define Filter_PulseInBand_SIGN_BIT              (0x00800000u)
#define Filter_PulseInBand_SIGN_BYTE             (0xFF000000u)

/* Component's enable/disable state */
#define Filter_PulseInBand_ENABLED               (0x01u)
#define Filter_PulseInBand_DISABLED              (0x00u)

/* SetCoherency API constants */
#define Filter_PulseInBand_KEY_LOW               (0x00u)
#define Filter_PulseInBand_KEY_MID               (0x01u)
#define Filter_PulseInBand_KEY_HIGH              (0x02u)


/*******************************************************************************
* FILT component macros.
*******************************************************************************/

#define Filter_PulseInBand_ClearInterruptSource() \
   (Filter_PulseInBand_SR_REG = Filter_PulseInBand_ALL_INTR )

#define Filter_PulseInBand_IsInterruptChannelA() \
    (Filter_PulseInBand_SR_REG & Filter_PulseInBand_CHANNEL_A_INTR)

#define Filter_PulseInBand_IsInterruptChannelB() \
    (Filter_PulseInBand_SR_REG & Filter_PulseInBand_CHANNEL_B_INTR)


/*******************************************************************************
* FILT component DFB registers.
*******************************************************************************/

/* DFB Status register */
#define Filter_PulseInBand_SR_REG             (* (reg8 *) Filter_PulseInBand_DFB__SR)
#define Filter_PulseInBand_SR_PTR             (  (reg8 *) Filter_PulseInBand_DFB__SR)

/* DFB Control register */
#define Filter_PulseInBand_CR_REG             (* (reg8 *) Filter_PulseInBand_DFB__CR)
#define Filter_PulseInBand_CR_PTR             (  (reg8 *) Filter_PulseInBand_DFB__CR)


/*******************************************************************************
* DFB.COHER bit field defines.
*******************************************************************************/

/* STAGEA key coherency mask */
#define Filter_PulseInBand_STAGEA_COHER_MASK    0x03u

/* HOLDA key coherency mask */
#define Filter_PulseInBand_HOLDA_COHER_MASK    (0x03u << 4)

/* STAGEB key coherency mask */
#define Filter_PulseInBand_STAGEB_COHER_MASK    0x0Cu

/* HOLDB key coherency mask */
#define Filter_PulseInBand_HOLDB_COHER_MASK    (0x0Cu << 4)

#endif /* End FILT Header File */


/* [] END OF FILE */
