#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN		12  	
#define EN_USART1_RX 		1		

//extern u8 HMI_getvalue[4];
extern u8  USART_RX_BUF1[USART_REC_LEN]; 
extern u16 USART_RX_STA;         	
extern u16 USART_RX_STA1;         	
extern u8  USART_RX_BUF3[USART_REC_LEN]; 
extern u16 USART_RX_STA3;   
extern u8  USART_RX_BUF6[USART_REC_LEN]; 
extern u16 USART_RX_STA6;   

void uart1_init(u32 bound);
void uart3_init(u32 bound);
void uart6_init(u32 bound);
void UARTsendb(u8 k);
#endif


