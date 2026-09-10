#ifndef __DIP_H
#define __DIP_H	 
#include "sys.h" 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define DIP1 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) //PE4
#define DIP2 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) //PE3 
#define DIP3 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) //PE2
#define DIP4 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13) //PE4
#define DIP5 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14) //PE3 
#define DIP6 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) //PE3 
#define DIP7 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8) //PE3 
#define DIP8 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14) //PE3 



void DIP_Init(void);	//IO初始化	

#endif
