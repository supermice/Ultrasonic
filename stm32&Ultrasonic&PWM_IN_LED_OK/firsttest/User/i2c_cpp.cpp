/**
  ******************************************************************************
  * @file    i2c_cpp.cpp
  * @author  wfh
  * @version V1.0
  * @date    2015.10.21   
  * @brief   This file provides the basic i2c function based on the stmfirmware v3.5.0
	* @edit		 2015.12.04by wfh.To compitable with mpu6050 and Ultrasonic
  ******************************************************************************
  * @attention
  *			To support both cpp and c file of the HAL, 
  *			this file provide c and cpp function both, even they have similar purpoes
  *			I2C1:PB6-I2C1_SCL、PB7-I2C1_SDA
  *			I2C2:PB10-I2C2_SCL、PB11-I2C2_SDA
  ******************************************************************************
  */
#include "i2c_cpp.h"

#define I2C_TimeOutReset this->I2C_TimeOutCounter=_i2c_configer._I2C_TimeOut;	//To reset I2C_TimeOutCounter
#define I2C_CheckTimeOut if(!((this->I2C_TimeOutCounter--) ? SUCCESS : ERROR)){return ERROR;}	//To avoid being tripped into "while" 
//#define I2C_CheckTimeOut if(!((this->I2C_TimeOutCounter--) ? SUCCESS : ERROR)){while(1){;}}

/* For C*/
//#define I2Cx_C I2C1 //x can be 1 or 2
#define I2C_TimeOutConf_C 1000
static u16 I2C_TimeOut_C = I2C_TimeOutConf_C;//During the while process will decress 1 per time For I2CTimeOurCheck

#define I2C_TimeOutReset_C I2C_TimeOut_C=I2C_TimeOutConf_C;
#define I2C_CheckTimeOut_C if(!(I2C_TimeOut_C-- ? SUCCESS : ERROR)){return ERROR;}
//#define I2C_CheckTimeOut_C __NOP();
/*End of for C */

