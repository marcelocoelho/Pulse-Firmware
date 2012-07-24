/*******************************************************************************
* File Name: CapSense_1_CompCH0.h  
* Version 1.70
*
*  Description:
*    This file contains the function prototypes and constants used in
*    the Analog Comparator User Module.
*
*  Note:
*     
*
********************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_COMP_CapSense_1_CompCH0_H) 
#define CY_COMP_CapSense_1_CompCH0_H

#include "cytypes.h"
#include "CyLib.h"
#include "cyfitter.h" 

#define CapSense_1_CompCH0_RECALMODE 0u


/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct CapSense_1_CompCH0_backupStruct
{
    uint8 enableState;
    uint8 compCRReg;
}CapSense_1_CompCH0_backupStruct;


/**************************************
*        Function Prototypes 
**************************************/

void    CapSense_1_CompCH0_Start(void);
void    CapSense_1_CompCH0_Stop(void)               ;
void    CapSense_1_CompCH0_SetSpeed(uint8 speed)    ;
uint8   CapSense_1_CompCH0_GetCompare(void)         ;
uint16  CapSense_1_CompCH0_ZeroCal(void)            ;
void    CapSense_1_CompCH0_LoadTrim(uint16 trimVal) ;
void CapSense_1_CompCH0_Init(void)                  ; 
void CapSense_1_CompCH0_Enable(void)                ;
void CapSense_1_CompCH0_SaveConfig(void);
void CapSense_1_CompCH0_RestoreConfig(void);
void CapSense_1_CompCH0_Sleep(void);
void CapSense_1_CompCH0_Wakeup(void)                ;
/* Below APIs are valid only for PSoC3 silicon.*/
#if (CY_PSOC3) 
    void CapSense_1_CompCH0_PwrDwnOverrideEnable(void) ;
    void CapSense_1_CompCH0_PwrDwnOverrideDisable(void) ;
#endif /* CY_PSOC3 */


/**************************************
*           API Constants        
**************************************/

/* Power constants for SetSpeed() function */
#define CapSense_1_CompCH0_SLOWSPEED   0x00u
#define CapSense_1_CompCH0_HIGHSPEED   0x01u
#define CapSense_1_CompCH0_LOWPOWER    0x02u


/***************************************
*         Trim Locations               
****************************************/

/* High speed trim values */
#define CapSense_1_CompCH0_HS_TRIM_TR0        (CY_GET_XTND_REG8(CapSense_1_CompCH0_ctComp__TRIM__TR0_HS))

#if (CY_PSOC3_ES3 || CY_PSOC5_ES2)
    #define CapSense_1_CompCH0_HS_TRIM_TR1    (CY_GET_XTND_REG8(CapSense_1_CompCH0_ctComp__TRIM__TR1_HS))
#endif /* (CY_PSOC3_ES3 || CY_PSOC5_ES2) */

/* Low speed trim values */
#define CapSense_1_CompCH0_LS_TRIM_TR0        (CY_GET_XTND_REG8(CapSense_1_CompCH0_ctComp__TRIM__TR0 + 1))

#if (CY_PSOC3_ES3 || CY_PSOC5_ES2)
    #define CapSense_1_CompCH0_LS_TRIM_TR1    (CY_GET_XTND_REG8(CapSense_1_CompCH0_ctComp__TRIM__TR1 + 1))
#endif /* CY_PSOC3_ES3 || CY_PSOC5_ES2 */


/**************************************
*           Parameter Defaults        
**************************************/

#define CapSense_1_CompCH0_DEFAULT_SPEED       0u 
#define CapSense_1_CompCH0_DEFAULT_HYSTERESIS  0u
#define CapSense_1_CompCH0_DEFAULT_POLARITY    0u
#define CapSense_1_CompCH0_DEFAULT_BYPASS_SYNC 1u
#define CapSense_1_CompCH0_DEFAULT_PWRDWN_OVRD 0u


/**************************************
*             Registers        
**************************************/

