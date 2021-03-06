/**
  ******************************************************************************
  * @file    PWM_out.c
  * @version V1.0
  * @date    2015.11.14
  ******************************************************************************
  * @attention
	*			PB6-I2C1_SCL、PB7-I2C1_SDA
	*			PB10-I2C2_SCL、PB11-I2C2_SDA
  *			PA6- TIM3_channel_1、PA7-TIM3_channel_2  输出50Hz的PWM
  *			PB8- TIM4_channel_3  
	*			PA1- TIM2_channel_2
  ******************************************************************************
  */

#include "PWM_OUT.h"
#include "stm32f10x.h"
#include "delay.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * 函数名：TIM3_GPIO_Config
 * 描述  ：配置TIM3复用输出PWM时用到的I/O 此路输出PWM到舵机+
 *         用到的IO口：	PA6 PA7
 *					PA6 servo  PA7 pwm out
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
	
 void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能定时器TIM3时钟 */

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* 使能GPIOA and GPIOB 时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	/*GPIO设置为复用推挽输出*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
 // GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1; //wfh commit 15.12.04

  //GPIO_Init(GPIOB, &GPIO_InitStructure);	//wfh commit 15.12.04
}
 
/*
 * 函数名： TIM4_GPIO_Config
 * 描述  ：配置TIM4复用输出PWM时用到的I/O  此路输出2.4KHz到蜂鸣器
 *         用到的IO口：PB8  BEEP
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
 void TIM4_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能定时器TIM4时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

	 /* 使能 GPIOB 时钟 */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOB Configuration: TIM4 channel 1 as alternate function push-pull */
	
	/*GPIO设置为复用推挽输出*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * 函数名： TIM2_GPIO_Config
 * 描述  ：配置TIM2复用输入PWM时用到的I/O
 *         用到的IO口：PA1 pwm输入
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */

void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能定时器TIM2时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	 
	/* 使能 GPIOA 时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* TIM2 channel 2 pin (PA.01) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



/*
 * 函数名TIM3_PWM_Init
 * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比 只使用 1 和 2 通偷�
 *         输出50Hz的PWM，PA6 PA7  
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */

void TIM3_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	//u16 CCR1_Val = 800;//18000;    
	u16 CCR1_Val = 1500;
	u16 CCR2_Val = 1500;
	//u16 CCR3_Val = 50;
	//u16 CCR4_Val = 50;

/* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 72 KHz.
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 7.5%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
  ----------------------------------------------------------------------- */

  /*定时器配置 */	
  /*当定时器从0计数到999，即为1000次，为一个定时周期*/	
	 //TIM_TimeBaseStructure.TIM_Period =999;//19999; 
	 TIM_TimeBaseStructure.TIM_Period =19999;
	 
  /*设置预分频:不分频则为72MHz*/	 
  //TIM_TimeBaseStructure.TIM_Prescaler = 0;
 /*设置时钟分频系数：不分频*/
	 TIM_TimeBaseStructure.TIM_Prescaler = 72;
	//设置预分频：
 	
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	
  /*设置定时器计数模式：向上计数模式*/	
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	

  /* PWM1 Mode configuration: Channel1 */
	/*配置为PWM模式1*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/* 设置跳变值，当计数器计数到这个值时，电平发生跳变 */
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  /*当定时器计数值小于CCR1_Val时为高电平*/	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
   /*使能通道1*/
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);



  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*设置通道2的电平跳变值，输出另外一个占空比的PWM*/
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  
  /*使能通道2*/
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

/*wfh commit @ 15.12.04*/

//   /* PWM1 Mode configuration: Channel3 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	/*设置通道3跳变值，当计数器计数到这个值时，电平发生跳变，输出另外一个占空比的PWM*/
//   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	
//   /*使能通道3*/
//   TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 

//   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
// 	
// 	

//   /* PWM1 Mode configuration: Channel4 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	
// 	/*设置通道4跳变值，当计数器计数到这个值时，电平发生跳变，输出另外一个占空比的PWM*/
//   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	
//   /*使能通道4*/
//   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	

//   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
// 	
	
  /*使能TIM3重载寄存器ARR*/
  TIM_ARRPreloadConfig(TIM3, ENABLE);			 

  /*使能定时器3*/
  TIM_Cmd(TIM3, ENABLE);              
}

//**************************************
//**************************************
/*
 * 函数名IM4_PWM_Init
 * 描述  ：配置TIM4输出的PWM信号的模式，如周期、极性、占空比
 *         此路输出约为 2731Hz驱动蜂鸣器 占空比50%
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */

void TIM4_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	//u16 CCR1_Val = 800;//18000;    
  //u16 CCR1_Val = 125;
 	//u16 CCR2_Val = 125; 
	u16 CCR3_Val = 183;
	//u16 CCR4_Val = 125;


/* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 72 KHz.
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 50%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 50%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 50%
  ----------------------------------------------------------------------- */

  /*定时器配置 */	
  /*当定时器从0计数到999，即为1000次，为一个定时周期*/	
	 //TIM_TimeBaseStructure.TIM_Period =999;//19999; 
	 TIM_TimeBaseStructure.TIM_Period =366;
	 
  /*设置预分频:不分频则为72MHz*/	 
  //TIM_TimeBaseStructure.TIM_Prescaler = 0;
 /*设置时钟分频系数：不分频*/
	 TIM_TimeBaseStructure.TIM_Prescaler = 72;
	//设置预分频：
 	
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	
  /*设置定时器计数模式：向上计数模式*/	
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	
	
	

//   /* PWM1 Mode configuration: Channel1 */
// 	/*配置为PWM模式1*/
//   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	
// 	/* 设置跳变值，当计数器计数到这个值时，电平发生跳变 */
//   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
// 	
//   /*当定时器计数值小于CCR1_Val时为高电平*/	
//   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
// 	
//    /*使能通道1*/
//   TIM_OC1Init(TIM4, &TIM_OCInitStructure);	

//   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
// 	
// 	

  /* PWM1 Mode configuration: Channel2 */
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/*设置通道2的电平跳变值，输出另外一个占空比的PWM*/
 // TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  
  /*使能通道2*/
 // TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  

  //TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	

  /* PWM1 Mode configuration: Channel3 */
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/*设置通道3跳变值，当计数器计数到这个值时，电平发生跳变，输出另外一个占空比的PWM*/
 TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  /*使能通道3*/
 TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 

 TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/*设置通道4跳变值，当计数器计数到这个值时，电平发生跳变，输出另外一个占空比的PWM*/
  //TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	
  /*使能通道4*/
 // TIM_OC4Init(TIM4, &TIM_OCInitStructure);	

  //TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  /*使能TIM4重载寄存器ARR*/
  TIM_ARRPreloadConfig(TIM4, ENABLE);			 

  /*使能定时器4*/
  TIM_Cmd(TIM4, ENABLE); 
 }	
 
 /*********************************
 *********************************/
 /*
 * 函数名IM2_PWM_Init
 * 描述  ：配置TIM2捕获PWM信号的模式，并计算周期和占空比
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
 void TIM2_PWM_Init(void)
 {
	 TIM_ICInitTypeDef  TIM_ICInitStructure;
	 
	 /*选择通道2， 通道1为从机*/
	 TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	 
	 /*上升沿触发*/
	 TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	 
	 /* 管脚与寄存器之间的对应关系 */
	 TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	 
	 /* 输入预分频，指多少个输入周期做一次捕获，例如选择4分频则每4个输入周期做一次捕获*/
	 TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	 
	 /* 滤波设置，经历几个周期跳变认定波形稳定 取0x0~0xf */
	 TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	
	 /* 根据参数配置TIM的外设信息 */
	 TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);
	 
	 /* Select the TIM2 Input Trigger: TI2FP2 */
	 /* IC2为时钟触发源 */
	 TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
	 
	 /* Select the slave Mode: Reset Mode */
	 /* TMI从模式：触发信号的上升沿重新初始化计数器和触发寄存器的更新时间 */
	 TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
	 
	 /* Enable the Master/Slave Mode */
	 /* 启动定时器的被动触发 */
	 TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

	 /* TIM enable counter */
	  /* 启动TIM2 */
	 TIM_Cmd(TIM2, ENABLE);
	 
	 /* Enable the CC2 Interrupt Request */
	 /* 打开中断 */
	 TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	 TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	 TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	 
 }
 
//  /*
//  * 函数名TIM2_IRQHandler
//  * 描述  ：中断服务函数
//  * 输入  ：无
//  * 输出  ：无
//  * 调用  ：内部调用
//  */
//  void TIM2_IRQHandler(void)
//  {
// 	 __IO uint16_t IC2Value = 0;
// 	 __IO uint16_t DutyCycle = 0;
// 	 __IO uint32_t Frequency = 0;
// 	 
//    /* Clear TIM2 Capture compare interrupt pending bit */
// 	 TIM_ClearITPendingBit(TIM2 , TIM_IT_CC2);
// 	 
// 	 /* Get the Input Capture value */	  
// 	 IC2Value = TIM_GetCapture2(TIM2);
// 	 
// 	 if (IC2Value != 0)
// 	 {
// 		 /* Duty cycle computation */
// 		DutyCycle = (TIM_GetCapture1(TIM2) * 100) / IC2Value;
// 		 
// 	  /* Frequency computation */
// 		Frequency = SystemCoreClock / IC2Value;
// 	}
// 	else
// 	{
// 		DutyCycle = 0;
// 		Frequency = 0;
// 	}

// 	}

 #ifdef __cplusplus
}
#endif



