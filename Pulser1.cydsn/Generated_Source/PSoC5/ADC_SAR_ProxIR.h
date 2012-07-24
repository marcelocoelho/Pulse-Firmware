/*******************************************************************************
* File Name: ADC_SAR_ProxIR.h
* Version 1.71
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Successive approximation ADC Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/



#if !defined(CY_ADC_SAR_ADC_SAR_ProxIR_H) /* CY_ADC_SAR_ADC_SAR_ProxIR_H */
#define CY_ADC_SAR_ADC_SAR_ProxIR_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC3 and CY_PSOC5 are available */
/* They are defined starting with cy_boot v2.30 */
#ifndef CY_PSOC5
    #error Component ADC_SAR_v1_71 requires cy_boot v2.30 or later
#endif /* End CY_PSOC5 */


/***************************************
*      Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct _ADC_SAR_ProxIR_BackupStruct
{
    uint8 enableState;
} ADC_SAR_ProxIR_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void ADC_SAR_ProxIR_Start(void);
void ADC_SAR_ProxIR_Stop(void);
void ADC_SAR_ProxIR_IRQ_Enable(void);
void ADC_SAR_ProxIR_IRQ_Disable(void);
void ADC_SAR_ProxIR_SetPower(uint8 power);
void ADC_SAR_ProxIR_SetResolution(uint8 resolution);
void ADC_SAR_ProxIR_StartConvert(void);
void ADC_SAR_ProxIR_StopConvert(void);
uint8 ADC_SAR_ProxIR_IsEndConversion(uint8 retMode);
int8 ADC_SAR_ProxIR_GetResult8(void);
int16 ADC_SAR_ProxIR_GetResult16(void);

void ADC_SAR_ProxIR_SetOffset(int16 offset);
void ADC_SAR_ProxIR_SetGain(int16 adcGain);
int16 ADC_SAR_ProxIR_CountsTo_mVolts(int16 adcCounts);
int32 ADC_SAR_ProxIR_CountsTo_uVolts(int16 adcCounts);
float ADC_SAR_ProxIR_CountsTo_Volts(int16 adcCounts);

void ADC_SAR_ProxIR_Init(void);
void ADC_SAR_ProxIR_Enable(void);

void ADC_SAR_ProxIR_SaveConfig(void);
void ADC_SAR_ProxIR_RestoreConfig(void);
void ADC_SAR_ProxIR_Sleep(void);
void ADC_SAR_ProxIR_Wakeup(void);

CY_ISR_PROTO( ADC_SAR_ProxIR_ISR );


/**************************************
*           API Constants
**************************************/

/* Resolution setting constants  */
#define ADC_SAR_ProxIR__BITS_12 12
#define ADC_SAR_ProxIR__BITS_10 10
#define ADC_SAR_ProxIR__BITS_8 8


/*   Constants for IsEndConversion() "retMode" parameter  */
#define ADC_SAR_ProxIR_RETURN_STATUS              (0x01u)
#define ADC_SAR_ProxIR_WAIT_FOR_RESULT            (0x00u)

/* Constants for SetPower(power), "power" paramter */
#define ADC_SAR_ProxIR__HIGHPOWER 0
#define ADC_SAR_ProxIR__MEDPOWER 1
#define ADC_SAR_ProxIR__LOWPOWER 2
#define ADC_SAR_ProxIR__MINPOWER 3

#define ADC_SAR_ProxIR_SAR_API_POWER_MASK         (0x03u)

/* Constants for Sleep mode states */
#define ADC_SAR_ProxIR_STARTED                    (0x02u)
#define ADC_SAR_ProxIR_ENABLED                    (0x01u)
#define ADC_SAR_ProxIR_DISABLED                   (0x00u)

#define ADC_SAR_ProxIR_MAX_FREQUENCY              (14000000u)       /*14Mhz*/

/**************************************
*    Enumerated Types and Parameters
**************************************/

/*  Reference constants */
#define ADC_SAR_ProxIR__INT_REF_NOT_BYPASSED 0
#define ADC_SAR_ProxIR__INT_REF_BYPASS 1
#define ADC_SAR_ProxIR__EXT_REF 2

