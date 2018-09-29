#ifndef __WEIGHER_H__
#define __WEIGHER_H__
#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************
	<< --- wei_sendReadMeasureValueCommand		2015-10-26 --- >>
	说明：往秤重模块发送读取寄存器值
	参数：
		address		= 设备485地址
	返回值：
*****************************************************************************************/
extern void wei_sendReadMeasureValueCommand(uint8_t address);

#ifdef __cplusplus
}
#endif

#endif // __WEIGHER_H__

//

