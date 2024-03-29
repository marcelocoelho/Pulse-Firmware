.ifndef INCLUDED_CYFITTERGNU_INC
.set INCLUDED_CYFITTERGNU_INC, 1
.include "cydevicegnu.inc"
.include "cydevicegnu_trm.inc"

/* USBUART_1_bus_reset */
.set USBUART_1_bus_reset__INTC_CLR_EN_REG, CYREG_NVIC_CLRENA0
.set USBUART_1_bus_reset__INTC_CLR_PD_REG, CYREG_NVIC_CLRPEND0
.set USBUART_1_bus_reset__INTC_MASK, 0x01
.set USBUART_1_bus_reset__INTC_NUMBER, 0
.set USBUART_1_bus_reset__INTC_PRIOR_NUM, 7
.set USBUART_1_bus_reset__INTC_PRIOR_REG, CYREG_NVIC_PRI_0
.set USBUART_1_bus_reset__INTC_SET_EN_REG, CYREG_NVIC_SETENA0
.set USBUART_1_bus_reset__INTC_SET_PD_REG, CYREG_NVIC_SETPEND0

/* USBUART_1_arb_int */
.set USBUART_1_arb_int__INTC_CLR_EN_REG, CYREG_NVIC_CLRENA0
.set USBUART_1_arb_int__INTC_CLR_PD_REG, CYREG_NVIC_CLRPEND0
.set USBUART_1_arb_int__INTC_MASK, 0x400000
.set USBUART_1_arb_int__INTC_NUMBER, 22
.set USBUART_1_arb_int__INTC_PRIOR_NUM, 7
.set USBUART_1_arb_int__INTC_PRIOR_REG, CYREG_NVIC_PRI_22
.set USBUART_1_arb_int__INTC_SET_EN_REG, CYREG_NVIC_SETENA0
.set USBUART_1_arb_int__INTC_SET_PD_REG, CYREG_NVIC_SETPEND0

/* USBUART_1_ord_int */
.set USBUART_1_ord_int__INTC_CLR_EN_REG, CYREG_NVIC_CLRENA0
.set USBUART_1_ord_int__INTC_CLR_PD_REG, CYREG_NVIC_CLRPEND0
.set USBUART_1_ord_int__INTC_MASK, 0x2000000
.set USBUART_1_ord_int__INTC_NUMBER, 25
.set USBUART_1_ord_int__INTC_PRIOR_NUM, 7
.set USBUART_1_ord_int__INTC_PRIOR_REG, CYREG_NVIC_PRI_25
.set USBUART_1_ord_int__INTC_SET_EN_REG, CYREG_NVIC_SETENA0
.set USBUART_1_ord_int__INTC_SET_PD_REG, CYREG_NVIC_SETPEND0

/* USBUART_1_sof_int */
.set USBUART_1_sof_int__INTC_CLR_EN_REG, CYREG_NVIC_CLRENA0
.set USBUART_1_sof_int__INTC_CLR_PD_REG, CYREG_NVIC_CLRPEND0
.set USBUART_1_sof_int__INTC_MASK, 0x20
.set USBUART_1_sof_int__INTC_NUMBER, 5
.set USBUART_1_sof_int__INTC_PRIOR_NUM, 7
.set USBUART_1_sof_int__INTC_PRIOR_REG, CYREG_NVIC_PRI_5
.set USBUART_1_sof_int__INTC_SET_EN_REG, CYREG_NVIC_SETENA0
.set USBUART_1_sof_int__INTC_SET_PD_REG, CYREG_NVIC_SETPEND0

