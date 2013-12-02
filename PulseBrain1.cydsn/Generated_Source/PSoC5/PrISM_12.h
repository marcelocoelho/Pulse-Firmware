/*******************************************************************************
* File Name: PrISM_12.h
* Version 2.20
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


#if !defined(CY_PrISM_PrISM_12_H)    /* CY_PrISM_PrISM_12_H */
#define CY_PrISM_PrISM_12_H

#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PrISM_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*    Initial Parameter Constants
***************************************/
                                     
#define PrISM_12_RESOLUTION             (8u)
#define PrISM_12_PULSE_TYPE_HARDCODED   (1u)


/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    #if(!PrISM_12_PULSE_TYPE_HARDCODED)
        uint8 cr;
    #endif /* End PrISM_12_PULSE_TYPE_HARDCODED */
    uint8 seed;
    uint8 seed_copy;
    uint8 polynom;
    #if(CY_UDB_V0)
        uint8 density0;
        uint8 density1;
    #endif /* End CY_UDB_V0 */
} PrISM_12_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void PrISM_12_Start(void) ;
void PrISM_12_Stop(void) ;
void PrISM_12_SetPulse0Mode(uint8 pulse0Type) ;
void PrISM_12_SetPulse1Mode(uint8 pulse1Type) ;
uint8 PrISM_12_ReadSeed(void) ;
void PrISM_12_WriteSeed(uint8 seed) ;
uint8 PrISM_12_ReadPolynomial(void) ;
void PrISM_12_WritePolynomial(uint8 polynomial) \
                                                                ;
uint8 PrISM_12_ReadPulse0(void) ;
void PrISM_12_WritePulse0(uint8 pulseDensity0) \
                                                                ;
uint8 PrISM_12_ReadPulse1(void) ;
void PrISM_12_WritePulse1(uint8 pulseDensity1) \
                                                                ;
void PrISM_12_Sleep(void) ;
void PrISM_12_Wakeup(void) ;
void PrISM_12_SaveConfig(void) ;
void PrISM_12_RestoreConfig(void) ;
void PrISM_12_Enable(void) ;
void PrISM_12_Init(void) ;


/***************************************
*          API Constants
***************************************/

/* Constants for SetPulse0Mode(), SetPulse1Mode(), pulse type */
#define PrISM_12_LESSTHAN_OR_EQUAL      (0x00u)
#define PrISM_12_GREATERTHAN_OR_EQUAL   (0x01u)


/***************************************
* Global variables external identifier
***************************************/

extern uint8 PrISM_12_initVar;


/***************************************
*    Initial Parameter Constants
***************************************/

#define PrISM_12_POLYNOM                (0xB8u)
#define PrISM_12_SEED                   (0xFFu)
#define PrISM_12_DENSITY0               (0x1u)
#define PrISM_12_DENSITY1               (0x1u)
#define PrISM_12_COMPARE0               (0u)
#define PrISM_12_COMPARE1               (0u)


/***************************************
*              Registers
***************************************/

#if (PrISM_12_RESOLUTION  <= 8u) /* 8bit - PrISM */
    #define PrISM_12_DENSITY0_REG       (* (reg8 *) PrISM_12_sC8_PrISMdp_u0__D0_REG)
    #define PrISM_12_DENSITY0_PTR       (  (reg8 *) PrISM_12_sC8_PrISMdp_u0__D0_REG)
    #define PrISM_12_DENSITY1_REG       (* (reg8 *) PrISM_12_sC8_PrISMdp_u0__D1_REG)
    #define PrISM_12_DENSITY1_PTR       (  (reg8 *) PrISM_12_sC8_PrISMdp_u0__D1_REG)
    #define PrISM_12_POLYNOM_REG        (* (reg8 *) PrISM_12_sC8_PrISMdp_u0__A1_REG)
    #define PrISM_12_POLYNOM_PTR        (  (reg8 *) PrISM_12_sC8_PrISMdp_u0__A1_REG)
    #define PrISM_12_SEED_REG           (* (reg8 *) PrISM_12_sC8_PrISMdp_u0__A0_REG)
    #define PrISM_12_SEED_PTR           (  (reg8 *) PrISM_12_sC8_PrISMdp_u0__A0_REG)
    #define PrISM_12_SEED_COPY_REG      (* (reg8 *) PrISM_12_sC8_PrISMdp_u0__F0_REG)
    #define PrISM_12_SEED_COPY_PTR      (  (reg8 *) PrISM_12_sC8_PrISMdp_u0__F0_REG)
    #define PrISM_12_AUX_CONTROL_REG    (* (reg8 *) PrISM_12_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PrISM_12_AUX_CONTROL_PTR    (  (reg8 *) PrISM_12_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
