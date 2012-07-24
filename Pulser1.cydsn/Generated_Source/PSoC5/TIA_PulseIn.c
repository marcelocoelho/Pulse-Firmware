/*******************************************************************************
* File Name: TIA_PulseIn.c  
* Version 1.80
*
* Description:
*  This file provides the source code to the API for the TIA User Module.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "TIA_PulseIn.h"

#if(TIA_PulseIn_CYDEV_VDDA_MV < TIA_PulseIn_MINIMUM_VDDA_THRESHOLD_MV)
    #include "TIA_PulseIn_bst_clk.h"
#endif /* TIA_PulseIn_MIN_VDDA */

/* Fixed configuration of SC Block only performed once */
uint8 TIA_PulseIn_initVar = 0u;
static TIA_PulseIn_BACKUP_STRUCT  TIA_PulseIn_backup;

/* Check to see if required defines such as CY_PSOC3 and CY_PSOC5 are available */
/* They are defined starting with cy_boot v2.30 */
#ifndef CY_PSOC3
#error Component TIA_v1_80 requires cy_boot v2.30 or later
#endif


/*******************************************************************************   
* Function Name: TIA_PulseIn_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  TIA_PulseIn_Start().
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  Yes
* 
*******************************************************************************/
void TIA_PulseIn_Init(void) 
{
    /* Set SC Block Mode to TIA - SCxx_CR0[3:1] */
    TIA_PulseIn_CR0_REG = TIA_PulseIn_MODE_TIA;  
    
    /* 
        Set SC Block CR1 Control Register fields to support TIA mode 
            drive field set by SetPower() API
            compensation Cap - 5.1 pF
            div2 - disable, n/a for TIA mode
            Mixer/PGA Gain - 0db, n/a for TIA mode
    */
    TIA_PulseIn_CR1_REG = (TIA_PulseIn_COMP_5P1PF |
                                TIA_PulseIn_DIV2_DISABLE |
                                TIA_PulseIn_GAIN_0DB);  

    /* 
        Set SC Block CR2 Control Register fields to support TIA mode
            Bias - Low
            r20_r40 - 40K, n/a for TIA mode
            redc - set by SetCapFB() API
            rval - set by SetResFB() API
            PGA Ground Ref - disable, n/a for TIA mode
    */
    TIA_PulseIn_CR2_REG = (TIA_PulseIn_BIAS_LOW |
                                TIA_PulseIn_R20_40B_40K |
                                TIA_PulseIn_GNDVREF_DI);
                          
    /* Set default resistive feedback value */
    TIA_PulseIn_SetResFB(TIA_PulseIn_INIT_RES_FEEDBACK);
      
    /* Set default capacitive feedback value */
    TIA_PulseIn_SetCapFB(TIA_PulseIn_INIT_CAP_FEEDBACK);

    /* Set default power */
    TIA_PulseIn_SetPower(TIA_PulseIn_INIT_POWER);
}


/*******************************************************************************   
* Function Name: TIA_PulseIn_Enable
********************************************************************************
*
* Summary:
*  Enables the TIA block operation
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  Yes
* 
*******************************************************************************/
void TIA_PulseIn_Enable(void) 
{
    /* Enable power to Amp in Active mode */
    TIA_PulseIn_PM_ACT_CFG_REG |= TIA_PulseIn_ACT_PWR_EN;

    /* Enable the power to Amp in Alternative active mode*/
    TIA_PulseIn_PM_STBY_CFG_REG |= TIA_PulseIn_STBY_PWR_EN;
    
    /* Enable SC Block boost clock control for low Vdda operation */
   #if(TIA_PulseIn_CYDEV_VDDA_MV < TIA_PulseIn_MINIMUM_VDDA_THRESHOLD_MV)
        TIA_PulseIn_BSTCLK_REG |= TIA_PulseIn_BST_CLK_EN; 
        TIA_PulseIn_bst_clk_Enable();
    #endif /* TIA_PulseIn_MIN_VDDA */
    
    /* PSoC3 ES2 or early, PSoC5 ES1 */
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
        /* Enable Pump only if voltage is below threshold */
        if (TIA_PulseIn_CYDEV_VDDA_MV < TIA_PulseIn_MINIMUM_VDDA_THRESHOLD_MV)
        {
            TIA_PulseIn_SC_MISC_REG |= TIA_PulseIn_PUMP_FORCE;
        }
        
    /* PSoC3 ES3 or later, PSoC5 ES2 or later */
    #elif (CY_PSOC3_ES3 || CY_PSOC5_ES2)
        /* Enable charge Pump clock for SC block */
        TIA_PulseIn_PUMP_CR1_REG |= TIA_PulseIn_PUMP_CR1_SC_CLKSEL;
        /* Enable Pump only if voltage is below threshold */
        if (TIA_PulseIn_CYDEV_VDDA_MV < TIA_PulseIn_MINIMUM_VDDA_THRESHOLD_MV)
        {
            TIA_PulseIn_SC_MISC_REG |= TIA_PulseIn_PUMP_FORCE;
        }
    #endif /* (CY_PSOC3_ES2 || CY_PSOC5_ES1) */
}


