#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	
#define EN_USART3_RX 			1		

extern u8  USART_RX_BUF3[USART_REC_LEN]; 
extern u16 USART_RX_STA3;         	

void uart3_init(u32 bound);
#endif


