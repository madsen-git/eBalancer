#include "flash.h"
#include "stm32f10x_flash.h"

#define FLASH_START_ADDR		0x0807F800	// 最后页起始地址，每页2k

void flash_write(uint32_t address, const uint8_t *pData, uint16_t dataLength)
{
	//uint16_t tmpVal;
	uint16_t *ptr = (uint16_t*)pData;
	uint16_t idx;
	//FLASH_Status status;
	if(pData == NULL || dataLength == 0) return;
	FLASH_Unlock();			// 解锁FLASH编程擦除控制器
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);	// 清除标志位
	/*********************************************************************************
		FLASH_FLAG_BSY				FLASH忙标志位
		FLASH_FLAG_EOP				FLASH操作结束标志位
		FLASH_FLAG_PGERR			FLASH编写错误标志位
		FLASH_FLAG_WRPRTERR			FLASH页面写保护错误标净
	**********************************************************************************/
	FLASH_ErasePage(FLASH_START_ADDR);		// 擦除指定地址页
	//FLASH_ProgramHalfWord(FLASH_START_ADDR+(address+i)*2,dat);		// 从指定页的addr地址开始写
	for(idx = 0; idx < dataLength/2; ++idx)
	{
		FLASH_ProgramHalfWord(FLASH_START_ADDR + address + idx*2, *ptr++);
	}
	//FLASH_ProgramHalfWord(FLASH_START_ADDR + address, 0x1234);
	//FLASH_ProgramHalfWord(FLASH_START_ADDR + address + 2, 0x5678);
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);	// 清除标志位
	FLASH_Lock();		// 锁定FLASH编程擦除控制器
}


void flash_read(uint32_t address, uint8_t *pDataBuf, uint16_t dataLength)
{
	uint8_t *ptr = (uint8_t*)(FLASH_START_ADDR + address);
	if(dataLength <= 0 || pDataBuf == NULL) return;
	while(dataLength-- > 0)
	{
		*pDataBuf++ = *ptr++;
	}
}

// 
