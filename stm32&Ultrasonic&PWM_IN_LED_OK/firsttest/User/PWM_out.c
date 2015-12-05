/**
  ******************************************************************************
  * @file    PWM_out.c
  * @version V1.0
  * @date    2015.11.14
  ******************************************************************************
  * @attention
	*			PB6-I2C1_SCL��PB7-I2C1_SDA
	*			PB10-I2C2_SCL��PB11-I2C2_SDA
  *			PA6- TIM3_channel_1��PA7-TIM3_channel_2  ���50Hz��PWM
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
 * ��������TIM3_GPIO_Config
 * ����  ������TIM3�������PWMʱ�õ���I/O ��·���PWM�����+
 *         �õ���IO�ڣ�	PA6 PA7
 *					PA6 servo  PA7 pwm out
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
	
 void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ�ܶ�ʱ��TIM3ʱ�� */

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* ʹ��GPIOA and GPIOB ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	/*GPIO����Ϊ�����������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
 // GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1; //wfh commit 15.12.04

  //GPIO_Init(GPIOB, &GPIO_InitStructure);	//wfh commit 15.12.04
}
 
/*
 * �������� TIM4_GPIO_Config
 * ����  ������TIM4�������PWMʱ�õ���I/O  ��·���2.4KHz��������
 *         �õ���IO�ڣ�PB8  BEEP
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
 void TIM4_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ�ܶ�ʱ��TIM4ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

	 /* ʹ�� GPIOB ʱ�� */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOB Configuration: TIM4 channel 1 as alternate function push-pull */
	
	/*GPIO����Ϊ�����������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * �������� TIM2_GPIO_Config
 * ����  ������TIM2��������PWMʱ�õ���I/O
 *         �õ���IO�ڣ�PA1 pwm����
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */

void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ�ܶ�ʱ��TIM2ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	 
	/* ʹ�� GPIOA ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* TIM2 channel 2 pin (PA.01) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



/*
 * �������TTIM3_PWM_Init
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ� ֻʹ�� 1 �� 2 ͨ͵�
 *         ���50Hz��PWM��PA6 PA7  
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */

void TIM3_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
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

  /*��ʱ������ */	
  /*����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����*/	
	 //TIM_TimeBaseStructure.TIM_Period =999;//19999; 
	 TIM_TimeBaseStructure.TIM_Period =19999;
	 
  /*����Ԥ��Ƶ:����Ƶ��Ϊ72MHz*/	 
  //TIM_TimeBaseStructure.TIM_Prescaler = 0;
 /*����ʱ�ӷ�Ƶϵ��������Ƶ*/
	 TIM_TimeBaseStructure.TIM_Prescaler = 72;
	//����Ԥ��Ƶ��
 	
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	
  /*���ö�ʱ������ģʽ�����ϼ���ģʽ*/	
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	

  /* PWM1 Mode configuration: Channel1 */
	/*����ΪPWMģʽ1*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/* ��������ֵ�������������������ֵʱ����ƽ�������� */
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  /*����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ*/	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
   /*ʹ��ͨ��1*/
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);



  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM*/
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  
  /*ʹ��ͨ��2*/
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

/*wfh commit @ 15.12.04*/

//   /* PWM1 Mode configuration: Channel3 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	/*����ͨ��3����ֵ�������������������ֵʱ����ƽ�������䣬�������һ��ռ�ձȵ�PWM*/
//   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	
//   /*ʹ��ͨ��3*/
//   TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 

//   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
// 	
// 	

//   /* PWM1 Mode configuration: Channel4 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	
// 	/*����ͨ��4����ֵ�������������������ֵʱ����ƽ�������䣬�������һ��ռ�ձȵ�PWM*/
//   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	
//   /*ʹ��ͨ��4*/
//   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	

//   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
// 	
	
  /*ʹ��TIM3���ؼĴ���ARR*/
  TIM_ARRPreloadConfig(TIM3, ENABLE);			 

  /*ʹ�ܶ�ʱ��3*/
  TIM_Cmd(TIM3, ENABLE);              
}