/*******************************************************************************
* Function Name: TIA_PulseIn_Start
********************************************************************************
*
* Summary:
*  The start function initializes the TIA with the default values and enables
*  power to the SC block
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Global variables:
*  TIA_PulseIn_initVar:  Used to check the initial configuration, modified 
*  when this function is called for the first time.
*
* Theory: 
*  Full initialization of the SC Block configuration registers is only perfomed
*  the first time the routine executes - global variable  is used to
*  TIA_PulseIn_initVar is used to indicate that the static configuration 
*  has been completed.
*
* Reentrant:
*  No
*
*******************************************************************************/
void TIA_PulseIn_Start(void) 
{

    /* This is to restore the value of register CR1 and CR2 which is saved 
      in prior to the modification in stop() API */
    #if (CY_PSOC5_ES1)
    if(TIA_PulseIn_initVar == 1u)
    {
        TIA_PulseIn_CR1_REG = TIA_PulseIn_backup.scCr1Reg;
        TIA_PulseIn_CR2_REG =TIA_PulseIn_backup.scCr2Reg;
    }
    #endif

    if(TIA_PulseIn_initVar == 0u)
    {
        TIA_PulseIn_initVar = 1u;  
        TIA_PulseIn_Init();
    }

    TIA_PulseIn_Enable();
}


/*******************************************************************************
* Function Name: TIA_PulseIn_Stop
********************************************************************************
*
* Summary:
*  Disables power to SC block.
*
* Parameters:  
*  void 
*
* Return: 
*  void 
*
* Reentrant: 
*  Yes
*
*******************************************************************************/
void TIA_PulseIn_Stop(void) 
{
    /* Disable pumps only if only one SC block is in use */
    if (TIA_PulseIn_PM_ACT_CFG_REG == TIA_PulseIn_ACT_PWR_EN)
    {
        TIA_PulseIn_SC_MISC_REG &= ~TIA_PulseIn_PUMP_FORCE;
    }
   
    /* Disble power to the Amp in Active mode template*/
    TIA_PulseIn_PM_ACT_CFG_REG &= ~TIA_PulseIn_ACT_PWR_EN;

    /* Disble power to the Amp in Alternative active mode template*/
    TIA_PulseIn_PM_STBY_CFG_REG &= ~TIA_PulseIn_STBY_PWR_EN;
    
    /* Disable SC Block boost clk control, if used (MinVdda < 2.7V) */
    #if(TIA_PulseIn_CYDEV_VDDA_MV < TIA_PulseIn_MINIMUM_VDDA_THRESHOLD_MV)
        TIA_PulseIn_BSTCLK_REG &= ~TIA_PulseIn_BST_CLK_EN;
        TIA_PulseIn_bst_clk_Disable();
    #endif /* TIA_PulseIn_MIN_VDDA */

    /* This sets TIA in zero current mode and output routes are valid */
    #if (CY_PSOC5_ES1)
        TIA_PulseIn_backup.scCr1Reg = TIA_PulseIn_CR1_REG;
        TIA_PulseIn_backup.scCr2Reg = TIA_PulseIn_CR2_REG;
        TIA_PulseIn_CR1_REG = TIA_PulseIn_SC_REG_CLR;
        TIA_PulseIn_CR2_REG = TIA_PulseIn_SC_REG_CLR;
    #endif

}


/*******************************************************************************
* Function Name: TIA_PulseIn_SetPower
********************************************************************************
*
* Summary:
*  Set the drive power of the TIA
*
* Parameters:  
*  Power:  Sets power level between (0) and (3) high power
*
* Return: 
*  void 
*
* Reentrant: 
*  Yes
*
*******************************************************************************/
void TIA_PulseIn_SetPower(uint8 power) 
{
    uint8 tmpCR;
    
    /* Sets drive bits in SC Block Control Register 1:  SCxx_CR[1:0] */    
    tmpCR = TIA_PulseIn_CR1_REG & ~TIA_PulseIn_DRIVE_MASK;
    tmpCR |= (power & TIA_PulseIn_DRIVE_MASK);
    TIA_PulseIn_CR1_REG = tmpCR;  
}


/*******************************************************************************
* Function Name: TIA_PulseIn_SetResFB
********************************************************************************
*
* Summary:
*  This function sets value for the resistive feedback of the amplifier.
*
* Parameters:  
*  res_feedback:  specifies resistive feedback value (see header file for gain 
*                 values.)
*
* Return: 
*  void 
*
* Reentrant: 
*  Yes
*
*******************************************************************************/
void TIA_PulseIn_SetResFB(uint8 res_feedback) 
{
    /* Only set new gain if it is a valid gain */
    if( res_feedback <= TIA_PulseIn_RES_FEEDBACK_MAX)
    {
        /* Clear SCxx_CR2 rval bits - SCxx_CR2[6:4] */
        TIA_PulseIn_CR2_REG &= ~TIA_PulseIn_RVAL_MASK;

        /* Set resistive feedback value */
        TIA_PulseIn_CR2_REG |= (res_feedback << 4);
    }
}


/*******************************************************************************
* Function Name: TIA_PulseIn_SetCapFB
********************************************************************************
*
* Summary:
*  This function sets the value for the capacitive feedback of the amplifier.
*
* Parameters:  
*  cap_feedback:  specifies capacitive feedback value (see header file for gain 
*                 values.)
*
* Return: 
*  void 
*
* Reentrant: 
*  Yes
*
*******************************************************************************/
void TIA_PulseIn_SetCapFB(uint8 cap_feedback) 
{
    /* Only set new gain if it is a valid gain */
    if( cap_feedback <= TIA_PulseIn_CAP_FEEDBACK_MAX)
    {
        /* Clear SCxx_CR2 redc bits -  - CR2[3:2] */
        TIA_PulseIn_CR2_REG &= ~TIA_PulseIn_REDC_MASK;

        /* Set redc capacitive feedback value */
        TIA_PulseIn_CR2_REG |= (cap_feedback << 2);
    }
}


/* [] END OF FILE */