#ifdef __cplusplus
extern "C" {
#endif
/**
  * @brief  I2C_WriteBytes_C :	C function To put definate datas on the I2C bus
  * @param  NumByteToWrite:	The data that will be send on the I2C bus
  *			SlaveID:	I2C Slave device address
  *			WriteAddr:	Register address in the Slave device
  *			NumByteToWrite:	The numbers of bytes in the NumByteToWrite
  * @retval ErrorStatus ERROR=0 ,SUCCESS=!ERROR
  */
ErrorStatus I2C_WriteBytes_C(I2C_TypeDef* I2Cx,const u8* BufferTowrite, u8 SlaveID, u8 WriteAddr, u8 NumByteToWrite){
		I2C_TypeDef* I2Cx_C;
		if(I2Cx == I2C1){
			I2Cx_C = I2C1;
		}
		else if(I2Cx == I2C2){
			I2Cx_C = I2C2;
		}
		I2C_TimeOutReset_C;	
		while(I2C_GetFlagStatus(I2Cx_C, I2C_FLAG_BUSY)){I2C_CheckTimeOut_C;}
		if(NumByteToWrite == 0 ){return ERROR;}
		/*disable interrupt*/
		//__disable_irq();

		/* Send STRAT condition */
		I2C_GenerateSTART(I2Cx_C, ENABLE);
				
		/* After sending the START condition (I2C_GenerateSTART() function) \
		the master has to wait for this event. It means that the Start condition\
		has been correctly released on the I2C bus (the bus is free, no other devices is communicating). */
		/* Test on EV5 and clear it */
		I2C_TimeOutReset_C;
		while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_MODE_SELECT)){I2C_CheckTimeOut_C;}//wfh need add break after some time
				
		/* Send SlaveID address to write */
		I2C_Send7bitAddress(I2Cx_C, SlaveID, I2C_Direction_Transmitter);
			
		/*We will be tripped @ nest EV6 check if we check EV8 here, so skip EV8 check*/
		/* strange things Test on EV8  */
		//I2C_TimeOutReset_C;
		//while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)){I2C_CheckTimeOut_C;}
		
		/*We will be tripped here with EV8_2 check, so skip EV8_2 check*/
		/*EV8_2 means that the data has been physically shifted out and output on the bus*/	
		/* Test on EV8_2 and clear it */
		//while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){I2C_CheckTimeOut;}	
					
		/*If we've check the EV8 right before than we will be tripped here with ack check*/
		/* Test on EV6 and clear it,check ack */
		I2C_TimeOutReset_C;
		while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut_C;}	
		
		/* Send the Slave's internal address to write to */
		I2C_SendData(I2Cx_C, WriteAddr);	
						
		/* strange things Test on EV8  */
		I2C_TimeOutReset_C;
		while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)){I2C_CheckTimeOut_C;}
			
		/*EV8_2 means that the data has been physically shifted out and output on the bus*/	
		/* Test on EV8_2 and clear it */
		I2C_TimeOutReset_C;
		while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){I2C_CheckTimeOut_C;}	
			
		/*We will be tripped here with ack check, so skip ack check*/
		/* Test on EV6 and clear it. check ack*/
		//while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut;}//wfh need add break after some time
		
		for(;NumByteToWrite >= 1;NumByteToWrite--){
			/* Send the byte to be written */
			I2C_SendData(I2Cx_C, *BufferTowrite); 	
			
			/* strange things Test on EV8  */
			I2C_TimeOutReset_C;
			while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)){I2C_CheckTimeOut_C;}
			
			/* Test on EV8_2 and clear it */
			I2C_TimeOutReset_C;
			while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){I2C_CheckTimeOut_C;}
			
			/*We will be tripped here with ack check, so skip ack check*/
			/* Test on EV6 and clear it. check ack*/
			//while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut;}//wfh need add break after some time
			BufferTowrite++;
		}	
		/* Send STOP condition */
		I2C_GenerateSTOP(I2Cx_C, ENABLE);	
		
	  /* Perform a read on SR1 and SR2 register to clear eventualaly pending flags */
//   (void)I2C1->SR1;
//   (void)I2C1->SR2;
		
	/*Enable interrupt*/	
	//__enable_irq();
	return SUCCESS;
}/*End of I2C_WriteBytes_C*/

/**
  * @brief  I2C_ReadBytes_C :	C function To read datas from the I2C bus
  * @param  ReadToBuffer:	The data that will be read from the I2C bus
  *			SlaveID:	I2C Slave device address
  *			WriteAddr:	Register address in the Slave device
  *			NumByteToWrite:	The numbers of bytes to read from I2C bus
  * @retval ErrorStatus ERROR=0 ,SUCCESS=!ERROR
  */
