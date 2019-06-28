/*
 * FLEXCAN.c
 *
 *  Created on: 27/06/2019
 *      Author: Hector
 */
#include "FLEXCAN.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"


/*******************************************************************************
 * VARIABLES
 ******************************************************************************/
flexcan_fd_frame_t CAN_txFrame, CAN_rxFrame;
static uint8_t CruiseControlSpeed = 0;

void FLEXCAN_Initialization(flexcan_mb_transfer_t *CAN_txXfer, flexcan_mb_transfer_t *CAN_rxXfer)
{
	flexcan_config_t flexcanConfig;
	flexcan_rx_mb_config_t CAN_mbConfig;
	/* Init FlexCAN module. */
	/*
	* flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
	* flexcanConfig.baudRate = 1000000U;
	* flexcanConfig.maxMbNum = 16;
	* flexcanConfig.enableLoopBack = false;
	* flexcanConfig.enableSelfWakeup = false;
	* flexcanConfig.enableIndividMask = false;
	* flexcanConfig.enableDoze = false;
	*/
	FLEXCAN_GetDefaultConfig(&flexcanConfig);
	flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
	flexcanConfig.enableLoopBack = false;
	FLEXCAN_FDInit(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ, BYTES_IN_MB, false);

	/* Setup Rx Message Buffer. */
	CAN_mbConfig.format = kFLEXCAN_FrameFormatStandard;
	CAN_mbConfig.type = kFLEXCAN_FrameTypeData;
	CAN_mbConfig.id = FLEXCAN_ID_STD(0x125); /*TBD MAYBE 0x02, o 0x03*/
	FLEXCAN_SetFDRxMbConfig(EXAMPLE_CAN, CAN_RX_MESSAGE_BUFFER_NUM, &CAN_mbConfig, true);

	/* Setup Tx Message Buffer. */
	FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN, CAN_TX_MESSAGE_BUFFER_NUM, true);
	/* Start receive data through Rx Message Buffer. */
	CAN_rxXfer->mbIdx = (uint8_t)CAN_RX_MESSAGE_BUFFER_NUM;
	CAN_rxXfer->framefd = &CAN_rxFrame;

	/* Prepare Tx Frame for sending. */
	CAN_txFrame.format = kFLEXCAN_FrameFormatStandard;
	CAN_txFrame.type = kFLEXCAN_FrameTypeData;
	CAN_txFrame.id = FLEXCAN_ID_STD(0x123);
	CAN_txFrame.length = DLC;

	CAN_txXfer->mbIdx = CAN_TX_MESSAGE_BUFFER_NUM;
	CAN_txXfer->framefd = &CAN_txFrame;
}


