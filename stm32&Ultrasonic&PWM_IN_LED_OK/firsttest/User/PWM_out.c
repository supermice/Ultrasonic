/**
  ******************************************************************************
  * @file    PWM_out.c
  * @version V1.0
  * @date    2015.11.14
  ******************************************************************************
  * @attention
	*			PB6-I2C1_SCL¡¢PB7-I2C1_SDA
	*			PB10-I2C2_SCL¡¢PB11-I2C2_SDA
  *			PA6- TIM3_channel_1¡¢PA7-TIM3_channel_2  Êä³ö50HzµÄPWM
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
 * º¯ÊıÃû£ºTIM3_GPIO_Config
 * ÃèÊö  £ºÅäÖÃTIM3¸´ÓÃÊä³öPWMÊ±ÓÃµ½µÄI/O ´ËÂ·Êä³öPWMµ½¶æ»ú+
 *         ÓÃµ½µÄIO¿Ú£º	PA6 PA7
 *					PA6 servo  PA7 pwm out
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ
 * µ÷ÓÃ  £ºÄÚ²¿µ÷ÓÃ
 */
	
 void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* Ê¹ÄÜ¶¨Ê±Æ÷TIM3Ê±ÖÓ */

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* Ê¹ÄÜGPIOA and GPIOB Ê±ÖÓ */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	/*GPIOÉèÖÃÎª¸´ÓÃÍÆÍìÊä³ö*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
 // GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1; //wfh commit 15.12.04

  //GPIO_Init(GPIOB, &GPIO_InitStructure);	//wfh commit 15.12.04
}
 
/*
 * º¯ÊıÃû£º TIM4_GPIO_Config
 * ÃèÊö  £ºÅäÖÃTIM4¸´ÓÃÊä³öPWMÊ±ÓÃµ½µÄI/O  ´ËÂ·Êä³ö2.4KHzµ½·äÃùÆ÷
 *         ÓÃµ½µÄIO¿Ú£ºPB8  BEEP
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ
 * µ÷ÓÃ  £ºÄÚ²¿µ÷ÓÃ
 */
 void TIM4_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Ê¹ÄÜ¶¨Ê±Æ÷TIM4Ê±ÖÓ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

	 /* Ê¹ÄÜ GPIOB Ê±ÖÓ */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOB Configuration: TIM4 channel 1 as alternate function push-pull */
	
	/*GPIOÉèÖÃÎª¸´ÓÃÍÆÍìÊä³ö*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * º¯ÊıÃû£º TIM2_GPIO_Config
 * ÃèÊö  £ºÅäÖÃTIM2¸´ÓÃÊäÈëPWMÊ±ÓÃµ½µÄI/O
 *         ÓÃµ½µÄIO¿Ú£ºPA1 pwmÊäÈë
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ
 * µ÷ÓÃ  £ºÄÚ²¿µ÷ÓÃ
 */

void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Ê¹ÄÜ¶¨Ê±Æ÷TIM2Ê±ÖÓ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	 
	/* Ê¹ÄÜ GPIOA Ê±ÖÓ */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* TIM2 channel 2 pin (PA.01) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



/*
 * º¯ÊıÃû£TTIM3_PWM_Init
 * ÃèÊö  £ºÅäÖÃTIM3Êä³öµÄPWMĞÅºÅµÄÄ£Ê½£¬ÈçÖÜÆÚ¡¢¼«ĞÔ¡¢Õ¼¿Õ±È Ö»Ê¹ÓÃ 1 ºÍ 2 Í¨ÍµÀ
 *         Êä³ö50HzµÄPWM£¬PA6 PA7  
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ
 * µ÷ÓÃ  £ºÄÚ²¿µ÷ÓÃ
 */

void TIM3_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWMĞÅºÅµçÆ½Ìø±äÖµ */
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

  /*¶¨Ê±Æ÷ÅäÖÃ */	
  /*µ±¶¨Ê±Æ÷´Ó0¼ÆÊıµ½999£¬¼´Îª1000´Î£¬ÎªÒ»¸ö¶¨Ê±ÖÜÆÚ*/	
	 //TIM_TimeBaseStructure.TIM_Period =999;//19999; 
	 TIM_TimeBaseStructure.TIM_Period =19999;
	 
  /*ÉèÖÃÔ¤·ÖÆµ:²»·ÖÆµÔòÎª72MHz*/	 
  //TIM_TimeBaseStructure.TIM_Prescaler = 0;
 /*ÉèÖÃÊ±ÖÓ·ÖÆµÏµÊı£º²»·ÖÆµ*/
	 TIM_TimeBaseStructure.TIM_Prescaler = 72;
	//ÉèÖÃÔ¤·ÖÆµ£º
 	
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	
  /*ÉèÖÃ¶¨Ê±Æ÷¼ÆÊıÄ£Ê½£ºÏòÉÏ¼ÆÊıÄ£Ê½*/	
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	

  /* PWM1 Mode configuration: Channel1 */
	/*ÅäÖÃÎªPWMÄ£Ê½1*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/* ÉèÖÃÌø±äÖµ£¬µ±¼ÆÊıÆ÷¼ÆÊıµ½Õâ¸öÖµÊ±£¬µçÆ½·¢ÉúÌø±ä */
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  /*µ±¶¨Ê±Æ÷¼ÆÊıÖµĞ¡ÓÚCCR1_ValÊ±Îª¸ßµçÆ½*/	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
   /*Ê¹ÄÜÍ¨µÀ1*/
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);



  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*ÉèÖÃÍ¨µÀ2µÄµçÆ½Ìø±äÖµ£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM*/
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  
  /*Ê¹ÄÜÍ¨µÀ2*/
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

