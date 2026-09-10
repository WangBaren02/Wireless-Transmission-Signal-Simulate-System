#ifndef __RELAY_H
#define __RELAY_H
#include "sys.h"

//RWLAY GPIO设置
//#define RELAY1 PAout(11)	// DS0
//#define RELAY2 PAout(12)	// DS1

void RELAY_Init(void);//继电器初始化
void RELAY_ctrl(u8 mode);	 				    
#endif
