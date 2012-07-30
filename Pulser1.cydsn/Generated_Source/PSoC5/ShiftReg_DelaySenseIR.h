/*******************************************************************************
* File Name: ShiftReg_DelaySenseIR.h
* Version 2.0
*
* Description:
*  This header file contains definitions associated with the Shift Register
*  component.
*
* Note: none
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/


#ifndef CY_SHIFTREG_ShiftReg_DelaySenseIR_H
#define CY_SHIFTREG_ShiftReg_DelaySenseIR_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define ShiftReg_DelaySenseIR_FIFO_SIZE                (4u)
#define ShiftReg_DelaySenseIR_USE_INPUT_FIFO           (0u)
#define ShiftReg_DelaySenseIR_USE_OUTPUT_FIFO          (0u)
#define ShiftReg_DelaySenseIR_SR_SIZE                  (26u)


/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */

typedef struct _ShiftReg_DelaySenseIR_backupStruct
{    
    uint8 enableState;
    
    uint32 saveSrA0Reg;
    uint32 saveSrA1Reg;

    #if(CY_PSOC3_ES2 || CY_PSOC5_ES1) /* PSoC3 ES2 or early, PSoC5 ES1*/
        uint32 saveSrIntMask;
    #endif /*(CY_PSOC3_ES2 || CY_PSOC5_ES1)*/

} ShiftReg_DelaySenseIR_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  ShiftReg_DelaySenseIR_Start(void);
void  ShiftReg_DelaySenseIR_Stop(void) ; 
void  ShiftReg_DelaySenseIR_Init(void) ;
void  ShiftReg_DelaySenseIR_Enable(void) ; 
void  ShiftReg_DelaySenseIR_RestoreConfig(void) ;
void  ShiftReg_DelaySenseIR_SaveConfig(void);
void  ShiftReg_DelaySenseIR_Sleep(void);
void  ShiftReg_DelaySenseIR_Wakeup(void) ; 
void  ShiftReg_DelaySenseIR_EnableInt(void) ;
void  ShiftReg_DelaySenseIR_DisableInt(void) ;
void  ShiftReg_DelaySenseIR_SetIntMode(uint8 interruptSource) ;
uint8 ShiftReg_DelaySenseIR_GetIntStatus(void) ;
void  ShiftReg_DelaySenseIR_WriteRegValue(uint32 shiftData) \
                    ;
uint32 ShiftReg_DelaySenseIR_ReadRegValue(void) ;
uint8    ShiftReg_DelaySenseIR_GetFIFOStatus(uint8 fifoId) ;

#if (ShiftReg_DelaySenseIR_USE_INPUT_FIFO == 1u)
    uint8 ShiftReg_DelaySenseIR_WriteData(uint32 shiftData)\
                    ;
#endif/*(ShiftReg_DelaySenseIR_USE_INPUT_FIFO == 1u)*/
#if (ShiftReg_DelaySenseIR_USE_OUTPUT_FIFO == 1u)
    uint32 ShiftReg_DelaySenseIR_ReadData(void) ;
#endif/*(ShiftReg_DelaySenseIR_USE_OUTPUT_FIFO == 1u)*/


/***************************************
*           API Constants
***************************************/

#define ShiftReg_DelaySenseIR_LOAD                     (0x01u)
#define ShiftReg_DelaySenseIR_STORE                    (0x02u)
#define ShiftReg_DelaySenseIR_RESET                    (0x04u)

#define ShiftReg_DelaySenseIR_IN_FIFO                  (0x01u)
#define ShiftReg_DelaySenseIR_OUT_FIFO                 (0x02u)

#define ShiftReg_DelaySenseIR_RET_FIFO_FULL            (0x00u)
#define ShiftReg_DelaySenseIR_RET_FIFO_NOT_EMPTY       (0x01u)
#define ShiftReg_DelaySenseIR_RET_FIFO_EMPTY           (0x02u)

#define ShiftReg_DelaySenseIR_RET_FIFO_NOT_DEFINED     (0xFEu)
#define ShiftReg_DelaySenseIR_RET_FIFO_BAD_PARAM       (0xFFu)

/***************************************
*    Enumerated Types and Parameters
***************************************/

#define ShiftReg_DelaySenseIR__LEFT 0
#define ShiftReg_DelaySenseIR__RIGHT 1



/***************************************
*    Initial Parameter Constants
***************************************/

#define ShiftReg_DelaySenseIR_SR_MASK                  (0x3FFFFFFu)
#define ShiftReg_DelaySenseIR_INT_SRC                  (0u)
#define ShiftReg_DelaySenseIR_DIRECTION                (1u)


/***************************************
*             Registers
***************************************/

/* Shift Register control register */

#define ShiftReg_DelaySenseIR_SR_CONTROL_REG       (* (reg8 *) \
    ShiftReg_DelaySenseIR_bSR_AsyncCtl_CtrlReg__CONTROL_REG)
#define ShiftReg_DelaySenseIR_SR_CONTROL_PTR       (  (reg8 *) \
    ShiftReg_DelaySenseIR_bSR_AsyncCtl_CtrlReg__CONTROL_REG)

