/*
 * APP.h
 *
 *  Created on: 10/07/2019
 *      Author: Hector
 */

#ifndef APP_H_
#define APP_H_

#include "stdint.h"

void CAN_SendSensorValues();
uint16_t ADC_EmissionData();
uint16_t ADC_TemperatureData();
uint16_t ADC_RpmData();
uint16_t ADC_BatteryData();

#endif /* APP_H_ */