/* USBUART_1_Dp_1 */
.set USBUART_1_Dp_1__0__MASK, 0x40
.set USBUART_1_Dp_1__0__PC, CYREG_PRT15_PC6
.set USBUART_1_Dp_1__0__PORT, 15
.set USBUART_1_Dp_1__0__SHIFT, 6
.set USBUART_1_Dp_1__AG, CYREG_PRT15_AG
.set USBUART_1_Dp_1__AMUX, CYREG_PRT15_AMUX
.set USBUART_1_Dp_1__BIE, CYREG_PRT15_BIE
.set USBUART_1_Dp_1__BIT_MASK, CYREG_PRT15_BIT_MASK
.set USBUART_1_Dp_1__BYP, CYREG_PRT15_BYP
.set USBUART_1_Dp_1__CTL, CYREG_PRT15_CTL
.set USBUART_1_Dp_1__DM0, CYREG_PRT15_DM0
.set USBUART_1_Dp_1__DM1, CYREG_PRT15_DM1
.set USBUART_1_Dp_1__DM2, CYREG_PRT15_DM2
.set USBUART_1_Dp_1__DR, CYREG_PRT15_DR
.set USBUART_1_Dp_1__INP_DIS, CYREG_PRT15_INP_DIS
.set USBUART_1_Dp_1__LCD_COM_SEG, CYREG_PRT15_LCD_COM_SEG
.set USBUART_1_Dp_1__LCD_EN, CYREG_PRT15_LCD_EN
.set USBUART_1_Dp_1__PORT, 15
.set USBUART_1_Dp_1__PRT, CYREG_PRT15_PRT
.set USBUART_1_Dp_1__PRTDSI__CAPS_SEL, CYREG_PRT15_CAPS_SEL
.set USBUART_1_Dp_1__PRTDSI__DBL_SYNC_IN, CYREG_PRT15_DBL_SYNC_IN
.set USBUART_1_Dp_1__PRTDSI__OE_SEL0, CYREG_PRT15_OE_SEL0
.set USBUART_1_Dp_1__PRTDSI__OE_SEL1, CYREG_PRT15_OE_SEL1
.set USBUART_1_Dp_1__PRTDSI__OUT_SEL0, CYREG_PRT15_OUT_SEL0
.set USBUART_1_Dp_1__PRTDSI__OUT_SEL1, CYREG_PRT15_OUT_SEL1
.set USBUART_1_Dp_1__PRTDSI__SYNC_OUT, CYREG_PRT15_SYNC_OUT
.set USBUART_1_Dp_1__PS, CYREG_PRT15_PS
.set USBUART_1_Dp_1__SLW, CYREG_PRT15_SLW

/* USBUART_1_ep_0 */
.set USBUART_1_ep_0__INTC_CLR_EN_REG, CYREG_NVIC_CLRENA0
.set USBUART_1_ep_0__INTC_CLR_PD_REG, CYREG_NVIC_CLRPEND0
.set USBUART_1_ep_0__INTC_MASK, 0x02
.set USBUART_1_ep_0__INTC_NUMBER, 1
.set USBUART_1_ep_0__INTC_PRIOR_NUM, 7
.set USBUART_1_ep_0__INTC_PRIOR_REG, CYREG_NVIC_PRI_1
.set USBUART_1_ep_0__INTC_SET_EN_REG, CYREG_NVIC_SETENA0
.set USBUART_1_ep_0__INTC_SET_PD_REG, CYREG_NVIC_SETPEND0

/* USBUART_1_ep_1 */
.set USBUART_1_ep_1__INTC_CLR_EN_REG, CYREG_NVIC_CLRENA0
.set USBUART_1_ep_1__INTC_CLR_PD_REG, CYREG_NVIC_CLRPEND0
.set USBUART_1_ep_1__INTC_MASK, 0x04
.set USBUART_1_ep_1__INTC_NUMBER, 2
.set USBUART_1_ep_1__INTC_PRIOR_NUM, 7
.set USBUART_1_ep_1__INTC_PRIOR_REG, CYREG_NVIC_PRI_2
.set USBUART_1_ep_1__INTC_SET_EN_REG, CYREG_NVIC_SETENA0
.set USBUART_1_ep_1__INTC_SET_PD_REG, CYREG_NVIC_SETPEND0

/* USBUART_1_ep_2 */
.set USBUART_1_ep_2__INTC_CLR_EN_REG, CYREG_NVIC_CLRENA0
.set USBUART_1_ep_2__INTC_CLR_PD_REG, CYREG_NVIC_CLRPEND0
.set USBUART_1_ep_2__INTC_MASK, 0x08
.set USBUART_1_ep_2__INTC_NUMBER, 3
.set USBUART_1_ep_2__INTC_PRIOR_NUM, 7
.set USBUART_1_ep_2__INTC_PRIOR_REG, CYREG_NVIC_PRI_3
.set USBUART_1_ep_2__INTC_SET_EN_REG, CYREG_NVIC_SETENA0
.set USBUART_1_ep_2__INTC_SET_PD_REG, CYREG_NVIC_SETPEND0