/*  Input Range setting constants */
#define ADC_SAR_ProxIR__VSS_TO_VREF 0
#define ADC_SAR_ProxIR__VSSA_TO_VDDA 1
#define ADC_SAR_ProxIR__VSSA_TO_VDAC 2
#define ADC_SAR_ProxIR__VNEG_VREF_DIFF 3
#define ADC_SAR_ProxIR__VNEG_VDDA_DIFF 4
#define ADC_SAR_ProxIR__VNEG_VDDA_2_DIFF 5
#define ADC_SAR_ProxIR__VNEG_VDAC_DIFF 6

/*  Sample Mode setting constants */
#define ADC_SAR_ProxIR__FREERUNNING 0
#define ADC_SAR_ProxIR__TRIGGERED 1

/*  Clock Source setting constants */
#define ADC_SAR_ProxIR__INTERNAL 1
#define ADC_SAR_ProxIR__EXTERNAL 0



/**************************************
*    Initial Parameter Constants
**************************************/

/* Default config values from user parameters */
#define ADC_SAR_ProxIR_DEFAULT_RESOLUTION     (12)   /* ADC resolution selected with parameters.*/
#define ADC_SAR_ProxIR_DEFAULT_CONV_MODE      (0)   /* Default conversion method */
#define ADC_SAR_ProxIR_DEFAULT_INTERNAL_CLK   (1)        /* Default clock selection */
#define ADC_SAR_ProxIR_DEFAULT_REFERENCE      (0)    /* Default reference */
#define ADC_SAR_ProxIR_DEFAULT_RANGE          (1)  /* ADC Input Range selection. */

/* Use VDDA voltage define directly from cyfitter.h when VDDA reference has been used */
#define ADC_SAR_ProxIR_DEFAULT_REF_VOLTAGE (((ADC_SAR_ProxIR_DEFAULT_REFERENCE != ADC_SAR_ProxIR__EXT_REF) && \
                                        ((ADC_SAR_ProxIR_DEFAULT_RANGE == ADC_SAR_ProxIR__VSSA_TO_VDDA) || \
                                        (ADC_SAR_ProxIR_DEFAULT_RANGE == ADC_SAR_ProxIR__VNEG_VDDA_2_DIFF))) ? \
                                        (CYDEV_VDDA / 2) : \
                                        ((ADC_SAR_ProxIR_DEFAULT_REFERENCE != ADC_SAR_ProxIR__EXT_REF) && \
                                        (ADC_SAR_ProxIR_DEFAULT_RANGE == ADC_SAR_ProxIR__VNEG_VDDA_2_DIFF)) ? \
                                        CYDEV_VDDA : (2.5))      /* ADC reference voltage. */
#define ADC_SAR_ProxIR_DEFAULT_REF_VOLTAGE_MV (((ADC_SAR_ProxIR_DEFAULT_REFERENCE != ADC_SAR_ProxIR__EXT_REF) &&\
                                        ((ADC_SAR_ProxIR_DEFAULT_RANGE == ADC_SAR_ProxIR__VSSA_TO_VDDA) || \
                                        (ADC_SAR_ProxIR_DEFAULT_RANGE == ADC_SAR_ProxIR__VNEG_VDDA_2_DIFF))) ? \
                                        (CYDEV_VDDA_MV / 2) : \
                                        ((ADC_SAR_ProxIR_DEFAULT_REFERENCE != ADC_SAR_ProxIR__EXT_REF) && \
                                        (ADC_SAR_ProxIR_DEFAULT_RANGE == ADC_SAR_ProxIR__VNEG_VDDA_2_DIFF)) ? \
                                        CYDEV_VDDA_MV : (2500))   /* ADC reference voltage in mV */
