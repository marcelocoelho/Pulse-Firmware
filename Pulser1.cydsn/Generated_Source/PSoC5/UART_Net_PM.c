/*******************************************************************************
* File Name: UART_Net.c
* Version 2.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "UART_Net.h"


/***************************************
* Local data allocation
***************************************/

static UART_Net_BACKUP_STRUCT  UART_Net_backup =
{
    /* enableState - disabled */
    0u,
};        



/*******************************************************************************
* Function Name: UART_Net_SaveConfig
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
*  UART_Net_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Net_SaveConfig(void)
{
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)

        #if(UART_Net_CONTROL_REG_REMOVED == 0u)
            UART_Net_backup.cr = UART_Net_CONTROL_REG;
        #endif /* End UART_Net_CONTROL_REG_REMOVED */

        #if( (UART_Net_RX_ENABLED) || (UART_Net_HD_ENABLED) )
            UART_Net_backup.rx_period = UART_Net_RXBITCTR_PERIOD_REG;
            UART_Net_backup.rx_mask = UART_Net_RXSTATUS_MASK_REG;
            #if (UART_Net_RXHW_ADDRESS_ENABLED)
                UART_Net_backup.rx_addr1 = UART_Net_RXADDRESS1_REG;
                UART_Net_backup.rx_addr2 = UART_Net_RXADDRESS2_REG;
            #endif /* End UART_Net_RXHW_ADDRESS_ENABLED */
        #endif /* End UART_Net_RX_ENABLED | UART_Net_HD_ENABLED*/

        #if(UART_Net_TX_ENABLED)
            #if(UART_Net_TXCLKGEN_DP)
                UART_Net_backup.tx_clk_ctr = UART_Net_TXBITCLKGEN_CTR_REG;
                UART_Net_backup.tx_clk_compl = UART_Net_TXBITCLKTX_COMPLETE_REG;
            #else
                UART_Net_backup.tx_period = UART_Net_TXBITCTR_PERIOD_REG;
            #endif /*End UART_Net_TXCLKGEN_DP */
            UART_Net_backup.tx_mask = UART_Net_TXSTATUS_MASK_REG;
        #endif /*End UART_Net_TX_ENABLED */

    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (CY_PSOC3_ES3 || CY_PSOC5_ES2)

        #if(UART_Net_CONTROL_REG_REMOVED == 0u)
            UART_Net_backup.cr = UART_Net_CONTROL_REG;
        #endif /* End UART_Net_CONTROL_REG_REMOVED */

    #endif  /* End PSOC3_ES3 || PSOC5_ES2 */
}


/*******************************************************************************
* Function Name: UART_Net_RestoreConfig
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
*  UART_Net_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Net_RestoreConfig(void)
{
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)

        #if(UART_Net_CONTROL_REG_REMOVED == 0u)
            UART_Net_CONTROL_REG = UART_Net_backup.cr;
        #endif /* End UART_Net_CONTROL_REG_REMOVED */

        #if( (UART_Net_RX_ENABLED) || (UART_Net_HD_ENABLED) )
            UART_Net_RXBITCTR_PERIOD_REG = UART_Net_backup.rx_period;
            UART_Net_RXSTATUS_MASK_REG = UART_Net_backup.rx_mask;
            #if (UART_Net_RXHW_ADDRESS_ENABLED)
                UART_Net_RXADDRESS1_REG = UART_Net_backup.rx_addr1;
                UART_Net_RXADDRESS2_REG = UART_Net_backup.rx_addr2;
            #endif /* End UART_Net_RXHW_ADDRESS_ENABLED */
        #endif  /* End (UART_Net_RX_ENABLED) || (UART_Net_HD_ENABLED) */

        #if(UART_Net_TX_ENABLED)
            #if(UART_Net_TXCLKGEN_DP)
                UART_Net_TXBITCLKGEN_CTR_REG = UART_Net_backup.tx_clk_ctr;
                UART_Net_TXBITCLKTX_COMPLETE_REG = UART_Net_backup.tx_clk_compl;
            #else
                UART_Net_TXBITCTR_PERIOD_REG = UART_Net_backup.tx_period;
            #endif /*End UART_Net_TXCLKGEN_DP */
            UART_Net_TXSTATUS_MASK_REG = UART_Net_backup.tx_mask;
        #endif /*End UART_Net_TX_ENABLED */

    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (CY_PSOC3_ES3 || CY_PSOC5_ES2)

        #if(UART_Net_CONTROL_REG_REMOVED == 0u)
            UART_Net_CONTROL_REG = UART_Net_backup.cr;
        #endif /* End UART_Net_CONTROL_REG_REMOVED */

    #endif  /* End PSOC3_ES3 || PSOC5_ES2 */
}


/*******************************************************************************
* Function Name: UART_Net_Sleep
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
*  UART_Net_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Net_Sleep(void)
{

    #if(UART_Net_RX_ENABLED || UART_Net_HD_ENABLED)
        if((UART_Net_RXSTATUS_ACTL_REG  & UART_Net_INT_ENABLE) != 0u) 
        {
            UART_Net_backup.enableState = 1u;
        }
        else
        {
            UART_Net_backup.enableState = 0u;
        }
    #else
        if((UART_Net_TXSTATUS_ACTL_REG  & UART_Net_INT_ENABLE) !=0u)
        {
            UART_Net_backup.enableState = 1u;
        }
        else
        {
            UART_Net_backup.enableState = 0u;
        }
    #endif /* End UART_Net_RX_ENABLED || UART_Net_HD_ENABLED*/

    UART_Net_Stop();
    UART_Net_SaveConfig();
}


/*******************************************************************************
* Function Name: UART_Net_Wakeup
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
*  UART_Net_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Net_Wakeup(void)
{
    UART_Net_RestoreConfig();
    #if( (UART_Net_RX_ENABLED) || (UART_Net_HD_ENABLED) )
        UART_Net_ClearRxBuffer();
    #endif /* End (UART_Net_RX_ENABLED) || (UART_Net_HD_ENABLED) */
    #if(UART_Net_TX_ENABLED || UART_Net_HD_ENABLED)
        UART_Net_ClearTxBuffer();
    #endif /* End UART_Net_TX_ENABLED || UART_Net_HD_ENABLED */

    if(UART_Net_backup.enableState != 0u)
    {
        UART_Net_Enable();
    } 
}


/* [] END OF FILE */
