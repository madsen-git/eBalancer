#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"

#include "led.h"
#include "usart.h"
#include "SCRectifier.h"
#include "weigher.h"
#include "motor.h"
#include "exti.h"
#include "modbus.h"
#include "flash.h"
#include "timer.h"
#include "scalestype.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 外部中断1					GA_Pin1 灰白
// 3V						绿
// GND					绿白
// 电机输出PWM输出		GA_Pin8	蓝白
// 5V						蓝
// GND					黄白
#define MSV_INVALID				0xFFFFFFFF		// 测量失败时返回的值
#define MSV_LOOPS				5				// 空载测量时的最大测量次数
#define MSV_THRESHOLD			250				// 两次测量间允许误差
//#define RECONFIRM_LOOPS		32				// 32次测量值相等时认为空载无物料了
#define SCR_ADDRESS				2				// 调工板地址
#define PACK_DELAY				0x400000		// 达到目标重后延迟本时间长后进入"再确认"阶段
#define MOTOR_LOOPS_P50			100000			// 电机旋转半圈时的计数值

#define	DEBUG_LOG				0				// 输出调试信息
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	工作时用的变量
typedef struct {
	float		fltSlope;			// 每克几个测量值 = (msvHigh - msvLow)/(logicHigh - logicLow)
	float		fltWeightPack;		// = weightPackInteger + weightPackDecimal/1000
	float		fltMarginError;		// = marginErrorInteger + marginErrorDecimal/1000
	int32_t		baseValue;			// 空载值，每次秤重前都要清零
	int32_t		airValue;			// 空中量
	int32_t		packValue;			// 目标包重对应的测量值
	int32_t		marginError;		// 误差对应的测量值
	int32_t		packMin;			// BSPack.weightPack - BSPack.marginError
	int32_t		packMax;			// BSPack.weightPack + BSPack.marginError
	
	int32_t		packUpPer1;			// = packValue * feederUpPer1/100
	int32_t		packUpPer2;			// = packValue * feederUpPer2/100
	uint8_t		bPackUpPer1;		// 是否执行过上per1
	uint8_t		bPackUpPer2;		// 是否执行过上per2
	int16_t		feederUpPower0;		// 初始输出功率值
	int16_t		feederUpPer1;		// 上给料器，目标重百分比值1
	int16_t		feederUpPower1;		// 到达 feederUpPer1 时输出的功率值
	int16_t		feederUpPer2;		// 上给料器，目标重百分比值2
	int16_t		feederUpPower2;		// 到达 feederUpPer2 时输出的功率值
	
	
	int32_t		packDownPer1;		// = packValue * feederDownPer1/100
	int32_t		packDownPer2;		// = packValue * feederDownPer2/100
	uint8_t		bPackDownPer1;		// 是否执行过下per1
	uint8_t		bPackDownPer2;		// 是否执行过下per2
	int16_t		feederDownPower0;	// 初始输出功率值
	int16_t		feederDownPer1;		// 上给料器，目标重百分比值1
	int16_t		feederDownPower1;	// 到达 feederDownPer1 时输出的功率值
	int16_t		feederDownPer2;		// 上给料器，目标重百分比值2
	int16_t		feederDownPower2;	// 到达 feederDownPer2 时输出的功率值
}BSWorker;




// 工作状态
typedef enum {
	BEWorkStepNone				= 0,	// 未知
	BEWorkStepScaleEmpty		,		// 空载测量
	BEWorkStepScaleWorking		,		// 秤重中
	BEWorkStepScaleReconfirm	,		// 再次确认秤重值
	BEWorkStepScaleMotoing				// 测量 电机工作中
}BEWorkStep;

// 工作标记
typedef enum {
	BESampleWorkNone		= 0,
	BESampleWorkMoto		= 1,		// 电机检测
	BESampleWorkPower		= 2,		// 调工板检测
	BESampleWorkScale		= 0x81,		// 测量
	BESampleWorkScaleOne	= 0x82,		// 测量 单次 检测次数MSV_LOOPS定
	BESampleWorkScaleEmpty	= 0x83,		// 测量 空载
	BESampleWorkScaleFull	= 0x84,		// 测量 满载
	BESampleWorkScaleMask	= 0x80		// 测量 掩码
}BESampleWork;