/* The power is set to normal power, ½, 1/3, ¼ power depending on the clock setting. */
#define ADC_SAR_ProxIR_DEFAULT_POWER  ((1 != ADC_SAR_ProxIR__INTERNAL) ? ADC_SAR_ProxIR__HIGHPOWER : \
                      (1900000 > (ADC_SAR_ProxIR_MAX_FREQUENCY / 2)) ? ADC_SAR_ProxIR__HIGHPOWER : \
                      (1900000 > (ADC_SAR_ProxIR_MAX_FREQUENCY / 3)) ? ADC_SAR_ProxIR__MEDPOWER : \
                      (1900000 > (ADC_SAR_ProxIR_MAX_FREQUENCY / 4)) ? ADC_SAR_ProxIR__LOWPOWER : \
                                                                                        ADC_SAR_ProxIR__MINPOWER)
/* Constant for a global usage */
#define ADC_SAR_ProxIR_SAMPLE_PRECHARGE       (7) /* Number of additional clocks for sampling data*/


/***************************************
*              Registers
***************************************/

#define ADC_SAR_ProxIR_SAR_TR0_REG                (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__TR0 )
#define ADC_SAR_ProxIR_SAR_TR0_PTR                (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__TR0 )
#define ADC_SAR_ProxIR_SAR_CSR0_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR0 )
#define ADC_SAR_ProxIR_SAR_CSR0_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR0 )
#define ADC_SAR_ProxIR_SAR_CSR1_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR1 )
#define ADC_SAR_ProxIR_SAR_CSR1_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR1 )
#define ADC_SAR_ProxIR_SAR_CSR2_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR2 )
#define ADC_SAR_ProxIR_SAR_CSR2_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR2 )
#define ADC_SAR_ProxIR_SAR_CSR3_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR3 )
#define ADC_SAR_ProxIR_SAR_CSR3_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR3 )
#define ADC_SAR_ProxIR_SAR_CSR4_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR4 )
#define ADC_SAR_ProxIR_SAR_CSR4_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR4 )
#define ADC_SAR_ProxIR_SAR_CSR5_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR5 )
#define ADC_SAR_ProxIR_SAR_CSR5_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR5 )
#define ADC_SAR_ProxIR_SAR_CSR6_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR6 )
#define ADC_SAR_ProxIR_SAR_CSR6_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR6 )
#define ADC_SAR_ProxIR_SAR_CSR7_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR7 )
#define ADC_SAR_ProxIR_SAR_CSR7_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CSR7 )
#define ADC_SAR_ProxIR_SAR_SW0_REG                (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW0 )
#define ADC_SAR_ProxIR_SAR_SW0_PTR                (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW0 )
#define ADC_SAR_ProxIR_SAR_SW2_REG                (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW2 )
#define ADC_SAR_ProxIR_SAR_SW2_PTR                (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW2 )
#define ADC_SAR_ProxIR_SAR_SW3_REG                (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW3 )
#define ADC_SAR_ProxIR_SAR_SW3_PTR                (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW3 )
#define ADC_SAR_ProxIR_SAR_SW4_REG                (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW4 )
#define ADC_SAR_ProxIR_SAR_SW4_PTR                (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW4 )
#define ADC_SAR_ProxIR_SAR_SW6_REG                (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW6 )
#define ADC_SAR_ProxIR_SAR_SW6_PTR                (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__SW6 )
#define ADC_SAR_ProxIR_SAR_CLK_REG                (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CLK )
#define ADC_SAR_ProxIR_SAR_CLK_PTR                (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__CLK )
#define ADC_SAR_ProxIR_SAR_WRK0_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__WRK0 )
#define ADC_SAR_ProxIR_SAR_WRK0_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__WRK0 )
#define ADC_SAR_ProxIR_SAR_WRK1_REG               (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__WRK1 )
#define ADC_SAR_ProxIR_SAR_WRK1_PTR               (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__WRK1 )
#define ADC_SAR_ProxIR_PWRMGR_SAR_REG             (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__PM_ACT_CFG )     
#define ADC_SAR_ProxIR_PWRMGR_SAR_PTR             (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__PM_ACT_CFG )    
#define ADC_SAR_ProxIR_STBY_PWRMGR_SAR_REG        (* (reg8 *) ADC_SAR_ProxIR_ADC_SAR__PM_STBY_CFG )     
#define ADC_SAR_ProxIR_STBY_PWRMGR_SAR_PTR        (  (reg8 *) ADC_SAR_ProxIR_ADC_SAR__PM_STBY_CFG )    

