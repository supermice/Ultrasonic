/**
  ******************************************************************************
  *
  *     @file    i2c.h
  *     @author  Jaly
  *     @date    2015/1/3 
  *     @brief   extern lib driver support
  * 
  ******************************************************************************
  */ 
#ifndef __I2C_H_
  #define __I2C_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"    
//#include "include.h"

/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public variables -------------------------  ---------------------------------*/

/* Public function prototypes -----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/  
/*  extern lib support driver */

typedef enum
{
OK=0,
NOK=1,
WAIT=2,
}Status;

#ifdef __cplusplus
extern "C" {
#endif
void I2C_DriverInit(uint32_t I2Cx_ClockSpeed);
void I2C_ChangeSpeed(uint32_t I2Cx_ClockSpeed);

Status I2C_WriteByte(uint8_t SlaveAdd, uint8_t Addr, uint8_t Data);
Status I2C_ReadByte(uint8_t ID, uint8_t RegAddr, uint8_t *Data);
Status I2C_WritePage(uint8_t ID, uint8_t* pBuffer, uint16_t RegAddr, uint8_t nByte);
Status I2C_ReadPage(uint8_t ID, uint8_t* pBuffer, uint16_t RegAddr, uint8_t nByte);
#ifdef __cplusplus
}
#endif


#endif /* end undef __I2C_H_ */
