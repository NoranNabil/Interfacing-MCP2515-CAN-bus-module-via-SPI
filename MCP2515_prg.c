/*
 * MPC2515_prg.c
 *
 *  Created on: Mar 14, 2024
 *      Author: Norana
 */


/***************** includes ***************/
#include "MCP2515_int.h"

#include "avr/delay.h"
/***************** helper functions definitions **************/
u8 H_MCP_ReadRegister (u8 register_name);
void H_MCP_ReadRegisters (u8 registers_start_address,u8 num_registers,u8 *data);
void H_MCP_SetRegister (u8 register_name , u8 data);
void H_MCP_SetRegisters (u8 registers_start_address,u8 num_registers,u8 *data);
void H_MCP_ModifyRegister(u8 register_name,u8 mask, u8 data); // just for specific registers //The mask byte determines which bits in the register willbe allowed to change. A ‘1’ in the mask byte will allow a bit in the register to change
void H_MCP_Reset(void);
void H_MCP_SetNormalMode(void); // for CAN message transfer
void H_MCP_SetConfigMode(void); // for masks and filters configuration


/************************* APIS Declarations ***********************/
void H_MCP_Init(void){
	u8 temp = 0;
	u8 temp_data [14] ={0};
	// 1-pins init
	M_DIO_void_setPinDirection(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_DIRECTION_OUTPUT); //cs
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH); // cs is active low
	M_DIO_void_setPinDirection(DIO_PORTB_ID,DIO_PIN5,DIO_PIN_DIRECTION_OUTPUT); //SI
	M_DIO_void_setPinDirection(DIO_PORTB_ID,DIO_PIN6,DIO_PIN_DIRECTION_INPUT); //SO
	M_DIO_void_setPinDirection(DIO_PORTB_ID,DIO_PIN7,DIO_PIN_DIRECTION_OUTPUT); //SCK
	// 2-spi inti
	M_SPI_void_init(SPI_normal_speed,SPI_prescalar_4_2,SPI_idle_low,SPI_sample_first,SPI_MSB_first,SPI_master); //// big problem
	// 3-H_MCP reset
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW);
	M_SPI_void_sendChar(H_MCP_RESET_Inst);
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
	// delay after reset -  Oscillator Start-up Timer
	_delay_ms(10);
	// 4-configuration of data rates
	// 5-enter normal mode
	H_MCP_SetNormalMode();
	// 6-make sure you are in the normal mode
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW);
	temp = H_MCP_ReadRegister(H_MCP_CANSTAT_0_Reg);
	while ((temp & 0xe0) != 0){
		H_MCP_SetNormalMode();
		M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW);
		temp = H_MCP_ReadRegister(H_MCP_CANSTAT_0_Reg);
		M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
	}
	// 7-clear TX registers
	H_MCP_SetRegisters (H_MCP_TXB0CTRL_Reg,14,temp_data);
	H_MCP_SetRegisters (H_MCP_TXB1CTRL_Reg,14,temp_data);
	H_MCP_SetRegisters (H_MCP_TXB2CTRL_Reg,14,temp_data);
	// 8-clear RX control registers
	H_MCP_SetRegister(H_MCP_RXB0CTRL_Reg,0);
	H_MCP_SetRegister(H_MCP_RXB1CTRL_Reg,0);
	// 9-diable interrupts : CANINTE

	// 10-clear masks
	H_MCP_SetMask(RX_B0,0); // accept all id bits
	H_MCP_SetMask(RX_B1,0);
	// 11-clear filters
	H_MCP_SetFilter(0,0);
	H_MCP_SetFilter(0,0);
	H_MCP_SetFilter(0,0);
	H_MCP_SetFilter(0,0);
	H_MCP_SetFilter(0,0);
	H_MCP_SetFilter(0,0);
}

