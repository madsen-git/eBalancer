#include "weigher.h"
#include "modbus.h"
#include "usart.h"


void wei_sendReadMeasureValueCommand(uint8_t address)
{
	// HEADER_LENGTH_RTU (1) + function (1) + address (2) + number (2) + CRC (2)
	//									0       1       2     3     4     5     6     7
	// 请求报文						   地址  功能码  起始地址2Byte  reg数量     CRC
	uint8_t request[_MIN_REQ_LENGTH] = {0x01,  0x03,   0x00, 0x28, 0x00, 0x02, 0x44, 0x03};	// 01 03 00 28 00 02 44 03 --> 01 03 04 00 00 49 8E 4D C7 
//	// rtu报头
//	rtu_buildRequestBasis(address, 3, 0x28, 2, request);
//	// crc
//	rtu_sendMsgPre(request, 6);
	uint8_t reqlen = rtu_buildRequestByReadRegisters(address, 0x28, 2, request, _MIN_REQ_LENGTH);
	//usart_printHex(BEUSARTNumber1, (char *)request, reqlen);
	g_rxLengthToRead2 = 9;
	g_rxCurrentIndex2 = 0;
	usart_printData(BEUSARTNumber2, (char *)request, reqlen);
}

// 