void rcc_configuration(void) 
{
	//uint32_t periph = 0;
	SystemInit();
	SysTick_Config(72000);	   //配置SYSTICK时钟节拍为1ms一次
	
	
}

void delay(uint32_t deep)
{
	for(;deep > 0; --deep);
}

/*****************************************************************************************
	<< --- initData		2015-12-27 --- >>
	说明：初始化秤重前的数据
	参数：
	返回值：
*****************************************************************************************/
//void initData(void);

/*****************************************************************************************
	<< --- readyWork		2015-12-27 --- >>
	说明：准备测量，如：空载值
	参数：
		zeroValue	= 空载时的测量值
	返回值：
*****************************************************************************************/
void readyWork(int32_t zeroValue);

//////////////////////////////////////////////////////////////////////////////////////////
// 启动秤重
void startScale(BESampleWork swork);
//////////////////////////////////////////////////////////////////////////////////////////
// 停止秤重
void stopScale();
// 响应数据
void sendRespondData(uint8_t cmdA, uint8_t cmdB, const void *pData, uint8_t dataLen);
// 往pc输出信息
void printDebug(char *info);


BSScaleWork			c_scaleWork = {0};		// 当前秤重物料及配置
BSWorker			c_worker = {0};			// 秤重参数

static BEWorkStep	c_workStep = BEWorkStepNone;		// 秤重步骤
static BESampleWork	c_sampleWork = BESampleWorkNone;	// 工作方式
// 秤重开关
//static uint8_t		c_bScalerSwitch = false;
static uint32_t		c_loop;			// 功能计数
static int32_t		c_loopSum;		// 功能值和


