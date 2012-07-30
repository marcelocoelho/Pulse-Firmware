/*******************************************************************************
* File Name: UART_Debug.c
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

#include "UART_Debug.h"


/***************************************
* Local data allocation
***************************************/

static UART_Debug_BACKUP_STRUCT  UART_Debug_backup =
{
    /* enableState - disabled */
    0u,
};        



/*******************************************************************************
* Function Name: UART_Debug_SaveConfig
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
*  UART_Debug_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Debug_SaveConfig(void)
{
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)

        #if(UART_Debug_CONTROL_REG_REMOVED == 0u)
            UART_Debug_backup.cr = UART_Debug_CONTROL_REG;
        #endif /* End UART_Debug_CONTROL_REG_REMOVED */

        #if( (UART_Debug_RX_ENABLED) || (UART_Debug_HD_ENABLED) )
            UART_Debug_backup.rx_period = UART_Debug_RXBITCTR_PERIOD_REG;
            UART_Debug_backup.rx_mask = UART_Debug_RXSTATUS_MASK_REG;
            #if (UART_Debug_RXHW_ADDRESS_ENABLED)
                UART_Debug_backup.rx_addr1 = UART_Debug_RXADDRESS1_REG;
                UART_Debug_backup.rx_addr2 = UART_Debug_RXADDRESS2_REG;
            #endif /* End UART_Debug_RXHW_ADDRESS_ENABLED */
        #endif /* End UART_Debug_RX_ENABLED | UART_Debug_HD_ENABLED*/

        #if(UART_Debug_TX_ENABLED)
            #if(UART_Debug_TXCLKGEN_DP)
                UART_Debug_backup.tx_clk_ctr = UART_Debug_TXBITCLKGEN_CTR_REG;
                UART_Debug_backup.tx_clk_compl = UART_Debug_TXBITCLKTX_COMPLETE_REG;
            #else
                UART_Debug_backup.tx_period = UART_Debug_TXBITCTR_PERIOD_REG;
            #endif /*End UART_Debug_TXCLKGEN_DP */
            UART_Debug_backup.tx_mask = UART_Debug_TXSTATUS_MASK_REG;
        #endif /*End UART_Debug_TX_ENABLED */

    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (CY_PSOC3_ES3 || CY_PSOC5_ES2)

        #if(UART_Debug_CONTROL_REG_REMOVED == 0u)
            UART_Debug_backup.cr = UART_Debug_CONTROL_REG;
        #endif /* End UART_Debug_CONTROL_REG_REMOVED */

    #endif  /* End PSOC3_ES3 || PSOC5_ES2 */
}


/*******************************************************************************
* Function Name: UART_Debug_RestoreConfig
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
*  UART_Debug_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Debug_RestoreConfig(void)
{
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if (CY_PSOC3_ES2 || CY_PSOC5_ES1)

        #if(UART_Debug_CONTROL_REG_REMOVED == 0u)
            UART_Debug_CONTROL_REG = UART_Debug_backup.cr;
        #endif /* End UART_Debug_CONTROL_REG_REMOVED */

        #if( (UART_Debug_RX_ENABLED) || (UART_Debug_HD_ENABLED) )
            UART_Debug_RXBITCTR_PERIOD_REG = UART_Debug_backup.rx_period;
            UART_Debug_RXSTATUS_MASK_REG = UART_Debug_backup.rx_mask;
            #if (UART_Debug_RXHW_ADDRESS_ENABLED)
                UART_Debug_RXADDRESS1_REG = UART_Debug_backup.rx_addr1;
                UART_Debug_RXADDRESS2_REG = UART_Debug_backup.rx_addr2;
            #endif /* End UART_Debug_RXHW_ADDRESS_ENABLED */
        #endif  /* End (UART_Debug_RX_ENABLED) || (UART_Debug_HD_ENABLED) */

        #if(UART_Debug_TX_ENABLED)
            #if(UART_Debug_TXCLKGEN_DP)
                UART_Debug_TXBITCLKGEN_CTR_REG = UART_Debug_backup.tx_clk_ctr;
                UART_Debug_TXBITCLKTX_COMPLETE_REG = UART_Debug_backup.tx_clk_compl;
            #else
                UART_Debug_TXBITCTR_PERIOD_REG = UART_Debug_backup.tx_period;
            #endif /*End UART_Debug_TXCLKGEN_DP */
            UART_Debug_TXSTATUS_MASK_REG = UART_Debug_backup.tx_mask;
        #endif /*End UART_Debug_TX_ENABLED */

    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (CY_PSOC3_ES3 || CY_PSOC5_ES2)

        #if(UART_Debug_CONTROL_REG_REMOVED == 0u)
            UART_Debug_CONTROL_REG = UART_Debug_backup.cr;
        #endif /* End UART_Debug_CONTROL_REG_REMOVED */

    #endif  /* End PSOC3_ES3 || PSOC5_ES2 */
}


/*******************************************************************************
* Function Name: UART_Debug_Sleep
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
*  UART_Debug_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Debug_Sleep(void)
{

    #if(UART_Debug_RX_ENABLED || UART_Debug_HD_ENABLED)
        if((UART_Debug_RXSTATUS_ACTL_REG  & UART_Debug_INT_ENABLE) != 0u) 
        {
            UART_Debug_backup.enableState = 1u;
        }
        else
        {
            UART_Debug_backup.enableState = 0u;
        }
    #else
        if((UART_Debug_TXSTATUS_ACTL_REG  & UART_Debug_INT_ENABLE) !=0u)
        {
            UART_Debug_backup.enableState = 1u;
        }
        else
        {
            UART_Debug_backup.enableState = 0u;
        }
    #endif /* End UART_Debug_RX_ENABLED || UART_Debug_HD_ENABLED*/

    UART_Debug_Stop();
    UART_Debug_SaveConfig();
}


/*******************************************************************************
* Function Name: UART_Debug_Wakeup
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
*  UART_Debug_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Debug_Wakeup(void)
{
    UART_Debug_RestoreConfig();
    #if( (UART_Debug_RX_ENABLED) || (UART_Debug_HD_ENABLED) )
        UART_Debug_ClearRxBuffer();
    #endif /* End (UART_Debug_RX_ENABLED) || (UART_Debug_HD_ENABLED) */
    #if(UART_Debug_TX_ENABLED || UART_Debug_HD_ENABLED)
        UART_Debug_ClearTxBuffer();
    #endif /* End UART_Debug_TX_ENABLED || UART_Debug_HD_ENABLED */

    if(UART_Debug_backup.enableState != 0u)
    {
        UART_Debug_Enable();
    } 
}


/* [] END OF FILE */
