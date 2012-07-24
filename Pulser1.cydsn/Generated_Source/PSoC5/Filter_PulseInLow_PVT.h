/*******************************************************************************
* File Name: Filter_PulseInLow_PVT.h
* Version 2.0
*
* Description:
*  This header file contains internal definitions for the FILT component.
*  It must be included after Filter_PulseInLow.h.
*
* Note:
* 
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_FILTER_Filter_PulseInLow_PVT_H) /* FILT Header File */
#define Filter_PulseInLow_PVT_H

#include "Filter_PulseInLow.H"


/*******************************************************************************
* FILT component internal function prototypes.
*******************************************************************************/

extern void Filter_PulseInLow_SetInterruptMode(uint8 events) ;
extern void Filter_PulseInLow_SetDMAMode(uint8 events) ;


/*******************************************************************************
* FILT component internal variables.
*******************************************************************************/

extern const uint8 CYCODE Filter_PulseInLow_control[]; 
extern const uint8 CYCODE Filter_PulseInLow_data_a[];
extern const uint8 CYCODE Filter_PulseInLow_data_b[];
extern const uint8 CYCODE Filter_PulseInLow_cfsm[];
extern const uint8 CYCODE Filter_PulseInLow_acu[];


/*******************************************************************************
* FILT component internal constants.
*******************************************************************************/

/* Parameters */
#define Filter_PulseInLow_INIT_INTERRUPT_MODE    (0u)
#define Filter_PulseInLow_INIT_DMA_MODE          (0u)
#define Filter_PulseInLow_INIT_COHER             (0x00u)
#define Filter_PulseInLow_INIT_DALIGN            (0x0Fu)

/* RAM memory map offsets */
#define Filter_PulseInLow_DA_RAM                 (void XDATA *) (Filter_PulseInLow_DFB__DPA_SRAM_DATA)
#define Filter_PulseInLow_DB_RAM                 (void XDATA *) (Filter_PulseInLow_DFB__DPB_SRAM_DATA)
#define Filter_PulseInLow_CSA_RAM                (void XDATA *) (Filter_PulseInLow_DFB__CSA_SRAM_DATA)
#define Filter_PulseInLow_CSB_RAM                (void XDATA *) (Filter_PulseInLow_DFB__CSB_SRAM_DATA)
#define Filter_PulseInLow_CFSM_RAM               (void XDATA *) (Filter_PulseInLow_DFB__FSM_SRAM_DATA)
#define Filter_PulseInLow_ACU_RAM                (void XDATA *) (Filter_PulseInLow_DFB__ACU_SRAM_DATA)

/* RAM register space sizes in bytes. */
#define Filter_PulseInLow_DA_RAM_SIZE            (0x200u)
#define Filter_PulseInLow_DB_RAM_SIZE            (0x200u)
#define Filter_PulseInLow_CSA_RAM_SIZE           (0x100u)
#define Filter_PulseInLow_CSB_RAM_SIZE           (0x100u)
#define Filter_PulseInLow_CFSM_RAM_SIZE          (0x100u)
#define Filter_PulseInLow_ACU_RAM_SIZE           (0x040u)


/*******************************************************************************
* FILT component internal register contents.
*******************************************************************************/

#define Filter_PulseInLow_PM_ACT_MSK             Filter_PulseInLow_DFB__PM_ACT_MSK 
#define Filter_PulseInLow_PM_STBY_MSK            Filter_PulseInLow_DFB__PM_STBY_MSK 

#define Filter_PulseInLow_RUN_MASK               (0x01u)
#define Filter_PulseInLow_EVENT_MASK             (0x1Fu)
#define Filter_PulseInLow_SR_EVENT_SHIFT         (0x03u)
#define Filter_PulseInLow_DMA_CTRL_MASK          (0x0Fu)
#define Filter_PulseInLow_RAM_DIR_BUS            (0x3Fu)
#define Filter_PulseInLow_RAM_DIR_DFB            (0x00u)


/*******************************************************************************
* FILT component internal DFB registers.
*******************************************************************************/

/* DFB interrupt control register */
#define Filter_PulseInLow_INT_CTRL_REG           (* (reg8 *) Filter_PulseInLow_DFB__INT_CTRL)
#define Filter_PulseInLow_INT_CTRL_PTR           (  (reg8 *) Filter_PulseInLow_DFB__INT_CTRL)

/* DFB DMA control register */
#define Filter_PulseInLow_DMA_CTRL_REG           (* (reg8 *) Filter_PulseInLow_DFB__DMA_CTRL)
#define Filter_PulseInLow_DMA_CTRL_PTR           (  (reg8 *) Filter_PulseInLow_DFB__DMA_CTRL)

