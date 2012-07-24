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
	pPulse->scaledPulseMin=PulserPulseMinOuter;
	pPulse->scaledPulseMax=PulserPulseMaxOuter;
	pPulse->updated=0;
	pPulse->brightnessIR256=PulserMinIR256;
}

void pulserProcessPulseSample(Pulser *pPulse)
{
	uint32 rawPulse=ADC_PulseIn_GetResult32();
	pPulse->curRawPulseVal= rawPulse >> 2;
	
	if (pPulse->curRawPulseVal < PulserIRTargetLow)
	{ // too dim, crank it up
		if (pPulse->brightnessIR256 < PulserMaxIR256)
		{
			pPulse->brightnessIR256 +=142;
		}
	}
	if (pPulse->curRawPulseVal > PulserIRTargetHigh)
	{ // too dim, crank it up
		if (pPulse->brightnessIR256 > PulserMinIR256)
		{
			pPulse->brightnessIR256 -=142;
		}
	}
	
	pPulse->curFilteredPulseVal=Filter_PulseInBand_Read24(PulserChan0);

	Filter_PulseInBand_Write24(PulserChan0, rawPulse);

	int32 curFP=pPulse->curFilteredPulseVal; // current filtered pulse;

	pPulse->scaledPulseVal=(255*(curFP-pPulse->scaledPulseMax))/(pPulse->scaledPulseMax-pPulse->scaledPulseMin);
	if (pPulse->scaledPulseVal < 0)
	{
		pPulse->scaledPulseVal=0;
	}
	if (pPulse->scaledPulseVal > 255)
	{
		pPulse->scaledPulseVal=255;
	}
	/////
	if (pPulse->scaledPulseMin > PulserPulseMinOuter && pPulse->scaledPulseMin > curFP )
	{
		pPulse->scaledPulseMin-=10; // =curFP;
		pPulse->lastMinRescaleTimer=PulseRescalTimeout;
	}
	
	if (pPulse->lastMinRescaleTimer > 0)
	{
		pPulse->lastMinRescaleTimer--;
	}
	if (pPulse->lastMinRescaleTimer==0)
	{
		if (pPulse->scaledPulseMin < PulserPulseMinInner)
		{
			pPulse->scaledPulseMin++;
		}
	}
	////
	if (pPulse->scaledPulseMax < PulserPulseMaxOuter && pPulse->scaledPulseMax < curFP )
	{
		pPulse->scaledPulseMax+=10; // =curFP;
		pPulse->lastMaxRescaleTimer=PulseRescalTimeout;
	}
	
	if (pPulse->lastMaxRescaleTimer > 0)
	{
		pPulse->lastMaxRescaleTimer--;
	}
	if (pPulse->lastMaxRescaleTimer==0)
	{
		if (pPulse->scaledPulseMax > PulserPulseMaxInner)
		{
			pPulse->scaledPulseMax--;
		}
	}
	//

	pPulse->updated=1;
}






/* [] END OF FILE */
