
/**
  ******************************************************************************
  * @file    delay.h
  * @author  wfh
  * @version V1.0
  * @date    2015.10.27
  * @brief   This file provides the delay function and system interrupt manage function
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
	
	#include "stm32f10x.h"
	

#ifndef __LED_H
#define	__LED_H

typedef enum _LED_State{
	L_NUM_State = 0,
	M_NUM_State,
	R_NUM_State
}LED_State_t;


#ifdef __cplusplus
extern "C" {
#endif
	
	void LED_GPIO_Config(void);	
	void LED_Angle_Display(const int* AngleToDis);
	void LED_NUM_Display(const float* numToDis);
	
#ifdef __cplusplus
}
#endif

#endif /* __PWM_OUTPUT_H */



