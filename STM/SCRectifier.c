#include "SCRectifier.h"
#include "usart.h"

/*****************************************************************************************
	<< --- scr_crc		2015-10-20 --- >>
	说明：对数据crc校验
	参数：
		pdata	= 被校验的数据
		len		= 要校验的数据长度为len-1,pdata最后一位存放校验和
	返回值：
*****************************************************************************************/
static void scr_crc(uint8_t *pdata, uint16_t len)
{
	uint16_t idx;
	if(!pdata || len == 0) return;
	pdata[len-1] = 0;
	for(idx = 0; idx < len-1; ++idx)
	{
		pdata[len-1] += pdata[idx];
	}
}


void scr_adjustPower(uint8_t address, uint8_t outway, uint8_t toPower)
{
	//					0    1    2    3	4	 5	  6    7    8
	uint8_t cmd[9] = {0xAA,0xBB,0xCC,0x04,0xCC,0xCC,0x00,0x00,0xCC};
	// 设备地址
	cmd[2] = address;
	// 通道号
	cmd[4] = outway;
	// 输出功率
	cmd[5] = toPower;
	// crc
	scr_crc(cmd, sizeof(cmd));
	
	usart_printData(BEUSARTNumber2, (char *)cmd, sizeof(cmd));
}

void scr_stopPower(uint8_t address, uint8_t outway)
{
	//					0    1    2    3	4	 5	  6    7    8
	uint8_t cmd[9] = {0xAA,0xBB,0xCC,0x04,0xCC,0xCC,0x00,0x00,0xCC};
	// 设备地址
	cmd[2] = address;
	// 通道号
	cmd[4] = outway;
	// 输出功率
	cmd[5] = 0xFF;
	// crc
	scr_crc(cmd, sizeof(cmd));
	
	usart_printData(BEUSARTNumber2, (char *)cmd, sizeof(cmd));
}

void scr_stopPowerAll(uint8_t address)
{
	// 控制4路输出功率的指令为9个字节(迅速执行功率命令)
	//					0    1    2    3	4	 5	  6    7    8
	uint8_t cmd[9] = {0xAA,0xBB,0xCC,0x01,0xCC,0xCC,0xCC,0xCC,0xCC};
	// 设备地址
	cmd[2] = address;
	// 通道号1的功率
	cmd[4] = 0xFF;
	// 通道号1的功率
	cmd[5] = 0xFF;
	// 通道号1的功率
	cmd[6] = 0xFF;
	// 通道号1的功率
	cmd[7] = 0xFF;
	// crc
	scr_crc(cmd, sizeof(cmd));
	
	usart_printData(BEUSARTNumber2, (char *)cmd, sizeof(cmd));

}
//
