/**
  ******************************************************************************
  * @file    delay.h
  * @author  wfh
  * @version V1.0
  * @date    2015.10.27
  * @brief   This file provides the delay function and system interrupt manage function
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "delay.h"

#define Tick_division 1000 //1ms per SysTick interrupt

volatile u32 TimingDelay_ms;//Used @ "stm32f10xit.c"
extern volatile u32 Time_ms ;	//Defined @stm32f10x_it.c  [main.cpp wfh edit 15.12.4]
static u32 Time_ms_to_Return = 0;

void Delay_timewasetd(void)//About 125ms
{
	u16 i,j;
		for(i=0;i<=1000;i++)
		{
		for(j=0;j<1000;j++)
			{;}
		}
}
/*****************************************************************************
** 函数名称: Delay_ms
** 功能描述: 用Systick延时

*****************************************************************************/
void Delay_ms(u32 nTime)
{ 
  TimingDelay_ms = nTime;

  while(TimingDelay_ms != 0);
}


void SysTick_Init(void)
{
	
	/* SystemCoreClock / 1000    1ms中断一次
	 * SystemCoreClock / 100000	 10us中断一次
	 * SystemCoreClock / 1000000 1us中断一次
	 */
while(SysTick_Config( SystemCoreClock  / Tick_division));	//Systick 配置延时n*ms	
}

/**
  * @brief Get the Time in ms since system boot
  * @param  None
  * @retval Time in ms since system boot
  * @ATTENTION
  *		There may be bugs when call this function in main funchtion.
  *		Maybe dueto the Time_ms is defined in mian.cpp
  */
u32 stm32_get_ms(void){
	Time_ms_to_Return = Time_ms;
	return Time_ms_to_Return;
}

unsigned long long stm32_get_us(void){
	return Time_ms * 1000 + (SystemCoreClock  / Tick_division - SysTick->VAL);
}


/*****************************************************************************
** 函数名称: NVIC_Configuration
** 功能描述: 系统中断管理
*****************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	       //设置中断组为2 主副优先级各两位
	  
	/* Enable the EXTI0_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = (u8)SysTick_IRQn;    	//中断线0	//(u8)is used to avoid compiler warning
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//响应优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//允许EXTI0_IRQn中断
	NVIC_Init(&NVIC_InitStructure);
	
		//NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
































