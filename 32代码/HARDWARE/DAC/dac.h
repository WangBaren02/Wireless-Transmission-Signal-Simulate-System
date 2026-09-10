#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"

#define MIN_GAIN (-2)  			// 可设置的最小增益
#define MAX_GAIN (40)   		// 可设置的最大增益

#define AUTORELOADVALUE  2000				// 设定
#define VREF  3287

void Dac1_Init(void);		//DAC通道1初始化	 	
void Dac2_Init(void);
void Dac1_Set_Vol(u16 vol);	//设置通道1输出电压
void Dac2_Set_Vol(u16 vol);
void DAC_Reset1(int AGAIN);
void DAC_Reset2(int AGAIN);
#endif