ErrorStatus I2C_ReadBytes_C(I2C_TypeDef* I2Cx, u8* ReadToBuffer, u8 SlaveID, u8 ReadAddr, u16 NumByteToRead)
{  
	if(NumByteToRead == 0 )
		{return ERROR;}
	I2C_TypeDef* I2Cx_C;
		if(I2Cx == I2C1){
			I2Cx_C = I2C1;
		}
		else if(I2Cx == I2C2){
			I2Cx_C = I2C2;
		}	

	I2C_TimeOutReset_C;
  while(I2C_GetFlagStatus(I2Cx_C, I2C_FLAG_BUSY)){I2C_CheckTimeOut_C;} // Added by Najoua 27/08/2008
		/*Disable interrupt*/	
		//__disable_irq();

  /* Send START condition */
  I2C_GenerateSTART(I2Cx_C, ENABLE);
  
  /*wfh After sending the START condition (I2C_GenerateSTART() function) \
	the master has to wait for this event. It means that the Start condition\
	has been correctly released on the I2C bus (the bus is free, no other devices is communicating). */
  /* Test on EV5 and clear it */
	I2C_TimeOutReset_C;
  while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_MODE_SELECT)){I2C_CheckTimeOut_C;}

  /* Send SlaveID address to read */
  I2C_Send7bitAddress(I2Cx_C, SlaveID, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it ,check ack*/
	I2C_TimeOutReset_C;
  while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut_C;}
  
  /* Clear EV6 by setting again the PE bit */
  //I2C_Cmd(I2C1, ENABLE);//wfh why need this ?

  /* Send the Slave's internal address to read from */
  I2C_SendData(I2Cx_C, ReadAddr);  
			
  /* Test on EV8_2 and clear it */
  I2C_TimeOutReset_C;
  while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){I2C_CheckTimeOut_C;}
	  
  /*We will be tripped here with ack check, so skip ack check*/  
  /* Test on EV6 and clear it ,check ack*/
  //while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut;}//wfh commit
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2Cx_C, ENABLE);
  
  /* Test on EV5 and clear it ,we need to check EV5 after generate a start signal*/
  I2C_TimeOutReset_C;
  while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_MODE_SELECT)){I2C_CheckTimeOut_C;}
  
  /* Send SlaveID address to READ */
  I2C_Send7bitAddress(I2Cx_C, SlaveID, I2C_Direction_Receiver);
  
  
  /* Test on EV6 OF RECEIVER MODE and clear it ,to check ack*/
  I2C_TimeOutReset_C;
  while(!I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){I2C_CheckTimeOut_C;}
  
  //(void)I2Cx_C->SR2;   	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
  I2C_AcknowledgeConfig(I2Cx_C, ENABLE);//stm32 will ack after received data
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2Cx_C, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2Cx_C, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2Cx_C, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *ReadToBuffer = I2C_ReceiveData(I2Cx_C);

      /* Point to the next location where the byte read will be saved */
      ReadToBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2Cx_C, ENABLE);
  
  /*Enable interrupt*/	
	//__enable_irq();
  
  return SUCCESS;
}/*End of I2C_ReadBytes_C*/

/**
  * @brief  I2C_Write_dmp :	C function To put definate datas on the I2C bus
  *			It's the middle level function between Hardware Abstract Level(inv_mpu.c)
  *          and the BSP. Convert the parameters and the return valuel.
  * @param  data:	The data that will be put on the I2C bus
  *			SlaveID:	I2C Slave device address
  *			Addr:	Register address in the Slave device
  *			length:	The numbers of bytes to write on the I2C bus
  * @retval OK-return 0,  BAD-return 1
  */
u8 I2C_Write_dmp(u8 SlaveID, u8 Addr,u8 length, u8 *data){
	if(I2C_WriteBytes_C(I2C1, data, SlaveID, Addr, length)){return 0;}
	else{return 0x01;}
}

/**
  * @brief  I2C_Read_dmp :	C function To read datas from the I2C bus
  *			It's the middle level function between Hardware Abstract Level(inv_mpu.c)
  *          and the BSP. Convert the parameters and the return valuel.
  * @param  data:	The data that will be read from the I2C bus
  *			SlaveID:	I2C Slave device address
  *			Addr:	Register address in the Slave device
  *			length:	The numbers of bytes to read from the I2C bus
  * @retval OK-return 0,  BAD-return 1
  */
u8 I2C_Read_dmp(u8 SlaveID, u8 Addr,u8 length,u8 *data){
	if(I2C_ReadBytes_C(I2C1, data,SlaveID, Addr, length)){return 0;/*SUCCESS*/}
	else{return 0x01;/*ERROR*/}
}
// u8 I2C_Write_page(u8 SlaveID, u8 Addr,u8 length,u8 *data){
// 	if(I2C_WritePage(SlaveID, data, Addr, length))
// 		{return 0x01;/*ERROR*/}
// 	else{return 0;/*SUCCESS*/}
// }
// u8 I2C_Read_page(u8 SlaveID, u8 Addr,u8 length,u8 *data){
// 	if(I2C_ReadPage(SlaveID, data, Addr, length)){return 0x01;/*ERROR*/}
// 	else{return 0;/*SUCCESS*/}
//}




