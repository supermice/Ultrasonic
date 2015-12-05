/**
  ******************************************************************************
  *
  *     @file    i2c.c
  *     @author  Jaly
  *     @date    2015/1/3 
  *     @brief    
  * 
  ******************************************************************************
  */ 
#ifndef __I2C_C_
  #define __I2C_C_


/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// #define I2C_SCL               GPIO_Pin_10
// #define I2C_SDA               GPIO_Pin_11
#define I2C_SCL               GPIO_Pin_6
#define I2C_SDA               GPIO_Pin_7
#define I2C_PORT              GPIOB
// #define I2Cx                  I2C2
#define I2Cx                  I2C1

/* Private macro -------------------------------------------------------------*/
#define I2Cx_SLAVE_ADDRESS7   0xd0
    
#define I2C_TIME_MAX          123456
    
/* Private variables ---------------------------------------------------------*/
I2C_InitTypeDef I2C_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/  
#define I2C_TimeCheck()     if(!(TimeCheck--))  \
                            {                   \
                              __enable_irq();   \
                              return NOK;\
                            }
                                
/***--------------------------------------------------------------------------*/
/**
 * @brief  : sensor_driver_init, 
 * @praram : None
 * @retval : None
 * @author : Jaly
 * @time   : 2015/1/2
 **/
														
														
														
void I2C_DriverInit(uint32_t I2Cx_ClockSpeed)
{
	GPIO_InitTypeDef GPIO_InitStructure;  
  
  I2C_DeInit(I2Cx);
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);     /* enable the clock */
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  
	/* Configure IO connected to IIC */
  GPIO_InitStructure.GPIO_Pin =  I2C_SCL | I2C_SDA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;

  
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);
  
	I2C_SoftwareResetCmd(I2Cx, ENABLE);
  
	I2C_DeInit(I2Cx);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2Cx_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2Cx_ClockSpeed;    
	
  I2C_SoftwareResetCmd(I2Cx, DISABLE);
  
  I2C_Cmd(I2Cx, ENABLE);   
  
  I2C_Init(I2Cx, &I2C_InitStructure);
  
  return ; 
}

/***--------------------------------------------------------------------------*/
/**
 * @brief  : I2C_ChangeSpeed, to meet different hardware needs
 * @praram : target iic speed, < 400k 
 * @retval : OK(0) or NOK(1)
 * @author : Jaly
 * @time   : 2015/1/16
 * @tip    : pass 
 **/
void I2C_ChangeSpeed(uint32_t I2Cx_ClockSpeed)
{
//  I2C_InitStructure.I2C_ClockSpeed = I2Cx_ClockSpeed;
//  
//  I2C_Init(I2Cx, &I2C_InitStructure);
//  
//  I2C_SoftwareResetCmd(I2Cx, ENABLE);
//  
//  I2C_Cmd(I2Cx, ENABLE);   
  
  return ;
}

/***--------------------------------------------------------------------------*/
/**
 * @brief  : I2C_WriteByte, write Data to red
 * @praram : hardware ID ,and reg add, and Data 
 * @retval : ok nor not
 * @author : Jaly
 * @time   : 2015/1/2
 **/
Status I2C_WriteByte(uint8_t SlaveAdd, uint8_t Addr, uint8_t Data)
{
  uint32_t TimeCheck = I2C_TIME_MAX;
  
  __disable_irq();
  
  /*!< While the bus is busy */
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
  {I2C_TimeCheck();}  
  
	I2C_GenerateSTART(I2Cx, ENABLE);	//产生起始条件

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))	//等待ACK
  {I2C_TimeCheck();}
  
	I2C_Send7bitAddress(I2Cx, SlaveAdd, I2C_Direction_Transmitter);	//向设备发送设备地址

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	//等待ACK
  {I2C_TimeCheck();}
  
	I2C_SendData(I2Cx, Addr);	//寄存器地址

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))	//等待ACK
  {I2C_TimeCheck();}
  
	I2C_SendData(I2Cx,Data);	//发送数据

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))	//发送完成
  {I2C_TimeCheck();}
  
	I2C_GenerateSTOP(I2Cx, ENABLE);   	//产生结束信号

  __enable_irq();
  
  return OK;
}

/***--------------------------------------------------------------------------*/
/**
 * @brief  : I2C_ReadByte, read the red to Data
 * @praram : hardware ID ,and reg add, and Data 
 * @retval : ok nor not 
 * @author : Jaly
 * @time   : 2015/1/2
 **/
