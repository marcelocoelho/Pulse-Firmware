/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*  This is source code for example project of the 4ch dual SAR with SPI Master
*  and USBUART.
*  
* Parameters used:
*  SPI Master
*   Mode                0(CPHA ==0, CPOL ==0)                
*   Data lines          MOSI+MISO
*   Shift direction     MSB First
*   DataBits            16 
*   Bit Rate            1Mbit/s
*   Clock source        External 
*
*  USBFS
*	Configured as a CDC device (see datasheet and USBUART example project)
*	Note that for USB operation, a 24MHz crystal needs to be installed on 
*	processor module for CY8CKIT-001.
*
*  SAR ADCs
*  	Sample mode	 		Triggered
* 	Clock source 		Internal
*	Resolution			12 bits
*	Conversion Rate		100000 SPS
*	Input range			+/- Vdda (Differential)
*	Reference			Internal Vref bypassed
*	Bypass Capacitors	10uF each
*
*  IDAC
*	Polarity			Positive
*	Range				0-31.875 uA
*	Speed				Slow
*	Value				10uA
*
*  Analog Mux
*	Mux type			Differential
*	Channels			4
*	
*  Clocking
*	IMO source			24 MHz Xtal
*	USB					48 MHz (IMO*2)
*	ILO					100 kHz
*
* In this project, data input to two 4ch SAR ADCs via two 4ch analog muxes
* is converted and sent to a USB host device and a SPI slave device. To 
* accomplish this, the central PSoC device implements a SPI Master and USB UART
* for communication. The USB host device can select which channel of the mux is
* to be read. 
*
* To access the PSoC using the USBUART follow these steps:
*	1. Connect CY8CKIT-001 to the PC using USB cable.
*	2. Select the USB_UART.inf file from the project directory, as the driver 
*      for this example once Windows asks for it. 
*	3. Go to Start > Devices and Printers and identify COM port number 
*	   associated with the project.
*	4. Open a terminal emulation software and connect to COM port number
*	   identified in previous step.
*	5. The input window will open with a blank screen. Type in a channel value.
*
*******************************************************************************/

#include <device.h>
#include <stdio.h>

#define RD_BUFFER_LEN  		64
#define WR_BUFFER_LEN  		64
#define MUX_SIZE			4

/* ASCII value of decimal zero is 48 */
#define ASCII_DECIMAL_ZERO	48

/* Any value above 0x07 is an illegal input for mux channel selection */
#define ERROR_MASK 			0xF8

/* Function that encapsulates the process of writing text strings to USBUART */
void PrintToUSBUART(char8 * outText);

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  main() performs following functions:
*   1. Starts all used components.
*   2. Enumerates and handles data transfer between PC and PSoC via USBUART.
*	3. Chooses mux channel to be read according to USB UART data .
*   4. Reads ADC converted data, sends this data to SPI SLAVE via a SPI Master
*	   and also back to the USBUART.
*   5. Displays ADC converted data on Character LCD for debugging.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void main()
{
    /* adcReading holds the converted 16-bit data from the ADC */
	uint16 adcReading;
    uint8 rdBuffer[RD_BUFFER_LEN];
	uint8 wrBuffer[WR_BUFFER_LEN];
	uint8 rdValue;
	uint8 temp;
	
	/* Enable Global interrupts - used for USB communication */
    CyGlobalIntEnable;
	
	UART_Debug_Start();
	
	USBUART_Central_Start(0, USBUART_Central_5V_OPERATION);
	Clock_1_Start();
	PrISM_1_Start();
	PrISM_2_Start();
	PrISM_3_Start();
	PrISM_4_Start();
	PrISM_5_Start();
	PrISM_6_Start();
	
	PrISM_7_Start();
	PrISM_8_Start();
	PrISM_9_Start();
	PrISM_10_Start();
	PrISM_11_Start();
	PrISM_12_Start();
	
	PrISM_13_Start();
	PrISM_14_Start();

	Pin_Debug_LED_Write(1);
	Pin_Panel_LED_0_Write(1);
	Pin_Panel_LED_1_Write(0);
	Pin_Panel_LED_2_Write(1);

	UART_Debug_PutString("The Brain 0.9876 is Thinking!\r\n");
	/* Wait for Device to enumerate */

 
	
	int USB_configured=0;
	int blue0_val=0;
	int warm0_val=85;
	int cool0_val=170;
	int big_count=0;
	int blink=0;
	for(;;)
	{
		if (!USB_configured)
		{
	    	if (USBUART_Central_GetConfiguration())
			{
			   /* Enumeration is complete, enable OUT endpoint for received data from Host */
    		USBUART_Central_CDC_Init();
			UART_Debug_PutString("\r\nUSB Configured!\r\n");
				USB_configured=1;
			}
		}
		PrISM_1_WritePulse0(warm0_val);
		PrISM_1_WritePulse1(cool0_val);
		PrISM_2_WritePulse0(blue0_val);
		
		big_count++;
		if (big_count > 100000)
		{
			big_count=0;
			Pin_Panel_LED_0_Write(blink);
			if (blink==0)
				blink=1;
			else
				blink=0;

			warm0_val++;
			if (warm0_val > 255)
				warm0_val=0;
				
			cool0_val++;
			if (cool0_val > 255)
				cool0_val=0;
				
			blue0_val++;
			if (blue0_val > 255)
				blue0_val=0;
		}
			
	UART_Debug_PutString("Think! Think!\r");
		if (USB_configured)	
		{
			UART_Debug_PutString("\r\nGetting USB data\r\n");
			PrintToUSBUART("Please choose the channel (0-7) \n\r");
		
		/* Wait for input data from PC */
			while(USBUART_Central_DataIsReady() == 0u);                
			USBUART_Central_GetAll(rdBuffer);
		}
		
		/* Convert ASCII value in rdBuffer to numerical value:
		 * Note that ASCII 48, 49,...,57 (decimal) corresponds to 0,1,...,9
		 */
		rdValue = rdBuffer[0] - ASCII_DECIMAL_ZERO;
		
		
	}  /* End of forever loop */
}  /* End of main */

/*******************************************************************************
* Function Name: PrintToUSBUART
********************************************************************************
* Summary:
*  The input string passed to the function is printed to the USBUART
*  interface when the CDC device is ready.
*
* Parameters:
*  Text to be printed to USBUART as a string.
*
* Return:
*  None.
*
*******************************************************************************/

void PrintToUSBUART(char8 * outText)
{
	/* Wait till the CDC device is ready before sending data */
	while(USBUART_Central_CDCIsReady() == 0u);
	/* Send strlen number of characters of wrBuffer to USBUART */
	USBUART_Central_PutData((uint8 *)outText, strlen(outText));
}


/* [] END OF FILE */
