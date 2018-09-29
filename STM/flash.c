#include "flash.h"
#include "stm32f10x_flash.h"

#define FLASH_START_ADDR		0x0807F800	// ���ҳ��ʼ��ַ��ÿҳ2k

void flash_write(uint32_t address, const uint8_t *pData, uint16_t dataLength)
{
	//uint16_t tmpVal;
	uint16_t *ptr = (uint16_t*)pData;
	uint16_t idx;
	//FLASH_Status status;
	if(pData == NULL || dataLength == 0) return;
	FLASH_Unlock();			// ����FLASH��̲���������
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);	// �����־λ
	/*********************************************************************************
		FLASH_FLAG_BSY				FLASHæ��־λ
		FLASH_FLAG_EOP				FLASH����������־λ
		FLASH_FLAG_PGERR			FLASH��д�����־λ
		FLASH_FLAG_WRPRTERR			FLASHҳ��д��������꾻
	**********************************************************************************/
	FLASH_ErasePage(FLASH_START_ADDR);		// ����ָ����ַҳ
	//FLASH_ProgramHalfWord(FLASH_START_ADDR+(address+i)*2,dat);		// ��ָ��ҳ��addr��ַ��ʼд
	for(idx = 0; idx < dataLength/2; ++idx)
	{
		FLASH_ProgramHalfWord(FLASH_START_ADDR + address + idx*2, *ptr++);
	}
	//FLASH_ProgramHalfWord(FLASH_START_ADDR + address, 0x1234);
	//FLASH_ProgramHalfWord(FLASH_START_ADDR + address + 2, 0x5678);
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);	// �����־λ
	FLASH_Lock();		// ����FLASH��̲���������
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
