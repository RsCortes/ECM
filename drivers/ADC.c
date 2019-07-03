/*
 * ADC.c
 *
 *  Created on: 02/04/2019
 *      Author: Hector
 */

#include "fsl_adc8.h"
#include "board.h"
#include "ADC.h"

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"

adc16_channel_config_t ADC_Initialization(void)
{
	adc16_config_t adc8ConfigStruct;
	adc16_channel_config_t adc8ChannelConfigStruct;

	ADC16_GetDefaultConfig(&adc8ConfigStruct);
	adc8ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	ADC16_Init(ADC8_BASE, &adc8ConfigStruct);
	ADC16_EnableHardwareTrigger(ADC8_BASE, false); /* Make sure the software trigger is used. */
	adc8ChannelConfigStruct.channelNumber = ADC8_USER_CHANNEL;
	adc8ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
	adc8ChannelConfigStruct.enableDifferentialConversion = false;

	return adc8ChannelConfigStruct;
}