/* DFB ram direction register */
#define Filter_PulseInLow_RAM_DIR_REG            (* (reg8 *) Filter_PulseInLow_DFB__RAM_DIR)
#define Filter_PulseInLow_RAM_DIR_PTR            (  (reg8 *) Filter_PulseInLow_DFB__RAM_DIR)

/* DFB DSI control register */
#define Filter_PulseInLow_DSI_CTRL_REG           (* (reg8 *) Filter_PulseInLow_DFB__DSI_CTRL)
#define Filter_PulseInLow_DSI_CTRL_PTR           (  (reg8 *) Filter_PulseInLow_DFB__DSI_CTRL)

/* DFB HOLDA(Output LSB) register */
#define Filter_PulseInLow_HOLDA_REG              (* (reg8 *) Filter_PulseInLow_DFB__HOLDA)
#define Filter_PulseInLow_HOLDA_PTR              (  (reg8 *) Filter_PulseInLow_DFB__HOLDA)

/* DFB HOLDAH(Output MSB) register */
#define Filter_PulseInLow_HOLDAH_REG             (* (reg8 *) Filter_PulseInLow_DFB__HOLDAH)
#define Filter_PulseInLow_HOLDAH_PTR             (  (reg8 *) Filter_PulseInLow_DFB__HOLDAH)

/* DFB HOLDAM(Output middle) register */
#define Filter_PulseInLow_HOLDAM_REG             (* (reg8 *) Filter_PulseInLow_DFB__HOLDAM)
#define Filter_PulseInLow_HOLDAM_PTR             (  (reg8 *) Filter_PulseInLow_DFB__HOLDAM)

/* DFB HOLDB(Output LSB) register */
#define Filter_PulseInLow_HOLDB_REG              (* (reg8 *) Filter_PulseInLow_DFB__HOLDB)
#define Filter_PulseInLow_HOLDB_PTR              (  (reg8 *) Filter_PulseInLow_DFB__HOLDB)

/* DFB HOLDBH(Output MSB) register */
#define Filter_PulseInLow_HOLDBH_REG             (* (reg8 *) Filter_PulseInLow_DFB__HOLDBH)
#define Filter_PulseInLow_HOLDBH_PTR             (  (reg8 *) Filter_PulseInLow_DFB__HOLDBH)

/* DFB HOLDBM(Output middle) register */
#define Filter_PulseInLow_HOLDBM_REG             (* (reg8 *) Filter_PulseInLow_DFB__HOLDBM)
#define Filter_PulseInLow_HOLDBM_PTR             (  (reg8 *) Filter_PulseInLow_DFB__HOLDBM)

/* Active mode power control register */
#define Filter_PulseInLow_PM_ACT_CFG_REG         (* (reg8 *) Filter_PulseInLow_DFB__PM_ACT_CFG)
#define Filter_PulseInLow_PM_ACT_CFG_PTR         (  (reg8 *) Filter_PulseInLow_DFB__PM_ACT_CFG)

/* Alternative active mode power control register */
#define Filter_PulseInLow_PM_STBY_CFG_REG        (* (reg8 *) Filter_PulseInLow_DFB__PM_STBY_CFG)
#define Filter_PulseInLow_PM_STBY_CFG_PTR        (  (reg8 *) Filter_PulseInLow_DFB__PM_STBY_CFG)

/* DFB ram enable register */
#define Filter_PulseInLow_RAM_EN_REG             (* (reg8 *) Filter_PulseInLow_DFB__RAM_EN)
#define Filter_PulseInLow_RAM_EN_PTR             (  (reg8 *) Filter_PulseInLow_DFB__RAM_EN)

/* DFB STAGEA(Input LSB) register */
#define Filter_PulseInLow_STAGEA_REG             (* (reg8 *) Filter_PulseInLow_DFB__STAGEA)
#define Filter_PulseInLow_STAGEA_PTR             (  (reg8 *) Filter_PulseInLow_DFB__STAGEA)

/* DFB STAGEAH(Input MSB) register */
#define Filter_PulseInLow_STAGEAH_REG            (* (reg8 *) Filter_PulseInLow_DFB__STAGEAH)
#define Filter_PulseInLow_STAGEAH_PTR            (  (reg8 *) Filter_PulseInLow_DFB__STAGEAH)

