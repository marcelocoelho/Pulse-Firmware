/*******************************************************************************
* File Name: UART_Sensor_PM.c
* Version 2.30
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART_Sensor.h"


/***************************************
* Local data allocation
***************************************/

static UART_Sensor_BACKUP_STRUCT  UART_Sensor_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: UART_Sensor_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_Sensor_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Sensor_SaveConfig(void)
{
    #if (CY_UDB_V0)

        #if(UART_Sensor_CONTROL_REG_REMOVED == 0u)
            UART_Sensor_backup.cr = UART_Sensor_CONTROL_REG;
        #endif /* End UART_Sensor_CONTROL_REG_REMOVED */

        #if( (UART_Sensor_RX_ENABLED) || (UART_Sensor_HD_ENABLED) )
            UART_Sensor_backup.rx_period = UART_Sensor_RXBITCTR_PERIOD_REG;
            UART_Sensor_backup.rx_mask = UART_Sensor_RXSTATUS_MASK_REG;
            #if (UART_Sensor_RXHW_ADDRESS_ENABLED)
                UART_Sensor_backup.rx_addr1 = UART_Sensor_RXADDRESS1_REG;
                UART_Sensor_backup.rx_addr2 = UART_Sensor_RXADDRESS2_REG;
            #endif /* End UART_Sensor_RXHW_ADDRESS_ENABLED */
        #endif /* End UART_Sensor_RX_ENABLED | UART_Sensor_HD_ENABLED*/

        #if(UART_Sensor_TX_ENABLED)
            #if(UART_Sensor_TXCLKGEN_DP)
                UART_Sensor_backup.tx_clk_ctr = UART_Sensor_TXBITCLKGEN_CTR_REG;
                UART_Sensor_backup.tx_clk_compl = UART_Sensor_TXBITCLKTX_COMPLETE_REG;
            #else
                UART_Sensor_backup.tx_period = UART_Sensor_TXBITCTR_PERIOD_REG;
            #endif /*End UART_Sensor_TXCLKGEN_DP */
            UART_Sensor_backup.tx_mask = UART_Sensor_TXSTATUS_MASK_REG;
        #endif /*End UART_Sensor_TX_ENABLED */


    #else /* CY_UDB_V1 */

        #if(UART_Sensor_CONTROL_REG_REMOVED == 0u)
            UART_Sensor_backup.cr = UART_Sensor_CONTROL_REG;
        #endif /* End UART_Sensor_CONTROL_REG_REMOVED */

    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: UART_Sensor_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_Sensor_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Sensor_RestoreConfig(void)
{

    #if (CY_UDB_V0)

        #if(UART_Sensor_CONTROL_REG_REMOVED == 0u)
            UART_Sensor_CONTROL_REG = UART_Sensor_backup.cr;
        #endif /* End UART_Sensor_CONTROL_REG_REMOVED */

        #if( (UART_Sensor_RX_ENABLED) || (UART_Sensor_HD_ENABLED) )
            UART_Sensor_RXBITCTR_PERIOD_REG = UART_Sensor_backup.rx_period;
            UART_Sensor_RXSTATUS_MASK_REG = UART_Sensor_backup.rx_mask;
            #if (UART_Sensor_RXHW_ADDRESS_ENABLED)
                UART_Sensor_RXADDRESS1_REG = UART_Sensor_backup.rx_addr1;
                UART_Sensor_RXADDRESS2_REG = UART_Sensor_backup.rx_addr2;
            #endif /* End UART_Sensor_RXHW_ADDRESS_ENABLED */
        #endif  /* End (UART_Sensor_RX_ENABLED) || (UART_Sensor_HD_ENABLED) */

        #if(UART_Sensor_TX_ENABLED)
            #if(UART_Sensor_TXCLKGEN_DP)
                UART_Sensor_TXBITCLKGEN_CTR_REG = UART_Sensor_backup.tx_clk_ctr;
                UART_Sensor_TXBITCLKTX_COMPLETE_REG = UART_Sensor_backup.tx_clk_compl;
            #else
                UART_Sensor_TXBITCTR_PERIOD_REG = UART_Sensor_backup.tx_period;
            #endif /*End UART_Sensor_TXCLKGEN_DP */
            UART_Sensor_TXSTATUS_MASK_REG = UART_Sensor_backup.tx_mask;
        #endif /*End UART_Sensor_TX_ENABLED */

    #else /* CY_UDB_V1 */

        #if(UART_Sensor_CONTROL_REG_REMOVED == 0u)
            UART_Sensor_CONTROL_REG = UART_Sensor_backup.cr;
        #endif /* End UART_Sensor_CONTROL_REG_REMOVED */

    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: UART_Sensor_Sleep
********************************************************************************
*
* Summary:
*  Stops and saves the user configuration. Should be called
*  just prior to entering sleep.
*
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_Sensor_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Sensor_Sleep(void)
{

    #if(UART_Sensor_RX_ENABLED || UART_Sensor_HD_ENABLED)
        if((UART_Sensor_RXSTATUS_ACTL_REG  & UART_Sensor_INT_ENABLE) != 0u)
        {
            UART_Sensor_backup.enableState = 1u;
        }
        else
        {
            UART_Sensor_backup.enableState = 0u;
        }
    #else
        if((UART_Sensor_TXSTATUS_ACTL_REG  & UART_Sensor_INT_ENABLE) !=0u)
        {
            UART_Sensor_backup.enableState = 1u;
        }
        else
        {
            UART_Sensor_backup.enableState = 0u;
        }
    #endif /* End UART_Sensor_RX_ENABLED || UART_Sensor_HD_ENABLED*/

    UART_Sensor_Stop();
    UART_Sensor_SaveConfig();
}


/*******************************************************************************
* Function Name: UART_Sensor_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_Sensor_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Sensor_Wakeup(void)
{
    UART_Sensor_RestoreConfig();
    #if( (UART_Sensor_RX_ENABLED) || (UART_Sensor_HD_ENABLED) )
        UART_Sensor_ClearRxBuffer();
    #endif /* End (UART_Sensor_RX_ENABLED) || (UART_Sensor_HD_ENABLED) */
    #if(UART_Sensor_TX_ENABLED || UART_Sensor_HD_ENABLED)
        UART_Sensor_ClearTxBuffer();
    #endif /* End UART_Sensor_TX_ENABLED || UART_Sensor_HD_ENABLED */

    if(UART_Sensor_backup.enableState != 0u)
    {
        UART_Sensor_Enable();
    }
}


/* [] END OF FILE */
