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

// update rate is 100 Hz
enum {pulse_history_len=60, pulse_averager_ratio=5}; // 3.0 seconds, downsampled by 5

typedef struct
{
	int32 curRawPulseVal;
	int32 curFilteredPulseVal;
	int32 curPulseAGCLevel;
	int32 scaledPulseVal;
	int32 scaledPulseMin;
	int32 scaledPulseMax;
	uint16 brightnessIR256; // fixed point >> 8 to use
	
	int32 pulse_max_history[pulse_history_len];
	int32 pulse_min_history[pulse_history_len];
	int pulse_history_index;
	int32 pulse_average_count;
	int32 pulse_max_average;
	int32 pulse_min_average;
	
	uint16 lastMinRescaleTimer;
	uint16 lastMaxRescaleTimer;
	int32 zeroCrossNextPulseMax;
	int32 zeroCrossNextPulseMin;
	uint32 lastMaxFoundTimer;
	int crossedZero;
	int updated;
} Pulser;
	
enum 
{
	PulserPulseChan=0, 
	PulserAGCChan=1,
	PulserMaxIR256=240*256, PulserMinIR256=5*256, PulserIRTargetLow=27000, PulserIRTargetHigh=35000,
	PulserPulseMinOuter=-300,  // -1000, // -800, // -1999,
	PulserPulseMinInner=-40, // -99,
	PulserPulseMaxInner=100, // 99,
	PulserPulseMaxOuter=600, //1000, // 800, // 1999,
	PulseRescaleTimeout=400   // 4 seconds at 100 Hz
};

void pulserInit(Pulser *pPulse);

void pulserProcessPulseSample(Pulser *pPulse);

//[] END OF FILE

