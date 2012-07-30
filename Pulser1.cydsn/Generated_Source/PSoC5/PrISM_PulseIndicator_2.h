/*******************************************************************************
* File Name: PrISM_PulseIndicator_2.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the PrISM
*  Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_PrISM_PrISM_PulseIndicator_2_H)    /* CY_PrISM_PrISM_PulseIndicator_2_H */
#define CY_PrISM_PrISM_PulseIndicator_2_H

#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* PSoC3 ES2 or early */
#define PrISM_PulseIndicator_2_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A) && \
                                     (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))

/* PSoC5 ES1 or early */
#define PrISM_PulseIndicator_2_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A) && \
                                     (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define PrISM_PulseIndicator_2_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A) && \
                                     (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))

/* PSoC5 ES2 or later */
#define PrISM_PulseIndicator_2_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A) && \
                                     (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))
                                     
#define PrISM_PulseIndicator_2_RESOLUTION             (8u)
#define PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED   (1u)


/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct _PrISM_PulseIndicator_2_backupStruct
{
    uint8 enableState;
    #if(!PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED)
        uint8 cr;
    #endif /*End PrISM_PulseIndicator_2_PULSE_TYPE_HARDCODED*/
    uint8 seed;
    uint8 seed_copy;
    uint8 polynom;
    #if(PrISM_PulseIndicator_2_PSOC3_ES2 || PrISM_PulseIndicator_2_PSOC5_ES1) /* PSoC3 ES2 or early, PSoC5 ES1*/
        uint8 density0;
        uint8 density1;
    #endif /*End PrISM_PulseIndicator_2_PSOC3_ES2 || PrISM_PulseIndicator_2_PSOC5_ES1*/
} PrISM_PulseIndicator_2_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void PrISM_PulseIndicator_2_Start(void);
void PrISM_PulseIndicator_2_Stop(void) ;
void PrISM_PulseIndicator_2_SetPulse0Mode(uint8 pulse0Type) ;
void PrISM_PulseIndicator_2_SetPulse1Mode(uint8 pulse1Type) ;
uint8 PrISM_PulseIndicator_2_ReadSeed(void) ;
void PrISM_PulseIndicator_2_WriteSeed(uint8 seed) ;
uint8 PrISM_PulseIndicator_2_ReadPolynomial(void) ;
void PrISM_PulseIndicator_2_WritePolynomial(uint8 polynomial) \
                                                                ;
uint8 PrISM_PulseIndicator_2_ReadPulse0(void) ;
void PrISM_PulseIndicator_2_WritePulse0(uint8 pulseDensity0) \
                                                                ;
uint8 PrISM_PulseIndicator_2_ReadPulse1(void) ;
void PrISM_PulseIndicator_2_WritePulse1(uint8 pulseDensity1) \
                                                                ;
void PrISM_PulseIndicator_2_Sleep(void);
void PrISM_PulseIndicator_2_Wakeup(void) ;
void PrISM_PulseIndicator_2_SaveConfig(void);
void PrISM_PulseIndicator_2_RestoreConfig(void) ;
void PrISM_PulseIndicator_2_Enable(void) ;
void PrISM_PulseIndicator_2_Init(void) ;


/***************************************
*          API Constants
***************************************/

/* Constants for SetPulse0Mode(), SetPulse1Mode(), pulse type */
#define PrISM_PulseIndicator_2_LESSTHAN_OR_EQUAL      (0x00u)
#define PrISM_PulseIndicator_2_GREATERTHAN_OR_EQUAL   (0x01u)


/***************************************
*    Initial Parameter Constants
***************************************/

#define PrISM_PulseIndicator_2_POLYNOM                (0xB8u)
#define PrISM_PulseIndicator_2_SEED                   (0xFFu)
#define PrISM_PulseIndicator_2_DENSITY0               (0x1u)
#define PrISM_PulseIndicator_2_DENSITY1               (0x1u)


/***************************************
*              Registers
***************************************/

#if (PrISM_PulseIndicator_2_RESOLUTION  <= 8u) /* 8bit - PrISM */
    #define PrISM_PulseIndicator_2_DENSITY0_REG       (* (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__D0_REG)
    #define PrISM_PulseIndicator_2_DENSITY0_PTR       (  (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__D0_REG)
    #define PrISM_PulseIndicator_2_DENSITY1_REG       (* (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__D1_REG)
    #define PrISM_PulseIndicator_2_DENSITY1_PTR       (  (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__D1_REG)
    #define PrISM_PulseIndicator_2_POLYNOM_REG        (* (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__A1_REG)
    #define PrISM_PulseIndicator_2_POLYNOM_PTR        (  (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__A1_REG)
    #define PrISM_PulseIndicator_2_SEED_REG           (* (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__A0_REG)
    #define PrISM_PulseIndicator_2_SEED_PTR           (  (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__A0_REG)
    #define PrISM_PulseIndicator_2_SEED_COPY_REG      (* (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__F0_REG)
    #define PrISM_PulseIndicator_2_SEED_COPY_PTR      (  (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__F0_REG)
    #define PrISM_PulseIndicator_2_AUX_CONTROL_REG    (* (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PrISM_PulseIndicator_2_AUX_CONTROL_PTR    (  (reg8 *) PrISM_PulseIndicator_2_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
