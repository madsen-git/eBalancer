#include "exti.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

volatile uint8_t g_motorReady = 0;		// ����Ƿ��λ�������жϾ���Ϊ����

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
	/*	���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������1�� 3 
		0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
		1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
		2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
		3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
		4�飺  ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  */
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
	/* Enable the EXTI1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				// �ⲿ�ж�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// ��ռ���ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				// �����ȼ�1  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	// ��������AFIO�ⲿ�ж����üĴ���AFIO_EXTICR1������ѡ��EXTI2�ⲿ�жϵ�����Դ��PC2��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);		// �ⲿ�ж�����AFIO--ETXI1
	// 
	
}


void exti_start(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;						//PC2 ��Ϊ����K2 ���״̬
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void exti_stop(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;						//PC2 ��Ϊ����K2 ���״̬
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}
// 
