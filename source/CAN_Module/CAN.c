/*
 * CAN.c
 *
 *  Created on: 01/07/2019
 *      Author: Rodrigo Serrano
 */

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "CAN.h"
#include "stdbool.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section
///////////////////////////////////////////////////////////////////////////////////////////////////

#define EXAMPLE_CAN CAN0
#define EXAMPLE_CAN_CLKSRC kCLOCK_BusClk
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define USE_CANFD (1)
/*
 *    DWORD_IN_MB    DLC    BYTES_IN_MB             Maximum MBs
 *    2              8      kFLEXCAN_8BperMB        32
 *    4              10     kFLEXCAN_16BperMB       21
 *    8              13     kFLEXCAN_32BperMB       12
 *    16             15     kFLEXCAN_64BperMB       7
 *
 * Dword in each message buffer, Length of data in bytes, Payload size must align,
 * and the Message Buffers are limited corresponding to each payload configuration:
 */
#define DWORD_IN_MB (8)
#define DLC (13)
#define BYTES_IN_MB kFLEXCAN_32BperMB
#define RX_MESSAGE_BUFFER_NUM (8)
#define TX_MESSAGE_BUFFER_NUM (9)

#define MIDDLELAYER_BUFFER_LENGTH (7)
#define MAX_APP_BUFFERS (5)
#define CAN_FD_BUFFER_LENGTH (8)

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	uint8_t u8_BufferRcvEmissions [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvEmgcyStop [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvRPM [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvFan [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvTemperature [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferRcvThrottle [MIDDLELAYER_BUFFER_LENGTH];
}st_CANReceiveBuffers;

typedef struct
{
	//uint8_t u8_BufferSendEmissions [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendEmgcyStop [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendRPM [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendFan [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendTemperature [MIDDLELAYER_BUFFER_LENGTH];
	uint8_t u8_BufferSendThrottle [MIDDLELAYER_BUFFER_LENGTH];
}st_CANSendBuffers;

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

/* flexcan callback */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData);

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section
///////////////////////////////////////////////////////////////////////////////////////////////////

volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_handle_t flexcanHandle;
flexcan_mb_transfer_t txXfer;
flexcan_mb_transfer_t rxXfer;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t txFrame;
flexcan_fd_frame_t rxFrame;
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section
///////////////////////////////////////////////////////////////////////////////////////////////////

static st_CANReceiveBuffers RcvApplicationBuffers;
static st_CANSendBuffers SendApplicationBuffers;

static st_CANReceiveBuffers *pst_ReceiveAppBuffers = &RcvApplicationBuffers;
static st_CANSendBuffers *pst_SendAppBuffers = &SendApplicationBuffers;

static uint8_t u8_CANIncomingData[3];

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section
///////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
//	pstStruct->u8_BufferRcvEmgcyStop[0]

    switch (status)
    {
        /* Process FlexCAN Rx event. */
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                rxComplete = true;
            }
            break;

        /* Process FlexCAN Tx event. */
        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                txComplete = true;
            }
            break;

        default:
            break;
    }
}

/*!
 * @brief FlexCAN configuration function
 */
void vFUN_FlexCANConfig ( void )
{
	flexcan_config_t flexcanConfig;
	flexcan_rx_mb_config_t mbConfig;

		FLEXCAN_GetDefaultConfig(&flexcanConfig);
		flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
		flexcanConfig.enableLoopBack = true;
		FLEXCAN_FDInit(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ, BYTES_IN_MB, false);

		/* Setup Rx Message Buffer. */
		mbConfig.format = kFLEXCAN_FrameFormatStandard;
		mbConfig.type = kFLEXCAN_FrameTypeData;
		mbConfig.id = FLEXCAN_ID_STD(0x123);
		FLEXCAN_SetFDRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);

		/* Setup Tx Message Buffer. */
		FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

		/* Create FlexCAN handle structure and set call back function. */
		FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

		/* Start receive data through Rx Message Buffer. */
		rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
		rxXfer.framefd = &rxFrame;
		FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);

		/* Prepare Tx Frame for sending. */
		txFrame.format = kFLEXCAN_FrameFormatStandard;
		txFrame.type = kFLEXCAN_FrameTypeData;
		txFrame.id = FLEXCAN_ID_STD(0x124);
		txFrame.length = DLC;

		txXfer.framefd = &txFrame;

		/* Send data through Tx Message Buffer. */
		txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
}

/*!
 * @brief FlexCAN send data function
 */
void vFUN_FlexCANSendNonBlocking ( uint32_t u32Module, uint32_t Value1 )
{
	txFrame.dataWord[0] = u32Module;
	txFrame.dataWord[1] = Value1;
	FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
}

/*!
 * @brief FlexCAN receive data function
 */
void vFUN_FlexCANReceiveNonBlocking ( void )
{
	if(rxComplete)
	{
		//TODO Procesing
		rxComplete = false;
		FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
	}
	else
	{
		rxComplete = false;
		FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
	}
}

void vFUN_Start2Listen_FlexCAN ( void )
{
	rxComplete = false;
	FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
}

void vFUN_CheckIfReceivedAMessage (void)
{
	if(rxComplete == true)
	{
		vFUN_FlexCANReceiveNonBlocking();
	}
	else
	{
		/*Wait for a message*/
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