/* renamed registers for backward compatible */
#define ADC_SAR_ProxIR_SAR_WRK0                   ADC_SAR_ProxIR_SAR_WRK0_REG
#define ADC_SAR_ProxIR_SAR_WRK1                   ADC_SAR_ProxIR_SAR_WRK1_REG

/* This is only valid if there is an internal clock */
#if(ADC_SAR_ProxIR_DEFAULT_INTERNAL_CLK)
    /* Clock Power manager Reg */
    #define ADC_SAR_ProxIR_PWRMGR_CLK_REG         (* (reg8 *) ADC_SAR_ProxIR_theACLK__PM_ACT_CFG )
    #define ADC_SAR_ProxIR_PWRMGR_CLK_PTR         (  (reg8 *) ADC_SAR_ProxIR_theACLK__PM_ACT_CFG )
    #define ADC_SAR_ProxIR_STBY_PWRMGR_CLK_REG    (* (reg8 *) ADC_SAR_ProxIR_theACLK__PM_STBY_CFG )
    #define ADC_SAR_ProxIR_STBY_PWRMGR_CLK_PTR    (  (reg8 *) ADC_SAR_ProxIR_theACLK__PM_STBY_CFG )
#endif /*End ADC_SAR_ProxIR_DEFAULT_INTERNAL_CLK */


/**************************************
*       Register Constants
**************************************/
/* PM_ACT_CFG (Active Power Mode CFG Register) Power enable mask */
#define ADC_SAR_ProxIR_ACT_PWR_SAR_EN             ADC_SAR_ProxIR_ADC_SAR__PM_ACT_MSK 

/* PM_STBY_CFG (Alternate Active Power Mode CFG Register) Power enable mask */
#define ADC_SAR_ProxIR_STBY_PWR_SAR_EN            ADC_SAR_ProxIR_ADC_SAR__PM_STBY_MSK 

/* This is only valid if there is an internal clock */
#if(ADC_SAR_ProxIR_DEFAULT_INTERNAL_CLK)
    /* Power enable mask */
    #define ADC_SAR_ProxIR_ACT_PWR_CLK_EN         ADC_SAR_ProxIR_theACLK__PM_ACT_MSK
    /* Standby power enable mask */
    #define ADC_SAR_ProxIR_STBY_PWR_CLK_EN        ADC_SAR_ProxIR_theACLK__PM_STBY_MSK
#endif /*End ADC_SAR_ProxIR_DEFAULT_INTERNAL_CLK */

/* Priority of the ADC_SAR_IRQ interrupt. */
#define ADC_SAR_ProxIR_INTC_PRIOR_NUMBER          ADC_SAR_ProxIR_IRQ__INTC_PRIOR_NUM

/* ADC_SAR_IRQ interrupt number */
#define ADC_SAR_ProxIR_INTC_NUMBER                ADC_SAR_ProxIR_IRQ__INTC_NUMBER
/******************************************/
/* SAR.TR0 Trim Register 0 definitions    */
/******************************************/