// 秤重数据反馈
BSScaleItem 		c_scaleItem = {0};
int main()
{
#if DEBUG_LOG == 1
	char buf[80];
#endif
	//int32_t idx, val;
	int32_t msv;		// 测量值
	int32_t msvLast;	// 最后测量值
	int32_t msvAir;		// 保护空中量的测量值
	float 	fltMsv;

	// 板子参数设定
	rcc_configuration();
	// usart
	usart_config(BEUSARTNumber1, 19200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
	usart_config(BEUSARTNumber2, 19200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
	
	led_on(BELEDNumber1);
	led_on(BELEDNumber2);
	led_on(BELEDNumber3);
	usart_print(BEUSARTNumber1,"系统测试 >>>>>>>>>>>>>>>>\r\n");
	
	// 电机
	motor_config();
	// 外中断
	exti_config();
	// 
	timer_config();
	// 初始化
	msv = msvLast = msvAir = 0;
	fltMsv = 0;
	
	c_loop = 0;
	c_loopSum = 0;
	c_workStep = BEWorkStepNone;
	//c_bScalerSwitch = false;
	// 
	while(1)
	{
		++g_loopMoto;
		// 接收客户控制命令
		if(g_rxOver1)
		{
			if(g_rxCurrentIndex1 > 2 && g_rxBuffer1[g_rxCurrentIndex1-2] == '\r' && g_rxBuffer1[g_rxCurrentIndex1-1] == '\n')
			{
				if(g_rxBuffer1[0] == CMD_MASTER_START_A && g_rxBuffer1[1] == CMD_MASTER_START_B)
				{	// 启动测量
					if(sizeof(c_scaleWork) == g_rxBuffer1[2])
					{
						sendRespondData(CMD_RSP_MASTER_START_A, CMD_RSP_MASTER_START_B, NULL, 0);
						memcpy(&c_scaleWork, &g_rxBuffer1[3], sizeof(c_scaleWork));
						startScale(BESampleWorkScale);
					}
				}
				else if(g_rxBuffer1[0] == CMD_MASTER_STOP_A && g_rxBuffer1[1] == CMD_MASTER_STOP_B)
				{	// 停止测量
					sendRespondData(CMD_RSP_MASTER_STOP_A, CMD_RSP_MASTER_STOP_B, NULL, 0);
					stopScale();
				}
				else if(g_rxBuffer1[0] == CMD_MOTOR_START_A && g_rxBuffer1[1] == CMD_MOTOR_START_B)
				{	// 电机测试
					// 准备启动外中断检测
					c_sampleWork = BESampleWorkMoto;
					// 启动电机
					motor_start();
				}
				else if(g_rxBuffer1[0] == CMD_POWER_START_A && g_rxBuffer1[1] == CMD_POWER_START_B)
				{	// 调功板测试
					// 格式：命令id(2B) + 地址(1B) + 通道(1B) + 强度[0-255](1B) + 终止码(2B)
					// 如 “40 01 02 01 FF 0D 0A”表示关闭
					uint8_t addr = g_rxBuffer1[2];
					uint8_t channel = g_rxBuffer1[3];
					uint8_t power = g_rxBuffer1[4];
					scr_adjustPower(addr, channel, power);
				}
				else if(g_rxBuffer1[0] == CMD_SCALE_ONE_A && g_rxBuffer1[1] == CMD_SCALE_ONE_B)
				{	// 读一次测量值
					startScale(BESampleWorkScaleOne);
				}
				else if(g_rxBuffer1[0] == CMD_SCALE_EMPTY_A && g_rxBuffer1[1] == CMD_SCALE_EMPTY_B)
				{	// 空载时的测量值
					startScale(BESampleWorkScaleEmpty);
				}
				else if(g_rxBuffer1[0] == CMD_SCALE_FULL_A && g_rxBuffer1[1] == CMD_SCALE_FULL_B)
				{	// 满量程时的测量值
					startScale(BESampleWorkScaleFull);
				}
				else if(g_rxBuffer1[0] == CMD_SCALE_INIT_A && g_rxBuffer1[1] == CMD_SCALE_INIT_B)
				{
					if(sizeof(c_scaleWork.scaler.busface) == g_rxBuffer1[2])
					{
						memcpy(&c_scaleWork.scaler.busface, &g_rxBuffer1[3], sizeof(c_scaleWork.scaler.busface));
						sendRespondData(CMD_RSP_SCALE_INIT_A, CMD_RSP_SCALE_INIT_B, NULL, 0);
					}
				}
			}
			// 
			g_rxOver1 = false;
			g_rxCurrentIndex1 = 0;
		}
		if(c_sampleWork == BESampleWorkNone)
			continue;
		// 电机转动一次测试
		if(c_sampleWork == BESampleWorkMoto)
		{
			// 电机是否就位，产生中断就认为就绪
			if(g_motorReady)	
			{
//				#if DEBUG_LOG == 1
//					sprintf(buf, "---> moto stop loop:%d  step:%d\r\n", g_loopMoto, c_workStep);
//					printDebug(buf);
//				#endif
				sendRespondData(CMD_RSP_MOTOR_STOP_A, CMD_RSP_MOTOR_STOP_B, (const void *)&g_loopMoto, sizeof(g_loopMoto));
				exti_stop();
				motor_stop();
				c_sampleWork = BESampleWorkNone;
				c_workStep = BEWorkStepNone;			
			}
			if(g_loopMoto == MOTOR_LOOPS_P50)
			{	// 电机启动转了约1半时启动外部中断侦测
//				#if DEBUG_LOG == 1
//					sprintf(buf, "---> moto start exti loop:%d\r\n", g_loopMoto);
//					printDebug(buf);
//				#endif
				exti_start();
			}
			continue;
		}
		// 测量
		if(c_sampleWork & BESampleWorkScaleMask)
		{
			// 电机是否就位，产生中断就认为就绪
			if(g_motorReady)	
			{
				// 串口通信比较慢与电机转动（快）有冲突
//				#if DEBUG_LOG == 1
//					sprintf(buf, "---> moto stop loop:%d  step:%d\r\n", g_loopMoto, c_workStep);
//					printDebug(buf);
//				#endif
				//
				exti_stop();
				motor_stop();
				// 准备新的测量
				c_workStep = BEWorkStepScaleEmpty;		// 开始测量前，先测量空载值
				c_loop = 0;
				c_loopSum = 0;
				g_rxOver2 = false;
				g_rxCurrentIndex2 = 0;
				delay(PACK_DELAY);
				timer_start();					
			}
			if(c_workStep == BEWorkStepScaleMotoing && g_loopMoto == MOTOR_LOOPS_P50)
			{	// 电机启动转了约1半后启动外部中断侦测
//				#if DEBUG_LOG == 1
//					sprintf(buf, "---> moto start exti loop:%d\r\n", g_loopMoto);
//					printDebug(buf);
//				#endif
				exti_start();
			}
			//
			if(g_rxOver2)
			{
				msv = rtu_ads_resolveData(g_rxBuffer2, g_rxCurrentIndex2, NULL);
				
				// 无效测量值，下一次
				if(msv == -1)
				{
					//sprintf(buf, "--->   本次测量无效！ 测量值：%u\r\n", msv);
					//usart_print(BEUSARTNumber1, buf);
					g_rxOver2 = false;
					g_rxCurrentIndex2 = 0;
					continue;
				}
				
				// 测量
				if(c_sampleWork == BESampleWorkScale)
				{	
					// 测量中
					if(c_workStep == BEWorkStepScaleWorking)
					{
						msvAir = msv + c_worker.airValue;
						if(msvAir > c_worker.packMin)
						{
							#if DEBUG_LOG == 1
								fltMsv = (msv - c_worker.baseValue)/c_worker.fltSlope;
								sprintf(buf, "---> to minValue msv:%d(%0.2f) [%d-%d] msvAir:%d airV:%d \r\n", msv, fltMsv, c_worker.packMin, c_worker.packMax, msvAir, c_worker.airValue);
								//usart_print(BEUSARTNumber1, buf);
								sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
							#endif
							// 停止测量
							timer_stop();
							// 停止下料
							scr_stopPowerAll(c_scaleWork.scaler.busface.addressSCR);
							// 延迟一会后进入“再测量”阶段
							delay(PACK_DELAY);
							// 启动“再测量”
							timer_start();
							
							// 准备确认测量值
							c_workStep = BEWorkStepScaleReconfirm;
							msvLast = msv;		// 用于计算空中量
							g_rxOver2 = false;
							g_rxCurrentIndex2 = 0;
							continue;
						}
						else if(!c_worker.bPackUpPer1 && msvAir >= c_worker.feederUpPer1)
						{
							#if DEBUG_LOG == 1
								sprintf(buf, "---> power up 1 msv:%d per:%d\r\n", msv, c_worker.feederUpPer1);
								sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
							#endif
							c_worker.bPackUpPer1 = true;
							scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.upChannel, c_worker.feederUpPower1);
						}
						else if(!c_worker.bPackUpPer2 && msvAir >= c_worker.feederUpPer2)
						{
							#if DEBUG_LOG == 1
								sprintf(buf, "---> power up 2 msv:%d per:%d\r\n", msv, c_worker.feederUpPer2);
								sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
							#endif
							c_worker.bPackUpPer2 = true;
							scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.upChannel, c_worker.feederUpPower2);
						}
						else if(!c_worker.bPackDownPer1 && msvAir >= c_worker.feederDownPer1)
						{
							#if DEBUG_LOG == 1
								sprintf(buf, "---> power down 1 msv:%d per:%d\r\n", msv, c_worker.feederDownPer1);
								sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
							#endif
							c_worker.bPackDownPer1 = true;
							scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.downChannel, c_worker.feederDownPower1);
						}
						else if(!c_worker.bPackDownPer2 && msvAir >= c_worker.feederDownPer2)
						{
							#if DEBUG_LOG == 1
								sprintf(buf, "---> power down 2 msv:%d per:%d\r\n", msv, c_worker.feederDownPer2);
								sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
							#endif
							c_worker.bPackDownPer2 = true;
							scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.downChannel, c_worker.feederDownPower2);
						}
					}
					else if(c_workStep == BEWorkStepScaleReconfirm)
					{
						// 再确认测量值，这时电机已经停了RECONFIRM_LOOPS
						// 计算空中量
						c_worker.airValue = msv - msvLast;
						if(c_worker.airValue < 0) c_worker.airValue = -c_worker.airValue;
						
						fltMsv = (msv - c_worker.baseValue)/c_worker.fltSlope;
						#if DEBUG_LOG == 1
							sprintf(buf, "---> reconfirm msv:%d(%0.2f) [%d-%d] msvLast:%d airV:%d \r\n", msv, fltMsv, c_worker.packMin, c_worker.packMax, msvLast, c_worker.airValue);
							sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
						#endif
						// 不足重量则继续秤重
						if(msv < c_worker.packMin)
						{
							#if DEBUG_LOG == 1
								sprintf(buf, "---> scale continue msv:%d(%0.2f) [%d-%d] msvLast:%d airV:%d \r\n", msv, fltMsv, c_worker.packMin, c_worker.packMax, msvLast, c_worker.airValue);
								sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
							#endif
							// 
							c_workStep = BEWorkStepScaleWorking;
							g_rxOver2 = false;
							g_rxCurrentIndex2 = 0;
							// 启动下料
							scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.upChannel, c_worker.feederUpPower2);
							scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.downChannel, c_worker.feederDownPower2);
							continue;
						}
						
						// 停止测量，等待电机就位
						timer_stop();
						// 
						
						c_scaleItem.weightInteger = (int)fltMsv;
						c_scaleItem.weightDecimal = (int)(fltMsv*1000 - c_scaleItem.weightInteger*1000);
						if(msv > c_worker.packMax)
						{
							c_scaleItem.bOk = false;
							++c_scaleWork.pack.failedCount;
							#if DEBUG_LOG == 1
								sprintf(buf, "---> to max msv:%d(%0.2f) pack:%d failed:%d real:%d airV:%d \r\n", msv, fltMsv, c_scaleWork.pack.packCount, c_scaleWork.pack.failedCount, c_scaleWork.pack.realCount, c_worker.airValue);
								sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
							#endif
						}
						else
						{
							c_scaleItem.bOk = true;
							++c_scaleWork.pack.realCount;
							#if DEBUG_LOG == 1
								sprintf(buf, "---> to OKKK msv:%d(%0.2f) pack:%d failed:%d real:%d airV:%d \r\n", msv, fltMsv, c_scaleWork.pack.packCount, c_scaleWork.pack.failedCount, c_scaleWork.pack.realCount, c_worker.airValue);
								sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
							#endif
						}
						
						// 反馈给用户
						sendRespondData(CMD_RSP_SCALE_ITEM_A, CMD_RSP_SCALE_ITEM_B, &c_scaleItem, sizeof(c_scaleItem));
						// 准备外中断
						c_workStep = BEWorkStepScaleMotoing;
						// 启动电机
						motor_start();
					}
					else if(c_workStep == BEWorkStepScaleEmpty)
					{	// 每次测量前要先测量空载值
						if(c_loop < MSV_LOOPS)
						{
							c_loopSum += msv;
							if(c_loop == 0)
							{
								msvLast = msv;	// 暂存大值
								msvAir = msv;	// 暂存小值
							}
							else
							{	// 找最大最小值
								if(msvLast > msv)
									msvLast = msv;
								else if(msvAir < msv)
									msvAir = msv;
							}
							//
							++c_loop;
							if(c_loop == MSV_LOOPS)
							{
								msv = (c_loopSum - msvLast - msvAir)/(MSV_LOOPS - 2);
								msvLast = msvAir = 0;
								// 
								// 开始正式测量
								c_workStep = BEWorkStepScaleWorking;
								readyWork(msv);
								#if DEBUG_LOG == 1
								sprintf(buf, "---> empty:%u pack:%0.2f merr:%0.2f airV:%d base:%d %d[%d-%d] \r\n", msv, c_worker.fltWeightPack, c_worker.fltMarginError, c_worker.airValue, c_worker.baseValue, c_worker.packValue, c_worker.packMin, c_worker.packMax);
									sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
								#endif
								// 启动下料
								scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.upChannel, c_worker.feederUpPower0);
								scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.downChannel, c_worker.feederDownPower0);
							}
						}
					}
				}
				else if(c_sampleWork == BESampleWorkScaleEmpty || c_sampleWork == BESampleWorkScaleFull || c_sampleWork == BESampleWorkScaleOne)
				{	// 测量测试 空载  1kg  测量一次
					if(c_loop < MSV_LOOPS)
					{
						c_loopSum += msv;
						if(c_loop == 0)
						{
							msvLast = msv;	// 暂存大值
							msvAir = msv;	// 暂存小值
						}
						else
						{	// 找最大最小值
							if(msvLast > msv)
								msvLast = msv;
							else if(msvAir < msv)
								msvAir = msv;
						}
						//
						++c_loop;
						if(c_loop == MSV_LOOPS)
						{
							msv = (c_loopSum - msvLast - msvAir)/(MSV_LOOPS - 2);
							// 
							stopScale();
							if(c_sampleWork == BESampleWorkScaleEmpty)
								sendRespondData(CMD_RSP_SCALE_EMPTY_A, CMD_RSP_SCALE_EMPTY_B, &msv, sizeof(msv));
							else if(c_sampleWork == BESampleWorkScaleFull)
								sendRespondData(CMD_RSP_SCALE_FULL_A, CMD_RSP_SCALE_FULL_B, &msv, sizeof(msv));
							else if(c_sampleWork == BESampleWorkScaleOne)
								sendRespondData(CMD_RSP_SCALE_ONE_A, CMD_RSP_SCALE_ONE_B, &msv, sizeof(msv));
							delay(PACK_DELAY);
						}
					}
				}
				// 不能移到前面,等处理好了再开启下次接收
				g_rxOver2 = false;
				g_rxCurrentIndex2 = 0;	
			}

		}
	}
	
	return 0;
}