#define CapSense_1_CompCH0_CR      (* (reg8 *) CapSense_1_CompCH0_ctComp__CR )   /* Config register   */
#define CapSense_1_CompCH0_CR_PTR  (  (reg8 *) CapSense_1_CompCH0_ctComp__CR )
#define CapSense_1_CompCH0_CLK     (* (reg8 *) CapSense_1_CompCH0_ctComp__CLK )  /* Comp clock control register */
#define CapSense_1_CompCH0_CLK_PTR (  (reg8 *) CapSense_1_CompCH0_ctComp__CLK )
#define CapSense_1_CompCH0_SW0     (* (reg8 *) CapSense_1_CompCH0_ctComp__SW0 )  /* Routing registers */
#define CapSense_1_CompCH0_SW0_PTR (  (reg8 *) CapSense_1_CompCH0_ctComp__SW0 )
#define CapSense_1_CompCH0_SW2     (* (reg8 *) CapSense_1_CompCH0_ctComp__SW2 )
#define CapSense_1_CompCH0_SW2_PTR (  (reg8 *) CapSense_1_CompCH0_ctComp__SW2 )
#define CapSense_1_CompCH0_SW3     (* (reg8 *) CapSense_1_CompCH0_ctComp__SW3 )
#define CapSense_1_CompCH0_SW3_PTR (  (reg8 *) CapSense_1_CompCH0_ctComp__SW3 )
#define CapSense_1_CompCH0_SW4     (* (reg8 *) CapSense_1_CompCH0_ctComp__SW4 )
#define CapSense_1_CompCH0_SW4_PTR (  (reg8 *) CapSense_1_CompCH0_ctComp__SW4 )
#define CapSense_1_CompCH0_SW6     (* (reg8 *) CapSense_1_CompCH0_ctComp__SW6 )
#define CapSense_1_CompCH0_SW6_PTR (  (reg8 *) CapSense_1_CompCH0_ctComp__SW6 )

/* Trim registers */
/* PSoC3 ES2 or early, PSoC5 ES1 or early */
#if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
    #define CapSense_1_CompCH0_TR      (* (reg8 *) CapSense_1_CompCH0_ctComp__TR )   /* Trim registers */
    #define CapSense_1_CompCH0_TR_PTR  (  (reg8 *) CapSense_1_CompCH0_ctComp__TR )
#endif /* CY_PSOC3_ES2 || CY_PSOC5_ES1 */

/* PSoC3 ES3 or later, PSoC5 ES2 or later */
#if (CY_PSOC3_ES3 || CY_PSOC5_ES2) 
    #define CapSense_1_CompCH0_TR0         (* (reg8 *) CapSense_1_CompCH0_ctComp__TR0 ) /* Trim register for P-type load */
    #define CapSense_1_CompCH0_TR0_PTR     (  (reg8 *) CapSense_1_CompCH0_ctComp__TR0 ) 
    #define CapSense_1_CompCH0_TR1         (* (reg8 *) CapSense_1_CompCH0_ctComp__TR1 ) /* Trim register for N-type load */
    #define CapSense_1_CompCH0_TR1_PTR     (  (reg8 *) CapSense_1_CompCH0_ctComp__TR1 ) 
#endif /* CY_PSOC3_ES3 || CY_PSOC5_ES2 */

#define CapSense_1_CompCH0_WRK             (* (reg8 *) CapSense_1_CompCH0_ctComp__WRK )  /* Working register - output */
#define CapSense_1_CompCH0_WRK_PTR         (  (reg8 *) CapSense_1_CompCH0_ctComp__WRK )
#define CapSense_1_CompCH0_PWRMGR          (* (reg8 *) CapSense_1_CompCH0_ctComp__PM_ACT_CFG )  /* Active Power manager */
#define CapSense_1_CompCH0_PWRMGR_PTR      ( (reg8 *) CapSense_1_CompCH0_ctComp__PM_ACT_CFG )
#define CapSense_1_CompCH0_STBY_PWRMGR     (* (reg8 *) CapSense_1_CompCH0_ctComp__PM_STBY_CFG )  /* Standby Power manager */
#define CapSense_1_CompCH0_STBY_PWRMGR_PTR ( (reg8 *) CapSense_1_CompCH0_ctComp__PM_STBY_CFG )


/**************************************
*       Register Constants        
**************************************/

