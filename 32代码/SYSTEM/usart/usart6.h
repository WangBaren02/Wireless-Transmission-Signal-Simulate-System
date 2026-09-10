#ifndef __USART6_H
#define __USART6_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	
#define EN_USART6_RX 			1		

extern u8  USART_RX_BUF6[USART_REC_LEN]; 
extern u16 USART_RX_STA6;         	

void uart6_init(u32 bound);
#endif


