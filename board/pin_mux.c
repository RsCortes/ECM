/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKW36Z512xxx4
package_id: MKW36Z512VHT4
mcu_data: ksdk2_0
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*FUNCTION**********************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 *END**************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '42', peripheral: LPUART0, signal: RX, pin_signal: PTC6/LLWU_P14/RF_RFOSC_EN/I2C1_SCL/LPUART0_RX/TPM2_CH0/BSM_FRAME}
  - {pin_num: '43', peripheral: LPUART0, signal: TX, pin_signal: PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/LPUART0_TX/TPM2_CH1/BSM_DATA, open_drain: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTA16 is configured as TPM0_CH0 */
    PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt5);

	/* PORTB0 is configured as TPM0_CH1 */
    PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_MuxAlt5);

    /* PORTB1 is configured as ADC0_SE1 */
    PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_PinDisabledOrAnalog);

    /* PORTB2 is configured as ADC0_SE3 */
    PORT_SetPinMux(PORTB, PIN2_IDX, kPORT_PinDisabledOrAnalog);

    /* PORTB3 is configured as ADC0_SE2 */
    PORT_SetPinMux(PORTB, PIN3_IDX, kPORT_PinDisabledOrAnalog);

    /* PORTB18 is configured as ADC0_SE4 */
    PORT_SetPinMux(PORTB, PIN18_IDX, kPORT_PinDisabledOrAnalog);

    /* PORTc1 is configured as GPIO_OUT */
    PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAsGpio);

    /* PORTC3 is configured as CAN0_TX */
    PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt9);

    /* PORTC4 is configured as CAN0_RX */
    PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt9);

    /* PORTA16 (pin 4) is configured as PTA16 */
    PORT_SetPinMux(BOARD_INITPINS_LED_GREEN_PORT, BOARD_INITPINS_LED_GREEN_PIN, kPORT_MuxAsGpio);

}

/*******************************************************************************
 * EOF
 ******************************************************************************/