#ifdef __cplusplus
}/*End of extern "C" */
#endif

/**
  * @brief  I2C::Set_I2C_Configer :	Class function To the set data of the I2C driver configer. 
  * @param  i2c_configer:	This type is defined @i2c_cpp.h 
  * 		typedef struct _i2c_config_
  *			{
  *				GPIO_InitTypeDef  GPIO_InitStructure;
  *				I2C_InitTypeDef  I2C_InitStructure ; 
  *				I2C_TypeDef* I2Cx ;
  *				u16	_I2C_TimeOut;
  *			}i2c_config_t;			
  * @retval None
  */
void I2C::Set_I2C_Configer(i2c_config_t i2c_configer){
	this->_i2c_configer = i2c_configer;
}

/**
  * @brief  I2C::Get_I2C_Configer :	Class function To get the I2C driver's configer. 
  * @param  i2c_configer:	This type is defined @i2c_cpp.h 
  * 		typedef struct _i2c_config_
  *			{
  *				GPIO_InitTypeDef  GPIO_InitStructure;
  *				I2C_InitTypeDef  I2C_InitStructure ; 
  *				I2C_TypeDef* I2Cx ;
  *				u16	_I2C_TimeOut;
  *			}i2c_config_t;			
  * @retval None
  */
i2c_config_t& I2C::Get_I2C_Configer(){
	return this->_i2c_configer;
}

/**
  * @brief  I2C::I2Cx_Init :	Class function To Init the I2C driver according to _i2c_configer. 
  * @param  None		
  * @retval ErrorStatus ERROR=0 ,SUCCESS=!ERROR
  */
