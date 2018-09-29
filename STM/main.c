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
// �ⲿ�ж�1					GA_Pin1 �Ұ�
// 3V						��
// GND					�̰�
// ������PWM���		GA_Pin8	����
// 5V						��
// GND					�ư�
#define MSV_INVALID				0xFFFFFFFF		// ����ʧ��ʱ���ص�ֵ
#define MSV_LOOPS				5				// ���ز���ʱ������������
#define MSV_THRESHOLD			250				// ���β������������
//#define RECONFIRM_LOOPS		32				// 32�β���ֵ���ʱ��Ϊ������������
#define SCR_ADDRESS				2				// �������ַ
#define PACK_DELAY				0x400000		// �ﵽĿ���غ��ӳٱ�ʱ�䳤�����"��ȷ��"�׶�
#define MOTOR_LOOPS_P50			100000			// �����ת��Ȧʱ�ļ���ֵ

#define	DEBUG_LOG				0				// ���������Ϣ
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	����ʱ�õı���
typedef struct {
	float		fltSlope;			// ÿ�˼�������ֵ = (msvHigh - msvLow)/(logicHigh - logicLow)
	float		fltWeightPack;		// = weightPackInteger + weightPackDecimal/1000
	float		fltMarginError;		// = marginErrorInteger + marginErrorDecimal/1000
	int32_t		baseValue;			// ����ֵ��ÿ�γ���ǰ��Ҫ����
	int32_t		airValue;			// ������
	int32_t		packValue;			// Ŀ����ض�Ӧ�Ĳ���ֵ
	int32_t		marginError;		// ����Ӧ�Ĳ���ֵ
	int32_t		packMin;			// BSPack.weightPack - BSPack.marginError
	int32_t		packMax;			// BSPack.weightPack + BSPack.marginError
	
	int32_t		packUpPer1;			// = packValue * feederUpPer1/100
	int32_t		packUpPer2;			// = packValue * feederUpPer2/100
	uint8_t		bPackUpPer1;		// �Ƿ�ִ�й���per1
	uint8_t		bPackUpPer2;		// �Ƿ�ִ�й���per2
	int16_t		feederUpPower0;		// ��ʼ�������ֵ
	int16_t		feederUpPer1;		// �ϸ�������Ŀ���ذٷֱ�ֵ1
	int16_t		feederUpPower1;		// ���� feederUpPer1 ʱ����Ĺ���ֵ
	int16_t		feederUpPer2;		// �ϸ�������Ŀ���ذٷֱ�ֵ2
	int16_t		feederUpPower2;		// ���� feederUpPer2 ʱ����Ĺ���ֵ
	
	
	int32_t		packDownPer1;		// = packValue * feederDownPer1/100
	int32_t		packDownPer2;		// = packValue * feederDownPer2/100
	uint8_t		bPackDownPer1;		// �Ƿ�ִ�й���per1
	uint8_t		bPackDownPer2;		// �Ƿ�ִ�й���per2
	int16_t		feederDownPower0;	// ��ʼ�������ֵ
	int16_t		feederDownPer1;		// �ϸ�������Ŀ���ذٷֱ�ֵ1
	int16_t		feederDownPower1;	// ���� feederDownPer1 ʱ����Ĺ���ֵ
	int16_t		feederDownPer2;		// �ϸ�������Ŀ���ذٷֱ�ֵ2
	int16_t		feederDownPower2;	// ���� feederDownPer2 ʱ����Ĺ���ֵ
}BSWorker;




// ����״̬
typedef enum {
	BEWorkStepNone				= 0,	// δ֪
	BEWorkStepScaleEmpty		,		// ���ز���
	BEWorkStepScaleWorking		,		// ������
	BEWorkStepScaleReconfirm	,		// �ٴ�ȷ�ϳ���ֵ
	BEWorkStepScaleMotoing				// ���� ���������
}BEWorkStep;

// �������
typedef enum {
	BESampleWorkNone		= 0,
	BESampleWorkMoto		= 1,		// ������
	BESampleWorkPower		= 2,		// ��������
	BESampleWorkScale		= 0x81,		// ����
	BESampleWorkScaleOne	= 0x82,		// ���� ���� ������MSV_LOOPS��
	BESampleWorkScaleEmpty	= 0x83,		// ���� ����
	BESampleWorkScaleFull	= 0x84,		// ���� ����
	BESampleWorkScaleMask	= 0x80		// ���� ����
}BESampleWork;

void rcc_configuration(void) 
{
	//uint32_t periph = 0;
	SystemInit();
	SysTick_Config(72000);	   //����SYSTICKʱ�ӽ���Ϊ1msһ��
	
	
}

