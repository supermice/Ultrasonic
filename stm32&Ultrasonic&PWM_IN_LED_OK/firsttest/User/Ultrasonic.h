#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif
	
ErrorStatus Ultrasonic_init(void);
ErrorStatus Ultrasonic_start_detection(void);//Need wait for data ready before read data from ultrasonic
uint16_t Ultrasonic_read_data(void);//Wait 70ms to 100ms after sending deteding command
	
#ifdef __cplusplus
}
#endif
	
#endif

