#ifndef __WEIGHER_H__
#define __WEIGHER_H__
#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************
	<< --- wei_sendReadMeasureValueCommand		2015-10-26 --- >>
	˵����������ģ�鷢�Ͷ�ȡ�Ĵ���ֵ
	������
		address		= �豸485��ַ
	����ֵ��
*****************************************************************************************/
extern void wei_sendReadMeasureValueCommand(uint8_t address);

#ifdef __cplusplus
}
#endif

#endif // __WEIGHER_H__

//

