/**
  ******************************************************************************
  * @file    main.cpp
  * @author  wfh
  * @version V1.0 Alpha
  * @date    2015.10.27
  * @brief   This file is the main file of this project, which used for
  *			 	mpu6050 DMP function and send quaternion through uart1
  ******************************************************************************
  * @attention
  *		ST Firmware V3.5.0
  *		Send DMP quaternion @ UART1 200Hz 115200 Tx (PA.09) Rx (PA.10)
  *		I2C1 Enable PB6-I2C1_SCL、PB7-I2C1_SDA
	*   I2C2 Enable PB10-I2C1_SCL、PB11-I2C1_SDA
  *		Systick interrupt Enable @1000Hz
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "i2c_cpp.h"
#include "delay.h"
#include "uart_cpp.h" 
#include "inv_mpu.h"
#include "PWM_OUT.h"
#include "LED.h"

/*Used for transfer quat_dmp to quaternion*/
typedef  struct _quaternion
{
    float w;
    float x;
    float y;
    float z;
}quaternion;
quaternion dmp_quat;

/*Used for trasfer raw sensor data to physical data and calibration*/
float accel[3],gyro[3];
/*Record ms since boot by Systick*/
//volatile u32 Time_ms = 0;
volatile unsigned long long Time_us = 0;
/*Used for Send UART data @200Hz*/
u32 LastSendData_Time_ms = 0;
u32 ThisSendData_Time_ms = 0;
/*Used for record mpu6050 fifo raw sensor data*/
short gyro_raw[3],accel_raw[3],acc_Tosend[3],gyro_Tosend[3];
/*Used for record DMP raw quaternion data*/
long quat_dmp[4];
/*Used to record the temperature*/
long temperature;float tempe;
/*Get the time in ms, USELESS*/
unsigned long timestamp_dmp;
/*Read how many datas still in mpu6050 FIFO*/
u8 more;
/*Read mask of sensors read from FIFO in mpu6050.*/
short sensors_flag;
/*The bias of accel and gyro To calibrate the accel and gyro data */
int16_t acce_offset[3]={-500,-10,600};//x,y,z buqing's sensor  wfh 15.10.31 
float acce_amplify[3]={1.0129, 1.0, 0.9769};
int16_t gyro_offset[3]={0,0,0};//x,y,z  
/*Used to caculate the CHECK byte of one uart frame*/
u8 UartDataCheck=0;
/*The header and ender of one uart frame*/
u8 UartStartFlag=0xE5;
u8 UartEndFlag = 0xEE;
/*To receive the return value of the dmp_read_fifo function, IT'S NECESSARY!*/
u8 status=0;

//__IO uint16_t IC2Value = 0;//used @stm32f10x_it.c 
__IO uint16_t DutyCycle = 0;//used @stm32f10x_it.c 
__IO uint32_t Frequency = 0;//used @stm32f10x_it.c 

//u8 i=0,j=0,k=0;
//u16 sumi=0,sumj=0,sumk=0,TIM_OverFlowN=0;
//volatile u32 ccr1=0,ccrr=0;
u32 time_ms = 0;


//=============================================================================
//文件名称：main
//功能概要：主函数
//参数说明：无
//函数返回：int
//=============================================================================

