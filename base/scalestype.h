/*****************************************************************************************
* 项  目：通用
* 说  明：
*	
*	
* 日  期：2016-12-15				
* 作  者：madsen	
*****************************************************************************************/
#ifndef __BASETYPE_H__
#define __BASETYPE_H__
//#pragma once


// 命令控制符
#define CMD_CONTROL_END_A			0x0D			// 数据截至符高位
#define CMD_CONTROL_END_B			0x0A			//			低位


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 控制命令 格式：命令(2B)+秤重值(4B)+尾部(2B \r\n)
#define CMD_MASTER_START_A			0x10			// 启动秤重
#define CMD_MASTER_START_B			0x01			// 
#define CMD_MASTER_STOP_A			0x10			// 停止秤重
#define CMD_MASTER_STOP_B			0x02			// 

#define CMD_SCALE_INIT_A			0x20			// 初始化设备
#define CMD_SCALE_INIT_B			0x01			// 
#define CMD_SCALE_ONE_A				0x20			// 读一次测量值
#define CMD_SCALE_ONE_B				0x02			// 
#define CMD_SCALE_EMPTY_A			0x20			// 空载测量
#define CMD_SCALE_EMPTY_B			0x04			// 
#define CMD_SCALE_FULL_A			0x20			// 满载测量
#define CMD_SCALE_FULL_B			0x08			// 

#define CMD_MOTOR_START_A			0x30			// 启动电机
#define CMD_MOTOR_START_B			0x01			// 
#define CMD_MOTOR_STOP_A			0x30			// 停止电机
#define CMD_MOTOR_STOP_B			0x02			// 
#define CMD_MOTOR_READY_A			0x30			// 电机就位，即：捕捉到外中断
#define CMD_MOTOR_READY_B			0x04			// 电机就位

#define CMD_POWER_START_A			0x40			// 调功板
#define CMD_POWER_START_B			0x01			// 

// 发送给用户的响应命令
#define CMD_RSP_FLAG_A				0x80
#define CMD_RSP_FLAG_B				0x80
#define CMD_RSP_MASTER_START_A		(CMD_RSP_FLAG_A | CMD_MASTER_START_A)	// 0x90启动秤重
#define CMD_RSP_MASTER_START_B		(CMD_RSP_FLAG_B | CMD_MASTER_START_B)	// 0x81
#define CMD_RSP_MASTER_STOP_A		(CMD_RSP_FLAG_A | CMD_MASTER_STOP_A)	// 0x90停止秤重
#define CMD_RSP_MASTER_STOP_B		(CMD_RSP_FLAG_B | CMD_MASTER_STOP_B)	// 0x82
#define CMD_RSP_MOTOR_STOP_A		(CMD_RSP_FLAG_A | CMD_MOTOR_STOP_A)		// 电机测试 电机就位
#define CMD_RSP_MOTOR_STOP_B		(CMD_RSP_FLAG_B | CMD_MOTOR_STOP_B)
#define CMD_RSP_SCALE_INIT_A		(CMD_RSP_FLAG_A | CMD_SCALE_INIT_A)
#define CMD_RSP_SCALE_INIT_B		(CMD_RSP_FLAG_B | CMD_SCALE_INIT_B)
#define CMD_RSP_SCALE_ONE_A			(CMD_RSP_FLAG_A | CMD_SCALE_ONE_A)
#define CMD_RSP_SCALE_ONE_B			(CMD_RSP_FLAG_B | CMD_SCALE_ONE_B)
#define CMD_RSP_SCALE_EMPTY_A		(CMD_RSP_FLAG_A | CMD_SCALE_EMPTY_A)
#define CMD_RSP_SCALE_EMPTY_B		(CMD_RSP_FLAG_B | CMD_SCALE_EMPTY_B)
#define CMD_RSP_SCALE_FULL_A		(CMD_RSP_FLAG_A | CMD_SCALE_FULL_A)
#define CMD_RSP_SCALE_FULL_B		(CMD_RSP_FLAG_B | CMD_SCALE_FULL_B)
#define CMD_RSP_SCALE_ITEM_A		0xF0			// 秤重结果反馈
#define CMD_RSP_SCALE_ITEM_B		0xF0
#define CMD_RSP_SCALE_STATE_A		0xF1			// 秤重状态反馈
#define CMD_RSP_SCALE_STATE_B		0xF1

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-7		xutong	电子秤接口，初始化时传输给板子
typedef struct
{
	int8_t		addressWeigher;			// 秤重模块地址
	int8_t		addressSCR;				// 调工板地址
	int8_t		upChannel;				// 上通道端口
	int8_t		downChannel;			// 下通道端口
}BSScalerBusface;

typedef struct 
{
	BSScalerBusface	busface;

	int32_t		logicLow;				// 下压逻辑值，如：50克或0克
	int32_t		logicHigh;				// 上压逻辑值，如：1000克
	int32_t		msvLow;					// 下压实测值
	int32_t		msvHigh;				// 上压实测值

}BSScaler;


//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-7		xutong	秤重时的通道功率输出
typedef struct {
	int16_t		feederUpPower0;			// 初始输出功率值
	int16_t		feederUpPer1;			// 上给料器，目标重百分比值1
	int16_t		feederUpPower1;			// 到达 feederUpPer1 时输出的功率值
	int16_t		feederUpPer2;			// 上给料器，目标重百分比值2
	int16_t		feederUpPower2;			// 到达 feederUpPer2 时输出的功率值

	int16_t		feederDownPower0;		// 初始输出功率值
	int16_t		feederDownPer1;			// 上给料器，目标重百分比值1
	int16_t		feederDownPower1;		// 到达 feederDownPer1 时输出的功率值
	int16_t		feederDownPer2;			// 上给料器，目标重百分比值2
	int16_t		feederDownPower2;		// 到达 feederDownPer2 时输出的功率值
}BSPowerConfig;

//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-10		xutong	打包信息
typedef struct {
	// 目标包重。如：123.25
	int16_t		weightPackInteger;		// 目标包重。逻辑值整数部分 123
	int16_t		weightPackDecimal;		// 目标包重。逻辑值小数部分 250	扩大1000倍
										// 目标包重的误差范围。如：0.5
	int16_t		marginErrorInteger;		// 目标包重的误差范围。逻辑值整数部分 0
	int16_t		marginErrorDecimal;		// 目标包重的误差范围。逻辑值小数部分 500	扩大1000倍
	int16_t		packCount;				// 打包的数量
	int16_t		realCount;				// 合格的打包数量
	int16_t		failedCount;			// 丢弃的包数量

}BSPack;


//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-7		xutong	开始秤重时下载的参数
typedef struct 
{
	BSPack			pack;
	BSPowerConfig	power;
	BSScaler		scaler;

}BSScaleWork;

//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-12		xutong	发送给用户的秤重数据
typedef struct 
{
	int32_t		weightInteger;			// 包重。逻辑值整数部分 123
	int32_t		weightDecimal;			// 包重。逻辑值小数部分 250	扩大1000倍
	uint8_t		bOk;					// 是否合格

}BSScaleItem;
///////////////////////////////////////////////


#pragma pack(pop)





#endif // __BASETYPE_H__