/* USBUART_1_ep_3 */
.set USBUART_1_ep_3__INTC_CLR_EN_REG, CYREG_NVIC_CLRENA0
.set USBUART_1_ep_3__INTC_CLR_PD_REG, CYREG_NVIC_CLRPEND0
.set USBUART_1_ep_3__INTC_MASK, 0x10
.set USBUART_1_ep_3__INTC_NUMBER, 4
.set USBUART_1_ep_3__INTC_PRIOR_NUM, 7
.set USBUART_1_ep_3__INTC_PRIOR_REG, CYREG_NVIC_PRI_4
.set USBUART_1_ep_3__INTC_SET_EN_REG, CYREG_NVIC_SETENA0
.set USBUART_1_ep_3__INTC_SET_PD_REG, CYREG_NVIC_SETPEND0

/* USBUART_1_USB */
.set USBUART_1_USB__ARB_CFG, CYREG_USB_ARB_CFG
.set USBUART_1_USB__ARB_EP1_CFG, CYREG_USB_ARB_EP1_CFG
.set USBUART_1_USB__ARB_EP1_INT_EN, CYREG_USB_ARB_EP1_INT_EN
.set USBUART_1_USB__ARB_EP1_SR, CYREG_USB_ARB_EP1_SR
.set USBUART_1_USB__ARB_EP2_CFG, CYREG_USB_ARB_EP2_CFG
.set USBUART_1_USB__ARB_EP2_INT_EN, CYREG_USB_ARB_EP2_INT_EN
.set USBUART_1_USB__ARB_EP2_SR, CYREG_USB_ARB_EP2_SR
.set USBUART_1_USB__ARB_EP3_CFG, CYREG_USB_ARB_EP3_CFG
.set USBUART_1_USB__ARB_EP3_INT_EN, CYREG_USB_ARB_EP3_INT_EN
.set USBUART_1_USB__ARB_EP3_SR, CYREG_USB_ARB_EP3_SR
.set USBUART_1_USB__ARB_EP4_CFG, CYREG_USB_ARB_EP4_CFG
.set USBUART_1_USB__ARB_EP4_INT_EN, CYREG_USB_ARB_EP4_INT_EN
.set USBUART_1_USB__ARB_EP4_SR, CYREG_USB_ARB_EP4_SR
.set USBUART_1_USB__ARB_EP5_CFG, CYREG_USB_ARB_EP5_CFG
.set USBUART_1_USB__ARB_EP5_INT_EN, CYREG_USB_ARB_EP5_INT_EN
.set USBUART_1_USB__ARB_EP5_SR, CYREG_USB_ARB_EP5_SR
.set USBUART_1_USB__ARB_EP6_CFG, CYREG_USB_ARB_EP6_CFG
.set USBUART_1_USB__ARB_EP6_INT_EN, CYREG_USB_ARB_EP6_INT_EN
.set USBUART_1_USB__ARB_EP6_SR, CYREG_USB_ARB_EP6_SR
.set USBUART_1_USB__ARB_EP7_CFG, CYREG_USB_ARB_EP7_CFG
.set USBUART_1_USB__ARB_EP7_INT_EN, CYREG_USB_ARB_EP7_INT_EN
.set USBUART_1_USB__ARB_EP7_SR, CYREG_USB_ARB_EP7_SR
.set USBUART_1_USB__ARB_EP8_CFG, CYREG_USB_ARB_EP8_CFG
.set USBUART_1_USB__ARB_EP8_INT_EN, CYREG_USB_ARB_EP8_INT_EN
.set USBUART_1_USB__ARB_EP8_SR, CYREG_USB_ARB_EP8_SR
.set USBUART_1_USB__ARB_INT_EN, CYREG_USB_ARB_INT_EN
.set USBUART_1_USB__ARB_INT_SR, CYREG_USB_ARB_INT_SR
.set USBUART_1_USB__ARB_RW1_DR, CYREG_USB_ARB_RW1_DR
.set USBUART_1_USB__ARB_RW1_RA, CYREG_USB_ARB_RW1_RA
.set USBUART_1_USB__ARB_RW1_RA_MSB, CYREG_USB_ARB_RW1_RA_MSB
.set USBUART_1_USB__ARB_RW1_WA, CYREG_USB_ARB_RW1_WA
.set USBUART_1_USB__ARB_RW1_WA_MSB, CYREG_USB_ARB_RW1_WA_MSB
.set USBUART_1_USB__ARB_RW2_DR, CYREG_USB_ARB_RW2_DR
.set USBUART_1_USB__ARB_RW2_RA, CYREG_USB_ARB_RW2_RA
.set USBUART_1_USB__ARB_RW2_RA_MSB, CYREG_USB_ARB_RW2_RA_MSB
.set USBUART_1_USB__ARB_RW2_WA, CYREG_USB_ARB_RW2_WA
.set USBUART_1_USB__ARB_RW2_WA_MSB, CYREG_USB_ARB_RW2_WA_MSB
.set USBUART_1_USB__ARB_RW3_DR, CYREG_USB_ARB_RW3_DR
.set USBUART_1_USB__ARB_RW3_RA, CYREG_USB_ARB_RW3_RA
.set USBUART_1_USB__ARB_RW3_RA_MSB, CYREG_USB_ARB_RW3_RA_MSB
.set USBUART_1_USB__ARB_RW3_WA, CYREG_USB_ARB_RW3_WA
.set USBUART_1_USB__ARB_RW3_WA_MSB, CYREG_USB_ARB_RW3_WA_MSB
.set USBUART_1_USB__ARB_RW4_DR, CYREG_USB_ARB_RW4_DR
.set USBUART_1_USB__ARB_RW4_RA, CYREG_USB_ARB_RW4_RA
.set USBUART_1_USB__ARB_RW4_RA_MSB, CYREG_USB_ARB_RW4_RA_MSB
.set USBUART_1_USB__ARB_RW4_WA, CYREG_USB_ARB_RW4_WA
.set USBUART_1_USB__ARB_RW4_WA_MSB, CYREG_USB_ARB_RW4_WA_MSB
.set USBUART_1_USB__ARB_RW5_DR, CYREG_USB_ARB_RW5_DR
.set USBUART_1_USB__ARB_RW5_RA, CYREG_USB_ARB_RW5_RA
.set USBUART_1_USB__ARB_RW5_RA_MSB, CYREG_USB_ARB_RW5_RA_MSB
.set USBUART_1_USB__ARB_RW5_WA, CYREG_USB_ARB_RW5_WA
.set USBUART_1_USB__ARB_RW5_WA_MSB, CYREG_USB_ARB_RW5_WA_MSB
.set USBUART_1_USB__ARB_RW6_DR, CYREG_USB_ARB_RW6_DR
.set USBUART_1_USB__ARB_RW6_RA, CYREG_USB_ARB_RW6_RA
.set USBUART_1_USB__ARB_RW6_RA_MSB, CYREG_USB_ARB_RW6_RA_MSB
.set USBUART_1_USB__ARB_RW6_WA, CYREG_USB_ARB_RW6_WA
.set USBUART_1_USB__ARB_RW6_WA_MSB, CYREG_USB_ARB_RW6_WA_MSB
.set USBUART_1_USB__ARB_RW7_DR, CYREG_USB_ARB_RW7_DR
.set USBUART_1_USB__ARB_RW7_RA, CYREG_USB_ARB_RW7_RA
.set USBUART_1_USB__ARB_RW7_RA_MSB, CYREG_USB_ARB_RW7_RA_MSB
.set USBUART_1_USB__ARB_RW7_WA, CYREG_USB_ARB_RW7_WA
.set USBUART_1_USB__ARB_RW7_WA_MSB, CYREG_USB_ARB_RW7_WA_MSB
.set USBUART_1_USB__ARB_RW8_DR, CYREG_USB_ARB_RW8_DR
.set USBUART_1_USB__ARB_RW8_RA, CYREG_USB_ARB_RW8_RA
.set USBUART_1_USB__ARB_RW8_RA_MSB, CYREG_USB_ARB_RW8_RA_MSB
.set USBUART_1_USB__ARB_RW8_WA, CYREG_USB_ARB_RW8_WA
.set USBUART_1_USB__ARB_RW8_WA_MSB, CYREG_USB_ARB_RW8_WA_MSB
.set USBUART_1_USB__BUF_SIZE, CYREG_USB_BUF_SIZE
.set USBUART_1_USB__BUS_RST_CNT, CYREG_USB_BUS_RST_CNT
.set USBUART_1_USB__CR0, CYREG_USB_CR0
.set USBUART_1_USB__CR1, CYREG_USB_CR1
.set USBUART_1_USB__CWA, CYREG_USB_CWA
.set USBUART_1_USB__CWA_MSB, CYREG_USB_CWA_MSB
.set USBUART_1_USB__DMA_THRES, CYREG_USB_DMA_THRES
.set USBUART_1_USB__DMA_THRES_MSB, CYREG_USB_DMA_THRES_MSB
.set USBUART_1_USB__EP0_CNT, CYREG_USB_EP0_CNT
.set USBUART_1_USB__EP0_CR, CYREG_USB_EP0_CR
.set USBUART_1_USB__EP0_DR0, CYREG_USB_EP0_DR0
.set USBUART_1_USB__EP0_DR1, CYREG_USB_EP0_DR1
.set USBUART_1_USB__EP0_DR2, CYREG_USB_EP0_DR2
.set USBUART_1_USB__EP0_DR3, CYREG_USB_EP0_DR3
.set USBUART_1_USB__EP0_DR4, CYREG_USB_EP0_DR4
.set USBUART_1_USB__EP0_DR5, CYREG_USB_EP0_DR5
.set USBUART_1_USB__EP0_DR6, CYREG_USB_EP0_DR6
.set USBUART_1_USB__EP0_DR7, CYREG_USB_EP0_DR7
.set USBUART_1_USB__EP_ACTIVE, CYREG_USB_EP_ACTIVE
.set USBUART_1_USB__EP_TYPE, CYREG_USB_EP_TYPE
.set USBUART_1_USB__MEM_DATA, CYREG_USB_MEM_DATA_MBASE
.set USBUART_1_USB__OSCLK_DR0, CYREG_USB_OSCLK_DR0
.set USBUART_1_USB__OSCLK_DR1, CYREG_USB_OSCLK_DR1
.set USBUART_1_USB__PM_ACT_CFG, CYREG_PM_ACT_CFG5
.set USBUART_1_USB__PM_ACT_MSK, 0x01
.set USBUART_1_USB__PM_STBY_CFG, CYREG_PM_STBY_CFG5
.set USBUART_1_USB__PM_STBY_MSK, 0x01
.set USBUART_1_USB__SIE_EP1_CNT0, CYREG_USB_SIE_EP1_CNT0
.set USBUART_1_USB__SIE_EP1_CNT1, CYREG_USB_SIE_EP1_CNT1
.set USBUART_1_USB__SIE_EP1_CR0, CYREG_USB_SIE_EP1_CR0
.set USBUART_1_USB__SIE_EP2_CNT0, CYREG_USB_SIE_EP2_CNT0
.set USBUART_1_USB__SIE_EP2_CNT1, CYREG_USB_SIE_EP2_CNT1
.set USBUART_1_USB__SIE_EP2_CR0, CYREG_USB_SIE_EP2_CR0
.set USBUART_1_USB__SIE_EP3_CNT0, CYREG_USB_SIE_EP3_CNT0
.set USBUART_1_USB__SIE_EP3_CNT1, CYREG_USB_SIE_EP3_CNT1
.set USBUART_1_USB__SIE_EP3_CR0, CYREG_USB_SIE_EP3_CR0
.set USBUART_1_USB__SIE_EP4_CNT0, CYREG_USB_SIE_EP4_CNT0
.set USBUART_1_USB__SIE_EP4_CNT1, CYREG_USB_SIE_EP4_CNT1
.set USBUART_1_USB__SIE_EP4_CR0, CYREG_USB_SIE_EP4_CR0
.set USBUART_1_USB__SIE_EP5_CNT0, CYREG_USB_SIE_EP5_CNT0
.set USBUART_1_USB__SIE_EP5_CNT1, CYREG_USB_SIE_EP5_CNT1
.set USBUART_1_USB__SIE_EP5_CR0, CYREG_USB_SIE_EP5_CR0
.set USBUART_1_USB__SIE_EP6_CNT0, CYREG_USB_SIE_EP6_CNT0
.set USBUART_1_USB__SIE_EP6_CNT1, CYREG_USB_SIE_EP6_CNT1
.set USBUART_1_USB__SIE_EP6_CR0, CYREG_USB_SIE_EP6_CR0
.set USBUART_1_USB__SIE_EP7_CNT0, CYREG_USB_SIE_EP7_CNT0
.set USBUART_1_USB__SIE_EP7_CNT1, CYREG_USB_SIE_EP7_CNT1
.set USBUART_1_USB__SIE_EP7_CR0, CYREG_USB_SIE_EP7_CR0
.set USBUART_1_USB__SIE_EP8_CNT0, CYREG_USB_SIE_EP8_CNT0
.set USBUART_1_USB__SIE_EP8_CNT1, CYREG_USB_SIE_EP8_CNT1
.set USBUART_1_USB__SIE_EP8_CR0, CYREG_USB_SIE_EP8_CR0
.set USBUART_1_USB__SIE_EP_INT_EN, CYREG_USB_SIE_EP_INT_EN
.set USBUART_1_USB__SIE_EP_INT_SR, CYREG_USB_SIE_EP_INT_SR
.set USBUART_1_USB__SOF0, CYREG_USB_SOF0
.set USBUART_1_USB__SOF1, CYREG_USB_SOF1
.set USBUART_1_USB__USBIO_CR0, CYREG_USB_USBIO_CR0
.set USBUART_1_USB__USBIO_CR1, CYREG_USB_USBIO_CR1
.set USBUART_1_USB__USBIO_CR2, CYREG_USB_USBIO_CR2
.set USBUART_1_USB__USB_CLK_EN, CYREG_USB_USB_CLK_EN

