#include "key.h"
#include "delay.h" 

//按键初始化函数
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6; //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA,GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
		else if(KEY5==0)return 5;
		else if(KEY6==0)return 6;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1&&KEY6==1)key_up=1; 	    
 	return 0;// 无按键按下
}

//    案例
//    key = KEY_Scan(0);
//		switch(key)
//		{			
//      case 1:{LED1=!LED1;LED2=!LED2;break;}
//			case 2:{LED1=!LED1;LED2=!LED2;break;}
//			case 3:{LED1=!LED1;LED2=!LED2;break;}
//			case 4:{LED1=!LED1;LED2=!LED2;break;}
//			case 5:{LED1=!LED1;LED2=!LED2;break;}
//			case 6:{LED1=!LED1;LED2=!LED2;break;}
//		}


//void KEYBOARD_Init(void)
//{
//	
//	GPIO_InitTypeDef  GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA,GPIOE时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOA,GPIOE时钟
// 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_8; //KEY0 KEY1 KEY2对应引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
//	GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_8);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //KEY0 KEY1 KEY2对应引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO
//	GPIO_ResetBits(GPIOE,GPIO_Pin_0);
// 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6; //KEY0 KEY1 KEY2对应引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
//  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOE2,3,4
//	
// 
//} 
//u8 KEYBOARD[4][4] = {1,2,3,'A',4,5,6,'B',7,8,9,'C','*','0','#','D'};
//u8 keyboard_x,keyboard_y;
//	
//u8 KEYBOARD_Scan(void)
//{
//	for(keyboard_x=1;keyboard_x<=4;keyboard_x++)
//	{
//		     if(keyboard_x==1) {row1_1;row2_0;row3_0;row4_0;}
//	  else if(keyboard_x==2) {row1_0;row2_1;row3_0;row4_0;}
//		else if(keyboard_x==3) {row1_0;row2_0;row3_1;row4_0;}
//		else if(keyboard_x==4) {row1_0;row2_0;row3_0;row4_1;}
//		delay_ms(10);
//         if(col1==1) {keyboard_y=1;return KEYBOARD[keyboard_x-1][keyboard_y-1];break;} 
//		else if(col2==1) {keyboard_y=2;return KEYBOARD[keyboard_x-1][keyboard_y-1];break;}
//		else if(col3==1) {keyboard_y=3;return KEYBOARD[keyboard_x-1][keyboard_y-1];break;}
//		else if(col4==1) {keyboard_y=4;return KEYBOARD[keyboard_x-1][keyboard_y-1];break;}
//	}
//}