/* Bit Field  SAR_CAP_TRIM_ENUM */
#define ADC_SAR_ProxIR_SAR_CAP_TRIM_MASK          (0x07u)
#define ADC_SAR_ProxIR_SAR_CAP_TRIM_0             (0x00u)    /*decrease attenuation capacitor by 0*/
#define ADC_SAR_ProxIR_SAR_CAP_TRIM_1             (0x01u)    /*decrease by 0.5 LSB*/
#define ADC_SAR_ProxIR_SAR_CAP_TRIM_2             (0x02u)    /*decrease by 2*0.5 LSB = 1 LSB*/
#define ADC_SAR_ProxIR_SAR_CAP_TRIM_3             (0x03u)    /*decrease by 3*0.5 LSB = 1.5 LSB*/
#define ADC_SAR_ProxIR_SAR_CAP_TRIM_4             (0x04u)    /*decrease by 4*0.5 LSB = 2 LSB*/
#define ADC_SAR_ProxIR_SAR_CAP_TRIM_5             (0x05u)    /*decrease by 5*0.5 LSB = 2.5 LSB*/
#define ADC_SAR_ProxIR_SAR_CAP_TRIM_6             (0x06u)    /*decrease by 6*0.5 LSB = 3 LSB*/
#define ADC_SAR_ProxIR_SAR_CAP_TRIM_7             (0x07u)    /*decrease by 7*0.5 LSB = 3.5 LSB*/

#define ADC_SAR_ProxIR_SAR_TRIMUNIT               (0x08u)    /*Increase by 6fF*/

/*******************************************************/
/* SAR.CSR0 Satatus and Control Register 0 definitions */
/*******************************************************/

/* Bit Field  SAR_ICONT_LV_ENUM */
#define ADC_SAR_ProxIR_SAR_POWER_MASK             (0xC0u)
#define ADC_SAR_ProxIR_SAR_POWER_SHIFT            (0x06u)
#define ADC_SAR_ProxIR_ICONT_LV_0                 (0x00u)
#define ADC_SAR_ProxIR_ICONT_LV_1                 (0x40u)
#define ADC_SAR_ProxIR_ICONT_LV_2                 (0x80u)
#define ADC_SAR_ProxIR_ICONT_LV_3                 (0xC0u)
#define ADC_SAR_ProxIR_ICONT_FULL_CURRENT         (0x00u)

/* Bit Field SAR_RESET_SOFT_ENUM */
#define ADC_SAR_ProxIR_SAR_RESET_SOFT_NOTACTIVE   (0x00u)
#define ADC_SAR_ProxIR_SAR_RESET_SOFT_ACTIVE      (0x20u)

/* Bit Field  SAR_COHERENCY_EN_ENUM */
#define ADC_SAR_ProxIR_SAR_COHERENCY_EN_NOLOCK    (0x00u)
#define ADC_SAR_ProxIR_SAR_COHERENCY_EN_LOCK      (0x10u)

/* Bit Field  SAR_HIZ_ENUM */
#define ADC_SAR_ProxIR_SAR_HIZ_RETAIN             (0x00u)
#define ADC_SAR_ProxIR_SAR_HIZ_CLEAR              (0x08u)

/* Bit Field SAR_MX_SOF_ENUM */
#define ADC_SAR_ProxIR_SAR_MX_SOF_BIT             (0x00u)
#define ADC_SAR_ProxIR_SAR_MX_SOF_UDB             (0x04u)

/* Bit Field SAR_SOF_MODE_ENUM */
#define ADC_SAR_ProxIR_SAR_SOF_MODE_LEVEL         (0x00u)
#define ADC_SAR_ProxIR_SAR_SOF_MODE_EDGE          (0x02u)

/* Bit Field SAR_SOF_BIT_ENUM */
#define ADC_SAR_ProxIR_SAR_SOF_START_CONV         (0x01u)            /* Enable conversion */


/*******************************************************/
/* SAR.CSR1 Satatus and Control Register 1 definitions */
/*******************************************************/

/* Bit Field  SAR_MUXREF_ENUM */
#define ADC_SAR_ProxIR_SAR_MUXREF_MASK            (0xE0u)
#define ADC_SAR_ProxIR_SAR_MUXREF_NONE            (0x00u)
#define ADC_SAR_ProxIR_SAR_MUXREF_VDDA_2          (0x40u)
#define ADC_SAR_ProxIR_SAR_MUXREF_DAC             (0x60u)
#define ADC_SAR_ProxIR_SAR_MUXREF_1_024V          (0x80u)
#define ADC_SAR_ProxIR_SAR_MUXREF_1_20V           (0xA0u)

