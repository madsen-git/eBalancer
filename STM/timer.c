#include "timer.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"


volatile uint8_t 	g_timerIT = false;

void timer_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //����������
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;           	//TIM2�ж�ѡͨ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���жϿ���
	NVIC_Init(&NVIC_InitStructure);
	
	// ʹ��TIM2����
	TIM_DeInit(TIM2);	// ��λʱ��TIM2���ָ�����ʼ״̬
	TIM_TimeBaseStructure.TIM_Period = 3000;					// ��ʱ�����ڣ�����20us��һ�Σ�����50000��=1s
	TIM_TimeBaseStructure.TIM_Prescaler = 1439;					// Ԥ��Ƶ����72M/Prescaler+1=50KHz������20us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// TIM2ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_FLAG_Update, ENABLE);	// 
	//TIM_Cmd(TIM2, ENABLE);	// ʹ��TIM2		TIM2_IRQHandler()
}


void timer_start(void)
{
	TIM_Cmd(TIM2, ENABLE);
}
void timer_stop(void)
{
	TIM_Cmd(TIM2, DISABLE);
}
// 
