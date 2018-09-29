/**
  ******************************************************************************
  * @file USART/Interrupt/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and 
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "misc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"

#include "usart.h"
#include "exti.h"
#include "timer.h"
#include "weigher.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//extern uint8_t TxBuffer1[]; 
//extern uint8_t TxBuffer2[]; 
//extern uint8_t RxBuffer1[];
//extern uint8_t RxBuffer2[];
//extern /*__IO*/ uint8_t TxCounter1;
//extern /*__IO*/ uint8_t TxCounter2;
//extern /*__IO*/ uint8_t RxCounter1; 
//extern /*__IO*/ uint8_t RxCounter2;

//extern uint8_t rec_f,tx_flag;

//extern vu8 g_count;
//extern uint8_t g_rxBuf[30];


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval : None
  */
void USART1_IRQHandler(void)      //串口1 中断服务程序
{
	char rx;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	// //判断读寄存器是否非空
	{
		rx = USART_ReceiveData(USART1);   // 将读寄存器的数据缓存到接收缓冲区里
		//sprintf(pbuf, "%d-%d------->%02X\r\n", g_rxCurrentIndex, c_count, rx);
		//usart_print(BEUSARTNumber1, pbuf);
		g_rxBuffer1[g_rxCurrentIndex1++] = rx;
		if(g_rxBuffer1[g_rxCurrentIndex1 - 1] == 0x0A && g_rxBuffer1[g_rxCurrentIndex1 - 2] == 0x0D)
		{
			g_rxOver1 = true;
			g_txDataLen1 = g_rxCurrentIndex1;
			memcpy(g_txBuffer1, g_rxBuffer1, g_txDataLen1);
			g_txBuffer1[g_txDataLen1] = '\0';
		}
		if(g_rxCurrentIndex1 == USART_RX_BUFFER_LEN) g_rxCurrentIndex1 = 0;		// 从头来过
		
	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
	{ 
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
	}
}
/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval : None
  */
void USART2_IRQHandler(void)      //串口2 中断服务程序
{
 	char rx;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	// //判断读寄存器是否非空
	{
		rx = USART_ReceiveData(USART2);   // 将读寄存器的数据缓存到接收缓冲区里
		//sprintf(pbuf, "%d-%d------->%02X\r\n", g_rxCurrentIndex, c_count, rx);
		g_rxBuffer2[g_rxCurrentIndex2++] = rx;
		if(g_rxCurrentIndex2 == g_rxLengthToRead2)
		{
			g_rxOver2 = true;
		}
	}

	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
	{ 
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
	}
  
}

/****************************************************************************
* 名    称：void EXTI1_IRQHandler(void)
* 功    能：EXTI2中断处理程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)				  //判别是否有键按下
	{
		g_motorReady = 1;
		
		//_it0=2;	    										  //按键中断标志
		EXTI_ClearITPendingBit(EXTI_Line1);					  //清除中断请求标志
		//exti_stop();
	}
}


void TIM2_IRQHandler(void)
{
	//static int32_t val = 0;
	//char buf[40];
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		g_timerIT = true;
		//sprintf(buf, "定时器 %d\r\n", val++);
		//usart_print(BEUSARTNumber1, buf);
		if(!g_rxOver2)
		{
			// 清空接受缓冲区
			while(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
			{
				USART_ReceiveData(USART2);
			}
			USART2->SR;
			// 发生测量命令
			wei_sendReadMeasureValueCommand(1);
		}
	}
}
/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/
