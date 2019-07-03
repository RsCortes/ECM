/*
 * ADC.h
 *
 *  Created on: 02/04/2019
 *      Author: Hector
 */

#ifndef OWN_DRIVERS_ADC_H_
#define OWN_DRIVERS_ADC_H_

#include "fsl_adc8.h"

#define ADC8_BASE ADC0
#define ADC8_CHANNEL_GROUP 0U
#define ADC8_USER_CHANNEL 5U /* Temperature Sensor */

#define ADC8_IRQn ADC0_IRQn

adc16_channel_config_t ADC_Initialization(void);

#endif /* OWN_DRIVERS_ADC_H_ */
