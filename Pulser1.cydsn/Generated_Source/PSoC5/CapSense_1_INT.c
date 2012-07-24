/*******************************************************************************
* File Name: CapSense_1_INT.c
* Version 3.10
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CapSense_1.h"

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START CapSense_1_IsrCH_ISR_intc` */

/* `#END` */

/* Extern functions declarations, Start and Compete the scan */
extern void CapSense_1_PreScan(uint8 sensor) ;
#if (CapSense_1_DESIGN_TYPE == CapSense_1_ONE_CHANNEL_DESIGN)
    extern void CapSense_1_PostScan(uint8 sensor);
#else
    extern void CapSense_1_PostScanCh0(uint8 sensor);
    extern void CapSense_1_PostScanCh1(uint8 sensor);
#endif  /* End CapSense_1_DESIGN_TYPE */

 #if (CapSense_1_DESIGN_TYPE == CapSense_1_ONE_CHANNEL_DESIGN)
    extern uint8 CapSense_1_FindNextSensor(uint8 snsIndex) ;
#endif  /* End (CapSense_1_DESIGN_TYPE == CapSense_1_ONE_CHANNEL_DESIGN) */
 
#if (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN)
    extern uint8 CapSense_1_FindNextPair(uint8 snsIndex) ;
#endif  /* End (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN) */

/* Global variables and arrays */
extern volatile uint8 CapSense_1_csv;
extern volatile uint8 CapSense_1_sensorIndex;


