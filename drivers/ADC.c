/*
 * ADC.c
 *
 *  Created on: 02/04/2019
 *      Author: Hector
 */

#include "fsl_adc16.h"
#include "board.h"
#include "ADC.h"

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"

static uint16_t ADC16_EmissionValue = 0;
static uint16_t ADC16_TemperatureValue = 0;
static uint16_t ADC16_RPMValue = 0;
static uint16_t ADC16_BatteryValue = 0;

void ADC_Initialization(void)
{
	adc16_config_t adc8ConfigStruct;

	ADC16_GetDefaultConfig(&adc8ConfigStruct);
	adc8ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	ADC16_Init(ADC16_BASE, &adc8ConfigStruct);
	ADC16_EnableHardwareTrigger(ADC16_BASE, false); /* Make sure the software trigger is used. */

}

/**
 * @name:   CAM_Containter10ms
 * @brief:  This function will read the ADC channel needed, and will update the static variables
 * 			to get the ADC values, call the function ADC_GetAdcValue
 * @param:  ADC_u8Channel
 * 	valid parameters:   	[1-4]
 * 	invalid parameters:		[0,[5-255])
 * @return: None
 */
void ADC_ReadAdcValue(uint8_t ADC_u8Channel)
{
	if(ADC_u8Channel < 5 && ADC_u8Channel != 0)
	{
		switch(ADC_u8Channel)
		{
			case ADC16_EmissionRead:
				ADC16_vSoftwareTrigger(ADC16_BASE, ADC16_CHANNEL_GROUP, ADC16_EmissionRead);
				while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC16_BASE, ADC16_CHANNEL_GROUP)))
					{}
				ADC16_EmissionValue = ADC16_GetChannelConversionValue(ADC16_BASE, ADC16_CHANNEL_GROUP);
				break;

			case ADC16_TemperatureRead:
				ADC16_vSoftwareTrigger(ADC16_BASE, ADC16_CHANNEL_GROUP, ADC16_TemperatureRead);
				while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC16_BASE, ADC16_CHANNEL_GROUP)))
					{}
				ADC16_TemperatureValue = ADC16_GetChannelConversionValue(ADC16_BASE, ADC16_CHANNEL_GROUP);
				break;

			case ADC16_RPMRead:
				ADC16_vSoftwareTrigger(ADC16_BASE, ADC16_CHANNEL_GROUP, ADC16_RPMRead);
				while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC16_BASE, ADC16_CHANNEL_GROUP)))
					{}
				ADC16_RPMValue = ADC16_GetChannelConversionValue(ADC16_BASE, ADC16_CHANNEL_GROUP);
				break;

			case ADC16_BatteryRead:
				ADC16_vSoftwareTrigger(ADC16_BASE, ADC16_CHANNEL_GROUP, ADC16_BatteryRead);
				while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC16_BASE, ADC16_CHANNEL_GROUP)))
					{}
				ADC16_BatteryValue = ADC16_GetChannelConversionValue(ADC16_BASE, ADC16_CHANNEL_GROUP);
				break;

			default:
				ADC16_vSoftwareTrigger(ADC16_BASE, ADC16_CHANNEL_GROUP, ADC16_EmissionRead);  /*Default, in case of 0*/
				break;
		}
	}
	else
	{
		__asm("nop");
		/*Parameter value not in range*/
	}
}

uint16_t ADC_u16GetAdcValue(uint8_t u8Sensor)
{
	uint16_t u16ReturnValue = 0;
	switch(u8Sensor)
	{
	case ADC16_EmissionRead:
		u16ReturnValue = ADC16_EmissionValue;
		break;

	case ADC16_TemperatureRead:
		u16ReturnValue = ADC16_TemperatureValue;
		break;

	case ADC16_RPMRead:
		u16ReturnValue = ADC16_RPMValue;
		break;

	case ADC16_BatteryRead:
		u16ReturnValue = ADC16_BatteryValue;
		break;
	}

	return u16ReturnValue;
}