/*wfh commit @ 15.12.04*/

//   /* PWM1 Mode configuration: Channel3 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	/*ÉèÖÃÍ¨µÀ3Ìø±äÖµ£¬µ±¼ÆÊıÆ÷¼ÆÊıµ½Õâ¸öÖµÊ±£¬µçÆ½·¢ÉúÌø±ä£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM*/
//   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	
//   /*Ê¹ÄÜÍ¨µÀ3*/
//   TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 

//   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
// 	
// 	

//   /* PWM1 Mode configuration: Channel4 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	
// 	/*ÉèÖÃÍ¨µÀ4Ìø±äÖµ£¬µ±¼ÆÊıÆ÷¼ÆÊıµ½Õâ¸öÖµÊ±£¬µçÆ½·¢ÉúÌø±ä£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM*/
//   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	
//   /*Ê¹ÄÜÍ¨µÀ4*/
//   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	

//   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
// 	
	
  /*Ê¹ÄÜTIM3ÖØÔØ¼Ä´æÆ÷ARR*/
  TIM_ARRPreloadConfig(TIM3, ENABLE);			 

  /*Ê¹ÄÜ¶¨Ê±Æ÷3*/
  TIM_Cmd(TIM3, ENABLE);              
}

//**************************************
//**************************************
/*
 * º¯ÊıÃû£TIM4_PWM_Init
 * ÃèÊö  £ºÅäÖÃTIM4Êä³öµÄPWMĞÅºÅµÄÄ£Ê½£¬ÈçÖÜÆÚ¡¢¼«ĞÔ¡¢Õ¼¿Õ±È
 *         ´ËÂ·Êä³öÔ¼Îª 2731HzÇı¶¯·äÃùÆ÷ Õ¼¿Õ±È50%
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ
 * µ÷ÓÃ  £ºÄÚ²¿µ÷ÓÃ
 */

void TIM4_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWMĞÅºÅµçÆ½Ìø±äÖµ */
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

  /*¶¨Ê±Æ÷ÅäÖÃ */	
  /*µ±¶¨Ê±Æ÷´Ó0¼ÆÊıµ½999£¬¼´Îª1000´Î£¬ÎªÒ»¸ö¶¨Ê±ÖÜÆÚ*/	
	 //TIM_TimeBaseStructure.TIM_Period =999;//19999; 
	 TIM_TimeBaseStructure.TIM_Period =366;
	 
  /*ÉèÖÃÔ¤·ÖÆµ:²»·ÖÆµÔòÎª72MHz*/	 
  //TIM_TimeBaseStructure.TIM_Prescaler = 0;
 /*ÉèÖÃÊ±ÖÓ·ÖÆµÏµÊı£º²»·ÖÆµ*/
	 TIM_TimeBaseStructure.TIM_Prescaler = 72;
	//ÉèÖÃÔ¤·ÖÆµ£º
 	
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	
  /*ÉèÖÃ¶¨Ê±Æ÷¼ÆÊıÄ£Ê½£ºÏòÉÏ¼ÆÊıÄ£Ê½*/	
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	
	
	

//   /* PWM1 Mode configuration: Channel1 */
// 	/*ÅäÖÃÎªPWMÄ£Ê½1*/
//   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	
// 	/* ÉèÖÃÌø±äÖµ£¬µ±¼ÆÊıÆ÷¼ÆÊıµ½Õâ¸öÖµÊ±£¬µçÆ½·¢ÉúÌø±ä */
//   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
// 	
//   /*µ±¶¨Ê±Æ÷¼ÆÊıÖµĞ¡ÓÚCCR1_ValÊ±Îª¸ßµçÆ½*/	
//   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
// 	
//    /*Ê¹ÄÜÍ¨µÀ1*/
//   TIM_OC1Init(TIM4, &TIM_OCInitStructure);	