/* Bit Field  SAR_SWVP_SRC_ENUM */
#define ADC_SAR_ProxIR_SAR_SWVP_SRC_REG           (0x00u)
#define ADC_SAR_ProxIR_SAR_SWVP_SRC_UDB           (0x10u)

/* Bit Field  SAR_SWVP_SRC_ENUM */
#define ADC_SAR_ProxIR_SAR_SWVN_SRC_REG           (0x00u)
#define ADC_SAR_ProxIR_SAR_SWVN_SRC_UDB           (0x08u)

/* Bit Field  SAR_IRQ_MODE_ENUM */
#define ADC_SAR_ProxIR_SAR_IRQ_MODE_LEVEL         (0x00u)
#define ADC_SAR_ProxIR_SAR_IRQ_MODE_EDGE          (0x04u)

/* Bit Field  SAR_IRQ_MASK_ENUM */
#define ADC_SAR_ProxIR_SAR_IRQ_MASK_DIS           (0x00u)
#define ADC_SAR_ProxIR_SAR_IRQ_MASK_EN            (0x02u)

/* Bit Field  SAR_EOF_ENUM */
#define ADC_SAR_ProxIR_SAR_EOF_0                  (0x00u)
#define ADC_SAR_ProxIR_SAR_EOF_1                  (0x01u)

/*******************************************************/
/* SAR.CSR2 Satatus and Control Register 2 definitions */
/*******************************************************/

/* Bit Field  SAR_RESOLUTION_ENUM */
#define ADC_SAR_ProxIR_SAR_RESOLUTION_MASK        (0xC0u)
#define ADC_SAR_ProxIR_SAR_RESOLUTION_SHIFT       (0x06u)
#define ADC_SAR_ProxIR_SAR_RESOLUTION_12BIT       (0xC0u)
#define ADC_SAR_ProxIR_SAR_RESOLUTION_10BIT       (0x80u)
#define ADC_SAR_ProxIR_SAR_RESOLUTION_8BIT        (0x40u)

/* Bit Field SAR_SAMPLE_WIDTH_ENUM */
#define ADC_SAR_ProxIR_SAR_SAMPLE_WIDTH_MASK      (0x3Fu)
#define ADC_SAR_ProxIR_SAR_SAMPLE_WIDTH_MIN       (0x00u)   /* minimum sample time: +1 clock cycle */
#define ADC_SAR_ProxIR_SAR_SAMPLE_WIDTH           (ADC_SAR_ProxIR_SAMPLE_PRECHARGE - 0x02u)
#define ADC_SAR_ProxIR_SAR_SAMPLE_WIDTH_MAX       (0x07u)   /* maximum sample time: +8 clock cycles */

/*******************************************************/
/* SAR.CSR3 Satatus and Control Register 3 definitions */
/*******************************************************/

/* Bit Field  SAR_EN_CP_ENUM */
#define ADC_SAR_ProxIR_SAR_EN_CP_DIS              (0x00u)
#define ADC_SAR_ProxIR_SAR_EN_CP_EN               (0x80u)

/* Bit Field  SAR_EN_RESVDA_ENUM */
#define ADC_SAR_ProxIR_SAR_EN_RESVDA_DIS          (0x00u)
#define ADC_SAR_ProxIR_SAR_EN_RESVDA_EN           (0x40u)

/* Bit Field  SAR_PWR_CTRL_VCM_ENUM */
#define ADC_SAR_ProxIR_SAR_PWR_CTRL_VCM_MASK      (0x30u)
#define ADC_SAR_ProxIR_SAR_PWR_CTRL_VCM_0         (0x00u)

/* Bit Field  SAR_PWR_CTRL_VREF_ENUM */
#define ADC_SAR_ProxIR_SAR_PWR_CTRL_VREF_MASK     (0x0Cu)
#define ADC_SAR_ProxIR_SAR_PWR_CTRL_VREF_0        (0x00u)
#define ADC_SAR_ProxIR_SAR_PWR_CTRL_VREF_DIV_BY2  (0x04u)
#define ADC_SAR_ProxIR_SAR_PWR_CTRL_VREF_DIV_BY3  (0x08u)
#define ADC_SAR_ProxIR_SAR_PWR_CTRL_VREF_DIV_BY4  (0x0Cu)

