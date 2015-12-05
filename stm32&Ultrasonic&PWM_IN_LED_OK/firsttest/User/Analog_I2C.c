/**
  ******************************************************************************
  * @file    Analog_I2C.c
  * @author  wfh
  * @version V1.0
  * @date    2015.10.24
  * @brief   This file provides the basic i2c function based on the stmfirmware v3.5.0
  ******************************************************************************
  * @attention
  *		Use GPIO as I2C port
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "Analog_I2C.h"
 
/**
  * @brief 
  * @param  i2cx 	
  *     This parameter can be IIC1(= 0) or IIC2(= 1)
  *		IIC1 SCL--GPIO_Pin_6	SDA--GPIO_Pin_7
  *		IIC2 SCL--GPIO_Pin_10	SDA--GPIO_Pin_11
  * @retval SUCCESS(=!ERROR) / ERROR(=0) 
  */
ErrorStatus Anlog_I2C_Init(IICx i2cx){
	/*Enable GPIOB clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	if(i2cx){//IIC2
	/*IIC2 GPIO_Init*/
	GPIO_InitTypeDef GPIO_InitStructure =
	{ 
		GPIO_Pin_10 | GPIO_Pin_11,	/* PB10-I2C2_SCL¡¢PB11-I2C2_SDA*/
		GPIO_Speed_2MHz,
		GPIO_Mode_Out_OD	/*GP Open Drain Output*/
	};
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	}//End of IIC2
	
	
return SUCCESS;
}

