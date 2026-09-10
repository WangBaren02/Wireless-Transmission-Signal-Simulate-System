#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED端口定义
#define LED1 PCout(0)	// DS0
#define LED2 PCout(1)	// DS1	 
#define LED3 PCout(2)	// DS1	
void LED_Init(void);//初始化		 				    
#endif
