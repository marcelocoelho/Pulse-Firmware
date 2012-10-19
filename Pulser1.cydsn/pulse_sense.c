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
	
	pPulse->zeroCrossNextPulseMax=0;
	pPulse->zeroCrossNextPulseMin=0;
	pPulse->lastMaxFoundTimer=0;
	pPulse->crossedZero=0;
}


int going_down=0;

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
		else
			going_down=1;
	}
	if (pPulse->curRawPulseVal > PulserIRTargetHigh)
	{ // too dim, crank it up
		if (pPulse->brightnessIR256 > PulserMinIR256)
		{
			pPulse->brightnessIR256 -=142;
		}
		else
			going_down=0;
	}

	int32 lastFilteredPulseVal=pPulse->curFilteredPulseVal;

	pPulse->curFilteredPulseVal=-Filter_PulseInBand_Read24(PulserPulseChan); // convention is to have the 'pulsey' part positive

	Filter_PulseInBand_Write24(PulserPulseChan, rawPulse);

	int32 curFP=pPulse->curFilteredPulseVal; // current filtered pulse;

	if (  (lastFilteredPulseVal > 0 && pPulse->curFilteredPulseVal <= 0) ||
 	  	  (lastFilteredPulseVal < 0 && pPulse->curFilteredPulseVal >= 0)  )
	{
		pPulse->crossedZero=1;
	}

	pPulse->curPulseAGCLevel=Filter_PulseInBand_Read24(PulserAGCChan) >> 8;
	Filter_PulseInBand_Write24(PulserAGCChan, abs(curFP) << 8);

	pPulse->scaledPulseVal=(255*(curFP-pPulse->scaledPulseMin))/(pPulse->scaledPulseMax-pPulse->scaledPulseMin);
	if (pPulse->scaledPulseVal < 0)
	{
		pPulse->scaledPulseVal=0;
	}
	if (pPulse->scaledPulseVal > 255)
	{
		pPulse->scaledPulseVal=255;
	}
	
	//pPulse->scaledPulseMax=2*pPulse->curPulseAGCLevel;
	//pPulse->scaledPulseMin=-4*pPulse->curPulseAGCLevel;
	#if 0
	if ( curFP > pPulse->zeroCrossNextPulseMax )
	{
		pPulse->zeroCrossNextPulseMax=(400*curFP) >> 8;  // set to 150% of max
	}
	if ( curFP < pPulse->zeroCrossNextPulseMin)
	{
		pPulse->zeroCrossNextPulseMin=(400*curFP) >> 8;  // set to 150% of min
	}
	pPulse->lastMaxFoundTimer++;
	if (pPulse->crossedZero && pPulse->lastMaxFoundTimer > 300)
	{
		UART_Debug_PutString("\r\n*\r\n");
		pPulse->scaledPulseMax=pPulse->zeroCrossNextPulseMax;
		pPulse->scaledPulseMin=pPulse->zeroCrossNextPulseMin;
		
		pPulse->zeroCrossNextPulseMax=0;
		pPulse->zeroCrossNextPulseMin=0;
		pPulse->lastMaxFoundTimer=0;
		pPulse->crossedZero=0;
	}
	#endif
#if 0

	/////
	if (pPulse->scaledPulseMin > PulserPulseMinOuter && pPulse->scaledPulseMin > curFP )
	{
		pPulse->scaledPulseMin-=100; // =curFP;
		pPulse->lastMinRescaleTimer=PulseRescaleTimeout;
	}
	
	if (pPulse->lastMinRescaleTimer > 0)
	{
		pPulse->lastMinRescaleTimer--;
	}
	if (pPulse->lastMinRescaleTimer==0)
	{
		if (pPulse->scaledPulseMin < PulserPulseMinInner)
		{
			pPulse->scaledPulseMin+=10;
	//		pPulse->lastMinRescaleTimer=PulseRescaleTimeout;
		}
	}
	////
	if (pPulse->scaledPulseMax < PulserPulseMaxOuter && pPulse->scaledPulseMax < curFP )
	{
		pPulse->scaledPulseMax+=100; // =curFP;
		pPulse->lastMaxRescaleTimer=PulseRescaleTimeout;
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
	//		pPulse->lastMaxRescaleTimer=PulseRescaleTimeout;
		}
	}
	//
#endif
	pPulse->updated=1;
}






/* [] END OF FILE */
