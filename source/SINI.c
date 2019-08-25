/*
 * SINI.c
 *
 *  Created on: Jul 4, 2019
 *      Author: pmg
 */
#include "SINI.h"
#include "ADC.h"
#include <CAN_Module/CAN.h>


void SINI_vSystemInit ( void )
{
	/*!Add your Inits here*/
	ADC_Initialization();
	TPM_Setup();
	vFUN_FlexCANConfig();
	vFUN_Start2Listen_FlexCAN();


}