/* USBUART_1_Dm */
.set USBUART_1_Dm__0__MASK, 0x80
.set USBUART_1_Dm__0__PC, CYREG_PRT15_PC7
.set USBUART_1_Dm__0__PORT, 15
.set USBUART_1_Dm__0__SHIFT, 7
.set USBUART_1_Dm__AG, CYREG_PRT15_AG
.set USBUART_1_Dm__AMUX, CYREG_PRT15_AMUX
.set USBUART_1_Dm__BIE, CYREG_PRT15_BIE
.set USBUART_1_Dm__BIT_MASK, CYREG_PRT15_BIT_MASK
.set USBUART_1_Dm__BYP, CYREG_PRT15_BYP
.set USBUART_1_Dm__CTL, CYREG_PRT15_CTL
.set USBUART_1_Dm__DM0, CYREG_PRT15_DM0
.set USBUART_1_Dm__DM1, CYREG_PRT15_DM1
.set USBUART_1_Dm__DM2, CYREG_PRT15_DM2
.set USBUART_1_Dm__DR, CYREG_PRT15_DR
.set USBUART_1_Dm__INP_DIS, CYREG_PRT15_INP_DIS
.set USBUART_1_Dm__LCD_COM_SEG, CYREG_PRT15_LCD_COM_SEG
.set USBUART_1_Dm__LCD_EN, CYREG_PRT15_LCD_EN
.set USBUART_1_Dm__PORT, 15
.set USBUART_1_Dm__PRT, CYREG_PRT15_PRT
.set USBUART_1_Dm__PRTDSI__CAPS_SEL, CYREG_PRT15_CAPS_SEL
.set USBUART_1_Dm__PRTDSI__DBL_SYNC_IN, CYREG_PRT15_DBL_SYNC_IN
.set USBUART_1_Dm__PRTDSI__OE_SEL0, CYREG_PRT15_OE_SEL0
.set USBUART_1_Dm__PRTDSI__OE_SEL1, CYREG_PRT15_OE_SEL1
.set USBUART_1_Dm__PRTDSI__OUT_SEL0, CYREG_PRT15_OUT_SEL0
.set USBUART_1_Dm__PRTDSI__OUT_SEL1, CYREG_PRT15_OUT_SEL1
.set USBUART_1_Dm__PRTDSI__SYNC_OUT, CYREG_PRT15_SYNC_OUT
.set USBUART_1_Dm__PS, CYREG_PRT15_PS
.set USBUART_1_Dm__SLW, CYREG_PRT15_SLW

