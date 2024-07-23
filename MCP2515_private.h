/*
 * MPC2515_pri.h
 *
 *  Created on: Mar 14, 2024
 *      Author: Norana
 */

#ifndef MCP2515_H_MCP2515_PRIVATE_H_
#define MCP2515_H_MCP2515_PRIVATE_H_

/********************registers******************/
#define H_MCP_BFPCTRL_Reg		0x0C

#define H_MCP_TXRTSCTRL_Reg		0x0D
/*************************/
//can status registers
#define H_MCP_CANSTAT_0_Reg		0x0E
#define H_MCP_CANSTAT_1_Reg		0x1E
#define H_MCP_CANSTAT_2_Reg		0x2E
#define H_MCP_CANSTAT_3_Reg		0x3E
#define H_MCP_CANSTAT_4_Reg		0x4E
#define H_MCP_CANSTAT_5_Reg		0x5E
#define H_MCP_CANSTAT_6_Reg		0x6E
#define H_MCP_CANSTAT_7_Reg		0x7E
//bits
#define CANSTAT_OPMOD2_Bit		7 //operation mode , normal mode =000
#define CANSTAT_OPMOD1_Bit		6
#define CANSTAT_OPMOD0_Bit		5
/***************************/
//can control registers
#define H_MCP_CANCTRL_0_Reg		0x0F
#define H_MCP_CANCTRL_1_Reg		0x1F
#define H_MCP_CANCTRL_2_Reg		0x2F
#define H_MCP_CANCTRL_3_Reg		0x3F
#define H_MCP_CANCTRL_4_Reg		0x4F
#define H_MCP_CANCTRL_5_Reg		0x5F
#define H_MCP_CANCTRL_6_Reg		0x6F
#define H_MCP_CANCTRL_7_Reg		0x7F
//can control bits
#define CANCTRL_REQOP2_Bit		7 //request operation mode bits
#define CANCTRL_REQOP1_Bit		6 //request operation mode bits
#define CANCTRL_REQOP0_Bit		5 //request operation mode bits
#define CANCTRL_ABAT_Bit		4 //abort transmission of all messages in the three buffers
#define CANCTRL_OSM_Bit			3 //one shot mode enable if bit = 1
/***************************/
#define H_MCP_TEC_Reg				0x1C //TRANSMIT ERROR COUNTER REGISTER
#define H_MCP_REC_Reg				0x1D //RECEIVE ERROR COUNTER REGISTER

#define H_MCP_CNF3_Reg			0x28 //bit rate configurations

#define H_MCP_CNF2_Reg			0x29

#define H_MCP_CNF1_Reg			0x2A
/*************************/
#define H_MCP_CANINTE_Reg			0x2B //can interrupt enable register
#define H_MCP_CANINTF_Reg			0x2C //can interrupt flag register
//bits
#define CANINTF_TX2IF_Bit		4 //set if transmission has been done correctly
#define CANINTF_TX1IF_Bit		3
#define CANINTF_TX0IF_Bit		2
#define CANINTF_RX1IF_Bit		1 //set it reception has been done successfully
#define CANINTF_RX0IF_Bit		0

#define H_MCP_EFLG_Reg			0x2D