#if (CapSense_1_DESIGN_TYPE == CapSense_1_ONE_CHANNEL_DESIGN)   
    /*******************************************************************************
    * Function Name: CapSense_1_IsrCH0_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor and stores measure result.
    *   2. Stores result of current measure and starts next scan till all 
    *      enabled sensors will be scanned.
    *  This interrupt handler only used in one channel designs.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_1_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  CapSense_1_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(CapSense_1_IsrCH0_ISR)
    {
        /*  Place your Interrupt code here. */
        /* `#START CapSense_1_ONE_CHANNEL_IsrCH0_ISR` */

        /* `#END` */
    
        /* Save results and disable sensor */
        CapSense_1_PostScan(CapSense_1_sensorIndex);
                
        if ((CapSense_1_csv & CapSense_1_SW_CTRL_SINGLE_SCAN) != 0u)
        {
            CapSense_1_csv &= ~CapSense_1_SW_STS_BUSY;
        }
        else
        {
            /* Proceed the scanning */
            CapSense_1_sensorIndex = CapSense_1_FindNextSensor(CapSense_1_sensorIndex);
            
            /* Check end of scan */
            if(CapSense_1_sensorIndex < CapSense_1_TOTAL_SENSOR_COUNT)
            {
                CapSense_1_PreScan(CapSense_1_sensorIndex);
            }
            else
            {
                CapSense_1_csv &= ~CapSense_1_SW_STS_BUSY;
            } 
        }
    
        #if (CY_PSOC3_ES2 && (CapSense_1_IsrCH0__ES2_PATCH))
                CapSense_1_ISR_PATCH();
        #endif /* End (CY_PSOC3_ES2 && (CapSense_1_IsrCH0__ES2_PATCH)) */
    }
 #endif  /* End CapSense_1_DESIGN_TYPE */


 #if (CapSense_1_DESIGN_TYPE == CapSense_1_TWO_CHANNELS_DESIGN)   
    /*******************************************************************************
    * Function Name: CapSense_1_IsrCH0_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor or pair of sensors and stores measure result.
    *   2. Stores results of current measure when complete scan of sensors pair or 
    *      sensor scan and starts next enabled sensors pair or sensor scan. 
    *  This interrupt handler only used in two channel designs.
    *  The synchronization is added to scanning process. The ISR channel 0 or 
    *  channel 1 stores the result and starts next scan. It depends which channel
    *  scans longer. The biggest scan resolution parameter of sensor within pair 
    *  defines scanning time of pair.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_1_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  CapSense_1_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(CapSense_1_IsrCH0_ISR)
    {
        /*  Place your Interrupt code here. */
        /* `#START CapSense_1_TWO_CHANNELS_IsrCH0_ISR` */

        /* `#END` */
        
        CapSense_1_CONTROL_REG &= ~CapSense_1_CTRL_WINDOW_EN__CH0;
        CapSense_1_PostScanCh0(CapSense_1_sensorIndex);
        
        if((CapSense_1_CONTROL_REG & CapSense_1_CTRL_WINDOW_EN__CH1) == 0u)
        {
            if ((CapSense_1_csv & CapSense_1_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                CapSense_1_csv &= ~CapSense_1_SW_STS_BUSY;
            }
            else
            {
                /* Procced the scanning */
                CapSense_1_sensorIndex = CapSense_1_FindNextPair(CapSense_1_sensorIndex);
                
                /* Check end of scan conditions */
                if((CapSense_1_sensorIndex < CapSense_1_TOTAL_SENSOR_COUNT__CH0) || 
                   (CapSense_1_sensorIndex < CapSense_1_TOTAL_SENSOR_COUNT__CH1))
                {
                    CapSense_1_PreScan(CapSense_1_sensorIndex);
                }
                else
                {
                    CapSense_1_csv &= ~CapSense_1_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }
    
        #if (CY_PSOC3_ES2 && (CapSense_1_IsrCH0__ES2_PATCH))
                CapSense_1_ISR_PATCH();
        #endif /* End (CY_PSOC3_ES2 && (CapSense_1_IsrCH0__ES2_PATCH)) */
    }

    
    /*******************************************************************************
    * Function Name: CapSense_1_IsrCH1_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor or pair of sensors and stores measure result.
    *   2. Stores results of current measure when complete scan of sensors pair or 
    *      sensor scan and starts next enabled sensors pair or sensor scan. 
    *  This interrupt handler only used in two channel designs.
    *  The synchronization is added to scanning process. The ISR channel 0 or 
    *  channel 1 stores the result and starts next scan. It depends which channel
    *  scans longer. The biggest scan resolution parameter of sensor within pair 
    *  defines scanning time of pair.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_1_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  CapSense_1_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(CapSense_1_IsrCH1_ISR)
    {
        /*  Place your Interrupt code here */
        /* `#START CapSense_1_TWO_CHANNELS_IsrCH1_ISR` */

        /* `#END` */
        
        CapSense_1_CONTROL_REG &= ~CapSense_1_CTRL_WINDOW_EN__CH1;
        CapSense_1_PostScanCh1(CapSense_1_sensorIndex + CapSense_1_TOTAL_SENSOR_COUNT__CH0);
        
        if((CapSense_1_CONTROL_REG & CapSense_1_CTRL_WINDOW_EN__CH0) == 0u)
        {
            if ((CapSense_1_csv & CapSense_1_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                CapSense_1_csv &= ~CapSense_1_SW_STS_BUSY;
            }
            else 
            {
                /* Procced the scanning */
                CapSense_1_sensorIndex = CapSense_1_FindNextPair(CapSense_1_sensorIndex);
                
                /* Check end of scan conditions */
                if((CapSense_1_sensorIndex < CapSense_1_TOTAL_SENSOR_COUNT__CH0) || 
                   (CapSense_1_sensorIndex < CapSense_1_TOTAL_SENSOR_COUNT__CH1))
                {
                    CapSense_1_PreScan(CapSense_1_sensorIndex);
                }
                else
                {
                    CapSense_1_csv &= ~CapSense_1_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }
    
        #if (CY_PSOC3_ES2 && (CapSense_1_IsrCH1__ES2_PATCH))
                CapSense_1_ISR_PATCH();
        #endif /* End (CY_PSOC3_ES2 && (CapSense_1_IsrCH1__ES2_PATCH)) */
    }
#endif  /* End CapSense_1_DESIGN_TYPE */


/* [] END OF FILE */