#elif (PrISM_PulseIndicator_2_RESOLUTION <= 16u) /* 16bit - PrISM */
    #define PrISM_PulseIndicator_2_DENSITY0_PTR       ((reg16 *) PrISM_PulseIndicator_2_sC16_PrISMdp_u0__D0_REG)
    #define PrISM_PulseIndicator_2_DENSITY1_PTR       ((reg16 *) PrISM_PulseIndicator_2_sC16_PrISMdp_u0__D1_REG)
    #define PrISM_PulseIndicator_2_POLYNOM_PTR        ((reg16 *) PrISM_PulseIndicator_2_sC16_PrISMdp_u0__A1_REG)
    #define PrISM_PulseIndicator_2_SEED_PTR           ((reg16 *) PrISM_PulseIndicator_2_sC16_PrISMdp_u0__A0_REG)
    #define PrISM_PulseIndicator_2_SEED_COPY_PTR      ((reg16 *) PrISM_PulseIndicator_2_sC16_PrISMdp_u0__F0_REG)
    #define PrISM_PulseIndicator_2_AUX_CONTROL_PTR    ((reg16 *) PrISM_PulseIndicator_2_sC16_PrISMdp_u0__DP_AUX_CTL_REG)
#elif (PrISM_PulseIndicator_2_RESOLUTION <= 24u) /* 24bit - PrISM */
    #define PrISM_PulseIndicator_2_DENSITY0_PTR       ((reg32 *) PrISM_PulseIndicator_2_sC24_PrISMdp_u0__D0_REG)
    #define PrISM_PulseIndicator_2_DENSITY1_PTR       ((reg32 *) PrISM_PulseIndicator_2_sC24_PrISMdp_u0__D1_REG)
    #define PrISM_PulseIndicator_2_POLYNOM_PTR        ((reg32 *) PrISM_PulseIndicator_2_sC24_PrISMdp_u0__A1_REG)
    #define PrISM_PulseIndicator_2_SEED_PTR           ((reg32 *) PrISM_PulseIndicator_2_sC24_PrISMdp_u0__A0_REG)
    #define PrISM_PulseIndicator_2_SEED_COPY_PTR      ((reg32 *) PrISM_PulseIndicator_2_sC24_PrISMdp_u0__F0_REG)
    #define PrISM_PulseIndicator_2_AUX_CONTROL_PTR    ((reg32 *) PrISM_PulseIndicator_2_sC24_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PrISM_PulseIndicator_2_AUX_CONTROL2_PTR   ((reg32 *) PrISM_PulseIndicator_2_sC24_PrISMdp_u2__DP_AUX_CTL_REG)
#else /* 32bit - PrISM */
    #define PrISM_PulseIndicator_2_DENSITY0_PTR       ((reg32 *) PrISM_PulseIndicator_2_sC32_PrISMdp_u0__D0_REG)
    #define PrISM_PulseIndicator_2_DENSITY1_PTR       ((reg32 *) PrISM_PulseIndicator_2_sC32_PrISMdp_u0__D1_REG)
    #define PrISM_PulseIndicator_2_POLYNOM_PTR        ((reg32 *) PrISM_PulseIndicator_2_sC32_PrISMdp_u0__A1_REG)
    #define PrISM_PulseIndicator_2_SEED_PTR           ((reg32 *) PrISM_PulseIndicator_2_sC32_PrISMdp_u0__A0_REG)
    #define PrISM_PulseIndicator_2_SEED_COPY_PTR      ((reg32 *) PrISM_PulseIndicator_2_sC32_PrISMdp_u0__F0_REG)
    #define PrISM_PulseIndicator_2_AUX_CONTROL_PTR    ((reg32 *) PrISM_PulseIndicator_2_sC32_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PrISM_PulseIndicator_2_AUX_CONTROL2_PTR   ((reg32 *) PrISM_PulseIndicator_2_sC32_PrISMdp_u2__DP_AUX_CTL_REG)
#endif /* End PrISM_PulseIndicator_2_RESOLUTION */

#define PrISM_PulseIndicator_2_CONTROL_REG   (* (reg8 *) PrISM_PulseIndicator_2_AsyncCtl_ControlReg__CONTROL_REG)
#define PrISM_PulseIndicator_2_CONTROL_PTR   (  (reg8 *) PrISM_PulseIndicator_2_AsyncCtl_ControlReg__CONTROL_REG) 

/***************************************
*       Register Constants
***************************************/

#define PrISM_PulseIndicator_2_CTRL_ENABLE                                (0x01u)
#define PrISM_PulseIndicator_2_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL   (0x02u)
#define PrISM_PulseIndicator_2_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL   (0x04u)

#define PrISM_PulseIndicator_2_FIFO0_CLR                                  (0x01u)

/***************************************
* Renamed global variables or defines 
* for backward compatible
***************************************/
#define PrISM_PulseIndicator_2_ReadPusle0     PrISM_PulseIndicator_2_ReadPulse0
#define PrISM_PulseIndicator_2_ReadPusle1     PrISM_PulseIndicator_2_ReadPulse1


#endif  /* End CY_PrISM_PrISM_PulseIndicator_2_H */


/* [] END OF FILE */
