/*******************************************************************************
* File Name: PrISM_7.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the PrISM
*  Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_PrISM_PrISM_7_H)    /* CY_PrISM_PrISM_7_H */
#define CY_PrISM_PrISM_7_H

#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PrISM_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*    Initial Parameter Constants
***************************************/
                                     
#define PrISM_7_RESOLUTION             (8u)
#define PrISM_7_PULSE_TYPE_HARDCODED   (1u)


/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct _PrISM_7_backupStruct
{
    uint8 enableState;
    #if(!PrISM_7_PULSE_TYPE_HARDCODED)
        uint8 cr;
    #endif /* End PrISM_7_PULSE_TYPE_HARDCODED */
    uint8 seed;
    uint8 seed_copy;
    uint8 polynom;
    #if(CY_UDB_V0)
        uint8 density0;
        uint8 density1;
    #endif /* End CY_UDB_V0 */
} PrISM_7_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void PrISM_7_Start(void) ;
void PrISM_7_Stop(void) ;
void PrISM_7_SetPulse0Mode(uint8 pulse0Type) ;
void PrISM_7_SetPulse1Mode(uint8 pulse1Type) ;
uint8 PrISM_7_ReadSeed(void) ;
void PrISM_7_WriteSeed(uint8 seed) ;
uint8 PrISM_7_ReadPolynomial(void) ;
void PrISM_7_WritePolynomial(uint8 polynomial) \
                                                                ;
uint8 PrISM_7_ReadPulse0(void) ;
void PrISM_7_WritePulse0(uint8 pulseDensity0) \
                                                                ;
uint8 PrISM_7_ReadPulse1(void) ;
void PrISM_7_WritePulse1(uint8 pulseDensity1) \
                                                                ;
void PrISM_7_Sleep(void) ;
void PrISM_7_Wakeup(void) ;
void PrISM_7_SaveConfig(void) ;
void PrISM_7_RestoreConfig(void) ;
void PrISM_7_Enable(void) ;
void PrISM_7_Init(void) ;


/***************************************
*          API Constants
***************************************/

/* Constants for SetPulse0Mode(), SetPulse1Mode(), pulse type */
#define PrISM_7_LESSTHAN_OR_EQUAL      (0x00u)
#define PrISM_7_GREATERTHAN_OR_EQUAL   (0x01u)


/***************************************
*    Initial Parameter Constants
***************************************/

#define PrISM_7_POLYNOM                (0xB8u)
#define PrISM_7_SEED                   (0xFFu)
#define PrISM_7_DENSITY0               (0x1u)
#define PrISM_7_DENSITY1               (0x1u)


/***************************************
*              Registers
***************************************/

#if (PrISM_7_RESOLUTION  <= 8u) /* 8bit - PrISM */
    #define PrISM_7_DENSITY0_REG       (* (reg8 *) PrISM_7_sC8_PrISMdp_u0__D0_REG)
    #define PrISM_7_DENSITY0_PTR       (  (reg8 *) PrISM_7_sC8_PrISMdp_u0__D0_REG)
    #define PrISM_7_DENSITY1_REG       (* (reg8 *) PrISM_7_sC8_PrISMdp_u0__D1_REG)
    #define PrISM_7_DENSITY1_PTR       (  (reg8 *) PrISM_7_sC8_PrISMdp_u0__D1_REG)
    #define PrISM_7_POLYNOM_REG        (* (reg8 *) PrISM_7_sC8_PrISMdp_u0__A1_REG)
    #define PrISM_7_POLYNOM_PTR        (  (reg8 *) PrISM_7_sC8_PrISMdp_u0__A1_REG)
    #define PrISM_7_SEED_REG           (* (reg8 *) PrISM_7_sC8_PrISMdp_u0__A0_REG)
    #define PrISM_7_SEED_PTR           (  (reg8 *) PrISM_7_sC8_PrISMdp_u0__A0_REG)
    #define PrISM_7_SEED_COPY_REG      (* (reg8 *) PrISM_7_sC8_PrISMdp_u0__F0_REG)
    #define PrISM_7_SEED_COPY_PTR      (  (reg8 *) PrISM_7_sC8_PrISMdp_u0__F0_REG)
    #define PrISM_7_AUX_CONTROL_REG    (* (reg8 *) PrISM_7_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PrISM_7_AUX_CONTROL_PTR    (  (reg8 *) PrISM_7_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
