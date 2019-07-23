/*
 * ADC.h
 *
 *  Created on: 02/04/2019
 *      Author: Hector
 */

#ifndef OWN_DRIVERS_ADC_H_
#define OWN_DRIVERS_ADC_H_

#include "fsl_adc16.h"

#define ADC16_BASE ADC0
#define ADC16_CHANNEL_GROUP 0U
#define ADC16_EmissionRead 1U
#define ADC16_RPMRead 3U
#define ADC16_TemperatureRead 2U
#define ADC16_BatteryRead 4U

#define ADC16_RPMSensor 1U
#define ADC16_TempSensor 2U

void ADC_Initialization(void);
void ADC_ReadAdcValues();
uint16_t ADC_u16GetAdcValue(uint8_t u8Sensor);



#endif /* OWN_DRIVERS_ADC_H_ */
