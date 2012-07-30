/*******************************************************************************
* File Name: main.c
* Version 1.1
*
* Description:
* This file contains the main function for the Pulser1 application
*
* Note:
*
* Code tested with:
* PSoC Creator: 2.0
* Device Tested With: CY8C5568AXI-060
* Compiler    : ARMGCC 4.4.1, ARM RVDS Generic, ARM MDK Generic
*
********************************************************************************
* Copyright (2011), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. Use may be 
* limited by and subject to the applicable Cypress software license agreement. 
*******************************************************************************/
/*******************************************************************************
*                           THEORY OF OPERATION 
*  This file provides source code for the ADC to LCD with DAC example 
*  project. The firmware takes a voltage output from a potentiometer and
*  displays the ADC raw count on an LCD. The raw count also determines the 
*  clock divider value of the clock driving the DMA controller.  
*  A table of voltage values are sent to the DAC via DMA.  Then the output of 
*  the DAC goes through an analog buffer and to an LED. This will give a 
*  dimming/brightening affect on the LED at a really slow rate or a really fast 
*  rate depending on the raw count coming from the ADC.
*******************************************************************************/
#include <device.h>
   
#include "pulse_sense.h"   
   
/* LCD specific */
#define ROW_0                   0  
#define ROW_1                   1  
#define COLUMN_0                0  
#define COLUMN_9                9  
#define COLUMN_10               10  
#define COLUMN_11               11  
#define COLUMN_12               12  
#define COLUMN_13               13  
#define COLUMN_14               14  

/* For clearing Tens and Hundreds place */
#define CLEAR_TENS_HUNDREDS     "       "

/* For clearing Hundreds place */
#define CLEAR_HUNDREDS          "       "

/* DMA Configuration for DMA */
#define DMA_BYTES_PER_BURST     1
#define DMA_REQUEST_PER_BURST   1
#define DMA_SRC_BASE            (voltageWave)
#define DMA_DST_BASE            (CYDEV_PERIPH_BASE)

/* Variable declarations for DMA */
uint8 DMA_Chan;
uint8 DMA_TD[1];

/* Table of voltage values for DMA to send to the DAC. These values range 
 * between 0x3D and 0x9F because these are the two points where the LED 
 * is not visible and where the LED is saturated */
const uint8 voltageWave[] = 
{
 	0x6D, 0x6F, 0x71, 0x73, 0x75, 0x77, 0x79, 0x7B, 0x7D, 0x7F, 0x81, 0x83, 0x85, 
	0x87, 0x89, 0x8B, 0x8D, 0x8F, 0x91, 0x93, 0x95, 0x97, 0x99, 0x9B, 0x9C, 0x9D, 0x9D, 0x9E, 
	0x9E, 0x9F, 0x9F, 0x9F, 0x9E, 0x9E, 0x9E, 0x9C, 0x9C, 0x9B, 0x99, 0x97, 0x95, 0x93, 0x91, 
	0x8F, 0x8D, 0x8B, 0x89, 0x87, 0x85, 0x83, 0x81, 0x7F, 0x7D, 0x7B, 0x79, 0x77, 
	0x75, 0x73, 0x71, 0x6F, 0x6D, 0x6B, 0x69, 0x67, 0x65, 0x63, 0x61, 0x5F, 0x5D, 
	0x5B, 0x59, 0x57, 0x55, 0x53, 0x51, 0x4F, 0x4D, 0x4B, 0x49, 0x47, 0x45, 0x43, 
	0x41, 0x40, 0x40, 0x3F, 0x3F, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3F, 0x41, 
	0x43, 0x45, 0x47, 0x49, 0x4B, 0x4D, 0x4F, 0x51,
	0x53, 0x55, 0x57, 0x59, 0x5B, 0x5D, 0x5F, 0x61, 0x63, 0x65, 0x67, 0x69, 0x6B
};



Pulser g_Pulser;


void pulse_read_IR_handler()
{
	//Led_test1_Write(1);
	CyPins_SetPin(Led_test1_0);
	pulserProcessPulseSample(&g_Pulser);
	CyPins_ClearPin(Led_test1_0);
	//Led_test1_Write(0);
}



