#include "pwm.h"
#include "led.h"
#include "usart.h"


void TIM9_CH2_PWM_Init(u16 arr,u16 psc)
{		
	  //初始化结构体变量
    GPIO_InitTypeDef					GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;
    TIM_OCInitTypeDef					TIM_OCInitStructure;

    //RCC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //IO复用
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);

    //IO初始化
    GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //定时器初始化
    TIM_TimeBaseStructure.TIM_Period 				= arr;
    TIM_TimeBaseStructure.TIM_Prescaler 		= psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
		
    TIM_OCInitStructure.TIM_OCMode 			= TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;
		
    /***********解决普通输出时的互补BUG************/
    TIM_OCInitStructure.TIM_OCNIdleState	= TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_OutputNState	= TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNPolarity 	= TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC1Init(TIM9, &TIM_OCInitStructure);
    TIM_OC2Init(TIM9, &TIM_OCInitStructure);
		
    /************使能预装载寄存器****************/
    TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM9, ENABLE);                                                    //允许在定时器工作时向ARR的缓冲器中写入新值，以便在更新事件发生时载入覆盖以前的值

    TIM_CtrlPWMOutputs(TIM9, ENABLE);
		/************************/
    TIM_Cmd(TIM9, ENABLE);
//		TIM_SetCompare1(TIM9,4000);
//		TIM_SetCompare2(TIM9,4000);
} 