// 启动秤重
void startScale(BESampleWork swork)
{
	c_scaleWork.pack.failedCount = c_scaleWork.pack.realCount = 0;
	
	// 工作时用变量
	c_worker.fltSlope = (c_scaleWork.scaler.msvHigh - c_scaleWork.scaler.msvLow)/(c_scaleWork.scaler.logicHigh - c_scaleWork.scaler.logicLow);	// (bsWeigher.msvHigh - bsWeigher.msvLow)/(bsWeigher.logicHigh - bsWeigher.logicLow);
	c_worker.fltWeightPack = c_scaleWork.pack.weightPackInteger + c_scaleWork.pack.weightPackDecimal/1000.0f;	// bsPack.weightPackInteger + bsPack.weightPackDecimal/1000.0f;
	c_worker.fltMarginError = c_scaleWork.pack.marginErrorInteger + c_scaleWork.pack.marginErrorDecimal/1000.0f;	// bsPack.marginErrorInteger + bsPack.marginErrorDecimal/1000.0f;
	c_worker.packValue = c_worker.fltWeightPack * c_worker.fltSlope;		// 包重对应多少测量值
	c_worker.marginError = c_worker.fltMarginError * c_worker.fltSlope;		// 误差对应多少测量值
	c_worker.packMin = c_worker.baseValue - c_worker.marginError;
	c_worker.packMax = c_worker.baseValue + c_worker.marginError;
	c_worker.baseValue = 0;		// 空载值，每次秤重前都要重测
	c_worker.airValue = 0;
	// 
	c_worker.bPackUpPer1 = c_worker.bPackUpPer2 = false;
	c_worker.feederUpPower0 = c_scaleWork.power.feederUpPower0;
	c_worker.feederUpPower1 = c_scaleWork.power.feederUpPower1;
	c_worker.feederUpPower2 = c_scaleWork.power.feederUpPower2;
	c_worker.feederUpPer1 = c_scaleWork.power.feederUpPer1;
	c_worker.feederUpPer2 = c_scaleWork.power.feederUpPer2;
	c_worker.packUpPer1 = c_worker.packValue * c_worker.feederUpPer1 / 100.0f;
	c_worker.packUpPer2 = c_worker.packValue * c_worker.feederUpPer2 / 100.0f;
	// 
	c_worker.bPackDownPer1 = c_worker.bPackDownPer2 =false;
	c_worker.feederDownPower0 = c_scaleWork.power.feederDownPower0;
	c_worker.feederDownPower1 = c_scaleWork.power.feederDownPower1;
	c_worker.feederDownPower2 = c_scaleWork.power.feederDownPower2;
	c_worker.feederDownPer1 = c_scaleWork.power.feederDownPer1;
	c_worker.feederDownPer2 = c_scaleWork.power.feederDownPer2;
	c_worker.packDownPer1 = c_worker.packValue * c_worker.feederDownPer1 / 100.0f;
	c_worker.packDownPer2 = c_worker.packValue * c_worker.feederDownPer2 / 100.0f;
	// 启动测量，在计时器内发送秤重命令
	c_workStep = BEWorkStepScaleEmpty;	// 开始测量前，先测量空载值
	c_sampleWork = swork;
	c_loop = 0;
	c_loopSum = 0;
	//
	timer_start();	
}
// 停止秤重
void stopScale()
{
	c_workStep = BEWorkStepNone;
	c_loop = 0;
	c_loopSum = 0;
	
	timer_stop();
	motor_stop();
	exti_stop();
	scr_stopPowerAll(c_scaleWork.scaler.busface.addressSCR);
}


