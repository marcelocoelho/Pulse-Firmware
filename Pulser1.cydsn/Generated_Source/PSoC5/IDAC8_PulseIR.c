/*******************************************************************************
* File Name: IDAC8_PulseIR.c  
* Version 1.80
*
*  Description:
*    This file provides the source code to the API for the 8-bit Current 
*    DAC (IDAC8) User Module.
*
*   Note:
*     
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "IDAC8_PulseIR.h"   

#if (CY_PSOC5_ES1)
#include <CyLib.h>
#endif /* CY_PSOC5_ES1 */

uint8 IDAC8_PulseIR_initVar = 0u;


static IDAC8_PulseIR_backupStruct  IDAC8_PulseIR_backup;

/* Variable to decide whether or not to restore control register in Enable()
   API. This valid only for PSoC5 ES1 */
#if (CY_PSOC5_ES1)
uint8 IDAC8_PulseIR_restoreReg = 0u;
uint8 IDAC8_PulseIR_intrStatus = 0u;
#endif /* CY_PSOC5_ES1 */


/*******************************************************************************
* Function Name: IDAC8_PulseIR_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
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
*******************************************************************************/
void IDAC8_PulseIR_Init(void) 
{
    IDAC8_PulseIR_CR0 = (IDAC8_PulseIR_MODE_I | IDAC8_PulseIR_DEFAULT_RANGE )  ;    

    /* Set default data source */
    if(IDAC8_PulseIR_DEFAULT_DATA_SRC != 0u )    
    {
        IDAC8_PulseIR_CR1 = (IDAC8_PulseIR_DEFAULT_CNTL | IDAC8_PulseIR_DACBUS_ENABLE) ;   
    }
    else
    {
        IDAC8_PulseIR_CR1 = (IDAC8_PulseIR_DEFAULT_CNTL | IDAC8_PulseIR_DACBUS_DISABLE) ;   
    } 
    
    /*Controls polarity from UDB Control*/
    if(IDAC8_PulseIR_DEFAULT_POLARITY == IDAC8_PulseIR_HARDWARE_CONTROLLED)
    {
         IDAC8_PulseIR_CR1 |= IDAC8_PulseIR_IDIR_SRC_UDB;
    }
    else
    {
        IDAC8_PulseIR_CR1 |= IDAC8_PulseIR_DEFAULT_POLARITY;
    }
    /*Controls Current Source from UDB Control*/
    if(IDAC8_PulseIR_HARDWARE_ENABLE != 0u ) 
    {
        IDAC8_PulseIR_CR1 |= IDAC8_PulseIR_IDIR_CTL_UDB;
    }
    
    /* Set default strobe mode */
    if(IDAC8_PulseIR_DEFAULT_STRB != 0u)
    {
        IDAC8_PulseIR_Strobe |= IDAC8_PulseIR_STRB_EN ;
    }
    
    /* Set default speed */
    IDAC8_PulseIR_SetSpeed(IDAC8_PulseIR_DEFAULT_SPEED); 
    
    /* Set proper DAC trim */
    IDAC8_PulseIR_DacTrim();  
    
}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_Enable
********************************************************************************
* Summary:
*     Enable the IDAC8
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
*******************************************************************************/
void IDAC8_PulseIR_Enable(void) 
{
    IDAC8_PulseIR_PWRMGR |= IDAC8_PulseIR_ACT_PWR_EN;
    IDAC8_PulseIR_STBY_PWRMGR |= IDAC8_PulseIR_STBY_PWR_EN;

    /* This is to restore the value of register CR0 which is saved 
      in prior to the modification in stop() API */
    #if (CY_PSOC5_ES1)
    if(IDAC8_PulseIR_restoreReg == 1u)
    {
        IDAC8_PulseIR_CR0 = IDAC8_PulseIR_backup.DACCR0Reg;

        /* Clear the flag */
        IDAC8_PulseIR_restoreReg = 0u;
    }
    #endif /* CY_PSOC5_ES1 */
}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_Start
********************************************************************************
* Summary:
*  Set power level then turn on IDAC8.
*
* Parameters:  
*  power:   Sets power level between off (0) and (3) high power
*
* Return: 
*  (void) 
*
* Global variables:
*  IDAC8_PulseIR_initVar: Is modified when this function is called for 
*   the first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void IDAC8_PulseIR_Start(void) 
{
    /* Hardware initiazation only needs to occur the first time */
    if ( IDAC8_PulseIR_initVar == 0u)  
    {     
        IDAC8_PulseIR_Init();
       
        IDAC8_PulseIR_initVar = 1;
    }  
   
    /* Enable power to DAC */
    IDAC8_PulseIR_Enable();

    /* Set default value */
    IDAC8_PulseIR_SetValue(IDAC8_PulseIR_DEFAULT_DATA);

}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_Stop
********************************************************************************
* Summary:
*  Powers down IDAC8 to lowest power state.
*
* Parameters:  
*   (void)
*
* Return: 
*  (void)
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_PulseIR_Stop(void) 
{
   /* Disble power to DAC */
   IDAC8_PulseIR_PWRMGR &= ~IDAC8_PulseIR_ACT_PWR_EN;
   IDAC8_PulseIR_STBY_PWRMGR &= ~IDAC8_PulseIR_STBY_PWR_EN;
   
   #if (CY_PSOC5_ES1)
   
        /* Set the global variable  */
        IDAC8_PulseIR_restoreReg = 1u;

        /* Save the control register and then Clear it. */
        IDAC8_PulseIR_backup.DACCR0Reg = IDAC8_PulseIR_CR0;
        IDAC8_PulseIR_CR0 = (IDAC8_PulseIR_MODE_I | IDAC8_PulseIR_RANGE_3 | IDAC8_PulseIR_HS_HIGHSPEED);
    #endif /* CY_PSOC5_ES1 */
}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_SetSpeed
********************************************************************************
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
*******************************************************************************/
void IDAC8_PulseIR_SetSpeed(uint8 speed) 
{
   /* Clear power mask then write in new value */
   IDAC8_PulseIR_CR0 &= ~IDAC8_PulseIR_HS_MASK ;    
   IDAC8_PulseIR_CR0 |=  ( speed & IDAC8_PulseIR_HS_MASK) ;    
}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_SetPolarity
********************************************************************************
* Summary:
*  Sets IDAC to Sink or Source current.
*  
* Parameters:  
*  Polarity: Sets the IDAC to Sink or Source 
*   0x00 - Source
*   0x04 - Sink
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
#if(IDAC8_PulseIR_DEFAULT_POLARITY != IDAC8_PulseIR_HARDWARE_CONTROLLED)
void IDAC8_PulseIR_SetPolarity(uint8 polarity) 
{
    IDAC8_PulseIR_CR1 &= ~IDAC8_PulseIR_IDIR_MASK;                /* clear polarity bit */
    IDAC8_PulseIR_CR1 |= (polarity & IDAC8_PulseIR_IDIR_MASK);    /* set new value */
    
    IDAC8_PulseIR_DacTrim();
}
#endif