void H_MCP_SetCANBitRate(BitRate bit_rate){
	//1-enter config mode
	H_MCP_SetConfigMode();
	//2- set cfg registers
	switch (bit_rate){
		case CAN_1000KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_1000kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_1000kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_1000kBPS_CFG3);
			break;
		case CAN_500KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_500kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_500kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_500kBPS_CFG3);
			break;
		case CAN_250KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_250kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_250kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_250kBPS_CFG3);
			break;
		case CAN_200KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_200kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_200kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_200kBPS_CFG3);
			break;
		case CAN_125KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_125kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_125kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_125kBPS_CFG3);
			break;
		case CAN_100KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_100kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_100kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_100kBPS_CFG3);
			break;
		case CAN_80KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_80kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_80kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_80kBPS_CFG3);
			break;
		case CAN_50KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_50kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_50kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_50kBPS_CFG3);
				break;
		case CAN_40KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_40kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_40kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_40kBPS_CFG3);
			break;
		case CAN_33K3BPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_33k3BPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_33k3BPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_33k3BPS_CFG3);
			break;
		case CAN_31K25BPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_31k25BPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_31k25BPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_31k25BPS_CFG3);
			break;
		case CAN_20KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_20kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_20kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_20kBPS_CFG3);
			break;
		case CAN_10KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_10kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_10kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_10kBPS_CFG3);
			break;
		case CAN_5KBPS:
			H_MCP_SetRegister (H_MCP_CNF1_Reg,MCP_8MHz_5kBPS_CFG1);
			H_MCP_SetRegister (H_MCP_CNF2_Reg,MCP_8MHz_5kBPS_CFG2);
			H_MCP_SetRegister (H_MCP_CNF3_Reg,MCP_8MHz_5kBPS_CFG3);
			break;
		default:
			break;
	}
	// 3-return to normal mode
	H_MCP_SetNormalMode();
}

void H_MCP_OneShotMode_Enable(void){
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW);
	// 2-control register modify
	H_MCP_ModifyRegister(H_MCP_CANCTRL_0_Reg,0b00001000,0b00001000);
	// 3-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
}

void H_MCP_OneShotMode_Disable(void){
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW);
	// 2-control register modify
	H_MCP_ModifyRegister(H_MCP_CANCTRL_0_Reg,0b00001000,0b00001000);
	// 3-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
}