ErrorStatus I2C::I2Cx_Init(){
	
	/*Disable interrupt*/	
	//__disable_irq();
	if(_i2c_configer.I2Cx == I2C1){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);/* enable GPIOB clock*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);/* enable I2C1 clock*/
	/*
 		According to offical document "CD00190234",because of the limitation of stm32f103c8t6 NEED to do strange things
 	*/
	I2C_Cmd(I2C1,DISABLE);//step 1 this cmd will clear the PE bit in I2Cx_CR1 register		
	GPIO_InitTypeDef GPIO_InitStructure =
	{ 
		GPIO_Pin_6 | GPIO_Pin_7,	/* PB6-I2C1_SCL、PB7-I2C1_SDA*/
		GPIO_Speed_50MHz,
		GPIO_Mode_Out_OD	/*GP Open Drain Output*/
	};
	GPIO_Init(GPIOB, &GPIO_InitStructure);//step 2_1 set sda and scl mode as GPIO_Mode_Out_OD
	GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);//step 2_2 set sda and scl at low 
	//wfh for check pins value
	I2C_TimeOutReset;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) && GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) != Bit_SET ){I2C_CheckTimeOut;}//step 3
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);//step 4 reset sda at low
	I2C_TimeOutReset;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) != Bit_RESET ){I2C_CheckTimeOut;}//step 5 check sda at low
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);//step 6 set scl at low
	I2C_TimeOutReset; 	
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) != Bit_RESET ){I2C_CheckTimeOut;}//step 7 check scl at low	
	GPIO_SetBits(GPIOB,GPIO_Pin_6);	//step 8 set scl high
	I2C_TimeOutReset;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) != Bit_SET ){I2C_CheckTimeOut;}//step 9 check scl at high	
	GPIO_SetBits(GPIOB,GPIO_Pin_7);	//step 10 set sda high
	I2C_TimeOutReset;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) != Bit_SET ){I2C_CheckTimeOut;}//step 11 check sda at high	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; /*GP Open Drain Output*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;/* PB6-I2C1_SCL、PB7-I2C1_SDA*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//step 12 set scl and sda as GPIO_Mode_AF_OD	
	I2C_SoftwareResetCmd(I2C1, ENABLE);//step 13 this cmd will set SWRST bit in I2Cx_CR1 register
	I2C_SoftwareResetCmd(I2C1, DISABLE);//step 14 this cmd 	will reset SWRST bit in I2Cx_CR1 register
	I2C_Cmd(I2C1,ENABLE);	
	/*strange things done*/
	I2C_Init(I2C1, &_i2c_configer.I2C_InitStructure);				
	}
	
	else if(_i2c_configer.I2Cx == I2C2)
		
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);/* enable GPIOB clock*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);/* enable I2C2 clock*/
	/*
 		According to offical document "CD00190234",because of the limitation of stm32f103c8t6 NEED to do strange things
 	*/
	I2C_Cmd(I2C2,DISABLE);//step 1 this cmd will clear the PE bit in I2Cx_CR1 register		
	GPIO_InitTypeDef GPIO_InitStructure =
	{ 
		GPIO_Pin_10 | GPIO_Pin_11,	/* PB10-I2C2_SCL、PB11-I2C2_SDA*/
		GPIO_Speed_50MHz,
		GPIO_Mode_Out_OD	/*GP Open Drain Output*/
	};
	GPIO_Init(GPIOB, &GPIO_InitStructure);//step 2_1 set sda and scl mode as GPIO_Mode_Out_OD
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);//step 2_2 set sda and scl at low 
	//wfh for check pins value
	I2C_TimeOutReset;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) && GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) != Bit_SET ){I2C_CheckTimeOut;}//step 3
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);//step 4 reset sda at low
	I2C_TimeOutReset;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) != Bit_RESET ){I2C_CheckTimeOut;}//step 5 check sda at low
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);//step 6 set scl at low
	I2C_TimeOutReset; 	
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) != Bit_RESET ){I2C_CheckTimeOut;}//step 7 check scl at low	
	GPIO_SetBits(GPIOB,GPIO_Pin_10);	//step 8 set scl high
	I2C_TimeOutReset;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) != Bit_SET ){I2C_CheckTimeOut;}//step 9 check scl at high	
	GPIO_SetBits(GPIOB,GPIO_Pin_11);	//step 10 set sda high
	I2C_TimeOutReset;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) != Bit_SET ){I2C_CheckTimeOut;}//step 11 check sda at high	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; /*GP Open Drain Output*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;/* PB10-I2C2_SCL、PB11-I2C2_SDA*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//step 12 set scl and sda as GPIO_Mode_AF_OD	
	I2C_SoftwareResetCmd(I2C2, ENABLE);//step 13 this cmd will set SWRST bit in I2Cx_CR1 register
	I2C_SoftwareResetCmd(I2C2, DISABLE);//step 14 this cmd 	will reset SWRST bit in I2Cx_CR1 register
	I2C_Cmd(I2C2,ENABLE);	
	/*strange things done*/
	I2C_Init(I2C2, &_i2c_configer.I2C_InitStructure);	 
	}
	/*Enable interrupt*/	
	//__enable_irq();
	return SUCCESS;
}

/**
  * @brief  I2C_WriteBytes :	Class function To put definate datas on the I2C bus
  * @param  NumByteToWrite:	The data that will be send on the I2C bus
  *			SlaveID:	I2C Slave device address
  *			WriteAddr:	Register address in the Slave device
  *			NumByteToWrite:	The numbers of bytes in the NumByteToWrite
  * @retval ErrorStatus ERROR=0 ,SUCCESS=!ERROR
  */
