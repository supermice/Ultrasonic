/**
  ******************************************************************************
  * @file    Analog_I2C.h
  * @author  wfh
  * @version V1.0
  * @date    2015.10.24
  * @brief   This file provides the basic i2c function based on the stmfirmware v3.5.0
  ******************************************************************************
  * @attention
  *		Use GPIO as I2C port
  ******************************************************************************
  */

#ifndef _ANALOG_I2C_H_
#define	_ANALOG_I2C_H_

typedef enum I2Cx{
	IIC1 = 0,
	IIC2 =1
}IICx;

typedef struct _I2C_InitType{
	IICx i2cx;
}I2C_InitType;




#endif

