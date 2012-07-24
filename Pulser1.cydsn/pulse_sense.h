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


typedef struct
{
	int32 curRawPulseVal;
	int32 curFilteredPulseVal;
	int16 scaledPulseVal;
	uint16 brightnessIR256; // fixed point >> 8
	int updated;
} Pulser;
	
enum 
{
	PulserChan0=0, 
	PulserMaxIR256=240*256, PulserMinIR256=5*256, PulserIRTargetLow=14000, PulserIRTargetHigh=17000,
	PulserPulseMin=-156,
	PulserPulseMax=100
};

void pulserInit(Pulser *pPulse);

void pulserProcessPulseSample(Pulser *pPulse);

//[] END OF FILE

