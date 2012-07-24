/*******************************************************************************
* File Name: VDAC_PulseRef.c  
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
#include "VDAC_PulseRef.h"

#if (CY_PSOC5_ES1)
#include <CyLib.h>
#endif /* CY_PSOC5_ES1 */

/* Check to see if required defines such as CY_PSOC3 and CY_PSOC5 are available */
/* They are defined starting with cy_boot v2.30 */
#ifndef CY_PSOC3
#error Component VDAC8_v1_70 requires cy_boot v2.30 or later
#endif /* CY_PSOC3 */


uint8 VDAC_PulseRef_initVar = 0;

#if (CY_PSOC5_ES1)
uint8 VDAC_PulseRef_restoreVal = 0;
uint8 VDAC_PulseRef_intrStatus = 0u;
#endif /* CY_PSOC5_ES1 */

static VDAC_PulseRef_backupStruct VDAC_PulseRef_backup;


/*******************************************************************************
* Function Name: VDAC_PulseRef_Init
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
void VDAC_PulseRef_Init(void) 
{
    VDAC_PulseRef_CR0 = (VDAC_PulseRef_MODE_V )  ;    

    /* Set default data source */
    if(VDAC_PulseRef_DEFAULT_DATA_SRC != 0 )    
    {
        VDAC_PulseRef_CR1 = (VDAC_PulseRef_DEFAULT_CNTL | VDAC_PulseRef_DACBUS_ENABLE) ;
    }
    else
    {
        VDAC_PulseRef_CR1 = (VDAC_PulseRef_DEFAULT_CNTL | VDAC_PulseRef_DACBUS_DISABLE) ;
    } 
    
    /* Set default strobe mode */
    if(VDAC_PulseRef_DEFAULT_STRB != 0)
    {
        VDAC_PulseRef_Strobe |= VDAC_PulseRef_STRB_EN ;
    }
    
    /* Set default range */
    VDAC_PulseRef_SetRange(VDAC_PulseRef_DEFAULT_RANGE); 
    
    /* Set default speed */
    VDAC_PulseRef_SetSpeed(VDAC_PulseRef_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_Enable
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
void VDAC_PulseRef_Enable(void) 
{
    VDAC_PulseRef_PWRMGR |= VDAC_PulseRef_ACT_PWR_EN;
    VDAC_PulseRef_STBY_PWRMGR |= VDAC_PulseRef_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */

    #if (CY_PSOC5_ES1)
    if ( VDAC_PulseRef_restoreVal == 1) 
    {
         VDAC_PulseRef_CR0 = VDAC_PulseRef_backup.data_value;
         VDAC_PulseRef_restoreVal = 0;
    }
    #endif
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_Start
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
*  VDAC_PulseRef_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*
* Reenrancy: 
*  No
*
*******************************************************************************/
void VDAC_PulseRef_Start(void) 
{

    /* Hardware initiazation only needs to occure the first time */
    if ( VDAC_PulseRef_initVar == 0)  
    { 
        VDAC_PulseRef_Init();
       
        VDAC_PulseRef_initVar = 1;
    }
    
    /* Enable power to DAC */
    VDAC_PulseRef_Enable();
    
    /* Set default value */
    VDAC_PulseRef_SetValue(VDAC_PulseRef_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_Stop
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
void VDAC_PulseRef_Stop(void) 
{
   /* Disble power to DAC */
   VDAC_PulseRef_PWRMGR &= ~VDAC_PulseRef_ACT_PWR_EN;
   VDAC_PulseRef_STBY_PWRMGR &= ~VDAC_PulseRef_STBY_PWR_EN;
   
   
    /* This is a work around for PSoC5  ,
    this sets VDAC to current mode with output off */
    
    #if (CY_PSOC5_ES1)
        VDAC_PulseRef_backup.data_value = VDAC_PulseRef_CR0;
        VDAC_PulseRef_CR0 = VDAC_PulseRef_CUR_MODE_OUT_OFF;
        VDAC_PulseRef_restoreVal = 1;
    #endif
    
   
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_SetSpeed
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
void VDAC_PulseRef_SetSpeed(uint8 speed) 
{
   /* Clear power mask then write in new value */
   VDAC_PulseRef_CR0 &= ~VDAC_PulseRef_HS_MASK ;    
   VDAC_PulseRef_CR0 |=  ( speed & VDAC_PulseRef_HS_MASK) ;    
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_SetRange
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
void VDAC_PulseRef_SetRange(uint8 range) 
{
   VDAC_PulseRef_CR0 &= ~VDAC_PulseRef_RANGE_MASK ;      /* Clear existing mode */
   VDAC_PulseRef_CR0 |= ( range & VDAC_PulseRef_RANGE_MASK ) ; /*  Set Range  */
   VDAC_PulseRef_DacTrim();
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_SetValue
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
void VDAC_PulseRef_SetValue(uint8 value) 
{

   #if (CY_PSOC5_ES1)
       VDAC_PulseRef_intrStatus = CyEnterCriticalSection();
   #endif /* CY_PSOC5_ES1 */
   
   VDAC_PulseRef_Data = value;                             /*  Set Value  */
   
   /* PSOC3 silicons earlier to ES3 require a double write */
   #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)
       VDAC_PulseRef_Data = value;
   #endif /* CY_PSOC3_ES2 || CY_PSOC5_ES1 */
   
   #if (CY_PSOC5_ES1)
       CyExitCriticalSection(VDAC_PulseRef_intrStatus);
   #endif /* CY_PSOC5_ES1 */
   
}


/*******************************************************************************
* Function Name: VDAC_PulseRef_DacTrim
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
void VDAC_PulseRef_DacTrim(void) 
{
    uint8 mode;
    
    mode = ((VDAC_PulseRef_CR0 & VDAC_PulseRef_RANGE_MASK) >> 2) + VDAC_PulseRef_TRIM_M7_1V_RNG_OFFSET;
   VDAC_PulseRef_TR = CY_GET_XTND_REG8((uint8 *)(VDAC_PulseRef_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
