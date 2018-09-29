#include "motor.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "exti.h"
volatile uint32_t	g_loopMoto = 0;		// �������


void motor_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	
		// ʹ�ܶ�ʱ��4ʱ��
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	/* Enable the TIM4 Interrupt */

//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;					// tim4�ж�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				   
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure); 
	// GPIOD
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;						// PWM���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;						// ���ʹ���ź�
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*TIM4�ֲ����ù��ܿ���	 ��TIM4�ľֲ����ÿ���ʱ��PD12�ᱻ����ΪTIM1_CH1*/
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1 , ENABLE);			 

	// TIM3CLK=72MHz  Ԥ��Ƶϵ��Prescaler=2 ������Ƶ ��ʱ��ʱ��Ϊ24MHz
	// ���ݹ�ʽ ͨ�����ռ�ձ�=TIM3_CCR2/(TIM_Period+1),���Եõ�TIM_Pulse�ļ���ֵ	 
	// ����/�ȽϼĴ���2 TIM3_CCR2= CCR2_Val 	     
	TIM_TimeBaseStructure.TIM_Prescaler = 2;						// Ԥ��Ƶ��TIM3_PSC=3	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// ���������ϼ���ģʽ TIM3_CR1[4]=0
	TIM_TimeBaseStructure.TIM_Period = 360;						// �Զ���װ�ؼĴ���TIM3_APR  ȷ��Ƶ��Ϊ4KHz 		     
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;					// ʱ�ӷ�Ƶ���� TIM3_CR1[9:8]=00
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					// дTIM3���Ĵ�������
  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// PWMģʽ2 TIM3_CCMR1[14:12]=111 �����ϼ���ʱ��
																	// һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// ����/����2�������  OC2�ź��������Ӧ���������PB5
	TIM_OCInitStructure.TIM_Pulse = 20;							// ȷ��ռ�ձȣ����ֵ��������Ч��ƽ��ʱ�䡣
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		// �������  �͵�ƽ��Ч TIM3_CCER[5]=1;
         
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