/* LCD_LCDPort */
.set LCD_LCDPort__0__MASK, 0x01
.set LCD_LCDPort__0__PC, CYREG_PRT2_PC0
.set LCD_LCDPort__0__PORT, 2
.set LCD_LCDPort__0__SHIFT, 0
.set LCD_LCDPort__1__MASK, 0x02
.set LCD_LCDPort__1__PC, CYREG_PRT2_PC1
.set LCD_LCDPort__1__PORT, 2
.set LCD_LCDPort__1__SHIFT, 1
.set LCD_LCDPort__2__MASK, 0x04
.set LCD_LCDPort__2__PC, CYREG_PRT2_PC2
.set LCD_LCDPort__2__PORT, 2
.set LCD_LCDPort__2__SHIFT, 2
.set LCD_LCDPort__3__MASK, 0x08
.set LCD_LCDPort__3__PC, CYREG_PRT2_PC3
.set LCD_LCDPort__3__PORT, 2
.set LCD_LCDPort__3__SHIFT, 3
.set LCD_LCDPort__4__MASK, 0x10
.set LCD_LCDPort__4__PC, CYREG_PRT2_PC4
.set LCD_LCDPort__4__PORT, 2
.set LCD_LCDPort__4__SHIFT, 4
.set LCD_LCDPort__5__MASK, 0x20
.set LCD_LCDPort__5__PC, CYREG_PRT2_PC5
.set LCD_LCDPort__5__PORT, 2
.set LCD_LCDPort__5__SHIFT, 5
.set LCD_LCDPort__6__MASK, 0x40
.set LCD_LCDPort__6__PC, CYREG_PRT2_PC6
.set LCD_LCDPort__6__PORT, 2
.set LCD_LCDPort__6__SHIFT, 6
.set LCD_LCDPort__AG, CYREG_PRT2_AG
.set LCD_LCDPort__AMUX, CYREG_PRT2_AMUX
.set LCD_LCDPort__BIE, CYREG_PRT2_BIE
.set LCD_LCDPort__BIT_MASK, CYREG_PRT2_BIT_MASK
.set LCD_LCDPort__BYP, CYREG_PRT2_BYP
.set LCD_LCDPort__CTL, CYREG_PRT2_CTL
.set LCD_LCDPort__DM0, CYREG_PRT2_DM0
.set LCD_LCDPort__DM1, CYREG_PRT2_DM1
.set LCD_LCDPort__DM2, CYREG_PRT2_DM2
.set LCD_LCDPort__DR, CYREG_PRT2_DR
.set LCD_LCDPort__INP_DIS, CYREG_PRT2_INP_DIS
.set LCD_LCDPort__LCD_COM_SEG, CYREG_PRT2_LCD_COM_SEG
.set LCD_LCDPort__LCD_EN, CYREG_PRT2_LCD_EN
.set LCD_LCDPort__MASK, 0x7F
.set LCD_LCDPort__PORT, 2
.set LCD_LCDPort__PRT, CYREG_PRT2_PRT
.set LCD_LCDPort__PRTDSI__CAPS_SEL, CYREG_PRT2_CAPS_SEL
.set LCD_LCDPort__PRTDSI__DBL_SYNC_IN, CYREG_PRT2_DBL_SYNC_IN
.set LCD_LCDPort__PRTDSI__OE_SEL0, CYREG_PRT2_OE_SEL0
.set LCD_LCDPort__PRTDSI__OE_SEL1, CYREG_PRT2_OE_SEL1
.set LCD_LCDPort__PRTDSI__OUT_SEL0, CYREG_PRT2_OUT_SEL0
.set LCD_LCDPort__PRTDSI__OUT_SEL1, CYREG_PRT2_OUT_SEL1
.set LCD_LCDPort__PRTDSI__SYNC_OUT, CYREG_PRT2_SYNC_OUT
.set LCD_LCDPort__PS, CYREG_PRT2_PS
.set LCD_LCDPort__SHIFT, 0
.set LCD_LCDPort__SLW, CYREG_PRT2_SLW

