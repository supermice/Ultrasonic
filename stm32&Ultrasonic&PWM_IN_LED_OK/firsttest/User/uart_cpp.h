/**
  ******************************************************************************
  * @file    uart_cpp.h
  * @author  wfh
  * @version V1.0
  * @date    2015.10.25
  * @brief   This file is head of the uart_cpp.cpp .Based on the stm firmware v3.5.0 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef _UART_CPP_H_
#define _UART_CPP_H_

#include "stm32f10x.h"

/*For C function tell the compiler use C function to compile these  */
#ifdef __cplusplus
extern "C" {
#endif
void UARTx_Init(USART_TypeDef* USARTx);	
ErrorStatus UARTx_SendBytes(USART_TypeDef* USARTx,u16 length, u8 *DataToSend);
#ifdef __cplusplus
}
#endif

#endif
