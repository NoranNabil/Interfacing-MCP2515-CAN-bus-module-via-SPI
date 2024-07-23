/*
 * MPC2515_int.h
 *
 *  Created on: Mar 14, 2024
 *      Author: Norana
 */

#ifndef MCP2515_MCP2515_INT_H_
#define MCP2515_MCP2515_INT_H_

/************************* includes ***********************/
#include "..\\..\\LIB\STD_TYPES.h"
#include "..\\..\\LIB\BIT_MATH.h"

#include "..\\..\\MCAL\SPI\SPI_int.h"
#include "..\\..\\MCAL\DIO\DIO_int.h"

#include "MCP2515_config.h"
#include "MCP2515_private.h"
/************************* Macros *************************/
typedef enum{
	OK,
	NO_Avialble_Buffer,
	Wrong_Num_Bytes,
	Message_Not_Transmitted,
	NOT_OK
}error_t;

typedef enum{
	False,
	True
}Bool_t;

typedef struct{
	u16 id;
	u8 data_bytes;
	u8 Request_Flag;
	u8 Data[8];
}Recevied_Message_t;

typedef enum{
	RX_B0,
	RX_B1
}RX_Buffer;

typedef enum{
	CAN_1000KBPS,
	CAN_500KBPS,
	CAN_250KBPS,
	CAN_200KBPS,
	CAN_125KBPS,
	CAN_100KBPS,
	CAN_80KBPS,
	CAN_50KBPS,
	CAN_40KBPS,
	CAN_33K3BPS,
	CAN_31K25BPS,
	CAN_20KBPS,
	CAN_10KBPS,
	CAN_5KBPS
}BitRate;
/************************* APIS Prototypes ***********************/
void H_MCP_Init(void);
void H_MCP_SetCANBitRate(BitRate bit_rate);

// if the message is not sent successsfully at the first time, don't try to send it again
void H_MCP_OneShotMode_Enable(void);
void H_MCP_OneShotMode_Disable(void);

// Send message on the CAN bus
error_t H_MCP_SendMessage(u16 id,u8 *message,u8 num_bytes,u8 message_priority); // priority : 3,2,1,0 where 3 is the highest one

// Check if a message is received and return RX buffer number (zero or one ) for reading
Bool_t H_MCP_CheckReception(RX_Buffer *buffer_num);

// Rollover enable : if a message received for RX_B0 while it is containing a message write it to RX_B1 if it is available to decrease overflow error
void H_MCP_Rollover_Enable(void);
void H_MCP_Rollover_Disable(void);

// Two RX buffers
void H_MCP_FilterMatching_Enable(RX_Buffer buffer_num); // only if you want to receive messages that only match your filters--- filters and masks should be configured
void H_MCP_FilterMatching_Disable(RX_Buffer buffer_num); // recieve any message on the bus with on care for filteration
// Set mask value (only two masks for two buffers)
//  The mask essentially determines which bits to apply the acceptance filters to. If any mask bit is set to a zero, that bit will automatically be accepted,regardless of the filter bit
void H_MCP_SetMask(RX_Buffer buffer_num,u16 mask_value);
// set filter value
void H_MCP_SetFilter(u8 filter_num, u16 filter_value);


error_t H_MCP_ReceiveMessage(Recevied_Message_t *ptr_message, RX_Buffer buffer_num);

#endif /* MPC2515_MCP2515_INT_H_ */