void main()
{
    int16 voltageRawCount;
    
	pulserInit(&g_Pulser);
	
	AMux_ProxIR_Start();
	ShiftReg_DelaySenseIR_Start();
    ADC_PulseIn_Start();     
	IDAC8_PulseIR_Start();
	IDAC8_PulseIR_SetValue(5);
	Opamp_PulseRef_Start();
	VDAC_PulseRef_Start();
	VDAC_PulseRef_SetValue(20);
	TIA_PulseIn_Start();
    LCD_Start();            
	Filter_PulseInBand_Start();
	PWM_PulseLEDs_Start();
	PrISM_PulseIndicator_Start();
	
	ADC_SAR_ProxIR_Start();
	ADC_SAR_ProxIR_StartConvert();
	
	PrISM_PulseIndicator_WritePulse0(250);
    //VDAC_Start();               
    //Opamp_Start(); 
    
    /* Move the LCD cursor to Row 0, Column 0 */
    LCD_Position(ROW_0, COLUMN_0); 

    /* Print Label for the pot voltage raw count */
    LCD_PrintString("P1 "); 
    
    //DMA_Chan = DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, 
    //HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    //DMA_TD[0] = CyDmaTdAllocate();
    //CyDmaTdSetConfiguration(DMA_TD[0], 116, DMA_INVALID_TD, TD_INC_SRC_ADR);
//    CyDmaTdSetAddress(DMA_TD[0], LO16((uint32)voltageWave), LO16((uint32)VDAC_Data_PTR));
    //CyDmaChSetInitialTd(DMA_Chan, DMA_TD[0]);
    //CyDmaChEnable(DMA_Chan, 1);
   
    /* Clock will make burst requests to the DMAC */
    Clock_Start();
    
    ADC_PulseIn_StartConvert(); 
	
	CYGlobalIntEnable; 

	isr_PulseReadIR_Start();

	AMux_ProxIR_Select(0);
    while(1)
    {
        /* Wait for end of conversion */
        //ADC_PulseIn_IsEndConversion(ADC_PulseIn_WAIT_FOR_RESULT); 
        //voltageRawCount = ADC_PulseIn_GetResult16(); 
 
        if (g_Pulser.updated)
		{
			IDAC8_PulseIR_SetValue( (g_Pulser.brightnessIR256 >> 8) );

			uint16 filtered_down;
	        LCD_Position(ROW_1, 0); 
			if (g_Pulser.curFilteredPulseVal < 0)
			{
				filtered_down=(-g_Pulser.curFilteredPulseVal) >> 2;
		        LCD_PrintString("-       ");
			}
			else
			{
				filtered_down=g_Pulser.curFilteredPulseVal >> 2;
		        LCD_PrintString("        ");
			}				
			g_Pulser.updated=0;
	        LCD_Position(ROW_1, 1); 
	        LCD_PrintNumber(filtered_down);

	        LCD_Position(ROW_1, 13); 
	        LCD_PrintString("   ");
	        LCD_Position(ROW_1, 13); 
	        LCD_PrintNumber(g_Pulser.scaledPulseVal);
			
			PrISM_PulseIndicator_WritePulse0(g_Pulser.scaledPulseVal);

			voltageRawCount=g_Pulser.curRawPulseVal;
			
			uint16 prox1=ADC_SAR_ProxIR_GetResult16();

			LCD_Position(ROW_0, 3); 
	        LCD_PrintString("    ");
	        LCD_Position(ROW_0, 3); 
	        LCD_PrintNumber(prox1);
			
	        //LCD_Position(ROW_0, 3); 
	        //LCD_PrintString("-    ");
	        //LCD_Position(ROW_0, 4); 
	        //LCD_PrintNumber((uint16)(-g_Pulser.scaledPulseMin));

	        LCD_Position(ROW_1, 7); 
	        LCD_PrintString("    ");
	        LCD_Position(ROW_1, 7); 
	        LCD_PrintNumber(g_Pulser.scaledPulseMax);
			
	            /* Clear last characters */
	        LCD_Position(ROW_0, COLUMN_11); 
	        LCD_PrintString("     "); 
            LCD_Position(ROW_0,COLUMN_11); 
	        LCD_PrintNumber(voltageRawCount);

		}
    }
}

/* [] END OF FILE */
