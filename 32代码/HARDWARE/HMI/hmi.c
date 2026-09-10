#include "hmi.h"
#include "delay.h"
#include "oled.h"

void HMISends(char *buf1)		  //字符串发送函数
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(USART1,buf1[i]);  //发送一个字节
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		 	i++;
		}
	 else 
	 return ;

		}
}

void HMISendb(u8 k)		         //字节发送函数
{		 
	u8 i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(USART1,k);  //发送一个字节
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
	 else 
	 return ;

	 } 
} 

void HMISendbit(u8 k)		         //字节发送函数
{		 
	 if(k!=0)
	 	{
			USART_SendData(USART1,k);  //发送一个字节
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
	 else 
	 return ; 
} 

void HMISendstart(void)
	{
	 	delay_ms(200);
		HMISendb(0xff);
		delay_ms(200);
	}


//sprintf((char *)buf,"page0.t3.txt=\"%.1f\"",Ri);


void addt_HMI(u8 num,char *buff)
{
  char buf[200];
	u8 i;
  sprintf((char *)buf,"addt s0.id,0,%d\xff\xff\xff",num);
  //向曲线s0的通道0透传nmu个数据,addt指令不支持跨页面
  HMISends(buf);
 //等待适量时间
 delay_ms(200);

 for(i =0;i<num;i++)
 {
     HMISendbit(buff[i]);
 }
  sprintf((char *)buf,"\x01\xff\xff\xff");
  HMISends(buf);
}

void HMI_mode(u8 mode,float ma,float mf,float F,float dF,float Rc,float h)
{
	switch(mode)//4152637
	{
		case 0:{printf("t4.txt=\"CW\"\xff\xff\xff");
						printf("t4.txt=\"CW\"\xff\xff\xff");
						printf("t1.txt=\"\"\xff\xff\xff");
						printf("t5.txt=\"CW\"\xff\xff\xff");
						printf("t2.txt=\"\"\xff\xff\xff");
						printf("t6.txt=\"CW\"\xff\xff\xff");
						printf("t3.txt=\"\"\xff\xff\xff");
						printf("t7.txt=\"CW\"\xff\xff\xff");
						OLED_ShowNumber(2,2,0,9,12);
						break;}
		
		case 1:{printf("t4.txt=\"AM\"\xff\xff\xff");
						printf("t4.txt=\"AM\"\xff\xff\xff");
						printf("t1.txt=\"F(kHz)\"\xff\xff\xff");
						printf("t5.txt=\"%.1f\"\xff\xff\xff",F);
						printf("t2.txt=\"调制度:\"\xff\xff\xff");
						printf("t6.txt=\"%.2f\"\xff\xff\xff",ma);
						printf("t3.txt=\"\"\xff\xff\xff");
						printf("t7.txt=\"\"\xff\xff\xff");
						OLED_ShowNumber(2,2,1,9,12);
						OLED_Float(4,2,F,6);
						OLED_Float(6,2,ma,6);
						break;}
		
		case 2:{if(abs(Rc-3.2)<0.2)Rc=6;
						else if(abs(Rc-4.6)<0.2)Rc=8;
						else if(abs(Rc-8.6)<0.2)Rc=10;
						printf("t4.txt=\"ASK\"\xff\xff\xff");
						printf("t4.txt=\"ASK\"\xff\xff\xff");
						printf("t1.txt=\"Rc(kbps)\"\xff\xff\xff");
						printf("t5.txt=\"%.1f\"\xff\xff\xff",Rc);
						printf("t2.txt=\"\"\xff\xff\xff");
						printf("t6.txt=\"\"\xff\xff\xff");
						printf("t3.txt=\"\"\xff\xff\xff");
						printf("t7.txt=\"\"\xff\xff\xff");
						OLED_ShowNumber(2,2,2,9,12);
						OLED_Float(4,2,Rc,6);
						break;}
		
		case 3:{if(abs(Rc-6)<=0.5||abs(Rc-8)<=0.5||abs(Rc-10)<=0.5)delay_ms(200);
						printf("t4.txt=\"PSK\"\xff\xff\xff");
						printf("t4.txt=\"PSK\"\xff\xff\xff");
						printf("t1.txt=\"Rc(kbps)\"\xff\xff\xff");
						printf("t5.txt=\"%.1f\"\xff\xff\xff",Rc);
						printf("t2.txt=\"\"\xff\xff\xff");
						printf("t6.txt=\"\"\xff\xff\xff");
						printf("t3.txt=\"\"\xff\xff\xff");
						printf("t7.txt=\"\"\xff\xff\xff");
						OLED_ShowNumber(2,2,3,9,12);
						OLED_Float(4,2,Rc,6);
						break;}
		
		case 4:{printf("t4.txt=\"FSK\"\xff\xff\xff");
						printf("t4.txt=\"FSK\"\xff\xff\xff");
						printf("t1.txt=\"Rc(kbps)\"\xff\xff\xff");
						printf("t5.txt=\"%.1f\"\xff\xff\xff",Rc);
						printf("t2.txt=\"\"\xff\xff\xff");
						printf("t6.txt=\"\"\xff\xff\xff");
						printf("t3.txt=\"h\"\xff\xff\xff");
						printf("t7.txt=\"%.1f\"\xff\xff\xff",h);
						OLED_ShowNumber(2,2,4,9,12);
						OLED_Num2(4,2,Rc);
						OLED_Float(6,2,h,6);
						break;}
		
		case 5:{printf("t4.txt=\"FM\"\xff\xff\xff");
						printf("t4.txt=\"FM\"\xff\xff\xff");
						printf("t1.txt=\"F(kHz)\"\xff\xff\xff");
						printf("t5.txt=\"%.1f\"\xff\xff\xff",F);
						printf("t2.txt=\"调制度:\"\xff\xff\xff");
						printf("t6.txt=\"%.2f\"\xff\xff\xff",mf);
						printf("t3.txt=\"Df(kHz):\"\xff\xff\xff");
						printf("t7.txt=\"%.1f\"\xff\xff\xff",dF);
						OLED_ShowNumber(2,2,5,9,12);
						OLED_Float(4,2,F,6);
						OLED_Float(6,2,mf,6);
						OLED_Float(8,2,dF,6);
						break;}
		
		default:printf("t4.txt=\"Wait\"\xff\xff\xff");
	}
}
