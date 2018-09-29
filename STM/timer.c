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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //优先组设置
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;           	//TIM2中断选通
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断控制
	NVIC_Init(&NVIC_InitStructure);
	
	// 使能TIM2外设
	TIM_DeInit(TIM2);	// 复位时钟TIM2，恢复到初始状态
	TIM_TimeBaseStructure.TIM_Period = 3000;					// 定时器周期，就是20us记一次，计数50000次=1s
	TIM_TimeBaseStructure.TIM_Prescaler = 1439;					// 预分频数，72M/Prescaler+1=50KHz，就是20us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// TIM2时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_FLAG_Update, ENABLE);	// 
	//TIM_Cmd(TIM2, ENABLE);	// 使能TIM2		TIM2_IRQHandler()
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