/* Obsolete control reg name*/
#define ShiftReg_DelaySenseIR_SR_CONTROL               ShiftReg_DelaySenseIR_SR_CONTROL_REG

/* Shift Regisster interupt status register */

#define ShiftReg_DelaySenseIR_SR_STATUS_REG            (* (reg8 *) ShiftReg_DelaySenseIR_bSR_StsReg__STATUS_REG)
#define ShiftReg_DelaySenseIR_SR_STATUS_PTR            (  (reg8 *) ShiftReg_DelaySenseIR_bSR_StsReg__STATUS_REG)
#define ShiftReg_DelaySenseIR_SR_STATUS_MASK_REG       (* (reg8 *) ShiftReg_DelaySenseIR_bSR_StsReg__MASK_REG)
#define ShiftReg_DelaySenseIR_SR_STATUS_MASK_PTR       (  (reg8 *) ShiftReg_DelaySenseIR_bSR_StsReg__MASK_REG)
#define ShiftReg_DelaySenseIR_SR_AUX_CONTROL_REG       (* (reg8 *) ShiftReg_DelaySenseIR_bSR_StsReg__STATUS_AUX_CTL_REG)
#define ShiftReg_DelaySenseIR_SR_AUX_CONTROL_PTR       (  (reg8 *) ShiftReg_DelaySenseIR_bSR_StsReg__STATUS_AUX_CTL_REG)

/* Obsolete names */
#define ShiftReg_DelaySenseIR_SR_STATUS                ShiftReg_DelaySenseIR_SR_STATUS_REG
#define ShiftReg_DelaySenseIR_SR_STATUS_MASK           ShiftReg_DelaySenseIR_SR_STATUS_MASK_REG
#define ShiftReg_DelaySenseIR_SR_AUX_CONTROL           ShiftReg_DelaySenseIR_SR_AUX_CONTROL_REG


#define ShiftReg_DelaySenseIR_IN_FIFO_VAL_LSB_PTR      ((reg32 *) \
    ShiftReg_DelaySenseIR_bSR_sC32_BShiftRegDp_u0__F0_REG )
#define ShiftReg_DelaySenseIR_SHIFT_REG_LSB_PTR        ((reg32 *) \
    ShiftReg_DelaySenseIR_bSR_sC32_BShiftRegDp_u0__A0_REG )
#define ShiftReg_DelaySenseIR_SHIFT_REG_VALUE_LSB_PTR  ((reg32 *) \
    ShiftReg_DelaySenseIR_bSR_sC32_BShiftRegDp_u0__A1_REG )
#define ShiftReg_DelaySenseIR_OUT_FIFO_VAL_LSB_PTR     ((reg32 *) \
    ShiftReg_DelaySenseIR_bSR_sC32_BShiftRegDp_u0__F1_REG )

/***************************************
*       Register Constants
***************************************/

#define ShiftReg_DelaySenseIR_INTERRUPTS_ENABLE        (0x10u)
#define ShiftReg_DelaySenseIR_LOAD_INT_EN              (0x01u)
#define ShiftReg_DelaySenseIR_STORE_INT_EN             (0x02u)
#define ShiftReg_DelaySenseIR_RESET_INT_EN             (0x04u)
#define ShiftReg_DelaySenseIR_CLK_EN                   (0x01u)

#define ShiftReg_DelaySenseIR_RESET_INT_EN_MASK        (0xFBu)
#define ShiftReg_DelaySenseIR_LOAD_INT_EN_MASK         (0xFEu)
#define ShiftReg_DelaySenseIR_STORE_INT_EN_MASK        (0xFDu)
#define ShiftReg_DelaySenseIR_INTS_EN_MASK             (0x07u)

#define ShiftReg_DelaySenseIR_OUT_FIFO_CLR_BIT         (0x02u)

#if (ShiftReg_DelaySenseIR_USE_INPUT_FIFO == 1u)

    #define ShiftReg_DelaySenseIR_IN_FIFO_MASK         (0x18u)

    #define ShiftReg_DelaySenseIR_IN_FIFO_FULL         (0x00u)
    #define ShiftReg_DelaySenseIR_IN_FIFO_EMPTY        (0x01u)
    #define ShiftReg_DelaySenseIR_IN_FIFO_NOT_EMPTY    (0x02u)
    
#endif/*(ShiftReg_DelaySenseIR_USE_INPUT_FIFO == 1u)*/

#define ShiftReg_DelaySenseIR_OUT_FIFO_MASK            (0x60u)

#define ShiftReg_DelaySenseIR_OUT_FIFO_EMPTY           (0x00u)
#define ShiftReg_DelaySenseIR_OUT_FIFO_FULL            (0x01u)
#define ShiftReg_DelaySenseIR_OUT_FIFO_NOT_EMPTY       (0x02u)
#define ShiftReg_DelaySenseIR_IN_FIFO_SHFT_MASK        (0x03u)
#define ShiftReg_DelaySenseIR_OUT_FIFO_SHFT_MASK       (0x05u)

#endif /* CY_SHIFTREG_ShiftReg_DelaySenseIR_H */

/* [] END OF FILE */