ErrorStatus I2C::I2C_WriteBytes(u8* BufferTowrite, u8 SlaveID, u8 WriteAddr, u8 NumByteToWrite){
// 	if(_i2c_configer.I2Cx == I2C1){
// 		/* Send STRAT condition */
// 		I2C_GenerateSTART(I2C1, ENABLE);
// 		
// 		
// 		/*wfh After sending the START condition (I2C_GenerateSTART() function) \
// 		the master has to wait for this event. It means that the Start condition\
// 		has been correctly released on the I2C bus (the bus is free, no other devices is communicating). */
// 		/* Test on EV5 and clear it */
// 		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){;}//wfh need add break after some time
// 		
// 		/* Send SlaveID address to write */
// 		I2C_Send7bitAddress(I2C1, SlaveID, I2C_Direction_Transmitter);
// 			
// 		
// 		/*After checking on EV5 (start condition correctly released on the bus),\
// 		the master sends the address of the slave(s) with which it will communicate \
// 		(I2C_Send7bitAddress() function, it also determines the direction of the communication: \
// 		Master transmitter or Receiver). Then the master has to wait that a slave acknowledges \
// 		his address. If an acknowledge is sent on the bus, one of the following events will be set:*/
// 		/* Test on EV6 and clear it */
// 		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){;}//wfh need add break after some time
// 			
// 		/* Send the Slave's internal address to write to */
// 		I2C_SendData(I2C1, WriteAddr);	
// 			
// 		/*EV8_2 means that the data has been physically shifted out and output on the bus*/	
// 		/* Test on EV8_2 and clear it */
// 		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	
// 			
// 		/* Test on EV6 and clear it. check ack*/
// 		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){;}//wfh need add break after some time
// 			
// 		/* Send the byte to be written */
// 		I2C_SendData(I2C1, *BufferTowrite); 	
// 		
// 		/* Test on EV8 and clear it */
// 		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
// 			
// 		/* Test on EV6 and clear it. check ack*/
// 		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){;}//wfh need add break after some time
// 			
// 		/* Send STOP condition */
// 		I2C_GenerateSTOP(I2C1, ENABLE);	
// 	}
// 	else if(_i2c_configer.I2Cx == I2C2){
		I2C_TimeOutReset;
		while(I2C_GetFlagStatus(_i2c_configer.I2Cx, I2C_FLAG_BUSY)){I2C_CheckTimeOut;}
		if(NumByteToWrite == 0 ){return ERROR;}
		/*Disable interrupt*/	
		//__disable_irq();
		
		/* Send STRAT condition */
		I2C_GenerateSTART(_i2c_configer.I2Cx, ENABLE);
				
		/* After sending the START condition (I2C_GenerateSTART() function) \
		the master has to wait for this event. It means that the Start condition\
		has been correctly released on the I2C bus (the bus is free, no other devices is communicating). */
		/* Test on EV5 and clear it */
		I2C_TimeOutReset;
		while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){I2C_CheckTimeOut;}//wfh need add break after some time
				
		/* Send SlaveID address to write */
		I2C_Send7bitAddress(_i2c_configer.I2Cx, SlaveID, I2C_Direction_Transmitter);
				
		/* strange things Test on EV8  */
		I2C_TimeOutReset;
		while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)){I2C_CheckTimeOut;}
		
		/*We will be tripped @ nest EV6 check if we check EV8_2 here, so skip EV8 check*/
		/*EV8_2 means that the data has been physically shifted out and output on the bus*/	
		/* Test on EV8_2 and clear it */
		//while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){I2C_CheckTimeOut;}	
					
		/*If we've check the EV8 right before than we will be tripped here with ack check*/
		/* Test on EV6 and clear it,check ack */
		I2C_TimeOutReset;
		while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut;}//wfh need add break after some time
					
		/* Send the Slave's internal address to write to */
		I2C_SendData(_i2c_configer.I2Cx, WriteAddr);	
						
		/* strange things Test on EV8  */
		I2C_TimeOutReset;
		while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)){I2C_CheckTimeOut;}
			
		/*EV8_2 means that the data has been physically shifted out and output on the bus*/	
		/* Test on EV8_2 and clear it */
		I2C_TimeOutReset;
		while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){I2C_CheckTimeOut;}	
			
		/*We will be tripped here with ack check, so skip ack check*/
		/* Test on EV6 and clear it. check ack*/
		//while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut;}//wfh need add break after some time
			
		for(;NumByteToWrite >= 1;NumByteToWrite--){
			/* Send the byte to be written */
			I2C_SendData(_i2c_configer.I2Cx, *BufferTowrite); 	
			
			/* strange things Test on EV8  */
			I2C_TimeOutReset;
			while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)){I2C_CheckTimeOut;}
			
			/* Test on EV8_2 and clear it */
			I2C_TimeOutReset;
			while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){I2C_CheckTimeOut;}
			
			/*We will be tripped here with ack check, so skip ack check*/
			/* Test on EV6 and clear it. check ack*/
			//while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut;}//wfh need add break after some time
			BufferTowrite++;
		}	
		/* Send STOP condition */
		I2C_GenerateSTOP(_i2c_configer.I2Cx, ENABLE);	
	/*Enable interrupt*/	
	//__enable_irq();
	return SUCCESS;
}