error_t H_MCP_SendMessage(u16 id,u8 *message,u8 num_bytes,u8 message_priority){
	u8 temp = 0;
	u8 buffer_num = 0;
	// 1- check for the available buffer
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW);
	M_SPI_void_sendChar(H_MCP_READ_STATUS_Inst);
	M_SPI_void_receiveChar(&temp);
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
	if ((temp & 0x40) == 1){
		buffer_num = 2;
	}
	else if ((temp & 0x10) == 1){
		buffer_num = 1;
	}
	else if ((temp & 0x04) == 1){
		buffer_num = 0;
	}
	else {
		return NO_Avialble_Buffer;
	}
	// 2-
	switch(buffer_num){
		case 2:
			// 1-clear request and set the message priority
			if ((message_priority >= 0) && (message_priority <=3)){
				H_MCP_ModifyRegister(H_MCP_TXB2CTRL_Reg,0b00001011,message_priority);
			}
			else{
				H_MCP_ModifyRegister(H_MCP_TXB2CTRL_Reg,0b00001011,0x00);
			}
			// 2-prepare id
			H_MCP_SetRegister(H_MCP_TXB2SIDH_Reg,(u8)((id & 0b0000011111111000)>>3));
			H_MCP_SetRegister(H_MCP_TXB2SIDL_Reg,(u8)((id & 0b0000000000000111)<<5));
			// 3-load data registers
			if ((num_bytes <9) && (num_bytes>0)){
				H_MCP_SetRegister(H_MCP_TXB2DLC_Reg,num_bytes);
				H_MCP_SetRegisters (H_MCP_TXB2D0_Reg,num_bytes,message);
			}
			else{
				return Wrong_Num_Bytes;
			}
			// 4-request transmission
			H_MCP_ModifyRegister(H_MCP_TXB2CTRL_Reg,0b00001000,0b00001000);
			// 5-check on message transmission
			temp = H_MCP_ReadRegister(H_MCP_TXB2CTRL_Reg);
			if (((temp>>3) & 0x07) != 0){
				return Message_Not_Transmitted;
			}
			break;
		case 1:
			// 1-clear request and set the message priority
			if ((message_priority >= 0) && (message_priority <=3)){
				H_MCP_ModifyRegister(H_MCP_TXB1CTRL_Reg,0b00001011,message_priority);
			}
			else{
				H_MCP_ModifyRegister(H_MCP_TXB1CTRL_Reg,0b00001011,0x00);
			}
			// 2-prepare id
			H_MCP_SetRegister(H_MCP_TXB1SIDH_Reg,(u8)((id & 0b0000011111111000)>>3));
			H_MCP_SetRegister(H_MCP_TXB1SIDL_Reg,(u8)((id & 0b0000000000000111)<<5));
			// 3-load data registers
			if (num_bytes <9){
				H_MCP_SetRegister(H_MCP_TXB1DLC_Reg,num_bytes);
				H_MCP_SetRegisters (H_MCP_TXB1D0_Reg,num_bytes,message);
			}
			else{
				return Wrong_Num_Bytes;
			}
			// 4-request transmission
			H_MCP_ModifyRegister(H_MCP_TXB1CTRL_Reg,0b00001000,0b00001000);
			// 5-check on message transmission
			temp = H_MCP_ReadRegister(H_MCP_TXB1CTRL_Reg);
			if (((temp>>3) & 0x07) != 0){
				return Message_Not_Transmitted;
			}
			break;
		case 0:
			// 1-clear request and set the message priority
			if ((message_priority >= 0) && (message_priority <=3)){
				H_MCP_ModifyRegister(H_MCP_TXB0CTRL_Reg,0b00001011,message_priority);
			}
			else{
				H_MCP_ModifyRegister(H_MCP_TXB0CTRL_Reg,0b00001011,0x00);
			}
			// 2-prepare id
			H_MCP_SetRegister(H_MCP_TXB0SIDH_Reg,(u8)((id & 0b0000011111111000)>>3));
			H_MCP_SetRegister(H_MCP_TXB0SIDL_Reg,(u8)((id & 0b0000000000000111)<<5));
			// 3-load data registers
			if (num_bytes <9){
				H_MCP_SetRegister(H_MCP_TXB0DLC_Reg,num_bytes);
				H_MCP_SetRegisters (H_MCP_TXB0D0_Reg,num_bytes,message);
			}
			else{
				return Wrong_Num_Bytes;
			}
			// 4-request transmission
			H_MCP_ModifyRegister(H_MCP_TXB2CTRL_Reg,0b00001000,0b00001000);
			// 5-check on message transmission
			temp = H_MCP_ReadRegister(H_MCP_TXB2CTRL_Reg);
			if (((temp>>3) & 0x07) != 0){ //check if there is an error /////////////// problem : no check on message transmission
				return Message_Not_Transmitted;
			}
			break;
		default:break;
	}
	return OK;
}

Bool_t H_MCP_CheckReception(RX_Buffer *buffer_num){
	u8 status = 0;
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW); // cs is active low
	// 2-send read status command
	M_SPI_void_sendChar(H_MCP_READ_STATUS_Inst);
	// 3-read the status
	M_SPI_void_sendChar(0x00); //don't care
	M_SPI_void_receiveChar(&status);
	// 5-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
	// 6-check
	if ((status & 0x01) == 1){ //as RX1IF , RX0IF are bits 1,0
		*buffer_num = RX_B0;
	}
	else if ((status & 0x02) == 1){
		*buffer_num = RX_B1;
	}
	else{
		*buffer_num = 100;
		return False;
	}
	return True;
}

void H_MCP_Rollover_Enable(void){
	H_MCP_ModifyRegister(H_MCP_RXB0CTRL_Reg,0b0000100,0b00000100);
}
void H_MCP_Rollover_Disable(void){
	H_MCP_ModifyRegister(H_MCP_RXB0CTRL_Reg,0b0000100,0b00000000);
}

void H_MCP_FilterMatching_Enable(RX_Buffer buffer_num){
	switch(buffer_num){
		case RX_B0:
			H_MCP_ModifyRegister(H_MCP_RXB0CTRL_Reg,0b0110000,0b00000000);
			break;
		case RX_B1:
			H_MCP_ModifyRegister(H_MCP_RXB1CTRL_Reg,0b0110000,0b00000000);
			break;
		default:
			break;
	}
}
void H_MCP_FilterMatching_Disable(RX_Buffer buffer_num){
	switch(buffer_num){
		case RX_B0:
			H_MCP_ModifyRegister(H_MCP_RXB0CTRL_Reg,0b0110000,0b01100000);
			break;
		case RX_B1:
			H_MCP_ModifyRegister(H_MCP_RXB1CTRL_Reg,0b0110000,0b01100000);
			break;
		default:
			break;
	}
}

