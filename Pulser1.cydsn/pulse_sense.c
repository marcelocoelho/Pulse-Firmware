/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <device.h>
#include "pulse_sense.h"



void pulserInit(Pulser *pPulse)
{
	pPulse->curRawPulseVal=0;
	pPulse->curFilteredPulseVal=0;
	pPulse->updated=0;
	pPulse->brightnessIR256=PulserMinIR256;
}

void pulserProcessPulseSample(Pulser *pPulse)
{
	
	pPulse->curRawPulseVal=ADC_PulseIn_GetResult32() >> 2;
	
	if (pPulse->curRawPulseVal < PulserIRTargetLow)
	{ // too dim, crank it up
		if (pPulse->brightnessIR256 < PulserMaxIR256)
		{
			pPulse->brightnessIR256 +=42;
		}
	}
	if (pPulse->curRawPulseVal > PulserIRTargetHigh)
	{ // too dim, crank it up
		if (pPulse->brightnessIR256 > PulserMinIR256)
		{
			pPulse->brightnessIR256 -=42;
		}
	}
	
	pPulse->curFilteredPulseVal=Filter_PulseInBand_Read24(PulserChan0);

	Filter_PulseInBand_Write24(PulserChan0, pPulse->curRawPulseVal);

	pPulse->scaledPulseVal=pPulse->curFilteredPulseVal-PulserPulseMin;
	if (pPulse->scaledPulseVal < 0)
	{
		pPulse->scaledPulseVal=0;
	}
	if (pPulse->scaledPulseVal > 255)
	{
		pPulse->scaledPulseVal=255;
	}
	
	

	pPulse->updated=1;
}






/* [] END OF FILE */
