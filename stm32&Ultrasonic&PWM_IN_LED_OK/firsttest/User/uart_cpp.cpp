/**
  ******************************************************************************
  * @file    uart_cpp.cpp
  * @author  wfh
  * @version V1.0
  * @date    2015.10.25
  * @brief   This file is provide the uart function based on the stm firmware v3.5.0 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "uart_cpp.h"  
#define UartTimeCheck 1000*1000;//For UART Time Check 
  
/**
  * @brief Init UARTX  IT ONLY SUPPERT UART1 NOW !!!
  * @param  USART1 or USART2
  * @retval None
  */
void UARTx_Init(USART_TypeDef* USARTx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	if(USARTx ==USART1){
		/* ���ô���1 ��USART1�� ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	/*����GPIO�˿�����*/
  /* ���ô���1 ��USART1 Tx (PA.09)��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* ���ô���1 USART1 Rx (PA.10)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
		/* ����1����ģʽ��USART1 mode������ */
	USART_InitStructure.USART_BaudRate = 115200;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���
	}
	return ;
}

/*For C function tell the compiler use C function to compile these  */
#ifdef __cplusplus
extern "C" {
#endif
	/**
  * @brief UARTx_SendBytes  Send definte data to the uart
  * @param  USARTx:	This parameter can be USART1 or USART2
  *			length:	The number of bytes that will be send to the uart
  *			DataToSend: Data to send
  * @retval ErrorStatus ERROR=0 ,SUCCESS=!ERROR
  */
ErrorStatus UARTx_SendBytes(USART_TypeDef* USARTx, u16 length, u8 *DataToSend)
{
	
	u32 UartTimeChecker = UartTimeCheck;
	for(u16 i = 0; i<length; i++){
	/* ��Printf���ݷ������� */
	UartTimeChecker = UartTimeCheck;
	USART_SendData(USARTx, *DataToSend);
	while( USART_GetFlagStatus(USARTx,USART_FLAG_TC)!= SET){if(!UartTimeChecker--){return ERROR;}}
	DataToSend++;
	}
	return SUCCESS;
}
#ifdef __cplusplus
}
#endif