void H_MCP_SetMask(RX_Buffer buffer_num,u16 mask_value){
	u8 temp = 0;
	// 1- enter config mode
	H_MCP_SetConfigMode();
	// 2- write mask value
	// High
	temp = (u8)((mask_value & 0b0000011111111000) >> 3);

	switch(buffer_num){
		case RX_B0:
			// High
			H_MCP_SetRegister (H_MCP_RXM0SIDH_Reg ,temp);
			//Low
			temp = (u8)((mask_value & 0b0000000000000111) << 5);
			H_MCP_SetRegister (H_MCP_RXM0SIDL_Reg ,temp);
			break;
		case RX_B1:
			// High
			H_MCP_SetRegister (H_MCP_RXM1SIDH_Reg ,temp);
			//Low
			temp = (u8)((mask_value & 0b0000000000000111) << 5);
			H_MCP_SetRegister (H_MCP_RXM1SIDL_Reg ,temp);
			break;
		default:
			break;
	}
	// 3-return to normal mode for CAN messages transfer
	H_MCP_SetNormalMode();
}
void H_MCP_SetFilter(u8 filter_num, u16 filter_value){
	u8 temp = 0;
	// 1- enter config mode
	H_MCP_SetConfigMode();
	// 2- write mask value
	// High
	temp = (u8)((filter_value & 0b0000011111111000) >> 3);

	switch(filter_num){
		case 0:
			// High
			H_MCP_SetRegister (H_MCP_RXF0SIDH_Reg ,temp);
			//Low
			temp = (u8)((filter_value & 0b0000000000000111) << 5);
			H_MCP_SetRegister (H_MCP_RXF0SIDL_Reg ,temp);
			break;
		case 1:
			// High
			H_MCP_SetRegister (H_MCP_RXF1SIDH_Reg ,temp);
			//Low
			temp = (u8)((filter_value & 0b0000000000000111) << 5);
			H_MCP_SetRegister (H_MCP_RXF1SIDL_Reg ,temp);
			break;
		case 2:
			// High
			H_MCP_SetRegister (H_MCP_RXF2SIDH_Reg ,temp);
			//Low
			temp = (u8)((filter_value & 0b0000000000000111) << 5);
			H_MCP_SetRegister (H_MCP_RXF2SIDL_Reg ,temp);
			break;
		case 3:
			// High
			H_MCP_SetRegister (H_MCP_RXF3SIDH_Reg ,temp);
			//Low
			temp = (u8)((filter_value & 0b0000000000000111) << 5);
			H_MCP_SetRegister (H_MCP_RXF3SIDL_Reg ,temp);
			break;
		case 4:
			// High
			H_MCP_SetRegister (H_MCP_RXF4SIDH_Reg ,temp);
			//Low
			temp = (u8)((filter_value & 0b0000000000000111) << 5);
			H_MCP_SetRegister (H_MCP_RXF4SIDL_Reg ,temp);
			break;
		case 5:
			// High
			H_MCP_SetRegister (H_MCP_RXF5SIDH_Reg ,temp);
			//Low
			temp = (u8)((filter_value & 0b0000000000000111) << 5);
			H_MCP_SetRegister (H_MCP_RXF5SIDL_Reg ,temp);
			break;
		default:
			break;
	}
	// 3-return to normal mode for CAN messages transfer
	H_MCP_SetNormalMode();
}


