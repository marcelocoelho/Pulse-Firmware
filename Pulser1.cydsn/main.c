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
#include <stdio.h>
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



Pulser g_Pulser;


void pulse_read_IR_handler()
{
	//Led_test1_Write(1);
	CyPins_SetPin(Pin_DebugLED_0);
	pulserProcessPulseSample(&g_Pulser);
	CyPins_ClearPin(Pin_DebugLED_0);
	//Led_test1_Write(0);
}
///////////////


extern uint8 CapSense_1_SensorEnableMask[];



#define BUFFER_LEN  64
uint16 count;
uint8 buffer[BUFFER_LEN];
char8 lineStr[20];
uint8 state;

static char buff[200];

void main()
{
    uint32 voltageRawCount;
	int usb_state=0;
    
	CapSense_1_SensorEnableMask[0]=0xFF;

	CyPins_SetPin(Pin_DebugLED_0);
	pulserInit(&g_Pulser);
	UART_Debug_Start();
	UART_Net_Start();
	USBUART_1_Start(0, USBUART_1_5V_OPERATION);
	UART_Debug_PutString("\r\nPulser 1.35\r\n");
	AMux_ProxIR_Start();
	ShiftReg_DelaySenseIR_Start();
    ADC_PulseIn_Start();     
	IDAC8_PulseIR_Start();
	IDAC8_PulseIR_SetValue(5);
	Opamp_PulseRef_Start();
	VDAC_PulseRef_Start();
	VDAC_PulseRef_SetValue(20);
	UART_Debug_PutString("at A\r\n");
	TIA_PulseIn_Start();
//    LCD_Start();            
	Filter_PulseInBand_Start();
	PWM_PulseLEDs_Start();
	PrISM_LEDCool_Start();
	PrISM_LEDWarm_Start();
	PrISM_LEDRed_Start();
	
	UART_Debug_PutString("at B\r\n");
	//CapSense_1_Start();
   // CapSense_1_InitializeAllBaselines();
   // CapSense_1_ScanEnabledWidgets();

	ADC_SAR_ProxIR_Start();
	ADC_SAR_ProxIR_StartConvert();
		
	PWM_PulseLEDs_WriteCompare2(1);
    //VDAC_Start();               
    //Opamp_Start(); 
    
    /* Move the LCD cursor to Row 0, Column 0 */
//    LCD_Position(ROW_0, COLUMN_0); 

    /* Print Label for the pot voltage raw count */
//    LCD_PrintString("P1 "); 
    
    //DMA_Chan = DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, 
    //HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    //DMA_TD[0] = CyDmaTdAllocate();
    //CyDmaTdSetConfiguration(DMA_TD[0], 116, DMA_INVALID_TD, TD_INC_SRC_ADR);
//    CyDmaTdSetAddress(DMA_TD[0], LO16((uint32)voltageWave), LO16((uint32)VDAC_Data_PTR));
    //CyDmaChSetInitialTd(DMA_Chan, DMA_TD[0]);
    //CyDmaChEnable(DMA_Chan, 1);
   
    /* Clock will make burst requests to the DMAC */
//    Clock_Start();
    
    ADC_PulseIn_StartConvert(); 
	UART_Debug_PutString("at C\r\n");

	CYGlobalIntEnable; 

	isr_PulseReadIR_Start();

	enum {IR_PROX_NUM=3};
	int cur_ir_prox=0;
	uint16 ProxIR[IR_PROX_NUM];

	AMux_ProxIR_Select(cur_ir_prox);
	UART_Debug_PutString("about to start main loop\r\n");
    while(1)
    {
	// do USB Comm stuff
		if (usb_state==0)
		{ // USB not enumerated yet
			//UART_Debug_PutChar('x');
		    /* Wait for Device to enumerate */
			if (USBUART_1_GetConfiguration())
			{
		    /* Enumeration is done, enable OUT endpoint for receive data from Host */
				UART_Debug_PutString("Enumerated CDC!\r\n");
		    	USBUART_1_CDC_Init();
				UART_Debug_PutString("inited CDC!\r\n");
				usb_state=1;
			}
		}
		else
		{  // USB enumerated, check for communication
		    if(USBUART_1_DataIsReady() != 0u)               /* Check for input data from PC */
        	{   
	            count = USBUART_1_GetAll(buffer);           /* Read received data and re-enable OUT endpoint */
				int ii;
				for (ii=0; ii < count; ii++)
				{
					UART_Debug_PutChar(buffer[ii]);
				}
	            if(0) // count != 0u)
	            {
	                while(USBUART_1_CDCIsReady() == 0u);    /* Wait till component is ready to send more data to the PC */ 
	                USBUART_1_PutData(buffer, count);       /* Send data back to PC */
	            }
	        }  

		
		}
	// end of USB Comm stuff
	// do Cap Prox Sensing
	    if(!CapSense_1_IsBusy())
        {
			int prox_val1;
			int prox_val2;
            /* Update baseline for all the sensors */
           // CapSense_UpdateEnabledBaselines();
            CapSense_1_ScanEnabledWidgets();
			prox_val1 = CapSense_1_ReadSensorRaw(0); // CapSense_PROXIMITYSENSOR0__PROX);
			prox_val2 = CapSense_1_ReadSensorRaw(1); // CapSense_PROXIMITYSENSOR0__PROX);
			sprintf(buff, "Cap Prox 1: %5d,  2: %5d\r", prox_val1, prox_val2);
			UART_Debug_PutString(buff);
		}	
	
	// end of Cap Prox Sensing
        /* Wait for end of conversion */
        //ADC_PulseIn_IsEndConversion(ADC_PulseIn_WAIT_FOR_RESULT); 
        //voltageRawCount = ADC_PulseIn_GetResult16(); 
 
        if (g_Pulser.updated)
		{
			IDAC8_PulseIR_SetValue( (g_Pulser.brightnessIR256 >> 8) );
			g_Pulser.updated=0;
//	        LCD_Position(ROW_1, 1); 
//	        LCD_PrintNumber(filtered_down);

//	        LCD_Position(ROW_1, 13); 
///	        LCD_PrintString("   ");
//	        LCD_Position(ROW_1, 13); 
//	        LCD_PrintNumber(g_Pulser.scaledPulseVal);

			//PrISM_PulseIndicator_WritePulse0(g_Pulser.scaledPulseVal);
			PWM_PulseLEDs_WriteCompare2(g_Pulser.scaledPulseVal>>1);

			ADC_SAR_ProxIR_IsEndConversion(ADC_SAR_ProxIR_WAIT_FOR_RESULT);
			ProxIR[cur_ir_prox]=4095-ADC_SAR_ProxIR_GetResult16(); // reverse sense for test pots
			
			cur_ir_prox++;
			if (cur_ir_prox >= IR_PROX_NUM)
			{
				cur_ir_prox=0;
			}
			AMux_ProxIR_Select(cur_ir_prox);
			ADC_SAR_ProxIR_IsEndConversion(ADC_SAR_ProxIR_WAIT_FOR_RESULT);
			voltageRawCount=g_Pulser.curRawPulseVal;

			//sprintf(buff, "Prox 1: %5d,  2: %5d,  3: %5d, pulse raw=%5lu\r", ProxIR[0], ProxIR[1], ProxIR[2], voltageRawCount);
			//UART_Debug_PutString(buff);

			PrISM_LEDCool_WritePulse0(ProxIR[0] >> 4);
			PrISM_LEDWarm_WritePulse0(ProxIR[1] >> 4);
			PrISM_LEDRed_WritePulse0(ProxIR[2] >> 4);
			
			

//			LCD_Position(ROW_0, 3); 
//	        LCD_PrintString("    ");
//	        LCD_Position(ROW_0, 3); 
//	        LCD_PrintNumber(prox1);
			
	        //LCD_Position(ROW_0, 3); 
	        //LCD_PrintString("-    ");
	        //LCD_Position(ROW_0, 4); 
	        //LCD_PrintNumber((uint16)(-g_Pulser.scaledPulseMin));

//	        LCD_Position(ROW_1, 7); 
//	        LCD_PrintString("    ");
//	        LCD_Position(ROW_1, 7); 
//	        LCD_PrintNumber(g_Pulser.scaledPulseMax);
			//sprintf(buff, "Pulse: Raw=%5ld, Filtered=%5ld\r", g_Pulser.curRawPulseVal, g_Pulser.curFilteredPulseVal);
			
			//sprintf(buff, "Pulse: raw=%5ld, bright=%5d, Filtered=%5ld, scaled=%5ld, min=%5ld, max=%5ld\r",
			// g_Pulser.curRawPulseVal,
			//g_Pulser.brightnessIR256, g_Pulser.curFilteredPulseVal, g_Pulser.scaledPulseVal, g_Pulser.scaledPulseMin, g_Pulser.scaledPulseMax );
			
	            /* Clear last characters */
//	        LCD_Position(ROW_0, COLUMN_11); 
//	        LCD_PrintString("     "); 
  //          LCD_Position(ROW_0,COLUMN_11); 
	//        LCD_PrintNumber(voltageRawCount);
		}
		//UART_Debug_PutString("\r");
    }
}

/* [] END OF FILE */
