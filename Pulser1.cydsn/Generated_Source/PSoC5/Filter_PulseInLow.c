/*******************************************************************************
* File Name: Filter_PulseInLow.c
* Version 2.0
*
* Description:
*  This file provides the API source code for the FILT component.
*
* Note:
*  
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Filter_PulseInLow_PVT.h"


/*******************************************************************************
* FILT component internal variables.
*******************************************************************************/

uint8 Filter_PulseInLow_initVar = 0u;


/*******************************************************************************
* Function Name: Filter_PulseInLow_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  Filter_PulseInLow_Start().
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void Filter_PulseInLow_Init(void) 
{
        /* Power on DFB before initializing the RAMs */
    Filter_PulseInLow_PM_ACT_CFG_REG |= Filter_PulseInLow_PM_ACT_MSK;

    /* Turn off Run Bit */
    Filter_PulseInLow_CR_REG &= ~Filter_PulseInLow_RUN_MASK;
                
    /* Enable the DFB RAMS */
    Filter_PulseInLow_RAM_EN_REG = Filter_PulseInLow_RAM_DIR_BUS;
        
    /* Put DFB RAM on the bus */
    Filter_PulseInLow_RAM_DIR_REG = Filter_PulseInLow_RAM_DIR_BUS;
        
    /* Write DFB RAMs */
    /* Control Store RAMs */
    memcpy( Filter_PulseInLow_CSA_RAM,
        Filter_PulseInLow_control, Filter_PulseInLow_CSA_RAM_SIZE); 
    memcpy(Filter_PulseInLow_CSB_RAM,
        Filter_PulseInLow_control, Filter_PulseInLow_CSB_RAM_SIZE); 
    /* CFSM RAM */
    memcpy(Filter_PulseInLow_CFSM_RAM,
        Filter_PulseInLow_cfsm, Filter_PulseInLow_CFSM_RAM_SIZE); 
    /* DAta RAMs */
    memcpy(Filter_PulseInLow_DA_RAM,
        Filter_PulseInLow_data_a, Filter_PulseInLow_DA_RAM_SIZE); 
    memcpy(Filter_PulseInLow_DB_RAM,
        Filter_PulseInLow_data_b, Filter_PulseInLow_DB_RAM_SIZE); 
    /* ACU RAM */
    memcpy(Filter_PulseInLow_ACU_RAM,
        Filter_PulseInLow_acu, Filter_PulseInLow_ACU_RAM_SIZE); 

    /* Take DFB RAM off the bus */
    Filter_PulseInLow_RAM_DIR_REG = Filter_PulseInLow_RAM_DIR_DFB;

    /* Set up interrupt and DMA events */
    Filter_PulseInLow_SetInterruptMode(Filter_PulseInLow_INIT_INTERRUPT_MODE);
    Filter_PulseInLow_SetDMAMode(Filter_PulseInLow_INIT_DMA_MODE);
        
    /* Clear any pending interrupts */
    /* Bits [2..0] of this register are readonly. */
    Filter_PulseInLow_SR_REG = 0xf8;   
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_Enable
********************************************************************************
*  
* Summary: 
*  Enables the DFB run bit.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void Filter_PulseInLow_Enable(void) 
{
    /* Power on DFB in Active mode */
    Filter_PulseInLow_PM_ACT_CFG_REG |= Filter_PulseInLow_PM_ACT_MSK;
        
    /* Power on DFB in Alternative Active mode */
    Filter_PulseInLow_PM_STBY_CFG_REG |= Filter_PulseInLow_PM_STBY_MSK;

    /* Turn on Run Bit */
    Filter_PulseInLow_CR_REG |= Filter_PulseInLow_RUN_MASK;
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_Start
********************************************************************************
*
* Summary:
*  This method does the prep work necessary to setup DFB.  This includes loading 
*
* Parameters:  
*  void
* 
* Return: 
*  void
*
* Global variables:
*  Filter_PulseInLow_initVar:  Used to check the initial configuration,
*  modified when this function is called for the first time.
*
* Reentrant:
*  No
*
* Note: 
*  Use Filter_PulseInLow_InterruptConfig to control which events trigger 
*  interrupts in the DFB. 
*
*******************************************************************************/
void Filter_PulseInLow_Start()
{
     /* If not Initialized then initialize all required hardware and software */
    if(Filter_PulseInLow_initVar == 0u)
    {
        Filter_PulseInLow_Init();
        Filter_PulseInLow_initVar = 1u;
    }

    /* Enable the DFB block */
    Filter_PulseInLow_Enable();
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_Stop
********************************************************************************
*
* Summary:
*  Turn off the run bit.  If DMA control is used to feed the channels, allow 
*  arguments to turn one of the TD channels off. 
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void Filter_PulseInLow_Stop() 
{
    Filter_PulseInLow_CR_REG &= ~(Filter_PulseInLow_RUN_MASK);

    /* Power off DFB in Active mode */
    Filter_PulseInLow_PM_ACT_CFG_REG &= ~Filter_PulseInLow_PM_ACT_MSK;
    
    /* Power off DFB in Alternative Active mode */
    Filter_PulseInLow_PM_STBY_CFG_REG &= ~Filter_PulseInLow_PM_STBY_MSK;
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_Read8
********************************************************************************
*
* Summary:
*  Get the value in one of the DFB Output Holding Registers 
*
* Parameters:  
*  channel:  Filter_PulseInLow_CHANNEL_A or Filter_PulseInLow_CHANNEL_B
*            
* Return: 
*  The most significant 8 bits of the current output value sitting in the 
*  selected channel's holding register or 0x00 for invalid channel numbers.
*
*******************************************************************************/
uint8 Filter_PulseInLow_Read8(uint8 channel) 
{
    uint8 value;

    if (channel == Filter_PulseInLow_CHANNEL_A)
    {
        value = Filter_PulseInLow_HOLDAH_REG;
    }
    else if (channel == Filter_PulseInLow_CHANNEL_B)
    {
        value = Filter_PulseInLow_HOLDBH_REG;
    }
    else
    {
        value = 0x0u;
    }
    return value;
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_Read16
********************************************************************************
*
* Summary:
*  Get the value in one of the DFB Output Holding Registers 
*
* Parameters:  
*  channel:  Filter_PulseInLow_CHANNEL_A or Filter_PulseInLow_CHANNEL_B
*            
* Return: 
*  The most significant 16 bits of the current output value sitting in the 
*  selected channel's holding register or 0x0000 for invalid channel numbers
*
* Note:
*  Order of the read is important.  On the read of the high byte, the DFB clears
*  the data ready bit.
*
*******************************************************************************/
#if defined(__C51__) || defined(__CX51__) 

    uint16 Filter_PulseInLow_Read16(uint8 channel) 
    {
        uint16 val;
    
        if (channel == Filter_PulseInLow_CHANNEL_A)
        {        
            val = Filter_PulseInLow_HOLDAM_REG;
            val |= (uint16)(Filter_PulseInLow_HOLDAH_REG) << 8;
        }
        else if (channel == Filter_PulseInLow_CHANNEL_B)
        {      
            val = Filter_PulseInLow_HOLDBM_REG;
            val |= (uint16)Filter_PulseInLow_HOLDBH_REG << 8;
        }
        else
        {
            val = 0x0u;
        }
        return val;
    }

#else

    uint16 Filter_PulseInLow_Read16(uint8 channel) 
    {
        uint16 val;

        if (channel == Filter_PulseInLow_CHANNEL_A)
        {        
            val = Filter_PulseInLow_HOLDA16_REG;
        }
        else if (channel == Filter_PulseInLow_CHANNEL_B)
        {      
            val = Filter_PulseInLow_HOLDB16_REG;
        }
        else
        {
            val = 0x0u;
        }
        return val;
    }

#endif /* defined(__C51__) || defined(__CX51__) */


/*******************************************************************************
* Function Name: Filter_PulseInLow_Read24
********************************************************************************
*
* Summary:
*  Get the value in one of the DFB Output Holding Registers 
*
* Parameters:  
*  channel:  Filter_PulseInLow_CHANNEL_A or Filter_PulseInLow_CHANNEL_B
*            
* Return: 
*  The current 24-bit output value sitting in the selected channel's
*  holding register or 0x00000000 for invalid channel numbers
*
* Note:
*  Order of the read is important.  On the read of the high byte, the DFB clears
*  the data ready bit.
*
*******************************************************************************/
#if defined(__C51__) || defined(__CX51__)

    uint32 Filter_PulseInLow_Read24(uint8 channel) 
    {
        uint32 val;
    
        if (channel == Filter_PulseInLow_CHANNEL_A)
        {        
            val = Filter_PulseInLow_HOLDA_REG;
            val |= (uint32)(Filter_PulseInLow_HOLDAM_REG) << 8;
            val |= (uint32)(Filter_PulseInLow_HOLDAH_REG) << 16;
            
            /* SignExtend */
            if(val & Filter_PulseInLow_SIGN_BIT)
                val |= Filter_PulseInLow_SIGN_BYTE;
        }
        else if (channel == Filter_PulseInLow_CHANNEL_B)
        {      
            val = Filter_PulseInLow_HOLDB_REG;
            val |= (uint32)Filter_PulseInLow_HOLDBM_REG << 8;
            val |= (uint32)Filter_PulseInLow_HOLDBH_REG << 16;
            
            /* SignExtend */
            if(val & Filter_PulseInLow_SIGN_BIT)
                val |= Filter_PulseInLow_SIGN_BYTE;
        }
        else
        {
            val = 0x0u;
        }
        return val;
    }

#else

    uint32 Filter_PulseInLow_Read24(uint8 channel) 
    {
        uint32 val;
         
        if (channel == Filter_PulseInLow_CHANNEL_A)
        {        
            val = Filter_PulseInLow_HOLDA24_REG;
        }
        else if (channel == Filter_PulseInLow_CHANNEL_B)
        {      
            val = Filter_PulseInLow_HOLDB24_REG;
        }
        else
        {
            val = 0x0u;
        }
        return val;
    }

#endif /* defined(__C51__) || defined(__CX51__) */


/*******************************************************************************
* Function Name: Filter_PulseInLow_Write8
********************************************************************************
*
* Summary:
*  Set the value in one of the DFB Input Staging Registers 
*
* Parameters:  
*  channel:  Use either Filter_PulseInLow_CHANNEL_A or 
*            Filter_PulseInLow_CHANNEL_B as arguments to the function.  
*  sample:   The 8-bit, right justified input sample. 
*
* Return: 
*  void
*
* Note:
*  Order of the write is important.  On the load of the high byte, the DFB sets
*  the input ready bit.
*
*******************************************************************************/
void Filter_PulseInLow_Write8(uint8 channel, uint8 sample) 
{
    if (channel == Filter_PulseInLow_CHANNEL_A)
    {
        Filter_PulseInLow_STAGEAH_REG = sample;
    }
    else if (channel == Filter_PulseInLow_CHANNEL_B)
    {
        Filter_PulseInLow_STAGEBH_REG = sample;
    }
    /* No Final else statement: No value is loaded on bad channel input */
}


/*******************************************************************************
* Function Name: Filter_PulseInLow_Write16
********************************************************************************
*
* Summary:
*  Set the value in one of the DFB Input Staging Registers 
*
* Parameters:  
*  channel:  Use either Filter_PulseInLow_CHANNEL_A or 
*            Filter_PulseInLow_CHANNEL_B as arguments to the function.  
*  sample:   The 16-bit, right justified input sample. 
*
* Return: 
*  void
*
* Note:
*  Order of the write is important.  On the load of the high byte, the DFB sets
*  the input ready bit.
*
*******************************************************************************/
#if defined(__C51__) || defined(__CX51__)

    void Filter_PulseInLow_Write16(uint8 channel, uint16 sample) 
    {
        /* Write the STAGE MSB reg last as it signals a complete wrote to the 
           DFB.*/
        if (channel == Filter_PulseInLow_CHANNEL_A)
        {
            Filter_PulseInLow_STAGEAM_REG = (uint8)(sample);
            Filter_PulseInLow_STAGEAH_REG = (uint8)(sample >> 8 );
        }
        else if (channel == Filter_PulseInLow_CHANNEL_B)
        {
            Filter_PulseInLow_STAGEBM_REG = (uint8)(sample);
            Filter_PulseInLow_STAGEBH_REG = (uint8)(sample >> 8);
        }
        /* No Final else statement: No value is loaded on bad channel input */
    }

#else

    void Filter_PulseInLow_Write16(uint8 channel, uint16 sample) 
    {
        if (channel == Filter_PulseInLow_CHANNEL_A)
        {
            Filter_PulseInLow_STAGEA16_REG = sample;
        }
        else if (channel == Filter_PulseInLow_CHANNEL_B)
        {
            Filter_PulseInLow_STAGEB16_REG = sample;
        }
        /* No Final else statement: No value is loaded on bad channel input */
    }

#endif /* defined(__C51__) || defined(__CX51__) */


/*******************************************************************************
* Function Name: Filter_PulseInLow_Write24
********************************************************************************
*
* Summary:
*  Set the value in one of the DFB Input Staging Registers 
*
* Parameters:  
*  channel:  Use either Filter_PulseInLow_CHANNEL_A or 
*            Filter_PulseInLow_CHANNEL_B as arguments to the function.  
*  sample:   The 24-bit, right justified input sample inside of a uint32. 
*
* Return: 
*  void
*
* Note:
*  Order of the write is important.  On the load of the high byte, the DFB sets
*  the input ready bit.
*
*******************************************************************************/
#if defined(__C51__) || defined(__CX51__)

    void Filter_PulseInLow_Write24(uint8 channel, uint32 sample) 
    {
        /* Write the STAGE LSB reg last as it signals a complete wrote to 
           the DFB.*/
        if (channel == Filter_PulseInLow_CHANNEL_A)
        {
            Filter_PulseInLow_STAGEA_REG  = (uint8)(sample);
            Filter_PulseInLow_STAGEAM_REG = (uint8)(sample >> 8 );
            Filter_PulseInLow_STAGEAH_REG = (uint8)(sample >> 16);
        }
        else if (channel == Filter_PulseInLow_CHANNEL_B)
        {
            Filter_PulseInLow_STAGEB_REG = (uint8)(sample);
            Filter_PulseInLow_STAGEBM_REG = (uint8)(sample >> 8);
            Filter_PulseInLow_STAGEBH_REG = (uint8)(sample >> 16);
        }
        /* No Final else statement: No value is loaded on bad channel input */
    }

#else

    void Filter_PulseInLow_Write24(uint8 channel, uint32 sample) 
    {
        if (channel == Filter_PulseInLow_CHANNEL_A)
        {
            Filter_PulseInLow_STAGEA24_REG = sample;
        }
        else if (channel == Filter_PulseInLow_CHANNEL_B)
        {
            Filter_PulseInLow_STAGEB24_REG = sample;
        }
        /* No Final else statement: No value is loaded on bad channel input */
    }

#endif /* defined(__C51__) || defined(__CX51__) */


/*******************************************************************************
* Function Name: Filter_PulseInLow_SetCoherency
********************************************************************************
*
* Summary:
*  Sets the DFB coherency register with the user provided input 
*
* Parameters:  
*  channel:  Filter_PulseInLow_CHANNEL_A or Filter_PulseInLow_CHANNEL_B
*  byteSelect:  High byte, Middle byte or Low byte as the key coherency byte.
*            
* Return: 
*  None.
*
*******************************************************************************/
void Filter_PulseInLow_SetCoherency(uint8 channel, uint8 byteSelect) 
{
    if (channel == Filter_PulseInLow_CHANNEL_A)
    {
        Filter_PulseInLow_COHER_REG &= ~(Filter_PulseInLow_STAGEA_COHER_MASK | Filter_PulseInLow_HOLDA_COHER_MASK);
        Filter_PulseInLow_COHER_REG |= byteSelect;
        Filter_PulseInLow_COHER_REG |= (byteSelect << 4);
    }
    else if (channel == Filter_PulseInLow_CHANNEL_B)
    {
        Filter_PulseInLow_COHER_REG &= ~(Filter_PulseInLow_STAGEB_COHER_MASK | Filter_PulseInLow_HOLDB_COHER_MASK);
        Filter_PulseInLow_COHER_REG |= byteSelect << 2;
        Filter_PulseInLow_COHER_REG |= (byteSelect << 6);
    }
}


/* [] END OF FILE */
