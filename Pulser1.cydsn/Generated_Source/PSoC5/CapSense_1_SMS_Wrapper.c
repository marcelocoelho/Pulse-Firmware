/*******************************************************************************
* File Name: CapSense_1_SMS_Wrapper.c
* Version 3.30
*
* Description:
*  This file provides the source code of wrapper between CapSense CSD component 
*  and Auto Tuning library.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CapSense_1.h"
#include "CapSense_1_CSHL.h"

#if (CapSense_1_TUNING_METHOD == CapSense_1_AUTO_TUNING)

extern uint8 CapSense_1_noiseThreshold[];
extern uint8 CapSense_1_hysteresis[];

extern uint8 CapSense_1_widgetResolution[];

extern const uint8 CYCODE CapSense_1_widgetNumber[];
extern const uint8 CYCODE CapSense_1_numberOfSensors[];

extern uint8 CapSense_1_fingerThreshold[];
extern uint8 CapSense_1_idacSettings[];
extern uint8 CapSense_1_AnalogSwitchDivider[];

extern uint16 CapSense_1_SensorRaw[];
extern uint16 CapSense_1_SensorBaseline[];
extern uint8  CapSense_1_SensorSignal[];

extern void SMS_LIB_V3_CalculateThresholds(uint8 SensorNumber);
extern void SMS_LIB_V3_AutoTune1Ch(void);
extern void SMS_LIB_V3_AutoTune2Ch(void);

uint8 * SMS_LIB_noiseThreshold = CapSense_1_noiseThreshold;
uint8 * SMS_LIB_hysteresis = CapSense_1_hysteresis;

uint8 * SMS_LIB_widgetResolution = CapSense_1_widgetResolution;

const uint8 CYCODE * SMS_LIB_widgetNumber = CapSense_1_widgetNumber;
const uint8 CYCODE * SMS_LIB_numberOfSensors = CapSense_1_numberOfSensors;

uint8 * SMS_LIB_fingerThreshold = CapSense_1_fingerThreshold;
uint8 * SMS_LIB_idacSettings = CapSense_1_idacSettings;
uint8 * SMS_LIB_prescaler = CapSense_1_AnalogSwitchDivider;

uint16 * SMS_LIB_SensorRaw = CapSense_1_SensorRaw;
uint16 * SMS_LIB_SensorBaseline = CapSense_1_SensorBaseline;




uint8 SMS_LIB_Table1[CapSense_1_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table2[CapSense_1_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table3[CapSense_1_TOTAL_SENSOR_COUNT];
uint16 SMS_LIB_Table4[CapSense_1_TOTAL_SENSOR_COUNT];
uint16 SMS_LIB_Table5[CapSense_1_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table6[CapSense_1_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table7[CapSense_1_TOTAL_SENSOR_COUNT];

uint8 SMS_LIB_Table8[CapSense_1_END_OF_WIDGETS_INDEX];
uint8 SMS_LIB_Table9[CapSense_1_END_OF_WIDGETS_INDEX];
uint8 SMS_LIB_Table10[CapSense_1_END_OF_WIDGETS_INDEX];

uint8 SMS_LIB_Var1 = ();
uint16 SMS_LIB_Var2 = ();

uint8 SMS_LIB_TotalSnsCnt = CapSense_1_TOTAL_SENSOR_COUNT;
uint8 SMS_LIB_TotalScanSlCnt = CapSense_1_TOTAL_SCANSLOT_COUNT;
uint8 SMS_LIB_EndOfWidgInd = CapSense_1_END_OF_WIDGETS_INDEX;

#if (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN)
    uint8 SMS_LIB_TotalSnsCnt_CH0 = CapSense_1_TOTAL_SENSOR_COUNT__CH0;
    uint8 SMS_LIB_TotalSnsCnt_CH1 = CapSense_1_TOTAL_SENSOR_COUNT__CH1;
#else
    uint8 SMS_LIB_TotalSnsCnt_CH0 = 0;
    uint8 SMS_LIB_TotalSnsCnt_CH1 = 0;
#endif  /* (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN) */

/*******************************************************************************
* Function Name: SMS_LIB_ScanSensor
********************************************************************************
*
* Summary:
*  Wrapper to CapSense_1_ScanSensor function.
*
* Parameters:
*  SensorNumber:  Sensor number.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void SMS_LIB_ScanSensor(uint8 SensorNumber) 
{
    CapSense_1_ScanSensor(SensorNumber);
}

/*******************************************************************************
* Function Name: SMS_LIB_IsBusy
********************************************************************************
*
* Summary:
*  Wrapper to CapSense_1_IsBusy function.
*  
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
uint8 SMS_LIB_IsBusy(void) 
{
    return CapSense_1_IsBusy();
}


/*******************************************************************************
* Function Name: CapSense_1_CalculateThresholds
********************************************************************************
*
* Summary:
*  Wrapper to SMS_LIB_CalculateThresholds function.
*
* Parameters:
*  SensorNumber:  Sensor number.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void CapSense_1_CalculateThresholds(uint8 SensorNumber) 
{
    SMS_LIB_V3_CalculateThresholds(SensorNumber);
}


/*******************************************************************************
* Function Name: CapSense_1_AutoTune
********************************************************************************
*
* Summary:
*  Wrapper for SMS_LIB_AutoTune1Ch or SMS_LIB_AutoTune2Ch function.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void CapSense_1_AutoTune(void) 
{
    #if (CapSense_1_DESIGN_TYPE == CapSense_1_ONE_CHANNEL_DESIGN)
        SMS_LIB_V3_AutoTune1Ch();
    #elif (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN)
        SMS_LIB_V3_AutoTune2Ch();
    #endif /* (CapSense_1_DESIGN_TYPE == CapSense_1_ONE_CHANNEL_DESIGN) */
}

/*******************************************************************************
* Function Name: SMS_LIB_SetPrescaler
********************************************************************************
*
* Summary:
*  Empty wrapper for version compliance.
*
* Parameters:
*  prescaler:  prascaler value.
*
* Return:
*  None
*
*******************************************************************************/
void SMS_LIB_SetPrescaler(uint8 prescaler) 
{
    prescaler = prescaler;
}

void SMS_LIB_V3_SetAnalogSwitchesSrc_PRS(void) 
{
	CapSense_1_SetAnalogSwitchesSource(CapSense_1_ANALOG_SWITCHES_SRC_PRS);
}

void SMS_LIB_V3_SetAnalogSwitchesSrc_Prescaler(void) 
{
	CapSense_1_SetAnalogSwitchesSource(CapSense_1_ANALOG_SWITCHES_SRC_PRESCALER);
}

#endif  /* (CapSense_1_TUNING_METHOD == CapSense_1_AUTO_TUNING) */


/* [] END OF FILE */
