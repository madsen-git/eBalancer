#include "motor.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "exti.h"
volatile uint32_t	g_loopMoto = 0;		// 电机计数


void motor_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	
		// 使能定时器4时钟
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	/* Enable the TIM4 Interrupt */

//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;					// tim4中断
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				   
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure); 
	// GPIOD
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;						// PWM输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;						// 电机使能信号
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*TIM4局部复用功能开启	 在TIM4的局部复用开启时，PD12会被复用为TIM1_CH1*/
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1 , ENABLE);			 

	// TIM3CLK=72MHz  预分频系数Prescaler=2 经过分频 定时器时钟为24MHz
	// 根据公式 通道输出占空比=TIM3_CCR2/(TIM_Period+1),可以得到TIM_Pulse的计数值	 
	// 捕获/比较寄存器2 TIM3_CCR2= CCR2_Val 	     
	TIM_TimeBaseStructure.TIM_Prescaler = 2;						// 预分频器TIM3_PSC=3	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// 计数器向上计数模式 TIM3_CR1[4]=0
	TIM_TimeBaseStructure.TIM_Period = 360;						// 自动重装载寄存器TIM3_APR  确定频率为4KHz 		     
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;					// 时钟分频因子 TIM3_CR1[9:8]=00
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					// 写TIM3各寄存器参数
  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// PWM模式2 TIM3_CCMR1[14:12]=111 在向上计数时，
																	// 一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB5
	TIM_OCInitStructure.TIM_Pulse = 20;							// 确定占空比，这个值决定了有效电平的时间。
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		// 输出极性  低电平有效 TIM3_CCER[5]=1;
         
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);

}


void motor_start(void)
{
	g_motorReady = false;
	g_loopMoto = 0;
	//GPIO_SetBits(GPIOD, GPIO_Pin_2);
	TIM_Cmd(TIM1, ENABLE);
}

void motor_stop(void)
{
	g_motorReady = false;
	g_loopMoto = 0;
	//GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	TIM_Cmd(TIM1, DISABLE);
}
// 