void delay(uint32_t deep)
{
	for(;deep > 0; --deep);
}

/*****************************************************************************************
	<< --- initData		2015-12-27 --- >>
	˵������ʼ������ǰ������
	������
	����ֵ��
*****************************************************************************************/
//void initData(void);

/*****************************************************************************************
	<< --- readyWork		2015-12-27 --- >>
	˵����׼���������磺����ֵ
	������
		zeroValue	= ����ʱ�Ĳ���ֵ
	����ֵ��
*****************************************************************************************/
void readyWork(int32_t zeroValue);

//////////////////////////////////////////////////////////////////////////////////////////
// ��������
void startScale(BESampleWork swork);
//////////////////////////////////////////////////////////////////////////////////////////
// ֹͣ����
void stopScale();
// ��Ӧ����
void sendRespondData(uint8_t cmdA, uint8_t cmdB, const void *pData, uint8_t dataLen);
// ��pc�����Ϣ
void printDebug(char *info);


BSScaleWork			c_scaleWork = {0};		// ��ǰ�������ϼ�����
BSWorker			c_worker = {0};			// ���ز���

static BEWorkStep	c_workStep = BEWorkStepNone;		// ���ز���
static BESampleWork	c_sampleWork = BESampleWorkNone;	// ������ʽ
// ���ؿ���
//static uint8_t		c_bScalerSwitch = false;
static uint32_t		c_loop;			// ���ܼ���
static int32_t		c_loopSum;		// ����ֵ��


