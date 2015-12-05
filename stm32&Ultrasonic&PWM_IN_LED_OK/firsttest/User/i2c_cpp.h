/**
  ******************************************************************************
  * @file    i2c.h
  * @author  wfh
  * @version V1.0
  * @date    2015.10.21
  * @brief   This file is for i2c.cpp 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef _I2C_CPP_H_
#define _I2C_CPP_H_

#include "stm32f10x.h"

#define I2C_TimeOut 1000 //Compatibal with 20*1000 I2C_Speed
//#define I2C_Speed 400*1000 //Lower than 400kHz
#define I2C_Speed 400*1000 //Lower than 400kHz

typedef struct _i2c_config_
{
	//GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure ; 
	I2C_TypeDef* I2Cx ;
	u16	_I2C_TimeOut;
}i2c_config_t;



// static const I2C_InitTypeDef  I2C_InitStructure = {
// 	.I2C_ClockSpeed = 100 * 1000,
// 	.I2C_Mode = I2C_Mode_I2C,
// 	.I2C_DutyCycle =I2C_DutyCycle_2, /*wfh duty1/3 this parameter only make sense when I2C_ClockSpeed is higher than 100KHz*/
// 	.I2C_OwnAddress1 = 0xD0,	/*mpu6050*/
// 	.I2C_Ack = I2C_Ack_Enable,
// 	.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit
// };

static const i2c_config_t static_config = {//init configeration of I2C
	//.GPIO_InitStructure = 
// 	{
// 	GPIO_Pin_6 | GPIO_Pin_7,/* PB6-I2C1_SCL¡¢PB7-I2C1_SDA*/
// 	GPIO_Speed_50MHz,
// 	GPIO_Mode_AF_OD, /*Open Drain output*/
// // 	.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7,/* PB6-I2C1_SCL¡¢PB7-I2C1_SDA*/
// // 	.GPIO_Speed = GPIO_Speed_50MHz
// // 	.GPIO_Mode = GPIO_Mode_AF_OD, /*Open Drain output*/
// 	},
	
	//.I2C_InitStructure =  
	{
// 	.I2C_ClockSpeed = 100 * 1000,
// 	.I2C_Mode = I2C_Mode_I2C,
// 	.I2C_DutyCycle =I2C_DutyCycle_2, /*wfh duty1/3 this parameter only make sense when I2C_ClockSpeed is higher than 100KHz*/
// 	.I2C_OwnAddress1 = 0xD0,	/*mpu6050*/
// 	.I2C_Ack = I2C_Ack_Enable,
// 	.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit
	I2C_Speed,//lower than 400kHz
	I2C_Mode_I2C,
	I2C_DutyCycle_2, /*wfh duty1/3 this parameter only make sense when I2C_ClockSpeed is higher than 100KHz*/
	0xD0,	/*mpu6050*/
	I2C_Ack_Enable,
	I2C_AcknowledgedAddress_7bit
	},
	
	I2C1,/*mpu6050*/
	I2C_TimeOut
};

/*For C function tell the compiler use C function to compile these  */
#ifdef __cplusplus
extern "C" {
#endif
u8 I2C_Write_dmp(u8 SlaveID, u8 Addr,u8 length,u8 *data);//call I2C_WriteBytes_C
u8 I2C_Read_dmp(u8 SlaveID, u8 Addr,u8 length,u8 *data);//call I2C_ReadBytes_C
// u8 I2C_Write_page(u8 SlaveID, u8 Addr,u8 length,u8 *data);
// u8 I2C_Read_page(u8 SlaveID, u8 Addr,u8 length,u8 *data);
ErrorStatus I2C_ReadBytes_C(I2C_TypeDef* I2Cx, u8* ReadToBuffer, u8 SlaveID, u8 ReadAddr, u16 NumByteToRead);
ErrorStatus I2C_WriteBytes_C(I2C_TypeDef* I2Cx, const u8* BufferTowrite, u8 SlaveID, u8 WriteAddr, u8 NumByteToWrite);
#ifdef __cplusplus
}
#endif
/*C function ends*/

/*C++ Section*/
#ifdef __cplusplus

class I2C{
public:
	I2C(i2c_config_t i2c_configer=static_config):_i2c_configer(i2c_configer){
		I2C_TimeOutCounter = _i2c_configer._I2C_TimeOut;
	}
	virtual ~I2C(){}
		
	void Set_I2C_Configer(i2c_config_t i2c_configer);
	i2c_config_t& Get_I2C_Configer(void);
		
	ErrorStatus I2Cx_Init(void);
		
	//ErrorStatus I2C_Write_One_Byte(u8* BufferTowrite, u8 SlaveID, u8 WriteAddr, u8 CheckorNot);
	//ErrorStatus I2C_Read_One_Byte(u8* ReadToBuffer, u8 SlaveID, u8 ReadAddr);
		
	ErrorStatus I2C_WriteBytes(u8* BufferTowrite, u8 SlaveID, u8 WriteAddr, u8 NumByteToWrite);
	ErrorStatus I2C_ReadBytes(u8* ReadToBuffer, u8 SlaveID, u8 ReadAddr, u16 NumByteToRead);
private:
	i2c_config_t _i2c_configer;
    u16 I2C_TimeOutCounter;
};

#endif /*C++ Section*/



#endif
