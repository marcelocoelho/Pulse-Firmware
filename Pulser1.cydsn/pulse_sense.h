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
	int32 scaledPulseVal;
	int32 scaledPulseMin;
	int32 scaledPulseMax;
	uint16 brightnessIR256; // fixed point >> 8
	uint16 lastMinRescaleTimer;
	uint16 lastMaxRescaleTimer;
	int updated;
} Pulser;
	
enum 
{
	PulserChan0=0, 
	PulserMaxIR256=240*256, PulserMinIR256=5*256, PulserIRTargetLow=14000, PulserIRTargetHigh=17000,
	PulserPulseMinOuter=-800, // -1999,
	PulserPulseMinInner=-400, // -99,
	PulserPulseMaxInner=400, // 99,
	PulserPulseMaxOuter=800, // 1999,
	PulseRescalTimeout=200   // 2 seconds at 100 Hz
};

void pulserInit(Pulser *pPulse);

void pulserProcessPulseSample(Pulser *pPulse);

//[] END OF FILE