int main(int argc,char* argv[])
{ 
	//double range =0.88;

	//Delay_timewasetd();//About 125ms
	SysTick_Init();//This need be the last one to init to avoid UART initialize problem
	NVIC_Configuration();
	Delay_ms(100);// wait for steady voltage level
	
 	I2C mpu6050;
  mpu6050.I2Cx_Init();
	
	I2C Ultralsonic;
	i2c_config_t Ultralsonicconfiger = {
	{
	80*1000,	//I2C_Speed,lower than 400kHz,For Ultrasonic lower than 100kHz
	I2C_Mode_I2C,
	I2C_DutyCycle_2, /*wfh duty1/3 this parameter only make sense when I2C_ClockSpeed is higher than 100KHz*/
	0xE8,	/*Ultralsonic address*/
	I2C_Ack_Enable,
	I2C_AcknowledgedAddress_7bit
	},
	I2C2,/*Ultralsonic*/
	1000//I2C_TimeOut
	};
	
	Ultralsonic.Set_I2C_Configer(Ultralsonicconfiger);
	Ultralsonic.I2Cx_Init();
	
	UARTx_Init(USART1);
	dmp_init();
	Ultrasonic_init();
	

  TIM3_GPIO_Config();//Channel 1: servo_out(PA6)  Channel 2: PWM_OUT(PA7)
  TIM3_PWM_Init();
	
  TIM4_GPIO_Config();//BEEP PB8
  TIM4_PWM_Init();
	
  TIM2_GPIO_Config();//PWM输入捕捉
  TIM2_PWM_Init();
	
	LED_GPIO_Config();

	
	while(1)
	{
		if(stm32_get_ms()%2 == 0){//@500Hz
		status = dmp_read_fifo( gyro_raw, accel_raw, quat_dmp, &timestamp_dmp, &sensors_flag, &more);
		status+= mpu_get_temperature(&temperature,&timestamp_dmp);
		tempe = temperature/65536.f;
		for(int i=0;i<3;i++){
      //accel[i] = (acce_offset[i]+ accel_raw[i]) * (9.79363 / 16384.0);//2g 的加速度计范围
			accel[i] = acc_Tosend[i] * (9.79363 / 16384.0);
			//2^30 * (9.79363 / 16384.0) = 641835.3357
			acc_Tosend[i] = (short)((acce_offset[i]+ accel_raw[i]) * acce_amplify[i]);
		}
		for(int i=0;i<3;i++){
      gyro[i] = (gyro_offset[i] + gyro_raw[i]) * ((3.1415926/180.0) / 16.4);//2000度每秒的范围
			gyro_Tosend[i] = gyro_offset[i] + gyro_raw[i];
		}
		
	}//if(stm32_get_ms())
	
	}//while(1)
	
}
	
	
// 	while(1)
// 	{   	
// // 	  GPIO_ResetBits( GPIOA, GPIO_Pin_4);
// // 		GPIO_ResetBits( GPIOA, GPIO_Pin_3);	
// // 		GPIO_ResetBits( GPIOA, GPIO_Pin_2);
// // 		LED_DISPLAY6(); 
// // 		LED_DISPLAYDOT();
// // 		
// 		
// 		if(range<1.00)
// 		{ 
// 	
// 		
// 			uint16_t x=0;
// 			uint16_t m=0;
// 	    uint16_t n=0;
// // 		  uint16_t o=0;
// // 		  uint16_t p=0;
// //		  uint16_t q=0;
// //		  uint16_t r=0;
// 			
// 			/*个位显示0*/
// 			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
// 			LED_DISPLAY0();
// 			Delay_ms(5);
// 			LED_DISPLAYDOT();
// 			Delay_ms(2);
//       GPIO_SetBits( GPIOA,  GPIO_Pin_4);						
// 			
// 			x=range*100;
// 			m=x/10;		
// 			GPIO_ResetBits( GPIOA, GPIO_Pin_3);
// 			switch (m)
// 			{
// 				case 0:
// 					LED_DISPLAY0(); break;
// 				case 1:
// 					LED_DISPLAY1(); break;
// 				case 2:
// 					LED_DISPLAY2(); break;
// 				case 3:
// 					LED_DISPLAY3(); break;
// 				case 4:
// 					LED_DISPLAY4(); break;
// 				case 5:
// 					LED_DISPLAY5(); break;
// 				case 6:
// 					LED_DISPLAY6(); break;
// 				case 7:
// 					LED_DISPLAY7(); break;
// 				case 8:
// 					LED_DISPLAY8(); break;
// 				case 9:
// 					LED_DISPLAY9(); break;
// 		   }
// 			 		Delay_ms(5);
// 			    GPIO_SetBits( GPIOA,  GPIO_Pin_3);
// 			 
// 			 n=x%10;
// 			 GPIO_ResetBits( GPIOA, GPIO_Pin_2);
// 			 switch (n)
// 			{
// 				case 0:
// 					LED_DISPLAY0(); break;
// 				case 1:
// 					LED_DISPLAY1(); break;
// 				case 2:
// 					LED_DISPLAY2(); break;
// 				case 3:
// 					LED_DISPLAY3(); break;
// 				case 4:
// 					LED_DISPLAY4(); break;
// 				case 5:
// 					LED_DISPLAY5(); break;
// 				case 6:
// 					LED_DISPLAY6(); break;
// 				case 7:
// 					LED_DISPLAY7(); break;
// 				case 8:
// 					LED_DISPLAY8(); break;
// 				case 9:
// 					LED_DISPLAY9(); break;
// 		   }
// 			 		Delay_ms(5);
// 			    GPIO_SetBits( GPIOA,  GPIO_Pin_2);
// 			 
// 		 } 
// 			
// 		else if(range<10.00)
// 		{	
// 			uint16_t x=0;
//       uint16_t m=0;
//       uint16_t n=0;
// 		  uint16_t o=0;
// 		  uint16_t p=0;
// // 		  uint16_t q=0;
// // 		  uint16_t r=0;
// 			
// 			x=range*100;
// 			m=x/100;
// 			
// 			GPIO_ResetBits( GPIOA, GPIO_Pin_4);
// 		  LED_DISPLAYDOT();
//       Delay_ms(2);			
// 			
// 			switch (m)
// 			{
// 				case 0:
// 					LED_DISPLAY0(); break;
// 				case 1:
// 					LED_DISPLAY1(); break;
// 				case 2:
// 					LED_DISPLAY2(); break;
// 				case 3:
// 					LED_DISPLAY3(); break;
// 				case 4:
// 					LED_DISPLAY4(); break;
// 				case 5:
// 					LED_DISPLAY5(); break;
// 				case 6:
// 					LED_DISPLAY6(); break;
// 				case 7:
// 					LED_DISPLAY7(); break;
// 				case 8:
// 					LED_DISPLAY8(); break;
// 				case 9:
// 					LED_DISPLAY9(); break;	
// 			}		
// 					Delay_ms(2);
// 		      GPIO_SetBits( GPIOA,  GPIO_Pin_4);
// 			
// 	    n=x%100;
// 			o=n/10;
// 			
// 			GPIO_ResetBits( GPIOA, GPIO_Pin_3);
// 			switch (o)
// 			{
// 				case 0:
// 				  LED_DISPLAY0(); break;
// 				case 1:
// 					LED_DISPLAY1(); break;
// 				case 2:
// 					LED_DISPLAY2(); break;
// 				case 3:
// 					LED_DISPLAY3(); break;
// 				case 4:
// 					LED_DISPLAY4(); break;
// 				case 5:
// 					LED_DISPLAY5(); break;
// 				case 6:
// 					LED_DISPLAY6(); break;
// 				case 7:
// 					LED_DISPLAY7(); break;
// 				case 8:
// 					LED_DISPLAY8(); break;
// 				case 9:
// 					LED_DISPLAY9(); break;	
// 			}
// 					Delay_ms(2);
// 			    GPIO_SetBits( GPIOA,  GPIO_Pin_3);
// 			
// 		  p=n%10; 
// 			
// 			GPIO_ResetBits( GPIOA, GPIO_Pin_2);
// 			switch (p)
// 			{
// 				case 0:
// 				  LED_DISPLAY0(); break;
// 				case 1:
// 					LED_DISPLAY1(); break;
// 				case 2:
// 					LED_DISPLAY2(); break;
// 				case 3:
// 					LED_DISPLAY3(); break;
// 				case 4:
// 					LED_DISPLAY4(); break;
// 				case 5:
// 					LED_DISPLAY5(); break;
// 				case 6:
// 					LED_DISPLAY6(); break;
// 				case 7:
// 					LED_DISPLAY7(); break;
// 				case 8:
// 					LED_DISPLAY8(); break;
// 				case 9:
// 					LED_DISPLAY9(); break;					
// 		 }
// 		 Delay_ms(5);
// 		 GPIO_SetBits( GPIOA,  GPIO_Pin_2);
// 	 }
// 	else 
// 		 {
// 			uint16_t x=0;
// 			uint16_t m=0;
// 	    uint16_t  n=0;
// 		  uint16_t o=0;
// 		  uint16_t p=0;
// 		  uint16_t q=0;
// 		  //uint16_t r=0;
// 			
// 		  x=range*100;
// 			m=x/1000;
// 			 
// 			GPIO_ResetBits( GPIOA, GPIO_Pin_4);
// 			switch (m)
// 			{
// 				case 0:
// 					LED_DISPLAY0(); break;
// 				case 1:
// 					LED_DISPLAY1(); break;
// 				case 2:
// 					LED_DISPLAY2(); break;
// 				case 3:
// 					LED_DISPLAY3(); break;
// 				case 4:
// 					LED_DISPLAY4(); break;
// 				case 5:
// 					LED_DISPLAY5(); break;
// 				case 6:
// 					LED_DISPLAY6(); break;
// 				case 7:
// 					LED_DISPLAY7(); break;
// 				case 8:
// 					LED_DISPLAY8(); break;
// 				case 9:
// 					LED_DISPLAY9(); break;					
// 		 }
// 		      Delay_ms(5);
// 		      GPIO_SetBits( GPIOA,  GPIO_Pin_4);
// 		 
// 		 
// 		 n=x%1000;
// 		 o=n/100;
// 		 
// 		 GPIO_ResetBits( GPIOA, GPIO_Pin_3);
// 		 LED_DISPLAYDOT();
// 		 Delay_ms(5);
// 		 
// 		 switch (o)
// 			{
// 				case 0:
// 					LED_DISPLAY0(); break;
// 				case 1:
// 					LED_DISPLAY1(); break;
// 				case 2:
// 					LED_DISPLAY2(); break;
// 				case 3:
// 					LED_DISPLAY3(); break;
// 				case 4:
// 					LED_DISPLAY4(); break;
// 				case 5:
// 					LED_DISPLAY5(); break;
// 				case 6:
// 					LED_DISPLAY6(); break;
// 				case 7:
// 					LED_DISPLAY7(); break;
// 				case 8:
// 					LED_DISPLAY8(); break;
// 				case 9:
// 					LED_DISPLAY9(); break;					
// 		 }
// 		      Delay_ms(5);
// 		      GPIO_SetBits( GPIOA,  GPIO_Pin_3);
// 		 
// 		 p=n%100;
// 		 q=p/10;
// 		
// 		 GPIO_ResetBits( GPIOA, GPIO_Pin_2);
// 		 
// 		 switch (q)
// 			{
// 				case 0:
// 					LED_DISPLAY0(); break;
// 				case 1:
// 					LED_DISPLAY1(); break;
// 				case 2:
// 					LED_DISPLAY2(); break;
// 				case 3:
// 					LED_DISPLAY3(); break;
// 				case 4:
// 					LED_DISPLAY4(); break;
// 				case 5:
// 					LED_DISPLAY5(); break;
// 				case 6:
// 					LED_DISPLAY6(); break;
// 				case 7:
// 					LED_DISPLAY7(); break;
// 				case 8:
// 					LED_DISPLAY8(); break;
// 				case 9:
// 					LED_DISPLAY9(); break;					
// 		 }
// 		      Delay_ms(5);
// 		      GPIO_SetBits( GPIOA,  GPIO_Pin_2);
// 		 
//   		}