//   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
// 	
// 	

  /* PWM1 Mode configuration: Channel2 */
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/*ÉèÖÃÍ¨µÀ2µÄµçÆ½Ìø±äÖµ£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM*/
 // TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  
  /*Ê¹ÄÜÍ¨µÀ2*/
 // TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  

  //TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	

  /* PWM1 Mode configuration: Channel3 */
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/*ÉèÖÃÍ¨µÀ3Ìø±äÖµ£¬µ±¼ÆÊıÆ÷¼ÆÊıµ½Õâ¸öÖµÊ±£¬µçÆ½·¢ÉúÌø±ä£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM*/
 TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  /*Ê¹ÄÜÍ¨µÀ3*/
 TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 

 TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	/*ÉèÖÃÍ¨µÀ4Ìø±äÖµ£¬µ±¼ÆÊıÆ÷¼ÆÊıµ½Õâ¸öÖµÊ±£¬µçÆ½·¢ÉúÌø±ä£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM*/
  //TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	
  /*Ê¹ÄÜÍ¨µÀ4*/
 // TIM_OC4Init(TIM4, &TIM_OCInitStructure);	

  //TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  /*Ê¹ÄÜTIM4ÖØÔØ¼Ä´æÆ÷ARR*/
  TIM_ARRPreloadConfig(TIM4, ENABLE);			 

  /*Ê¹ÄÜ¶¨Ê±Æ÷4*/
  TIM_Cmd(TIM4, ENABLE); 
 }	
 
 /*********************************
 *********************************/
 /*
 * º¯ÊıÃû£TIM2_PWM_Init
 * ÃèÊö  £ºÅäÖÃTIM2²¶»ñPWMĞÅºÅµÄÄ£Ê½£¬²¢¼ÆËãÖÜÆÚºÍÕ¼¿Õ±È
 * ÊäÈë  £ºÎŞ
 * Êä³ö  £ºÎŞ
 * µ÷ÓÃ  £ºÄÚ²¿µ÷ÓÃ
 */
 void TIM2_PWM_Init(void)
 {
	 TIM_ICInitTypeDef  TIM_ICInitStructure;
	 
	 /*Ñ¡ÔñÍ¨µÀ2£¬ Í¨µÀ1Îª´Ó»ú*/
	 TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	 
	 /*ÉÏÉıÑØ´¥·¢*/
	 TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	 
	 /* ¹Ü½ÅÓë¼Ä´æÆ÷Ö®¼äµÄ¶ÔÓ¦¹ØÏµ */
	 TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	 
	 /* ÊäÈëÔ¤·ÖÆµ£¬Ö¸¶àÉÙ¸öÊäÈëÖÜÆÚ×öÒ»´Î²¶»ñ£¬ÀıÈçÑ¡Ôñ4·ÖÆµÔòÃ¿4¸öÊäÈëÖÜÆÚ×öÒ»´Î²¶»ñ*/
	 TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	 
	 /* ÂË²¨ÉèÖÃ£¬¾­Àú¼¸¸öÖÜÆÚÌø±äÈÏ¶¨²¨ĞÎÎÈ¶¨ È¡0x0~0xf */
	 TIM_ICInitStructure.TIM_ICFilter = 0x0f;
	
	 /* ¸ù¾İ²ÎÊıÅäÖÃTIMµÄÍâÉèĞÅÏ¢ */
	 TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);
	 
	 /* Select the TIM2 Input Trigger: TI2FP2 */
	 /* IC2ÎªÊ±ÖÓ´¥·¢Ô´ */
	 TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
	 
	 /* Select the slave Mode: Reset Mode */
	 /* TMI´ÓÄ£Ê½£º´¥·¢ĞÅºÅµÄÉÏÉıÑØÖØĞÂ³õÊ¼»¯¼ÆÊıÆ÷ºÍ´¥·¢¼Ä´æÆ÷µÄ¸üĞÂÊ±¼ä */
	 TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
	 
	 /* Enable the Master/Slave Mode */
	 /* Æô¶¯¶¨Ê±Æ÷µÄ±»¶¯´¥·¢ */
	 TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

	 /* TIM enable counter */
	  /* Æô¶¯TIM2 */
	 TIM_Cmd(TIM2, ENABLE);
	 
	 /* Enable the CC2 Interrupt Request */
	 /* ´ò¿ªÖĞ¶Ï */
	 TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	 TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	 TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	 
 }
 
//  /*
//  * º¯ÊıÃû£TTIM2_IRQHandler
//  * ÃèÊö  £ºÖĞ¶Ï·şÎñº¯Êı
//  * ÊäÈë  £ºÎŞ
//  * Êä³ö  £ºÎŞ
//  * µ÷ÓÃ  £ºÄÚ²¿µ÷ÓÃ
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



