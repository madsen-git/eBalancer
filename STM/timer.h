/*****************************************************************************************
*	启动tim2作为定时器，产生10毫秒的时钟
*
*
*
*
*****************************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__


#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint8_t 	g_timerIT;	
/*****************************************************************************************
	<< --- timer_config		2015-10-31 --- >>
	说明：配置TIM2，产生10ms的时钟
	参数：
	返回值：
*****************************************************************************************/
extern void timer_config(void);
	
extern void timer_start(void);
extern void timer_stop(void);
#ifdef __cplusplus
}
#endif


#endif // __TIMER_H__
// 
