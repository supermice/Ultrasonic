#ifndef __DELAY_H
#define __DELAY_H 	

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C"{
#endif
//extern u32 TimingDelay;
void Delay_timewasetd(void);
void Delay_ms(u32 nTime);
void SysTick_Init(void);
u32 stm32_get_ms(void);
void NVIC_Configuration(void);
#ifdef __cplusplus
}
#endif

#endif //#ifndef __DELAY_H





























