#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define gpio_get_ma GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)
#define contr_1 GPIO_SetBits(GPIOE,GPIO_Pin_8)
#define contr_0 GPIO_ResetBits(GPIOE,GPIO_Pin_8)


void ADC_TIM3_Init(u32 arr, u16 psc);
void TIM10_CH1_Cap_Init(u32 arr,u16 psc);
void TIM14_PWM_Init(u32 arr,u32 psc,u32 pwm);
void TIM2_Init(u32 arr, u16 psc);
void TIM5_Init(u32 arr, u16 psc);
u32 get_fre(void);

#endif