/**
  * @brief  I2C_ReadBytes :	Class function To read datas from the I2C bus
  * @param  ReadToBuffer:	The data that will be read from the I2C bus
  *			SlaveID:	I2C Slave device address
  *			WriteAddr:	Register address in the Slave device
  *			NumByteToWrite:	The numbers of bytes to read from I2C bus
  * @retval ErrorStatus ERROR=0 ,SUCCESS=!ERROR
  */
ErrorStatus I2C::I2C_ReadBytes(u8* ReadToBuffer, u8 SlaveID, u8 ReadAddr, u16 NumByteToRead)
{  
	if(NumByteToRead == 0 )
		{return ERROR;}
	/*Disable interrupt*/	
	//__disable_irq();
	I2C_TimeOutReset;
  while(I2C_GetFlagStatus(_i2c_configer.I2Cx, I2C_FLAG_BUSY)){I2C_CheckTimeOut;} // Added by Najoua 27/08/2008
       
  /* Send START condition */
  I2C_GenerateSTART(_i2c_configer.I2Cx, ENABLE);
  
  /*wfh After sending the START condition (I2C_GenerateSTART() function) \
	the master has to wait for this event. It means that the Start condition\
	has been correctly released on the I2C bus (the bus is free, no other devices is communicating). */
  /* Test on EV5 and clear it */
	I2C_TimeOutReset;
	while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){I2C_CheckTimeOut;}


  /* Send SlaveID address to read */
  I2C_Send7bitAddress(_i2c_configer.I2Cx, SlaveID, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it ,check ack*/
	I2C_TimeOutReset;
  while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut;}
  
  /* Clear EV6 by setting again the PE bit */
  //I2C_Cmd(I2C1, ENABLE);//wfh why need this ?

  /* Send the Slave's internal address to read from */
  I2C_SendData(_i2c_configer.I2Cx, ReadAddr);  
			
  /* Test on EV8_2 and clear it */
  I2C_TimeOutReset;
  while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){I2C_CheckTimeOut;}
	  
  /*We will be tripped here with ack check, so skip ack check*/  
  /* Test on EV6 and clear it ,check ack*/
  //while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){I2C_CheckTimeOut;}//wfh commit
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(_i2c_configer.I2Cx, ENABLE);
  
  /* Test on EV5 and clear it ,we need to check EV5 after generate a start signal*/
  I2C_TimeOutReset;
  while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){I2C_CheckTimeOut;}
  
  /* Send SlaveID address to READ */
  I2C_Send7bitAddress(_i2c_configer.I2Cx, SlaveID, I2C_Direction_Receiver);
  
  /* Test on EV6 OF RECEIVER MODE and clear it ,to check ack*/
  I2C_TimeOutReset;
  while(!I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){I2C_CheckTimeOut;}
  
  I2C_AcknowledgeConfig(_i2c_configer.I2Cx, ENABLE);//stm32 will ack after received data
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(_i2c_configer.I2Cx, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(_i2c_configer.I2Cx, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(_i2c_configer.I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *ReadToBuffer = I2C_ReceiveData(_i2c_configer.I2Cx);

      /* Point to the next location where the byte read will be saved */
      ReadToBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(_i2c_configer.I2Cx, ENABLE);
  /*Enable interrupt*/	
	//__enable_irq();
  
  return SUCCESS;
}







