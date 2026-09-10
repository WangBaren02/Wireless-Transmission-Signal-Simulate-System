#ifndef __HMI_H
#define __HMI_H
#include "sys.h"
#include "usart.h"
#include "stm32f4xx.h"

void HMISends(char *buf1);  //�ַ������ͺ���
void HMISendb(u8 k);
void HMISendbit(u8 k);		         //�ֽڷ��ͺ���
void HMISendstart(void);
void addt_HMI(u8 num,char *buff);
void HMI_mode(u8 mode,float ma,float mf,float F,float dF,float Rc,float h);

#endif

