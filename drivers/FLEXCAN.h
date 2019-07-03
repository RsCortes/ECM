/*
 * FLEXCAN.h
 *
 *  Created on: 27/06/2019
 *      Author: Hector
 */

#ifndef FLEXCAN_H_
#define FLEXCAN_H_

#include "fsl_flexcan.h" /*Esta en contra de mis reglas de programacion incluir.h en .h, pero es necesario en este caso*/
#include "stdint.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
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
#define DLC (13)
#define BYTES_IN_MB kFLEXCAN_32BperMB
#define CAN_RX_MESSAGE_BUFFER_NUM (8)
#define CAN_TX_MESSAGE_BUFFER_NUM (9)

/*******************************************************************************
 * PROTOTIPES
 ******************************************************************************/
void FLEXCAN_Initialization(flexcan_mb_transfer_t *CAN_txXfer, flexcan_mb_transfer_t *CAN_rxXfer);

#endif /* FLEXCAN_H_ */