// 		
//     //GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8))));
// 		//Delay_ms(500);
//   }
// }	//dmp_init();
	

	
// while(1){
// 	
// 	
// // 	status = dmp_read_fifo( gyro_raw, accel_raw, quat_dmp, &timestamp_dmp, &sensors_flag, &more);
// // 	status+= mpu_get_temperature(&temperature,&timestamp_dmp);
// // 	tempe = temperature/65536.f;
// 	for(int i=0;i<3;i++){
//       //accel[i] = (acce_offset[i]+ accel_raw[i]) * (9.79363 / 16384.0);//2g 的加速度计范围
// 			accel[i] = acc_Tosend[i] * (9.79363 / 16384.0);
// 			//2^30 * (9.79363 / 16384.0) = 641835.3357
// 			acc_Tosend[i] = (short)((acce_offset[i]+ accel_raw[i]) * acce_amplify[i]);
// 	}
// 	for(int i=0;i<3;i++){
//       gyro[i] = (gyro_offset[i] + gyro_raw[i]) * ((3.1415926/180.0) / 16.4);//2000度每秒的范围
// 			gyro_Tosend[i] = gyro_offset[i] + gyro_raw[i];
// 	}
// 	
// 	
// 		dmp_quat.w=quat_dmp[0]/1073741824.f;
//     dmp_quat.x=quat_dmp[1]/1073741824.f;
//     dmp_quat.y=quat_dmp[2]/1073741824.f;
//     dmp_quat.z=quat_dmp[3]/1073741824.f;
// 	
// 	UartDataCheck=0;//reset the check
// 	for(u8 i = 0; i<4; i++){
// 		UartDataCheck+=quat_dmp[i];
// 	}
// 	for(int i=0;i<3;i++){
// 		UartDataCheck+=acc_Tosend[i];
// 		UartDataCheck+=gyro_Tosend[i];
// 	}

// 	
//  	ThisSendData_Time_ms = Time_ms;
//  	if((ThisSendData_Time_ms-LastSendData_Time_ms) > 4){
// 		UARTx_SendBytes(USART1, 1, &UartStartFlag);
// 		UARTx_SendBytes(USART1, sizeof(quat_dmp), (u8*)quat_dmp);
// 		
// 		UARTx_SendBytes(USART1, sizeof(acc_Tosend), (u8*)acc_Tosend);
// 		UARTx_SendBytes(USART1, sizeof(gyro_Tosend), (u8*)gyro_Tosend);
// 		UARTx_SendBytes(USART1, 1, &UartDataCheck);
// 		UARTx_SendBytes(USART1, 1, &UartEndFlag);		
// 		LastSendData_Time_ms =ThisSendData_Time_ms;
// 	}//if((ThisSendData_Time_ms-LastSendData_Time_ms) > 4){
// 	

// 	}//while(1)

// //}//main