/* CR (Comp Control Register)             */
#define CapSense_1_CompCH0_CFG_MODE_MASK  0x78u
#define CapSense_1_CompCH0_FILTER_ON      0x40u
#define CapSense_1_CompCH0_HYST_OFF       0x20u
#define CapSense_1_CompCH0_CAL_ON         0x10u
#define CapSense_1_CompCH0_MX_AO          0x08u
#define CapSense_1_CompCH0_PWRDWN_OVRD    0x04u

#define CapSense_1_CompCH0_PWR_MODE_SHIFT 0x00u
#define CapSense_1_CompCH0_PWR_MODE_MASK  (0x03u << CapSense_1_CompCH0_PWR_MODE_SHIFT)
#define CapSense_1_CompCH0_PWR_MODE_SLOW  (0x00u << CapSense_1_CompCH0_PWR_MODE_SHIFT)
#define CapSense_1_CompCH0_PWR_MODE_FAST  (0x01u << CapSense_1_CompCH0_PWR_MODE_SHIFT)
#define CapSense_1_CompCH0_PWR_MODE_ULOW  (0x02u << CapSense_1_CompCH0_PWR_MODE_SHIFT)

/* CLK (Comp Clock Control Register)      */
#define CapSense_1_CompCH0_BYPASS_SYNC    0x10u
#define CapSense_1_CompCH0_SYNC_CLK_EN    0x08u
#define CapSense_1_CompCH0_SYNCCLK_MASK   (CapSense_1_CompCH0_BYPASS_SYNC | CapSense_1_CompCH0_SYNC_CLK_EN)

/* SW3 Routing Register definitions */
#define CapSense_1_CompCH0_CMP_SW3_INPCTL_MASK    0x09u   /* SW3 bits for inP routing control */

/* TR (Comp Trim Register)     */
#define CapSense_1_CompCH0_DEFAULT_CMP_TRIM  0x00u

/* PSoC3 ES2 or early, PSoC5 ES1 or early */
#if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
    #define CapSense_1_CompCH0_CMP_TRIM1_DIR  0x08u   /* Trim direction for N-type load for offset calibration */
    #define CapSense_1_CompCH0_CMP_TRIM1_MASK 0x07u   /* Trim for N-type load for offset calibration */
    #define CapSense_1_CompCH0_CMP_TRIM2_DIR  0x80u   /* Trim direction for P-type load for offset calibration */
    #define CapSense_1_CompCH0_CMP_TRIM2_MASK 0x70u   /* Trim for P-type load for offset calibration */
#endif /* CY_PSOC3_ES2 || CY_PSOC5_ES1 */

/* PSoC3 ES3 or later, PSoC5 ES2 or later */
#if (CY_PSOC3_ES3 || CY_PSOC5_ES2)
    #define CapSense_1_CompCH0_CMP_TR0_DIR 0x11u    /* Trim direction for N-type load for offset calibration */
    #define CapSense_1_CompCH0_CMP_TR0_MASK 0x1Fu   /* Trim for N-type load for offset calibration */
    #define CapSense_1_CompCH0_CMP_TR1_DIR 0x10u    /* Trim direction for P-type load for offset calibration */
    #define CapSense_1_CompCH0_CMP_TR1_MASK 0x1Fu   /* Trim for P-type load for offset calibration */ 
#endif /* CY_PSOC3_ES3 || CY_PSOC5_ES2 */


/* WRK (Comp output working register)     */ 
#define CapSense_1_CompCH0_CMP_OUT_MASK   CapSense_1_CompCH0_ctComp__WRK_MASK /* Specific comparator out mask */

/* PM_ACT_CFG7 (Active Power Mode CFG Register)     */ 
#define CapSense_1_CompCH0_ACT_PWR_EN     CapSense_1_CompCH0_ctComp__PM_ACT_MSK /* Power enable mask */

/* PM_STBY_CFG7 (Standby Power Mode CFG Register)     */ 
#define CapSense_1_CompCH0_STBY_PWR_EN     CapSense_1_CompCH0_ctComp__PM_STBY_MSK /* Standby Power enable mask */

#if (CY_PSOC5_ES1)
    /* For stop API changes mask to make the COMP register CR to 0X00  */
    #define CapSense_1_CompCH0_COMP_REG_CLR             (0x00u)
#endif /* CY_PSOC5_ES1 */

#endif /* CY_COMP_CapSense_1_CompCH0_H */


/* [] END OF FILE */