/* DFB STAGEAM(Input middle) register */
#define Filter_PulseInLow_STAGEAM_REG            (* (reg8 *) Filter_PulseInLow_DFB__STAGEAM)
#define Filter_PulseInLow_STAGEAM_PTR            (  (reg8 *) Filter_PulseInLow_DFB__STAGEAM)

/* DFB STAGEB(Input LSB) register */
#define Filter_PulseInLow_STAGEB_REG             (* (reg8 *) Filter_PulseInLow_DFB__STAGEB )
#define Filter_PulseInLow_STAGEB_PTR             (  (reg8 *) Filter_PulseInLow_DFB__STAGEB )

/* DFB STAGEBH(Input MSB) register */
#define Filter_PulseInLow_STAGEBH_REG            (* (reg8 *) Filter_PulseInLow_DFB__STAGEBH)
#define Filter_PulseInLow_STAGEBH_PTR            (  (reg8 *) Filter_PulseInLow_DFB__STAGEBH)

/* DFB STAGEBM(Input middle) register */
#define Filter_PulseInLow_STAGEBM_REG            (* (reg8 *) Filter_PulseInLow_DFB__STAGEBM)
#define Filter_PulseInLow_STAGEBM_PTR            (  (reg8 *) Filter_PulseInLow_DFB__STAGEBM)

/* DFB coherence register */
#define Filter_PulseInLow_COHER_REG              (* (reg8 *) Filter_PulseInLow_DFB__COHER)
#define Filter_PulseInLow_COHER_PTR              (  (reg8 *) Filter_PulseInLow_DFB__COHER)

/* DFB data align register */
#define Filter_PulseInLow_DALIGN_REG             (* (reg8 *) Filter_PulseInLow_DFB__DALIGN)
#define Filter_PulseInLow_DALIGN_PTR             (  (reg8 *) Filter_PulseInLow_DFB__DALIGN)

/* DFB semaphore register */
#define Filter_PulseInLow_SEMA_REG               (* (reg8 *) Filter_PulseInLow_DFB__SEMA)
#define Filter_PulseInLow_SEMA_PTR               (  (reg8 *) Filter_PulseInLow_DFB__SEMA)

/* DFB output register which are 16 bit aligned */
#define Filter_PulseInLow_HOLDA16_REG            (* (reg16 *) Filter_PulseInLow_DFB__HOLDAM)
#define Filter_PulseInLow_HOLDA16_PTR            (  (reg16 *) Filter_PulseInLow_DFB__HOLDAM)

#define Filter_PulseInLow_HOLDB16_REG            (* (reg16 *) Filter_PulseInLow_DFB__HOLDBM)
#define Filter_PulseInLow_HOLDB16_PTR            (  (reg16 *) Filter_PulseInLow_DFB__HOLDBM)

/* DFB input register which are 16 bit aligned */
#define Filter_PulseInLow_STAGEA16_REG           (* (reg16 *) Filter_PulseInLow_DFB__STAGEAM)
#define Filter_PulseInLow_STAGEA16_PTR           (  (reg16 *) Filter_PulseInLow_DFB__STAGEAM)

#define Filter_PulseInLow_STAGEB16_REG           (* (reg16 *) Filter_PulseInLow_DFB__STAGEBM)
#define Filter_PulseInLow_STAGEB16_PTR           (  (reg16 *) Filter_PulseInLow_DFB__STAGEBM)

/* DFB output registers which are 24 bit aligned */
#define Filter_PulseInLow_HOLDA24_REG            (* (reg32 *) Filter_PulseInLow_DFB__HOLDA)
#define Filter_PulseInLow_HOLDA24_PTR            (  (reg32 *) Filter_PulseInLow_DFB__HOLDA)

#define Filter_PulseInLow_HOLDB24_REG            (* (reg32 *) Filter_PulseInLow_DFB__HOLDB)
#define Filter_PulseInLow_HOLDB24_PTR            (  (reg32 *) Filter_PulseInLow_DFB__HOLDB)

/* DFB input registers which are 24 bit aligned */
#define Filter_PulseInLow_STAGEA24_REG           (* (reg32 *) Filter_PulseInLow_DFB__STAGEA)
#define Filter_PulseInLow_STAGEA24_PTR           (  (reg32 *) Filter_PulseInLow_DFB__STAGEA)

#define Filter_PulseInLow_STAGEB24_REG           (* (reg32 *) Filter_PulseInLow_DFB__STAGEB)
#define Filter_PulseInLow_STAGEB24_PTR           (  (reg32 *) Filter_PulseInLow_DFB__STAGEB)

#endif /* End FILT_PVT Header File */


/* [] END OF FILE */
