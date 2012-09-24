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
	int32 curPulseAGCLevel;
	int32 scaledPulseVal;
	int32 scaledPulseMin;
	int32 scaledPulseMax;
	uint16 brightnessIR256; // fixed point >> 8 to use
	uint16 lastMinRescaleTimer;
	uint16 lastMaxRescaleTimer;
	int32 zeroCrossNextPulseMax;
	int32 zeroCrossNextPulseMin;
	int updated;
} Pulser;
	
enum 
{
	PulserPulseChan=0, 
	PulserAGCChan=1,
	PulserMaxIR256=240*256, PulserMinIR256=5*256, PulserIRTargetLow=27000, PulserIRTargetHigh=35000,
	PulserPulseMinOuter=-8000, // -800, // -1999,
	PulserPulseMinInner=-40, // -99,
	PulserPulseMaxInner=100, // 99,
	PulserPulseMaxOuter=8000, // 800, // 1999,
	PulseRescaleTimeout=400   // 4 seconds at 100 Hz
};

void pulserInit(Pulser *pPulse);

void pulserProcessPulseSample(Pulser *pPulse);

//[] END OF FILE