/***** transmission registers******/
//control registers
#define H_MCP_TXB0CTRL_Reg		0x30 //transmission control 0
#define H_MCP_TXB1CTRL_Reg		0x40 //transmission control 1
#define H_MCP_TXB2CTRL_Reg		0x50 //transmission control 2
//control bits
#define TXBnCTRL_ABTF_Bit		6 // Message Aborted Flag bit
#define TXBnCTRL_MLOA_Bit		5 // Message Lost Arbitration bit
#define TXBnCTRL_TXERR_Bit		4 // Transmission Error Detected bit
#define TXBnCTRL_TXREQ_Bit		3 // Message Transmit Request bit
#define TXBnCTRL_TXP1_Bit		1 //priority (11 >>> highest, 10 >>> intermdiate high, 01>>> intermdiate low , 00>>lowest)
#define TXBnCTRL_TXP0_Bit		0
// id registers
//high
#define H_MCP_TXB0SIDH_Reg		0x31 //higher bits of standard id0 >>> SID10:SID3
#define H_MCP_TXB1SIDH_Reg		0x41 //higher bits of standard id0
#define H_MCP_TXB2SIDH_Reg		0x51 //higher bits of standard id0
//low
#define H_MCP_TXB0SIDL_Reg		0x32 //lower bits of standard id0
#define H_MCP_TXB1SIDL_Reg		0x42 //lower bits of standard id0
#define H_MCP_TXB2SIDL_Reg		0x52 //lower bits of standard id0
//bits
#define TXBnSIDL_SID2_Bit		7
#define TXBnSIDL_SID1_Bit		6
#define TXBnSIDL_SID0_Bit		5
#define TXBnSIDL_EXIDE_Bit		4 //exrended identifier enable bit =0 for standard identifier
// data size registers
#define H_MCP_TXB0DLC_Reg			0x35 // data size
#define H_MCP_TXB1DLC_Reg			0x45 // data size
#define H_MCP_TXB2DLC_Reg			0x55 // data size
//bits
#define TXBnDLC_RTR_Bit			6 //Remote Transmission Request bit ,1 = Transmitted message will be a remote transmit request,0 = Transmitted message will be a data frame
#define TXBnDLC_DLC3_Bit		3 //Sets the number of data bytes to be transmitted (0 to 8 bytes).
#define TXBnDLC_DLC2_Bit		2
#define TXBnDLC_DLC1_Bit		1
#define TXBnDLC_DLC0_Bit		0
//data registers
// buffer0
#define H_MCP_TXB0D0_Reg			0x36 //databyte 0
#define H_MCP_TXB0D1_Reg			0x37 //databyte 1
#define H_MCP_TXB0D2_Reg			0x38 //databyte 3
#define H_MCP_TXB0D3_Reg			0x39 //databyte 4
#define H_MCP_TXB0D4_Reg			0x3A //databyte 5
#define H_MCP_TXB0D5_Reg			0x3B //databyte 6
#define H_MCP_TXB0D6_Reg			0x3C //databyte 7
#define H_MCP_TXB0D7_Reg			0x3D //databyte 8
// buffer1
#define H_MCP_TXB1D0_Reg			0x46 //databyte 0
#define H_MCP_TXB1D1_Reg			0x47 //databyte 1
#define H_MCP_TXB1D2_Reg			0x48 //databyte 3
#define H_MCP_TXB1D3_Reg			0x49 //databyte 4
#define H_MCP_TXB1D4_Reg			0x4A //databyte 5
#define H_MCP_TXB1D5_Reg			0x4B //databyte 6
#define H_MCP_TXB1D6_Reg			0x4C //databyte 7
#define H_MCP_TXB1D7_Reg			0x4D //databyte 8
//buffer2
#define H_MCP_TXB2D0_Reg			0x56 //databyte 0
#define H_MCP_TXB2D1_Reg			0x57 //databyte 1
#define H_MCP_TXB2D2_Reg			0x58 //databyte 3
#define H_MCP_TXB2D3_Reg			0x59 //databyte 4
#define H_MCP_TXB2D4_Reg			0x5A //databyte 5
#define H_MCP_TXB2D5_Reg			0x5B //databyte 6
#define H_MCP_TXB2D6_Reg			0x5C //databyte 7
#define H_MCP_TXB2D7_Reg			0x5D //databyte 8
/***** reception registers******/
// Buffer 0
#define H_MCP_RXB0CTRL_Reg		0x60 //reception control
#define H_MCP_RXB0SIDH_Reg		0x61 // SID high
#define H_MCP_RXB0SIDL_Reg		0x62 // SID low
#define H_MCP_RXB0DLC_Reg		0x65 // data length
#define H_MCP_RXB0Dm_Reg		0x66 // data registers (66:6D)
// Buffer 0 - mask registers
#define H_MCP_RXM0SIDH_Reg		0x20
#define H_MCP_RXM0SIDL_Reg		0x21

// Buffer 1
#define H_MCP_RXB1CTRL_Reg		0x70 //reception control
#define H_MCP_RXB1SIDH_Reg		0x71 // SID high
#define H_MCP_RXB1SIDL_Reg		0x72 // SID low
#define H_MCP_RXB1DLC_Reg		0x75 // data length
#define H_MCP_RXB1Dm_Reg		0x76 // data registers (76:7D)
// Buffer 1 - mask registers
#define H_MCP_RXM1SIDH_Reg		0x24
#define H_MCP_RXM1SIDL_Reg		0x25

// filters registers
// f0
#define H_MCP_RXF0SIDH_Reg		0x00
#define H_MCP_RXF0SIDL_Reg		0x01
// f1
#define H_MCP_RXF1SIDH_Reg		0x04
#define H_MCP_RXF1SIDL_Reg		0x05
// f2
#define H_MCP_RXF2SIDH_Reg		0x08
#define H_MCP_RXF2SIDL_Reg		0x09
// f3
#define H_MCP_RXF3SIDH_Reg		0x10
#define H_MCP_RXF3SIDL_Reg		0x11
// f4
#define H_MCP_RXF4SIDH_Reg		0x14
#define H_MCP_RXF4SIDL_Reg		0x15
// f5
#define H_MCP_RXF5SIDH_Reg		0x18
#define H_MCP_RXF5SIDL_Reg		0x19

/********************/

