#ifndef __PWM_OUT_H
#define	__PWM_OUT_H

#ifdef __cplusplus
extern "C" {
#endif

void TIM3_GPIO_Config(void);
void TIM3_PWM_Init(void);

void TIM4_GPIO_Config(void);
void TIM4_PWM_Init(void);

void TIM2_GPIO_Config(void);
void TIM2_PWM_Init(void);
void TIM2_IRQHandler(void);
	
#ifdef __cplusplus
}
#endif

#endif /* __PWM_OUTPUT_H */
