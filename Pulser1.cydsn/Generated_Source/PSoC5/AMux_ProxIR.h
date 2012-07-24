/*******************************************************************************
* File Name: AMux_ProxIR.h
* Version 1.50
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_AMux_ProxIR_H)
#define CY_AMUX_AMux_ProxIR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void AMux_ProxIR_Start(void);
void AMux_ProxIR_Init(void);
void AMux_ProxIR_Stop(void);
void AMux_ProxIR_Select(uint8 channel) ;
void AMux_ProxIR_FastSelect(uint8 channel) ;
void AMux_ProxIR_DisconnectAll(void) ;
/* The Connect and Disconnect functions are declared elsewhere */
/* void AMux_ProxIR_Connect(uint8 channel); */
/* void AMux_ProxIR_Disconnect(uint8 channel); */


/***************************************
*     Initial Parameter Constants
***************************************/

#define AMux_ProxIR_CHANNELS  3
#define AMux_ProxIR_MUXTYPE   1


/***************************************
*             API Constants
***************************************/

#define AMux_ProxIR_NULL_CHANNEL  0xFFu
#define AMux_ProxIR_MUX_SINGLE   1
#define AMux_ProxIR_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_ProxIR_MUXTYPE == AMux_ProxIR_MUX_SINGLE
# define AMux_ProxIR_Connect(channel) AMux_ProxIR_Set(channel)
# define AMux_ProxIR_Disconnect(channel) AMux_ProxIR_Unset(channel)
#else
void AMux_ProxIR_Connect(uint8 channel) ;
void AMux_ProxIR_Disconnect(uint8 channel) ;
#endif

#endif /* CY_AMUX_AMux_ProxIR_H */


/* [] END OF FILE */