/* Miscellaneous */
/* -- WARNING: define names containing LEOPARD or PANTHER are deprecated and will be removed in a future release */
.set CYDEV_DEBUGGING_DPS_SWD, 2
.set CYDEV_CONFIG_UNUSED_IO_AllowButWarn, 0
.set CYDEV_CONFIGURATION_MODE_COMPRESSED, 0
.set CYDEV_CONFIG_FASTBOOT_ENABLED, 0
.set CYDEV_CHIP_REV_PANTHER_PRODUCTION, 1
.set CYDEV_CHIP_REVISION_5A_PRODUCTION, 1
.set CYDEV_CHIP_MEMBER_5A, 2
.set CYDEV_CHIP_FAMILY_PSOC5, 3
.set CYDEV_CHIP_DIE_PANTHER, 2
.set CYDEV_CHIP_DIE_EXPECT, CYDEV_CHIP_DIE_PANTHER
.set BCLK__BUS_CLK__HZ, 33000000
.set BCLK__BUS_CLK__KHZ, 33000
.set BCLK__BUS_CLK__MHZ, 33
.set CYDEV_CHIP_DIE_ACTUAL, CYDEV_CHIP_DIE_EXPECT
.set CYDEV_CHIP_DIE_LEOPARD, 1
.set CYDEV_CHIP_DIE_UNKNOWN, 0
.set CYDEV_CHIP_FAMILY_PSOC3, 1
.set CYDEV_CHIP_FAMILY_PSOC4, 2
.set CYDEV_CHIP_FAMILY_UNKNOWN, 0
.set CYDEV_CHIP_FAMILY_USED, CYDEV_CHIP_FAMILY_PSOC5
.set CYDEV_CHIP_JTAG_ID, 0x0E13C069
.set CYDEV_CHIP_MEMBER_3A, 1
.set CYDEV_CHIP_MEMBER_UNKNOWN, 0
.set CYDEV_CHIP_MEMBER_USED, CYDEV_CHIP_MEMBER_5A
.set CYDEV_CHIP_REVISION_3A_ES1, 0
.set CYDEV_CHIP_REVISION_3A_ES2, 1
.set CYDEV_CHIP_REVISION_3A_ES3, 3
.set CYDEV_CHIP_REVISION_3A_PRODUCTION, 3
.set CYDEV_CHIP_REVISION_5A_ES0, 0
.set CYDEV_CHIP_REVISION_5A_ES1, 1
.set CYDEV_CHIP_REVISION_USED, CYDEV_CHIP_REVISION_5A_PRODUCTION
.set CYDEV_CHIP_REV_EXPECT, CYDEV_CHIP_REV_PANTHER_PRODUCTION
.set CYDEV_CHIP_REV_LEOPARD_ES1, 0
.set CYDEV_CHIP_REV_LEOPARD_ES2, 1
.set CYDEV_CHIP_REV_LEOPARD_ES3, 3
.set CYDEV_CHIP_REV_LEOPARD_PRODUCTION, 3
.set CYDEV_CHIP_REV_PANTHER_ES0, 0
.set CYDEV_CHIP_REV_PANTHER_ES1, 1
.set CYDEV_CONFIGURATION_COMPRESSED, 1
.set CYDEV_CONFIGURATION_DMA, 0
.set CYDEV_CONFIGURATION_ECC, 0
.set CYDEV_CONFIGURATION_IMOENABLED, CYDEV_CONFIG_FASTBOOT_ENABLED
.set CYDEV_CONFIGURATION_MODE, CYDEV_CONFIGURATION_MODE_COMPRESSED
.set CYDEV_CONFIGURATION_MODE_DMA, 2
.set CYDEV_CONFIGURATION_MODE_UNCOMPRESSED, 1
.set CYDEV_CONFIG_UNUSED_IO, CYDEV_CONFIG_UNUSED_IO_AllowButWarn
.set CYDEV_CONFIG_UNUSED_IO_AllowWithInfo, 1
.set CYDEV_CONFIG_UNUSED_IO_Disallowed, 2
.set CYDEV_DATA_CACHE_ENABLED, 0
.set CYDEV_DEBUGGING_DPS, CYDEV_DEBUGGING_DPS_SWD
.set CYDEV_DEBUGGING_DPS_Disable, 3
.set CYDEV_DEBUGGING_DPS_JTAG_4, 1
.set CYDEV_DEBUGGING_DPS_JTAG_5, 0
.set CYDEV_DEBUGGING_ENABLE, 1
.set CYDEV_DEBUGGING_REQXRES, 1
.set CYDEV_DEBUGGING_XRES, 0
.set CYDEV_DEBUG_ENABLE_MASK, 0x01
.set CYDEV_DEBUG_ENABLE_REGISTER, CYREG_MLOGIC_DBG_DBE
.set CYDEV_DMA_CHANNELS_AVAILABLE, 24
.set CYDEV_ECC_ENABLE, 0
.set CYDEV_HEAP_SIZE, 0x1000
.set CYDEV_INSTRUCT_CACHE_ENABLED, 0
.set CYDEV_PROJ_TYPE, 0
.set CYDEV_PROJ_TYPE_BOOTLOADER, 1
.set CYDEV_PROJ_TYPE_LOADABLE, 2
.set CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER, 3
.set CYDEV_PROJ_TYPE_STANDARD, 0
.set CYDEV_PROTECTION_ENABLE, 0
.set CYDEV_STACK_SIZE, 0x4000
.set CYDEV_VDDA_MV, 3300
.set CYDEV_VDDD_MV, 3300
.set CYDEV_VDDIO0_MV, 3300
.set CYDEV_VDDIO1_MV, 3300
.set CYDEV_VDDIO2_MV, 3300
.set CYDEV_VDDIO3_MV, 3300
.set CYDEV_VIO0_MV, 3300
.set CYDEV_VIO1_MV, 3300
.set CYDEV_VIO2_MV, 3300
.set CYDEV_VIO3_MV, 3300
.set DMA_CHANNELS_USED__MASK0, 0x00000000
.set CYDEV_BOOTLOADER_ENABLE, 0
.endif
