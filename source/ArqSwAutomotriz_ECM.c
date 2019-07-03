/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    ArqSwAutomotriz_ECM.c
 * @brief   Application entry point.
 */

/*******************************************************************************
 * Imported drivers
 ******************************************************************************/

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKW36Z4.h"
#include "fsl_debug_console.h"
#include "DriverExample.h"
#include "FLEXCAN.h"
#include "ADC.h"
#include "fsl_adc8.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData);

/*********************************************************
 * Variables
 *********************************************************/
volatile bool CAN_txComplete = false;
volatile bool CAN_rxComplete = false;
flexcan_handle_t flexcanHandle;

static adc16_channel_config_t adc8ChannelConfigStruct;

//static uint8_t ADC8_u8ConversionValue = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        /* Process FlexCAN Rx event. */
        case kStatus_FLEXCAN_RxIdle:
            if (CAN_RX_MESSAGE_BUFFER_NUM == result)
            {
                CAN_rxComplete = true;
            }
            break;

        /* Process FlexCAN Tx event. */
        case kStatus_FLEXCAN_TxIdle:
            if (CAN_TX_MESSAGE_BUFFER_NUM == result)
            {
                CAN_txComplete = true;
            }
            break;
        default:
            break;
    }
}

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    flexcan_mb_transfer_t CAN_txXfer;
    flexcan_mb_transfer_t CAN_rxXfer;

   /*
    *       INITIALIZATION
    */
    FLEXCAN_Initialization(&CAN_txXfer, &CAN_rxXfer);

    adc8ChannelConfigStruct = ADC_Initialization();



    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &CAN_rxXfer);



    while(1) {
        /*Calling Driver Test*/
    	TestDriverFunction();
    	//A continuacion, un ejemplo de como manipular los datos de CAN, como el como recibir, y como mandar datos
    	/*if(CAN_rxComplete == true)
    	{
    		CAN_rxXfer.framefd->dataWord[0] == MENSAJE SE RECIBE AQUI;
    		CAN_rxComplete = false;
			FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &CAN_rxXfer);
    	}
    	else if((CAN_rxComplete == false)) NO RECIBIO MENSAJE
    	{
			CAN_rxComplete = false;
			FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &CAN_rxXfer);

			CAN_txXfer.framefd->dataWord[0] = MENSAJE A ENVIAR AQUI;  COMO ENVIAR DATOS
			CAN_txXfer.framefd->id = FLEXCAN_ID_STD(0x123);
			CAN_txComplete = false;
			FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &CAN_txXfer);
    	}
    	else CASO EXTREMO, NO DEBE DE LLEGAR AQUI
    	{
    		CAN_rxComplete = false;
       		FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &CAN_rxXfer);
    	}
    	*/
    }
    return 0 ;
}