void readyWork(int32_t zeroValue)
{
	c_worker.baseValue = zeroValue;
	// 初始化其它值
	c_worker.packMin = c_worker.baseValue + c_worker.packValue - c_worker.marginError;
	c_worker.packMax = c_worker.baseValue + c_worker.packValue + c_worker.marginError;
	//bsWorker.airValue = 0;
	// 
	c_worker.bPackUpPer1 = c_worker.bPackUpPer2 = false;
	c_worker.feederUpPer1 = c_worker.baseValue + c_worker.packUpPer1;
	c_worker.feederUpPer2 = c_worker.baseValue + c_worker.packUpPer2;
	// 
	c_worker.bPackDownPer1 = c_worker.bPackDownPer2 =false;
	c_worker.feederDownPer1 = c_worker.baseValue + c_worker.packDownPer1;
	c_worker.feederDownPer2 = c_worker.baseValue + c_worker.packDownPer2;
	
	c_loop = 0;
	c_loopSum = 0;

}

static int32_t c_printloop = 10000;
void sendRespondData(uint8_t cmdA, uint8_t cmdB, const void *pData, uint8_t dataLen)
{
	// 发送响应命令
	// 命令格式：命令标记(2B) + 数据长度(1B) + 数据 + 命令尾(2B \r\n)
	// dataLen 可以为0；pData可以为null
	
	char pbuf[255] = {0};
	pbuf[0] = cmdA;
	pbuf[1] = cmdB;
	pbuf[2] = dataLen;
	pbuf[dataLen+3] = '\r';
	pbuf[dataLen+4] = '\n';
	// 数据
	if(pData && dataLen > 0)
	{
		memcpy(&pbuf[3], pData, dataLen);
	}
	usart_printData(BEUSARTNumber1, pbuf, dataLen + 5);
	//c_printloop = 10000;
	//while(c_printloop > 0) --c_printloop;
}

// 往pc输出信息
void printDebug(char *info)
{
	usart_print(BEUSARTNumber1, info);
	//c_printloop = 5000;
	//while(c_printloop > 0) --c_printloop;
	
}