#elif (PrISM_7_RESOLUTION <= 16u) /* 16bit - PrISM */
    #define PrISM_7_DENSITY0_PTR       ((reg16 *) PrISM_7_sC16_PrISMdp_u0__D0_REG)
    #define PrISM_7_DENSITY1_PTR       ((reg16 *) PrISM_7_sC16_PrISMdp_u0__D1_REG)
    #define PrISM_7_POLYNOM_PTR        ((reg16 *) PrISM_7_sC16_PrISMdp_u0__A1_REG)
    #define PrISM_7_SEED_PTR           ((reg16 *) PrISM_7_sC16_PrISMdp_u0__A0_REG)
    #define PrISM_7_SEED_COPY_PTR      ((reg16 *) PrISM_7_sC16_PrISMdp_u0__F0_REG)
    #define PrISM_7_AUX_CONTROL_PTR    ((reg16 *) PrISM_7_sC16_PrISMdp_u0__DP_AUX_CTL_REG)
#elif (PrISM_7_RESOLUTION <= 24u) /* 24bit - PrISM */
    #define PrISM_7_DENSITY0_PTR       ((reg32 *) PrISM_7_sC24_PrISMdp_u0__D0_REG)
    #define PrISM_7_DENSITY1_PTR       ((reg32 *) PrISM_7_sC24_PrISMdp_u0__D1_REG)
    #define PrISM_7_POLYNOM_PTR        ((reg32 *) PrISM_7_sC24_PrISMdp_u0__A1_REG)
    #define PrISM_7_SEED_PTR           ((reg32 *) PrISM_7_sC24_PrISMdp_u0__A0_REG)
    #define PrISM_7_SEED_COPY_PTR      ((reg32 *) PrISM_7_sC24_PrISMdp_u0__F0_REG)
    #define PrISM_7_AUX_CONTROL_PTR    ((reg32 *) PrISM_7_sC24_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PrISM_7_AUX_CONTROL2_PTR   ((reg32 *) PrISM_7_sC24_PrISMdp_u2__DP_AUX_CTL_REG)
#else /* 32bit - PrISM */
    #define PrISM_7_DENSITY0_PTR       ((reg32 *) PrISM_7_sC32_PrISMdp_u0__D0_REG)
    #define PrISM_7_DENSITY1_PTR       ((reg32 *) PrISM_7_sC32_PrISMdp_u0__D1_REG)
    #define PrISM_7_POLYNOM_PTR        ((reg32 *) PrISM_7_sC32_PrISMdp_u0__A1_REG)
    #define PrISM_7_SEED_PTR           ((reg32 *) PrISM_7_sC32_PrISMdp_u0__A0_REG)
    #define PrISM_7_SEED_COPY_PTR      ((reg32 *) PrISM_7_sC32_PrISMdp_u0__F0_REG)
    #define PrISM_7_AUX_CONTROL_PTR    ((reg32 *) PrISM_7_sC32_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PrISM_7_AUX_CONTROL2_PTR   ((reg32 *) PrISM_7_sC32_PrISMdp_u2__DP_AUX_CTL_REG)
#endif /* End PrISM_7_RESOLUTION */

#define PrISM_7_CONTROL_REG   (* (reg8 *) PrISM_7_AsyncCtl_ControlReg__CONTROL_REG)
#define PrISM_7_CONTROL_PTR   (  (reg8 *) PrISM_7_AsyncCtl_ControlReg__CONTROL_REG)


/***************************************
*       Register Constants
***************************************/

#define PrISM_7_CTRL_ENABLE                                (0x01u)
#define PrISM_7_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL   (0x02u)
#define PrISM_7_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL   (0x04u)

#define PrISM_7_FIFO0_CLR                                  (0x01u)


/***************************************
* Renamed global variables or defines 
* for backward compatible
***************************************/
#define PrISM_7_ReadPusle0     PrISM_7_ReadPulse0
#define PrISM_7_ReadPusle1     PrISM_7_ReadPulse1


#endif  /* End CY_PrISM_PrISM_7_H */


/* [] END OF FILE */