// �������ݷ���
BSScaleItem 		c_scaleItem = {0};
int main()
{
#if DEBUG_LOG == 1
	char buf[80];
#endif
	//int32_t idx, val;
	int32_t msv;		// ����ֵ
	int32_t msvLast;	// ������ֵ
	int32_t msvAir;		// �����������Ĳ���ֵ
	float 	fltMsv;

	// ���Ӳ����趨
	rcc_configuration();
	// usart
	usart_config(BEUSARTNumber1, 19200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
	usart_config(BEUSARTNumber2, 19200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
	
	led_on(BELEDNumber1);
	led_on(BELEDNumber2);
	led_on(BELEDNumber3);
	usart_print(BEUSARTNumber1,"ϵͳ���� >>>>>>>>>>>>>>>>\r\n");
	
	// ���
	motor_config();
	// ���ж�
	exti_config();
	// 
	timer_config();
	// ��ʼ��
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
		// ���տͻ���������
		if(g_rxOver1)
		{
			if(g_rxCurrentIndex1 > 2 && g_rxBuffer1[g_rxCurrentIndex1-2] == '\r' && g_rxBuffer1[g_rxCurrentIndex1-1] == '\n')
			{
				if(g_rxBuffer1[0] == CMD_MASTER_START_A && g_rxBuffer1[1] == CMD_MASTER_START_B)
				{	// ��������
					if(sizeof(c_scaleWork) == g_rxBuffer1[2])
					{
						sendRespondData(CMD_RSP_MASTER_START_A, CMD_RSP_MASTER_START_B, NULL, 0);
						memcpy(&c_scaleWork, &g_rxBuffer1[3], sizeof(c_scaleWork));
						startScale(BESampleWorkScale);
					}
				}
				else if(g_rxBuffer1[0] == CMD_MASTER_STOP_A && g_rxBuffer1[1] == CMD_MASTER_STOP_B)
				{	// ֹͣ����
					sendRespondData(CMD_RSP_MASTER_STOP_A, CMD_RSP_MASTER_STOP_B, NULL, 0);
					stopScale();
				}
				else if(g_rxBuffer1[0] == CMD_MOTOR_START_A && g_rxBuffer1[1] == CMD_MOTOR_START_B)
				{	// �������
					// ׼���������жϼ��
					c_sampleWork = BESampleWorkMoto;
					// �������
					motor_start();
				}
				else if(g_rxBuffer1[0] == CMD_POWER_START_A && g_rxBuffer1[1] == CMD_POWER_START_B)
				{	// ���������
					// ��ʽ������id(2B) + ��ַ(1B) + ͨ��(1B) + ǿ��[0-255](1B) + ��ֹ��(2B)
					// �� ��40 01 02 01 FF 0D 0A����ʾ�ر�
					uint8_t addr = g_rxBuffer1[2];
					uint8_t channel = g_rxBuffer1[3];
					uint8_t power = g_rxBuffer1[4];
					scr_adjustPower(addr, channel, power);
				}
				else if(g_rxBuffer1[0] == CMD_SCALE_ONE_A && g_rxBuffer1[1] == CMD_SCALE_ONE_B)
				{	// ��һ�β���ֵ
					startScale(BESampleWorkScaleOne);
				}
				else if(g_rxBuffer1[0] == CMD_SCALE_EMPTY_A && g_rxBuffer1[1] == CMD_SCALE_EMPTY_B)
				{	// ����ʱ�Ĳ���ֵ
					startScale(BESampleWorkScaleEmpty);
				}
				else if(g_rxBuffer1[0] == CMD_SCALE_FULL_A && g_rxBuffer1[1] == CMD_SCALE_FULL_B)
				{	// ������ʱ�Ĳ���ֵ
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
		// ���ת��һ�β���
		if(c_sampleWork == BESampleWorkMoto)
		{
			// ����Ƿ��λ�������жϾ���Ϊ����
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
			{	// �������ת��Լ1��ʱ�����ⲿ�ж����
//				#if DEBUG_LOG == 1
//					sprintf(buf, "---> moto start exti loop:%d\r\n", g_loopMoto);
//					printDebug(buf);
//				#endif
				exti_start();
			}
			continue;
		}
		// ����
		if(c_sampleWork & BESampleWorkScaleMask)
		{
			// ����Ƿ��λ�������жϾ���Ϊ����
			if(g_motorReady)	
			{
				// ����ͨ�űȽ�������ת�����죩�г�ͻ
//				#if DEBUG_LOG == 1
//					sprintf(buf, "---> moto stop loop:%d  step:%d\r\n", g_loopMoto, c_workStep);
//					printDebug(buf);
//				#endif
				//
				exti_stop();
				motor_stop();
				// ׼���µĲ���
				c_workStep = BEWorkStepScaleEmpty;		// ��ʼ����ǰ���Ȳ�������ֵ
				c_loop = 0;
				c_loopSum = 0;
				g_rxOver2 = false;
				g_rxCurrentIndex2 = 0;
				delay(PACK_DELAY);
				timer_start();					
			}
			if(c_workStep == BEWorkStepScaleMotoing && g_loopMoto == MOTOR_LOOPS_P50)
			{	// �������ת��Լ1��������ⲿ�ж����
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
				
				// ��Ч����ֵ����һ��
				if(msv == -1)
				{
					//sprintf(buf, "--->   ���β�����Ч�� ����ֵ��%u\r\n", msv);
					//usart_print(BEUSARTNumber1, buf);
					g_rxOver2 = false;
					g_rxCurrentIndex2 = 0;
					continue;
				}
				
				// ����
				if(c_sampleWork == BESampleWorkScale)
				{	
					// ������
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
							// ֹͣ����
							timer_stop();
							// ֹͣ����
							scr_stopPowerAll(c_scaleWork.scaler.busface.addressSCR);
							// �ӳ�һ�����롰�ٲ������׶�
							delay(PACK_DELAY);
							// �������ٲ�����
							timer_start();
							
							// ׼��ȷ�ϲ���ֵ
							c_workStep = BEWorkStepScaleReconfirm;
							msvLast = msv;		// ���ڼ��������
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
						// ��ȷ�ϲ���ֵ����ʱ����Ѿ�ͣ��RECONFIRM_LOOPS
						// ���������
						c_worker.airValue = msv - msvLast;
						if(c_worker.airValue < 0) c_worker.airValue = -c_worker.airValue;
						
						fltMsv = (msv - c_worker.baseValue)/c_worker.fltSlope;
						#if DEBUG_LOG == 1
							sprintf(buf, "---> reconfirm msv:%d(%0.2f) [%d-%d] msvLast:%d airV:%d \r\n", msv, fltMsv, c_worker.packMin, c_worker.packMax, msvLast, c_worker.airValue);
							sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
						#endif
						// �����������������
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
							// ��������
							scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.upChannel, c_worker.feederUpPower2);
							scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.downChannel, c_worker.feederDownPower2);
							continue;
						}
						
						// ֹͣ�������ȴ������λ
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
						
						// �������û�
						sendRespondData(CMD_RSP_SCALE_ITEM_A, CMD_RSP_SCALE_ITEM_B, &c_scaleItem, sizeof(c_scaleItem));
						// ׼�����ж�
						c_workStep = BEWorkStepScaleMotoing;
						// �������
						motor_start();
					}
					else if(c_workStep == BEWorkStepScaleEmpty)
					{	// ÿ�β���ǰҪ�Ȳ�������ֵ
						if(c_loop < MSV_LOOPS)
						{
							c_loopSum += msv;
							if(c_loop == 0)
							{
								msvLast = msv;	// �ݴ��ֵ
								msvAir = msv;	// �ݴ�Сֵ
							}
							else
							{	// �������Сֵ
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
								// ��ʼ��ʽ����
								c_workStep = BEWorkStepScaleWorking;
								readyWork(msv);
								#if DEBUG_LOG == 1
								sprintf(buf, "---> empty:%u pack:%0.2f merr:%0.2f airV:%d base:%d %d[%d-%d] \r\n", msv, c_worker.fltWeightPack, c_worker.fltMarginError, c_worker.airValue, c_worker.baseValue, c_worker.packValue, c_worker.packMin, c_worker.packMax);
									sendRespondData(CMD_RSP_SCALE_STATE_A, CMD_RSP_SCALE_STATE_B, buf, sizeof(buf));
								#endif
								// ��������
								scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.upChannel, c_worker.feederUpPower0);
								scr_adjustPower(c_scaleWork.scaler.busface.addressSCR, c_scaleWork.scaler.busface.downChannel, c_worker.feederDownPower0);
							}
						}
					}
				}
				else if(c_sampleWork == BESampleWorkScaleEmpty || c_sampleWork == BESampleWorkScaleFull || c_sampleWork == BESampleWorkScaleOne)
				{	// �������� ����  1kg  ����һ��
					if(c_loop < MSV_LOOPS)
					{
						c_loopSum += msv;
						if(c_loop == 0)
						{
							msvLast = msv;	// �ݴ��ֵ
							msvAir = msv;	// �ݴ�Сֵ
						}
						else
						{	// �������Сֵ
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
				// �����Ƶ�ǰ��,�ȴ�������ٿ����´ν���
				g_rxOver2 = false;
				g_rxCurrentIndex2 = 0;	
			}

		}
	}
	
	return 0;
}


// ��������
void startScale(BESampleWork swork)
{
	c_scaleWork.pack.failedCount = c_scaleWork.pack.realCount = 0;
	
	// ����ʱ�ñ���
	c_worker.fltSlope = (c_scaleWork.scaler.msvHigh - c_scaleWork.scaler.msvLow)/(c_scaleWork.scaler.logicHigh - c_scaleWork.scaler.logicLow);	// (bsWeigher.msvHigh - bsWeigher.msvLow)/(bsWeigher.logicHigh - bsWeigher.logicLow);
	c_worker.fltWeightPack = c_scaleWork.pack.weightPackInteger + c_scaleWork.pack.weightPackDecimal/1000.0f;	// bsPack.weightPackInteger + bsPack.weightPackDecimal/1000.0f;
	c_worker.fltMarginError = c_scaleWork.pack.marginErrorInteger + c_scaleWork.pack.marginErrorDecimal/1000.0f;	// bsPack.marginErrorInteger + bsPack.marginErrorDecimal/1000.0f;
	c_worker.packValue = c_worker.fltWeightPack * c_worker.fltSlope;		// ���ض�Ӧ���ٲ���ֵ
	c_worker.marginError = c_worker.fltMarginError * c_worker.fltSlope;		// ����Ӧ���ٲ���ֵ
	c_worker.packMin = c_worker.baseValue - c_worker.marginError;
	c_worker.packMax = c_worker.baseValue + c_worker.marginError;
	c_worker.baseValue = 0;		// ����ֵ��ÿ�γ���ǰ��Ҫ�ز�
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
	// �����������ڼ�ʱ���ڷ��ͳ�������
	c_workStep = BEWorkStepScaleEmpty;	// ��ʼ����ǰ���Ȳ�������ֵ
	c_sampleWork = swork;
	c_loop = 0;
	c_loopSum = 0;
	//
	timer_start();	
}
// ֹͣ����
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
	// ��ʼ������ֵ
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
	// ������Ӧ����
	// �����ʽ��������(2B) + ���ݳ���(1B) + ���� + ����β(2B \r\n)
	// dataLen ����Ϊ0��pData����Ϊnull
	
	char pbuf[255] = {0};
	pbuf[0] = cmdA;
	pbuf[1] = cmdB;
	pbuf[2] = dataLen;
	pbuf[dataLen+3] = '\r';
	pbuf[dataLen+4] = '\n';
	// ����
	if(pData && dataLen > 0)
	{
		memcpy(&pbuf[3], pData, dataLen);
	}
	usart_printData(BEUSARTNumber1, pbuf, dataLen + 5);
	//c_printloop = 10000;
	//while(c_printloop > 0) --c_printloop;
}

// ��pc�����Ϣ
void printDebug(char *info)
{
	usart_print(BEUSARTNumber1, info);
	//c_printloop = 5000;
	//while(c_printloop > 0) --c_printloop;
	
}