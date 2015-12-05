/**********************************************************************************/
#include "stm32f10x.h"
#include "uart_cpp.h"  
#include "LED.h"
#include "delay.h"
#include "stdio.h"
#include "i2c_cpp.h"
#include "Ultrasonic.h"
/**************KS109***************/
#define SONAR_ADDR                  0xE8  /*超声波地址 */
#define SONAR_CONTROL_REG           0x02
#define SONAR_MODE_DISOUT_MAX_5M    0xBC    //0-11m范围，带温度补偿，返回mm   耗时 87ms
#define ULTRASONIC_DETECT_MODE_NO_COMPENSATION  0xBA //11m范围， 不带温度补偿，返回us 耗时 66ms
#define SONAE_POWER_FITER(n)        (0x70 + n)  //n 为降噪等级 一级降噪 电池供电，二级降噪 USB 供电
#define SONAR_DIS_SDA_WAIT          0xC3        //探测时I2C的SCL线不拉低
#define SONAR_LOW_DATA_REG          0x02
#define SONAR_HIGH_DATA_REG         0x03
static uint16_t range;	

typedef enum
{
OK=0,
NOK=1,
WAIT=2,
}Status;

Status I2C_WriteByte(uint8_t SlaveAdd, uint8_t Addr, uint8_t Data);
Status I2C_ReadPage(uint8_t SlaveAdd, uint8_t *pBuffer, uint16_t Addr, uint8_t nByte);


#ifdef __cplusplus
extern "C" {
#endif
/*!
 *  @fn	   	  sonar_init
 *  @brief    
 *  @param    None
 *  @retval   None
 *  @author   Jaly
 *  @date     2015/5/17
 *  @note     init i2c first !  wait for test!  WFH edit @ 2015.12.04
 */
ErrorStatus Ultrasonic_init(void)
{  
  uint8_t errCnt = 0;
  errCnt += I2C_WriteByte(SONAR_ADDR, SONAR_CONTROL_REG, SONAR_DIS_SDA_WAIT);
  //errCnt += I2C_WriteByte(SONAR_ADDR, SONAR_CONTROL_REG, SONAR_MODE_DISOUT_MAX_5M);
  errCnt += I2C_WriteByte(SONAR_ADDR, SONAR_CONTROL_REG, SONAE_POWER_FITER(1));//Should be 1 to 5
         
  if(errCnt != OK){
    return ERROR;
	}
  
  return SUCCESS;
}/* end of sonar_init */
 

/***--------------------------------------------------------------------------*/
/*!
 *  @fn	   	  sonar_test
 *  @brief    
 *  @param    None
 *  @retval   None
 *  @author   Jaly
 *  @date     2015/5/17
 *  @note     wait for test! wfh edited @2015.12.04 
 */
ErrorStatus Ultrasonic_start_detection(void)//Need wait for data ready before read data from ultrasonic
{ 
  uint8_t errCnt = 0;
  //uint8_t dat[2] = {0};
  //int i=0,j=0;
  
  //I2C_DriverInit(50000);      /* init i2c master mode */

  
//  while(1)
//  {    
//    errCnt += I2C_ReadPage(SONAR_ADDR, dat, SONAR_LOW_DATA_REG, 2);
//    range = dat[0] << 8 | dat[1];
    errCnt = I2C_WriteByte(SONAR_ADDR, SONAR_CONTROL_REG, ULTRASONIC_DETECT_MODE_NO_COMPENSATION);
    
     if(errCnt == OK)
     {
 			return SUCCESS;
// 				//UARTx_SendBytes(USART1, 2, (u8 *)&range);
// 			
//       //printf("sonar data = %d\r\n", range);
// 			//printf("sonar data = %d\n", range);
// 			
     }
 		else{
 		return ERROR;
 		}
// 		for(i=0;i<=1000;i++)
// 		{
// 		for(j=0;j<1000;j++)
// 			{;}
// 		}
//  }
}/* end of sonar_test */

uint16_t Ultrasonic_read_data(void)
{
	uint8_t errCnt = 0;
	uint8_t dat[2] = {0};

	errCnt = I2C_ReadPage(SONAR_ADDR, dat, SONAR_LOW_DATA_REG, 2);
  range = dat[0] << 8 | dat[1];

	if(errCnt == OK){
		return range;
	}
	else{
		return (uint16_t)-1;
	}
	
}
#ifdef __cplusplus
}
#endif
	
Status I2C_WriteByte(uint8_t SlaveAdd, uint8_t Addr, uint8_t Data){
	if(I2C_WriteBytes_C(I2C2, &Data, SlaveAdd, Addr, 1)){
		return OK;
	}
	else{
		return NOK;
	}
}

Status I2C_ReadPage(uint8_t SlaveAdd, uint8_t *pBuffer, uint16_t Addr, uint8_t nByte){
	if(I2C_ReadBytes_C(I2C2, pBuffer,SlaveAdd,Addr, nByte)){
	return OK;
	}
	else{
	return NOK;
	}
}

// ErrorStatus Sonar_Init(void){
// 	if(sonar_init()){
// 		return ERROR;
// 	}
// 	else{
// 		return SUCCESS;
// 	}
// }

/***--------------------------------------------------------------------------*/
/*!
 *  @fn	   	  sonar_read
 *  @brief    
 *  @param    None
 *  @retval   None
 *  @author   Jaly
 *  @date     2015/5/17
 *  @note     wait for test!
 */     
uint8_t dat[2] = {0};
Status sonar_read(uint16_t *altitude) 
{
  uint8_t errCnt = 0;

    
  /*! read back data  */
  errCnt += I2C_ReadPage(SONAR_ADDR, dat, SONAR_LOW_DATA_REG, 2);
  
  /*! distance calculate  */
  *altitude = (dat[0] << 8) | dat[1];
  
  /*! start transfer again  */
  errCnt += I2C_WriteByte(SONAR_ADDR, SONAR_CONTROL_REG, SONAR_MODE_DISOUT_MAX_5M);
  
  /*! check protocol communiction */
  if(errCnt == OK)    
  {
    return OK;
  }
  
  return NOK;
}/* end of sonar */


//          ..\..\Lib\STM32F10x_StdPeriph_Driver\inc;