/* Bit Field  SAR_EN_BUF_VCM_ENUM */
#define ADC_SAR_ProxIR_SAR_EN_BUF_VCM_DIS         (0x00u)
#define ADC_SAR_ProxIR_SAR_EN_BUF_VCM_EN          (0x02u)

/* Bit Field  SAR_EN_BUF_VREF_ENUM */
#define ADC_SAR_ProxIR_SAR_EN_BUF_VREF_DIS        (0x00u)
#define ADC_SAR_ProxIR_SAR_EN_BUF_VREF_EN         (0x01u)

/*******************************************************/
/* SAR.CSR4 Satatus and Control Register 4 definitions */
/*******************************************************/

/* Bit Field  SAR_DFT_INC_ENUM */
#define ADC_SAR_ProxIR_SAR_DFT_INC_MASK           (0x0Fu)
#define ADC_SAR_ProxIR_SAR_DFT_INC_DIS            (0x00u)
#define ADC_SAR_ProxIR_SAR_DFT_INC_EN             (0x0Fu)

/* Bit Field  SAR_DFT_INC_ENUM */
#define ADC_SAR_ProxIR_SAR_DFT_OUTC_MASK          (0x70u)
#define ADC_SAR_ProxIR_SAR_DFT_OUTC_DIS           (0x00u)
#define ADC_SAR_ProxIR_SAR_DFT_OUTC_EN            (0x70u)

/*******************************************************/
/* SAR.CSR5 Satatus and Control Register 5 definitions */
/*******************************************************/

/* Bit Field  SAR_DCEN_ENUM */
#define ADC_SAR_ProxIR_SAR_DCEN_0                 (0x00u)
#define ADC_SAR_ProxIR_SAR_DCEN_1                 (0x40u)

/* Bit Field  SAR_EN_CSEL_DFT_ENUM */
#define ADC_SAR_ProxIR_SAR_EN_CSEL_DFT_DISABLED   (0x00u)
#define ADC_SAR_ProxIR_SAR_EN_CSEL_DFT_ENABLED    (0x10u)

/* Bit Field  SAR_SEL_CSEL_DFT_ENUM */
#define ADC_SAR_ProxIR_SAR_SEL_CSEL_DFT_MASK      (0x0Fu)
#define ADC_SAR_ProxIR_SAR_SEL_CSEL_DFT_MIN       (0x00u)
#define ADC_SAR_ProxIR_SAR_SEL_CSEL_DFT_MAX       (0x0Fu)

/*******************************************************/
/* SAR.CSR6 Satatus and Control Register 6 definitions */
/*******************************************************/

/* Bit Field  SAR_REF_S_LSB_ENUM */
#define ADC_SAR_ProxIR_SAR_REF_S_LSB_MASK         (0xFFu)
#define ADC_SAR_ProxIR_SAR_REF_S_LSB_DIS          (0x00u)
#define ADC_SAR_ProxIR_SAR_REF_S0_LSB_EN          (0x01u)
#define ADC_SAR_ProxIR_SAR_REF_S1_LSB_EN          (0x02u)
#define ADC_SAR_ProxIR_SAR_REF_S2_LSB_EN          (0x04u)
#define ADC_SAR_ProxIR_SAR_REF_S3_LSB_EN          (0x08u)
#define ADC_SAR_ProxIR_SAR_REF_S4_LSB_EN          (0x10u)
#define ADC_SAR_ProxIR_SAR_REF_S5_LSB_EN          (0x20u)
#define ADC_SAR_ProxIR_SAR_REF_S6_LSB_EN          (0x40u)
#define ADC_SAR_ProxIR_SAR_REF_S7_LSB_EN          (0x80u)

/*******************************************************/
/* SAR.CSR7 Satatus and Control Register 7 definitions */
/*******************************************************/

