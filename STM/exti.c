#include "exti.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

volatile uint8_t g_motorReady = 0;		// 电机是否就位，产生中断就认为就绪

void exti_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	// 
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	// 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*	优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是1， 3 
		0组：  抢占优先级占0位， 响应优先级占4位
		1组：  抢占优先级占1位， 响应优先级占3位
		2组：  抢占优先级占2位， 响应优先级占2位
		3组：  抢占优先级占3位， 响应优先级占1位
		4组：  抢占优先级占4位， 响应优先级占0位  */
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
	/* Enable the EXTI1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				// 外部中断1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// 抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				// 子优先级1  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// 使能
	NVIC_Init(&NVIC_InitStructure);
	
	// 用于配置AFIO外部中断配置寄存器AFIO_EXTICR1，用于选择EXTI2外部中断的输入源是PC2。
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);		// 外部中断配置AFIO--ETXI1
	// 
	
}


void exti_start(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;						//PC2 作为键盘K2 检测状态
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void exti_stop(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;						//PC2 作为键盘K2 检测状态
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}
// 
