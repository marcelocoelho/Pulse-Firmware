/*******************************************************************************
* File Name: VDAC8_PulseRed.c  
* Version 1.70
*
*  Description:
*    This file provides the source code to the API for the 8-bit Voltage DAC 
*    (VDAC8) User Module.
*
*   Note:
*     Any unusual or non-standard behavior should be noted here. Other-
*     wise, this section should remain blank.
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "VDAC8_PulseRed.h"

#if (CY_PSOC5_ES1)
#include <CyLib.h>
#endif /* CY_PSOC5_ES1 */

/* Check to see if required defines such as CY_PSOC3 and CY_PSOC5 are available */
/* They are defined starting with cy_boot v2.30 */
#ifndef CY_PSOC3
#error Component VDAC8_v1_70 requires cy_boot v2.30 or later
#endif /* CY_PSOC3 */


uint8 VDAC8_PulseRed_initVar = 0;

#if (CY_PSOC5_ES1)
uint8 VDAC8_PulseRed_restoreVal = 0;
uint8 VDAC8_PulseRed_intrStatus = 0u;
#endif /* CY_PSOC5_ES1 */

static VDAC8_PulseRed_backupStruct VDAC8_PulseRed_backup;


/*******************************************************************************
* Function Name: VDAC8_PulseRed_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void:  
*
* Return: 
*  (void)
*
* Theory: 
*
* Side Effects: 
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_PulseRed_Init(void) 
{
    VDAC8_PulseRed_CR0 = (VDAC8_PulseRed_MODE_V )  ;    

    /* Set default data source */
    if(VDAC8_PulseRed_DEFAULT_DATA_SRC != 0 )    
    {
        VDAC8_PulseRed_CR1 = (VDAC8_PulseRed_DEFAULT_CNTL | VDAC8_PulseRed_DACBUS_ENABLE) ;
    }
    else
    {
        VDAC8_PulseRed_CR1 = (VDAC8_PulseRed_DEFAULT_CNTL | VDAC8_PulseRed_DACBUS_DISABLE) ;
    } 
    
    /* Set default strobe mode */
    if(VDAC8_PulseRed_DEFAULT_STRB != 0)
    {
        VDAC8_PulseRed_Strobe |= VDAC8_PulseRed_STRB_EN ;
    }
    
    /* Set default range */
    VDAC8_PulseRed_SetRange(VDAC8_PulseRed_DEFAULT_RANGE); 
    
    /* Set default speed */
    VDAC8_PulseRed_SetSpeed(VDAC8_PulseRed_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: VDAC8_PulseRed_Enable
********************************************************************************
* Summary:
*     Enable the VDAC8
* 
* Parameters:  
*  void:  
*
* Return: 
*  (void)
*
* Theory: 
*
* Side Effects: 
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_PulseRed_Enable(void) 
{
    VDAC8_PulseRed_PWRMGR |= VDAC8_PulseRed_ACT_PWR_EN;
    VDAC8_PulseRed_STBY_PWRMGR |= VDAC8_PulseRed_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */

    #if (CY_PSOC5_ES1)
    if ( VDAC8_PulseRed_restoreVal == 1) 
    {
         VDAC8_PulseRed_CR0 = VDAC8_PulseRed_backup.data_value;
         VDAC8_PulseRed_restoreVal = 0;
    }
    #endif
}


/*******************************************************************************
* Function Name: VDAC8_PulseRed_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as
*  executing the stop function.
*
* Parameters:  
*  Power:   Sets power level between off (0) and (3) high power
*
* Return: 
*  (void) 
*
* Global variables:
*  VDAC8_PulseRed_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*
* Reenrancy: 
*  No
*
*******************************************************************************/
void VDAC8_PulseRed_Start(void) 
{

    /* Hardware initiazation only needs to occure the first time */
    if ( VDAC8_PulseRed_initVar == 0)  
    { 
        VDAC8_PulseRed_Init();
       
        VDAC8_PulseRed_initVar = 1;
    }
    
    /* Enable power to DAC */
    VDAC8_PulseRed_Enable();
    
    /* Set default value */
    VDAC8_PulseRed_SetValue(VDAC8_PulseRed_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: VDAC8_PulseRed_Stop
********************************************************************************
*
* Summary:
*  Powers down DAC to lowest power state.
*
* Parameters:  
*  (void)  
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_PulseRed_Stop(void) 
{
   /* Disble power to DAC */
   VDAC8_PulseRed_PWRMGR &= ~VDAC8_PulseRed_ACT_PWR_EN;
   VDAC8_PulseRed_STBY_PWRMGR &= ~VDAC8_PulseRed_STBY_PWR_EN;
   
   
    /* This is a work around for PSoC5  ,
    this sets VDAC to current mode with output off */
    
    #if (CY_PSOC5_ES1)
        VDAC8_PulseRed_backup.data_value = VDAC8_PulseRed_CR0;
        VDAC8_PulseRed_CR0 = VDAC8_PulseRed_CUR_MODE_OUT_OFF;
        VDAC8_PulseRed_restoreVal = 1;
    #endif
    
   
}


/*******************************************************************************
* Function Name: VDAC8_PulseRed_SetSpeed
********************************************************************************
*
* Summary:
*  Set DAC speed
*
* Parameters:  
*  power:   Sets speed value 
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_PulseRed_SetSpeed(uint8 speed) 
{
   /* Clear power mask then write in new value */
   VDAC8_PulseRed_CR0 &= ~VDAC8_PulseRed_HS_MASK ;    
   VDAC8_PulseRed_CR0 |=  ( speed & VDAC8_PulseRed_HS_MASK) ;    
}


/*******************************************************************************
* Function Name: VDAC8_PulseRed_SetRange
********************************************************************************
*
* Summary:
*  Set one of three current ranges.
*
* Parameters:  
*  Range:  Sets one of Three valid ranges.
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_PulseRed_SetRange(uint8 range) 
{
   VDAC8_PulseRed_CR0 &= ~VDAC8_PulseRed_RANGE_MASK ;      /* Clear existing mode */
   VDAC8_PulseRed_CR0 |= ( range & VDAC8_PulseRed_RANGE_MASK ) ; /*  Set Range  */
   VDAC8_PulseRed_DacTrim();
}


/*******************************************************************************
* Function Name: VDAC8_PulseRed_SetValue
********************************************************************************
*
* Summary:
*  Set 8-bit DAC value
*
* Parameters:  
*  value:  Sets DAC value between 0 and 255.
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*******************************************************************************/
void VDAC8_PulseRed_SetValue(uint8 value) 
{

   #if (CY_PSOC5_ES1)
       VDAC8_PulseRed_intrStatus = CyEnterCriticalSection();
   #endif /* CY_PSOC5_ES1 */
   
   VDAC8_PulseRed_Data = value;                             /*  Set Value  */
   
   /* PSOC3 silicons earlier to ES3 require a double write */
   #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
       VDAC8_PulseRed_Data = value;
   #endif /* CY_PSOC3_ES2 || CY_PSOC5_ES1 */
   
   #if (CY_PSOC5_ES1)
       CyExitCriticalSection(VDAC8_PulseRed_intrStatus);
   #endif /* CY_PSOC5_ES1 */
   
}


/*******************************************************************************
* Function Name: VDAC8_PulseRed_DacTrim
********************************************************************************
*
* Summary:
*  Set the trim value for the given range.
*
* Parameters:  
*   range:  1V or 4V range.  See constants.  
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_PulseRed_DacTrim(void) 
{
    uint8 mode;
    
    mode = ((VDAC8_PulseRed_CR0 & VDAC8_PulseRed_RANGE_MASK) >> 2) + VDAC8_PulseRed_TRIM_M7_1V_RNG_OFFSET;
   VDAC8_PulseRed_TR = CY_GET_XTND_REG8((uint8 *)(VDAC8_PulseRed_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