/**************************** instructions ***************************/
#define H_MCP_RESET_Inst			0b11000000 //sets registers to the defaults && sets module to the configuration mode
#define H_MCP_READ_Inst			0b00000011 //read data from a register beginning at selected address , should be followed by byte of address
#define H_MCP_READ_RXB0SIDH_Inst	0b10010000 // Receive Buffer 0, Start at RXB0SIDH
#define H_MCP_READ_RXB0D0_Inst	0b10010010 // Receive Buffer 0, Start at RXB0D0
#define H_MCP_READ_RXB1SIDH_Inst	0b10010100 // Receive Buffer 1, Start at RXB1SIDH
#define H_MCP_READ_RXB1D0_Inst	0b10010110 // Receive Buffer 1, Start at RXB1D0
#define H_MCP_WRITE_Inst			0b00000010 //Writes data to the register beginning at the selected address, should be followed by the data
#define H_MCP_LOAD_TXB0SIDH_Inst	0b01000000 // TX Buffer 0, Start at TXB0SIDH
#define H_MCP_LOAD_TXB0D0_Inst	0b01000001 // TX Buffer 0, Start at TXB0D0
#define H_MCP_LOAD_TXB1SIDH_Inst	0b01000010 // TX Buffer 1, Start at TXB1SIDH
#define H_MCP_LOAD_TXB1D0_Inst	0b01000011 // TX Buffer 0, Start at TXB1D0
#define H_MCP_LOAD_TXB2SIDH_Inst	0b01000100 // TX Buffer 0, Start at TXB2SIDH
#define H_MCP_LOAD_TXB2D0_Inst	0b01000101 // TX Buffer 0, Start at TXB2D0
#define H_MCP_RTS_TXB2_Inst		0b10000100 //message request to send : TXB2 //////////////////////////////// problem 1
#define H_MCP_RTS_TXB1_Inst		0b10000010 //message request to send : TXB1
#define H_MCP_RTS_TXB0_Inst		0b10000001 //message request to send : TXB0
#define H_MCP_READ_STATUS_Inst	0b10100000 // reads several status bits for transmit and receive functions.
// reply with flags as follow >> b7(MSB-first received):b0(LSB) >>>[TX2IF(CANINTF),TX2REQ,TX1IF,TX1REQ,TX0IF,TX0REQ,RX1IF,RX0IF]
#define H_MCP_RX_STATUS_Inst		0b10110000 //indicates filter match and message type (standard, extended and/or remote) of received message. //look at data sheet page 70
#define H_MCP_BIT_MODIFY_Inst		0b00000101 // Allows the user to set or clear individual bits in a particular register, should followed by [address byte : mask byte : data byte]

/****************************** CAN bit rate masks ***********************/
#define MCP_8MHz_1000kBPS_CFG1 (0x00)
#define MCP_8MHz_1000kBPS_CFG2 (0x80)
#define MCP_8MHz_1000kBPS_CFG3 (0x80)

#define MCP_8MHz_500kBPS_CFG1 (0x00)
#define MCP_8MHz_500kBPS_CFG2 (0x90)
#define MCP_8MHz_500kBPS_CFG3 (0x82)

#define MCP_8MHz_250kBPS_CFG1 (0x00)
#define MCP_8MHz_250kBPS_CFG2 (0xB1)
#define MCP_8MHz_250kBPS_CFG3 (0x85)

#define MCP_8MHz_200kBPS_CFG1 (0x00)
#define MCP_8MHz_200kBPS_CFG2 (0xB4)
#define MCP_8MHz_200kBPS_CFG3 (0x86)

#define MCP_8MHz_125kBPS_CFG1 (0x01)
#define MCP_8MHz_125kBPS_CFG2 (0xB1)
#define MCP_8MHz_125kBPS_CFG3 (0x85)

#define MCP_8MHz_100kBPS_CFG1 (0x01)
#define MCP_8MHz_100kBPS_CFG2 (0xB4)
#define MCP_8MHz_100kBPS_CFG3 (0x86)

#define MCP_8MHz_80kBPS_CFG1 (0x01)
#define MCP_8MHz_80kBPS_CFG2 (0xBF)
#define MCP_8MHz_80kBPS_CFG3 (0x87)

#define MCP_8MHz_50kBPS_CFG1 (0x03)
#define MCP_8MHz_50kBPS_CFG2 (0xB4)
#define MCP_8MHz_50kBPS_CFG3 (0x86)

#define MCP_8MHz_40kBPS_CFG1 (0x03)
#define MCP_8MHz_40kBPS_CFG2 (0xBF)
#define MCP_8MHz_40kBPS_CFG3 (0x87)

#define MCP_8MHz_33k3BPS_CFG1 (0x47)
#define MCP_8MHz_33k3BPS_CFG2 (0xE2)
#define MCP_8MHz_33k3BPS_CFG3 (0x85)

#define MCP_8MHz_31k25BPS_CFG1 (0x07)
#define MCP_8MHz_31k25BPS_CFG2 (0xA4)
#define MCP_8MHz_31k25BPS_CFG3 (0x84)

#define MCP_8MHz_20kBPS_CFG1 (0x07)
#define MCP_8MHz_20kBPS_CFG2 (0xBF)
#define MCP_8MHz_20kBPS_CFG3 (0x87)

#define MCP_8MHz_10kBPS_CFG1 (0x0F)
#define MCP_8MHz_10kBPS_CFG2 (0xBF)
#define MCP_8MHz_10kBPS_CFG3 (0x87)

#define MCP_8MHz_5kBPS_CFG1 (0x1F)
#define MCP_8MHz_5kBPS_CFG2 (0xBF)
#define MCP_8MHz_5kBPS_CFG3 (0x87)

#endif /* MPC2515_H_MCP2515_PRIVATE_H_ */