/* Bit Field  SAR_REF_S_ENUM */
#define ADC_SAR_ProxIR_SAR_REF_S_MSB_MASK         (0x3Fu)
#define ADC_SAR_ProxIR_SAR_REF_S_MSB_DIS          (0x00u)
#define ADC_SAR_ProxIR_SAR_REF_S8_MSB_DIS         (0x01u)
#define ADC_SAR_ProxIR_SAR_REF_S9_MSB_EN          (0x02u)
#define ADC_SAR_ProxIR_SAR_REF_S10_MSB_EN         (0x04u)
#define ADC_SAR_ProxIR_SAR_REF_S11_MSB_EN         (0x08u)
#define ADC_SAR_ProxIR_SAR_REF_S12_MSB_EN         (0x10u)
#define ADC_SAR_ProxIR_SAR_REF_S13_MSB_EN         (0x20u)

/*******************************************************/
/* SAR.CLK SAR Clock Selection Register definitions    */
/*******************************************************/

/* Bit Field  MX_PUMPCLK_ENUM */
#define ADC_SAR_ProxIR_SAR_MX_PUMPCLK_MASK        (0xE0u)
#define ADC_SAR_ProxIR_SAR_MX_PUMPCLK_0           (0x00u)
#define ADC_SAR_ProxIR_SAR_MX_PUMPCLK_1           (0x20u)
#define ADC_SAR_ProxIR_SAR_MX_PUMPCLK_2           (0x40u)
#define ADC_SAR_ProxIR_SAR_MX_PUMPCLK_3           (0x60u)
#define ADC_SAR_ProxIR_SAR_MX_PUMPCLK_4           (0x80u)

/* Bit Field  BYPASS_SYNC_ENUM */
#define ADC_SAR_ProxIR_SAR_BYPASS_SYNC_0          (0x00u)
#define ADC_SAR_ProxIR_SAR_BYPASS_SYNC_1          (0x10u)

/* Bit Field  MX_CLK_EN_ENUM */
#define ADC_SAR_ProxIR_SAR_MX_CLK_EN              (0x08u)

/* Bit Field  MX_CLK_ENUM  */
#define ADC_SAR_ProxIR_SAR_MX_CLK_MASK            (0x07u)
#define ADC_SAR_ProxIR_SAR_MX_CLK_0               (0x00u)
#define ADC_SAR_ProxIR_SAR_MX_CLK_1               (0x01u)
#define ADC_SAR_ProxIR_SAR_MX_CLK_2               (0x02u)
#define ADC_SAR_ProxIR_SAR_MX_CLK_3               (0x03u)
#define ADC_SAR_ProxIR_SAR_MX_CLK_4               (0x04u)

/*********************************************************/
/* ANAIF.WRK.SARS.SOF SAR Global Start-of-frame register */
/*********************************************************/

/* Bit Field  SAR_SOF_BIT_ENUM */
#define ADC_SAR_ProxIR_SAR_SOFR_BIT_MASK          (0x03u)
#define ADC_SAR_ProxIR_SAR_SOFR_BIT_0             (0x00u)
#define ADC_SAR_ProxIR_SAR_SOFR_BIT_1             (0x01u)

/***********************************************/
/* SAR.SW3 SAR Analog Routing Register 3       */
/***********************************************/
#define ADC_SAR_ProxIR_SAR_VP_VSSA                (0x04u)
#define ADC_SAR_ProxIR_SAR_VN_AMX                 (0x10u)
#define ADC_SAR_ProxIR_SAR_VN_VREF                (0x20u)
#define ADC_SAR_ProxIR_SAR_VN_VSSA                (0x40u)
#define ADC_SAR_ProxIR_SAR_VN_MASK                (0x60u)

/***********************************************/
/* SAR.WRKx SAR Working Register               */
/***********************************************/
#define ADC_SAR_ProxIR_SAR_WRK_MAX                (0xFFFu)
#define ADC_SAR_ProxIR_SAR_DIFF_SHIFT             (0x800u)

#endif /* End CY_ADC_SAR_ADC_SAR_ProxIR_H */


/* [] END OF FILE */