/*******************************************************************************
* Function Name: IDAC8_PulseIR_SetRange
********************************************************************************
* Summary:
*  Set current range
*
* Parameters:  
*  Range:  Sets on of four valid ranges.
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_PulseIR_SetRange(uint8 range) 
{
   IDAC8_PulseIR_CR0 &= ~IDAC8_PulseIR_RANGE_MASK ;       /* Clear existing mode */
   IDAC8_PulseIR_CR0 |= ( range & IDAC8_PulseIR_RANGE_MASK );  /*  Set Range  */
   IDAC8_PulseIR_DacTrim();
}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_SetValue
********************************************************************************
* Summary:
*  Set DAC value
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
*******************************************************************************/
void IDAC8_PulseIR_SetValue(uint8 value) 
{

   #if (CY_PSOC5_ES1)
       IDAC8_PulseIR_intrStatus = CyEnterCriticalSection();
   #endif /* CY_PSOC5_ES1 */

   IDAC8_PulseIR_Data = value;         /*  Set Value  */
   
   /* PSOC3 silicons earlier to ES3 require a double write */
   #if (CY_PSOC3_ES2 ||CY_PSOC5_ES1 )
       IDAC8_PulseIR_Data = value;
   #endif /* CY_PSOC3_ES2 ||CY_PSOC5_ES1  */
   
   #if (CY_PSOC5_ES1)
       CyExitCriticalSection(IDAC8_PulseIR_intrStatus);
   #endif /* CY_PSOC5_ES1 */
}


/*******************************************************************************
* Function Name: IDAC8_PulseIR_DacTrim
********************************************************************************
* Summary:
*  Set the trim value for the given range.
*
* Parameters:  
*  None
*
* Return: 
*  (void) 
*
* Theory: 
*  Trim values for the IDAC blocks are stored in the "Customer Table" area in 
*  Row 1 of the Hidden Flash.  There are 8 bytes of trim data for each 
*  IDAC block.
*  The values are:
*       I Gain offset, min range, Sourcing
*       I Gain offset, min range, Sinking
*       I Gain offset, med range, Sourcing
*       I Gain offset, med range, Sinking
*       I Gain offset, max range, Sourcing
*       I Gain offset, max range, Sinking
*       V Gain offset, 1V range
*       V Gain offset, 4V range
*
* Side Effects:
*
*******************************************************************************/
void IDAC8_PulseIR_DacTrim(void) 
{
    uint8 mode;

    mode = ((IDAC8_PulseIR_CR0 & IDAC8_PulseIR_RANGE_MASK) >> 1);
    
    if((IDAC8_PulseIR_IDIR_MASK & IDAC8_PulseIR_CR1) == IDAC8_PulseIR_IDIR_SINK)
    {
        mode++;
    }

    IDAC8_PulseIR_TR = CY_GET_XTND_REG8((uint8 *)(IDAC8_PulseIR_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
