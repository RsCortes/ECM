/*
 * APP.c
 *
 *  Created on: 10/07/2019
 *      Author: Hector
 */

#include "APP.h"
#include "ADC.h"

void CAN_SendSensorValues()
{
	uint16_t SensorToSend = 0;

	SensorToSend = ADC_EmissionData();
	/*TODO with CAN drivers, use "send" api here and use "SensorToSend"*/
	SensorToSend = ADC_TemperatureData();
	/*TODO with CAN drivers, use "send" api here and use "SensorToSend"*/
	SensorToSend = ADC_RpmData();
	/*TODO with CAN drivers, use "send" api here and use "SensorToSend"*/
	SensorToSend = ADC_BatteryData();
	/*TODO with CAN drivers, use "send" api here and use "SensorToSend"*/

}

uint16_t ADC_EmissionData()
{
	uint16_t EmissionValue = 0;
	uint16_t ReturnValue = 0;
	EmissionValue = ADC_u16GetAdcValue(ADC16_EmissionRead);

	/*TODO Look for the sensor datasheet for values*/

	return ReturnValue;
}

uint16_t ADC_TemperatureData()
{
	uint16_t TemperatureValue = 0;
	uint16_t ReturnValue = 0;
	TemperatureValue = ADC_u16GetAdcValue(ADC16_TemperatureRead);

	/*TODO Look for the sensor datasheet for values*/

	return ReturnValue;
}

uint16_t ADC_RpmData()
{
	uint16_t RpmValue = 0;
	uint16_t ReturnValue = 0;
	RpmValue = ADC_u16GetAdcValue(ADC16_RPMRead);

	/*TODO Look for the sensor datasheet for values*/

	return ReturnValue;
}

uint16_t ADC_BatteryData()
{
	uint16_t BatteryValue = 0;
	uint16_t ReturnValue = 0;
	BatteryValue = ADC_u16GetAdcValue(ADC16_BatteryRead);

	/*TODO Look for the sensor datasheet for values*/

	return ReturnValue;
}