error_t H_MCP_ReceiveMessage(Recevied_Message_t *ptr_message, RX_Buffer buffer_num){
	u8 iter = 0;
	u8 temp [13] = {0};
	switch (buffer_num){
		case RX_B0:
			// 1- read id , dlc , data registers strating from idH to Data7
			H_MCP_ReadRegisters(H_MCP_RXB0SIDH_Reg,13,temp);
			// 2-check if it is a data or request
			iter = H_MCP_ReadRegister(H_MCP_RXB0CTRL_Reg);
			// 3-clear RXnIF
			H_MCP_ModifyRegister(H_MCP_CANINTF_Reg,0b00000001,0);
			break;
		case RX_B1:
			// 1- read id , dlc , data registers strating from idH to Data7
			H_MCP_ReadRegisters(H_MCP_RXB1SIDH_Reg,13,temp);
			// 2-check if it is a data or request
			iter = H_MCP_ReadRegister(H_MCP_RXB0CTRL_Reg);
			// 3-clear RXnIF
			H_MCP_ModifyRegister(H_MCP_CANINTF_Reg,0b00000010,0);
			break;
		default:
			return NOT_OK;
			break;
	}
	// 3- assign values in the struct
	ptr_message->Request_Flag = iter & 0b00001000;
	ptr_message->id = (temp[0]<< 3)+(temp[1]>>5);
	ptr_message->data_bytes = temp[4];
	for (iter = 0; iter<8 ; iter++){
		ptr_message->Data[iter] = temp[5+iter];
	}
	return OK;
}

/***************** helper functions declaration **************/


/***************** helper functions declarations **************/
u8 H_MCP_ReadRegister (u8 register_name){
	u8 data = 0;
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW); // cs is active low
	// 2-read command
	M_SPI_void_sendChar(H_MCP_READ_Inst);
	// 3-register address
	M_SPI_void_sendChar(register_name);
	// 4-return the read data
	M_SPI_void_sendChar(0x00); //don't care
	M_SPI_void_receiveChar(&data);//////////////// problem 2 : need just read without write?
	// 5-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);

	return data;
}
void H_MCP_ReadRegisters (u8 registers_start_address,u8 num_registers,u8 *data){
	u8 iter = 0;
	u8 temp = 0;
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW); // cs is active low
	// 2-read command
	M_SPI_void_sendChar(H_MCP_READ_Inst);
	// 3-register address
	M_SPI_void_sendChar(registers_start_address);
	// 4-read registers
	for (iter=0; iter<num_registers; iter++){
		M_SPI_void_sendChar(0x00); //don't care
		M_SPI_void_receiveChar(&temp);
		data[iter] = temp;
	}
	//////////////// problem 2 : need just read without write?
	// 5-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
}

void H_MCP_SetRegister (u8 register_name , u8 data){
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW); // cs is active low
	// 2-read command
	M_SPI_void_sendChar(H_MCP_WRITE_Inst);
	// 3-register address
	M_SPI_void_sendChar(register_name);
	// 4-return the read data
	M_SPI_void_sendChar(data);
	// 5-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
}

void H_MCP_SetRegisters (u8 registers_start_address,u8 num_registers,u8 *data){
	u8 iter = 0;
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW); // cs is active low
	// 2-read command
	M_SPI_void_sendChar(H_MCP_WRITE_Inst);
	// 3-register address
	M_SPI_void_sendChar(registers_start_address);
	// 4-read registers
	for (iter=0; iter<num_registers; iter++){
		M_SPI_void_sendChar(data[iter]);
	}
	// 5-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
}

void H_MCP_ModifyRegister(u8 register_name,u8 mask, u8 data){
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW); // cs is active low
	// 2-read command
	M_SPI_void_sendChar(H_MCP_BIT_MODIFY_Inst);
	// 3-register address
	M_SPI_void_sendChar(register_name);
	// 4-send mask
	M_SPI_void_sendChar(mask);
	// 5-data
	M_SPI_void_sendChar(data);
	// 6-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
}

void H_MCP_Reset(void){
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW);
	M_SPI_void_sendChar(H_MCP_RESET_Inst);
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
}
void H_MCP_SetNormalMode(void){
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW);
	// 2-control register modify
	H_MCP_ModifyRegister(H_MCP_CANCTRL_0_Reg,0b11100000,0b00000000);
	// 3-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
}

void H_MCP_SetConfigMode(void){
	// 1-ship select
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_LOW);
	// 2-control register modify
	H_MCP_ModifyRegister(H_MCP_CANCTRL_0_Reg,0b11100000,0b10000000);
	// 3-ship unselect
	M_DIO_void_setPinValue(DIO_PORTB_ID,DIO_PIN4,DIO_PIN_HIGH);
}