Status I2C_ReadByte(uint8_t SlaveAdd, uint8_t Addr, uint8_t *Data)
{
  uint32_t TimeCheck = I2C_TIME_MAX;
   
  __disable_irq();
  
  /*!< While the bus is busy */
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
  {I2C_TimeCheck();}
  
  /*!< Send START condition */
  I2C_GenerateSTART(I2Cx, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {I2C_TimeCheck();}
    
  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(I2Cx, SlaveAdd, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {I2C_TimeCheck();}
  
  /*!< enable the it */
  I2C_Cmd(I2Cx, ENABLE); 	
  
  /*!< Send the EEPROM's internal address to write to : only oneData Address */
  I2C_SendData(I2Cx, Addr);
 
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {I2C_TimeCheck();}
  
  I2C_GenerateSTART(I2Cx, ENABLE);	//重新发送

  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  //EV5
  {I2C_TimeCheck();}
  
  I2C_Send7bitAddress(I2Cx, SlaveAdd, I2C_Direction_Receiver);  //发送读地址

  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  //EV6  
  {I2C_TimeCheck();}
  
  I2C_AcknowledgeConfig(I2Cx, DISABLE);
  
  I2C_GenerateSTOP(I2Cx, ENABLE);

  while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)))	//关闭应答和停止条件产生
  {I2C_TimeCheck();}
  
  *Data = I2C_ReceiveData(I2Cx);
  
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  
  //I2C_GenerateSTOP(I2Cx, ENABLE);
  __enable_irq();
  
	return OK;
}

/***--------------------------------------------------------------------------*/
/**
 * @brief  : I2C_WritePage, write n bye data store in pBuffer to add from  Addr 
 * @praram : hardware ID and Data buffer, start reg add, buffer size
 * @retval : ok or not
 * @author : Jaly
 * @time   : 2015/1/2
 **/
Status I2C_WritePage(uint8_t SlaveAdd, uint8_t* pBuffer, uint16_t Addr, uint8_t nByte)
{ 
  uint8_t *pTemp = pBuffer;    /* protect Data */
  uint32_t TimeCheck = I2C_TIME_MAX;
    
  __disable_irq();

  /*!< While the bus is busy */
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) 
  {I2C_TimeCheck();}

  /*!< Send START condition */
  I2C_GenerateSTART(I2Cx, ENABLE);
  
  /*!< Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {I2C_TimeCheck();}
  
  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(I2Cx, SlaveAdd, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {I2C_TimeCheck();}
  
  /*!< Send the EEPROM's internal address to write to : only oneData Address */
  I2C_SendData(I2Cx, Addr);
 
  /*!< Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {I2C_TimeCheck();}
  
  while(nByte)
  {    
    I2C_SendData(I2Cx, *pTemp);     /* send Data here */
    pTemp++;
    nByte--;
  
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* Clear EV8  */
    {I2C_TimeCheck();}    
  }

  I2C_GenerateSTOP(I2Cx, ENABLE); /* stop signal  */
  
  /* Perform a read on SR1 and SR2 register to clear eventualaly pending flags */
  (void)I2Cx->SR1;
  (void)I2Cx->SR2;
     
  __enable_irq();
  
  return OK;         /* If all operations OK, return (0) */
}

/***--------------------------------------------------------------------------*/
/**
 * @brief  : I2C_ReadPage,  read nData data from add begin with ReadAddr 
 * @praram : hardware ID and databuffer and source add, also number of reading Data
 * @retval : ok or not
 * @author : Jaly
 * @time   : 2015/1/2
 **/
Status I2C_ReadPage(uint8_t SlaveAdd, uint8_t *pBuffer, uint16_t Addr, uint8_t nByte)
{
  uint32_t TimeCheck = I2C_TIME_MAX;
  uint8_t *pTemp = pBuffer;
  
  __disable_irq();
  
  /* busy waiting */  
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) 
  {I2C_TimeCheck();}  

  I2C_GenerateSTART(I2Cx, ENABLE);   /* Send START condition */
  
  /* Test on EV5 and clear it */    
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {I2C_TimeCheck();}  
  
  /* Send slave address for write */
  I2C_Send7bitAddress(I2Cx, SlaveAdd, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */    
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {I2C_TimeCheck();} 
  
  /* Send the  8位地址 */
  I2C_SendData(I2Cx, Addr); 
  
  /* Test on EV8 and clear it */    
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))   
  {I2C_TimeCheck();}     
 
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2Cx, ENABLE);           //restart，
  
  /* Test on EV5 and clear it */    
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {I2C_TimeCheck();}     
  
  /* Send RTC address for read */
  I2C_Send7bitAddress(I2Cx, SlaveAdd, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */    
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {I2C_TimeCheck();}     
  
  (void)I2Cx->SR2;   	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
  
  I2C_AcknowledgeConfig(I2Cx, ENABLE); 
  
  /* While there is data to be read */
  while(nByte)  
  {
    if(nByte == 1)
    {
      I2C_AcknowledgeConfig(I2Cx, DISABLE);   /* Disable Acknowledgement */      
      I2C_GenerateSTOP(I2Cx, ENABLE);         /* Send STOP Condition */
    }  

    /* Test on EV7 and clear it */    
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {I2C_TimeCheck();}     
      
    *pTemp = I2C_ReceiveData(I2Cx);      /* Read aData from the EEPROM */      
    pTemp++;            /* pint to next data  */
    nByte--;            /* Decrement the readDatas counter */
  }

  /* Enable Acknowledgement to be ready for another reception */
  //I2C_AcknowledgeConfig(I2Cx, ENABLE);
    
  __enable_irq();
  
  return OK;
}

#endif /* end undef __I2C_C_ */