//**************************************
//**************************************
/*
 * �������TIM4_PWM_Init
 * ����  ������TIM4�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 *         ��·���ԼΪ 2731Hz���������� ռ�ձ�50%
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */

void TIM4_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
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

  /*��ʱ������ */	
  /*����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����*/	
	 //TIM_TimeBaseStructure.TIM_Period =999;//19999; 
	 TIM_TimeBaseStructure.TIM_Period =366;
	 
  /*����Ԥ��Ƶ:����Ƶ��Ϊ72MHz*/	 
  //TIM_TimeBaseStructure.TIM_Prescaler = 0;
 /*����ʱ�ӷ�Ƶϵ��������Ƶ*/
	 TIM_TimeBaseStructure.TIM_Prescaler = 72;
	//����Ԥ��Ƶ��
 	
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	
  /*���ö�ʱ������ģʽ�����ϼ���ģʽ*/	
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	
	
	

//   /* PWM1 Mode configuration: Channel1 */
// 	/*����ΪPWMģʽ1*/
//   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	
// 	/* ��������ֵ�������������������ֵʱ����ƽ�������� */
//   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
// 	
//   /*����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ*/	
//   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
// 	
//    /*ʹ��ͨ��1*/
//   TIM_OC1Init(TIM4, &TIM_OCInitStructure);	

//   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
// 	
// 	

  /* PWM1 Mode configuration: Channel2 */
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/*����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM*/
 // TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  
  /*ʹ��ͨ��2*/
 // TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  

  //TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	

  /* PWM1 Mode configuration: Channel3 */
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/*����ͨ��3����ֵ�������������������ֵʱ����ƽ�������䣬�������һ��ռ�ձȵ�PWM*/
 TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  /*ʹ��ͨ��3*/
 TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 

 TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/*����ͨ��4����ֵ�������������������ֵʱ����ƽ�������䣬�������һ��ռ�ձȵ�PWM*/
  //TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	
  /*ʹ��ͨ��4*/
 // TIM_OC4Init(TIM4, &TIM_OCInitStructure);	

  //TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  /*ʹ��TIM4���ؼĴ���ARR*/
  TIM_ARRPreloadConfig(TIM4, ENABLE);			 

  /*ʹ�ܶ�ʱ��4*/
  TIM_Cmd(TIM4, ENABLE); 
 }	
 
 /*********************************
 *********************************/
 /*
 * �������TIM2_PWM_Init
 * ����  ������TIM2����PWM�źŵ�ģʽ�����������ں�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
 void TIM2_PWM_Init(void)
 {
	 TIM_ICInitTypeDef  TIM_ICInitStructure;
	 
	 /*ѡ��ͨ��2�� ͨ��1Ϊ�ӻ�*/
	 TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	 
	 /*�����ش���*/
	 TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	 
	 /* �ܽ���Ĵ���֮��Ķ�Ӧ��ϵ */
	 TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	 
	 /* ����Ԥ��Ƶ��ָ���ٸ�����������һ�β�������ѡ��4��Ƶ��ÿ4������������һ�β���*/
	 TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	 
	 /* �˲����ã������������������϶������ȶ� ȡ0x0~0xf */
	 TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	
	 /* ���ݲ�������TIM��������Ϣ */
	 TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);
	 
	 /* Select the TIM2 Input Trigger: TI2FP2 */
	 /* IC2Ϊʱ�Ӵ���Դ */
	 TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
	 
	 /* Select the slave Mode: Reset Mode */
	 /* TMI��ģʽ�������źŵ����������³�ʼ���������ʹ����Ĵ����ĸ���ʱ�� */
	 TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
	 
	 /* Enable the Master/Slave Mode */
	 /* ������ʱ���ı������� */
	 TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

	 /* TIM enable counter */
	  /* ����TIM2 */
	 TIM_Cmd(TIM2, ENABLE);
	 
	 /* Enable the CC2 Interrupt Request */
	 /* ���ж� */
	 TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	 TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	 TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	 
 }
 
//  /*
//  * �������TTIM2_IRQHandler
//  * ����  ���жϷ�����
//  * ����  ����
//  * ���  ����
//  * ����  ���ڲ�����
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



