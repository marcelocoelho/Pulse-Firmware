/*******************************************************************************
* File Name: TIA_PulseIn.c  
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the TIA User Module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "TIA_PulseIn.h"

#if (!CY_PSOC5A)
    #if (CYDEV_VARIABLE_VDDA == 1u)
        #include "CyScBoostClk.h"
    #endif /* (CYDEV_VARIABLE_VDDA == 1u) */
#endif /* (!CY_PSOC5A) */

/* Fixed configuration of SC Block only performed once */
uint8 TIA_PulseIn_initVar = 0u;

#if (CY_PSOC5A)
    static TIA_PulseIn_BACKUP_STRUCT  TIA_PulseIn_P5backup;
#endif /* (CY_PSOC5A) */


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
*******************************************************************************/
void TIA_PulseIn_Init(void) 
{
    /* Set SC Block Mode to TIA - SCxx_CR0[3:1] */
    TIA_PulseIn_CR0_REG = TIA_PulseIn_MODE_TIA;
    
    /*    Set SC Block CR1 Control Register fields to support TIA mode 
            drive field set by SetPower() API
            compensation Cap - 5.1 pF
            div2 - disable, n/a for TIA mode
            Mixer/PGA Gain - 0db, n/a for TIA mode   */

    TIA_PulseIn_CR1_REG = (TIA_PulseIn_COMP_5P1PF |
                                TIA_PulseIn_DIV2_DISABLE |
                                TIA_PulseIn_GAIN_0DB);


    /*    Set SC Block CR2 Control Register fields to support TIA mode
            Bias - Low
            r20_r40 - 40K, n/a for TIA mode
            redc - set by SetCapFB() API
            rval - set by SetResFB() API
            PGA Ground Ref - disable, n/a for TIA mode    */

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
*******************************************************************************/
void TIA_PulseIn_Enable(void) 
{
	/* This is to restore the value of register CR1 and CR2 which is saved 
      in prior to the modifications in stop() API */
	#if (CY_PSOC5A)
        if(TIA_PulseIn_P5backup.enableState == 1u)
        {
			TIA_PulseIn_CR1_REG = TIA_PulseIn_P5backup.scCR1Reg;
            TIA_PulseIn_CR2_REG =TIA_PulseIn_P5backup.scCR2Reg;
            TIA_PulseIn_P5backup.enableState = 0u;
        }
    #endif
    /* Enable power to Amp in Active mode */
    TIA_PulseIn_PM_ACT_CFG_REG |= TIA_PulseIn_ACT_PWR_EN;

    /* Enable the power to Amp in Alternative active mode*/
    TIA_PulseIn_PM_STBY_CFG_REG |= TIA_PulseIn_STBY_PWR_EN;
    
    TIA_PulseIn_PUMP_CR1_REG |= TIA_PulseIn_PUMP_CR1_SC_CLKSEL;
    
    #if (!CY_PSOC5A)
        #if (CYDEV_VARIABLE_VDDA == 1u)
            if(CyScPumpEnabled == 1u)
            {
                TIA_PulseIn_BSTCLK_REG &= (uint8)(~TIA_PulseIn_BST_CLK_INDEX_MASK);
                TIA_PulseIn_BSTCLK_REG |= TIA_PulseIn_BST_CLK_EN | CyScBoostClk__INDEX;
                TIA_PulseIn_SC_MISC_REG |= TIA_PulseIn_PUMP_FORCE;
                CyScBoostClk_Start();
            }
            else
            {
                TIA_PulseIn_BSTCLK_REG &= (uint8)(~TIA_PulseIn_BST_CLK_EN);
                TIA_PulseIn_SC_MISC_REG &= (uint8)(~TIA_PulseIn_PUMP_FORCE);
            }
        #endif /* (CYDEV_VARIABLE_VDDA == 1u) */
    #endif /* (!CY_PSOC5A) */
}


/*******************************************************************************
* Function Name: TIA_PulseIn_Start
********************************************************************************
*
* Summary:
*  The start function initializes the TIA with the default values and enables
*  power to the SC block.
*
* Parameters:
*  void
*
* Return:
*  void 
*
* Global variables:
*  TIA_PulseIn_initVar: Used to check the initial configuration, modified 
*  when this function is called for the first time.
*
* Theory:
*  Full initialization of the SC Block configuration registers is only perfomed
*  the first time the routine executes - global variable  is used to
*  TIA_PulseIn_initVar is used to indicate that the static configuration 
*  has been completed.
*
*******************************************************************************/
void TIA_PulseIn_Start(void) 
{
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
*******************************************************************************/
void TIA_PulseIn_Stop(void) 
{
     /* Disble power to the Amp in Active mode template*/
    TIA_PulseIn_PM_ACT_CFG_REG &= (uint8)(~TIA_PulseIn_ACT_PWR_EN);

    /* Disble power to the Amp in Alternative active mode template*/
    TIA_PulseIn_PM_STBY_CFG_REG &= (uint8)(~TIA_PulseIn_STBY_PWR_EN);
    
    #if (!CY_PSOC5A)
        #if (CYDEV_VARIABLE_VDDA == 1u)
            TIA_PulseIn_BSTCLK_REG &= (uint8)(~TIA_PulseIn_BST_CLK_EN);
            /* Disable pumps only if there aren't any SC block in use */
            if ((TIA_PulseIn_PM_ACT_CFG_REG & TIA_PulseIn_PM_ACT_CFG_MASK) == 0u)
            {
                TIA_PulseIn_SC_MISC_REG &= (uint8)(~TIA_PulseIn_PUMP_FORCE);
                TIA_PulseIn_PUMP_CR1_REG &= (uint8)(~TIA_PulseIn_PUMP_CR1_SC_CLKSEL);
                CyScBoostClk_Stop();
            }
        #endif /* CYDEV_VARIABLE_VDDA == 1u */
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
        
    /* This sets TIA in zero current mode and output routes are valid */
    #if (CY_PSOC5A)
        TIA_PulseIn_P5backup.scCR1Reg = TIA_PulseIn_CR1_REG;
        TIA_PulseIn_P5backup.scCR2Reg = TIA_PulseIn_CR2_REG;
        TIA_PulseIn_CR1_REG = 0x00u;
        TIA_PulseIn_CR2_REG = 0x00u;
        TIA_PulseIn_P5backup.enableState = 1u;
    #endif /* (CY_PSOC5A) */
}


/*******************************************************************************
* Function Name: TIA_PulseIn_SetPower
********************************************************************************
*
* Summary:
*  Set the drive power of the TIA.
*
* Parameters:
*  Power: Sets power level between (0) and (3) high power.
*
* Return:
*  void
*
*******************************************************************************/
void TIA_PulseIn_SetPower(uint8 power) 
{
    uint8 tmpCR;

    /* Sets drive bits in SC Block Control Register 1:  SCxx_CR[1:0] */
    tmpCR = TIA_PulseIn_CR1_REG & (uint8)(~TIA_PulseIn_DRIVE_MASK);
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
*  res_feedback: specifies resistive feedback value (see header file for gain 
*                values.)
*
* Return:
*  void
*
*******************************************************************************/
void TIA_PulseIn_SetResFB(uint8 res_feedback) 
{
    /* Only set new gain if it is a valid gain */
    if( res_feedback <= TIA_PulseIn_RES_FEEDBACK_MAX)
    {
        /* Clear SCxx_CR2 rval bits - SCxx_CR2[6:4] */
        TIA_PulseIn_CR2_REG &= (uint8)(~TIA_PulseIn_RVAL_MASK);

        /* Set resistive feedback value */
        TIA_PulseIn_CR2_REG |= (uint8)(res_feedback << 4);
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
*  cap_feedback: specifies capacitive feedback value (see header file for gain 
*                values.)
*
* Return:
*  void
*
*******************************************************************************/
void TIA_PulseIn_SetCapFB(uint8 cap_feedback) 
{
    /* Only set new gain if it is a valid gain */
    if( cap_feedback <= TIA_PulseIn_CAP_FEEDBACK_MAX)
    {
        /* Clear SCxx_CR2 redc bits -  - CR2[3:2] */
        TIA_PulseIn_CR2_REG &= (uint8)(~TIA_PulseIn_REDC_MASK);

        /* Set redc capacitive feedback value */
        TIA_PulseIn_CR2_REG |= (uint8)(cap_feedback << 2);
    }
}


/* [] END OF FILE */