#elif (PrISM_12_RESOLUTION <= 16u) /* 16bit - PrISM */
    #if(CY_PSOC3) /* 8-bit address space */
        #define PrISM_12_DENSITY0_PTR       ((reg16 *) PrISM_12_sC16_PrISMdp_u0__D0_REG)
        #define PrISM_12_DENSITY1_PTR       ((reg16 *) PrISM_12_sC16_PrISMdp_u0__D1_REG)
        #define PrISM_12_POLYNOM_PTR        ((reg16 *) PrISM_12_sC16_PrISMdp_u0__A1_REG)
        #define PrISM_12_SEED_PTR           ((reg16 *) PrISM_12_sC16_PrISMdp_u0__A0_REG)
        #define PrISM_12_SEED_COPY_PTR      ((reg16 *) PrISM_12_sC16_PrISMdp_u0__F0_REG)
        #define PrISM_12_AUX_CONTROL_PTR    ((reg16 *) PrISM_12_sC16_PrISMdp_u0__DP_AUX_CTL_REG)
    #else /* 16-bit address space */
        #define PrISM_12_DENSITY0_PTR       ((reg16 *) PrISM_12_sC16_PrISMdp_u0__16BIT_D0_REG)
        #define PrISM_12_DENSITY1_PTR       ((reg16 *) PrISM_12_sC16_PrISMdp_u0__16BIT_D1_REG)
        #define PrISM_12_POLYNOM_PTR        ((reg16 *) PrISM_12_sC16_PrISMdp_u0__16BIT_A1_REG)
        #define PrISM_12_SEED_PTR           ((reg16 *) PrISM_12_sC16_PrISMdp_u0__16BIT_A0_REG)
        #define PrISM_12_SEED_COPY_PTR      ((reg16 *) PrISM_12_sC16_PrISMdp_u0__16BIT_F0_REG)
        #define PrISM_12_AUX_CONTROL_PTR    ((reg16 *) PrISM_12_sC16_PrISMdp_u0__16BIT_DP_AUX_CTL_REG)
    #endif /* CY_PSOC3 */
#elif (PrISM_12_RESOLUTION <= 24u) /* 24bit - PrISM */
    #define PrISM_12_DENSITY0_PTR       ((reg32 *) PrISM_12_sC24_PrISMdp_u0__D0_REG)
    #define PrISM_12_DENSITY1_PTR       ((reg32 *) PrISM_12_sC24_PrISMdp_u0__D1_REG)
    #define PrISM_12_POLYNOM_PTR        ((reg32 *) PrISM_12_sC24_PrISMdp_u0__A1_REG)
    #define PrISM_12_SEED_PTR           ((reg32 *) PrISM_12_sC24_PrISMdp_u0__A0_REG)
    #define PrISM_12_SEED_COPY_PTR      ((reg32 *) PrISM_12_sC24_PrISMdp_u0__F0_REG)
    #define PrISM_12_AUX_CONTROL_PTR    ((reg32 *) PrISM_12_sC24_PrISMdp_u0__DP_AUX_CTL_REG)
#else /* 32bit - PrISM */
    #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
        #define PrISM_12_DENSITY0_PTR       ((reg32 *) PrISM_12_sC32_PrISMdp_u0__D0_REG)
        #define PrISM_12_DENSITY1_PTR       ((reg32 *) PrISM_12_sC32_PrISMdp_u0__D1_REG)
        #define PrISM_12_POLYNOM_PTR        ((reg32 *) PrISM_12_sC32_PrISMdp_u0__A1_REG)
        #define PrISM_12_SEED_PTR           ((reg32 *) PrISM_12_sC32_PrISMdp_u0__A0_REG)
        #define PrISM_12_SEED_COPY_PTR      ((reg32 *) PrISM_12_sC32_PrISMdp_u0__F0_REG)
        #define PrISM_12_AUX_CONTROL_PTR    ((reg32 *) PrISM_12_sC32_PrISMdp_u0__DP_AUX_CTL_REG)
    #else /* 32-bit address space */    
        #define PrISM_12_DENSITY0_PTR       ((reg32 *) PrISM_12_sC32_PrISMdp_u0__32BIT_D0_REG)
        #define PrISM_12_DENSITY1_PTR       ((reg32 *) PrISM_12_sC32_PrISMdp_u0__32BIT_D1_REG)
        #define PrISM_12_POLYNOM_PTR        ((reg32 *) PrISM_12_sC32_PrISMdp_u0__32BIT_A1_REG)
        #define PrISM_12_SEED_PTR           ((reg32 *) PrISM_12_sC32_PrISMdp_u0__32BIT_A0_REG)
        #define PrISM_12_SEED_COPY_PTR      ((reg32 *) PrISM_12_sC32_PrISMdp_u0__32BIT_F0_REG)
        #define PrISM_12_AUX_CONTROL_PTR    ((reg32 *) PrISM_12_sC32_PrISMdp_u0__32BIT_DP_AUX_CTL_REG)
    #endif
#endif /* End PrISM_12_RESOLUTION */

#define PrISM_12_CONTROL_REG   (* (reg8 *) PrISM_12_AsyncCtl_ControlReg__CONTROL_REG)
#define PrISM_12_CONTROL_PTR   (  (reg8 *) PrISM_12_AsyncCtl_ControlReg__CONTROL_REG)


/***************************************
*       Register Constants
***************************************/

#define PrISM_12_CTRL_ENABLE                                (0x01u)
#define PrISM_12_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL   (0x02u)
#define PrISM_12_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL   (0x04u)

#if (PrISM_12_RESOLUTION  <= 8u) /* 8bit - PrISM */
    #define PrISM_12_FIFO0_CLR                              (0x01u)
#elif (PrISM_12_RESOLUTION <= 16u) /* 16bit - PrISM */
    #define PrISM_12_FIFO0_CLR                              (0x0101u)
#elif (PrISM_12_RESOLUTION <= 24u) /* 24bit - PrISM */
    #define PrISM_12_FIFO0_CLR                              (0x010101Lu)
#else /* 32bit - PrISM */
    #define PrISM_12_FIFO0_CLR                              (0x01010101Lu)
#endif /* End PrISM_12_RESOLUTION */


/***************************************
* Renamed global variables or defines 
* for backward compatible
***************************************/
#define PrISM_12_ReadPusle0     PrISM_12_ReadPulse0
#define PrISM_12_ReadPusle1     PrISM_12_ReadPulse1


#endif  /* End CY_PrISM_PrISM_12_H */


/* [] END OF FILE */
