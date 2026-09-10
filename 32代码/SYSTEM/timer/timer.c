#include "timer.h"
#define ma_length 30

u8 ma_get[ma_length];
u8 flag_ma;
u8 cnt_ma;
u8 ma1,ma2,ma3,ma4;
u32 ma_tr;
u32 ma;


void ADC_TIM3_Init(u32 arr, u16 psc)
{
	GPIO_InitTypeDef     GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM3_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	                    		//使能TIM2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);                       	//使能GPIOB时钟      

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3); //GPIOA7复用为定时器14	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           //GPIOC3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //下拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB3
	
	TIM_TimeBaseStructure.TIM_Period = arr-1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM3_OCInitStructure.TIM_Pulse=50-1;//自动重装载值为49，占空比设置为50%，则这里应赋值25
	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       
	TIM_OC1Init(TIM3, &TIM3_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 

	TIM_Cmd(TIM3, ENABLE);

}

//定时器10通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数
void TIM10_CH1_Cap_Init(u32 arr,u16 psc)        //频率计
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef  TIM10_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  	//TIM10时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIOA8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB8

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM10); //PA0复用位定时器5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr-1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseStructure);
	

	//初始化TIM10输入捕获参数
	TIM10_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM10_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM10_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM10_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM10_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM10, &TIM10_ICInitStructure);
		
	TIM_ITConfig(TIM10,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM10,ENABLE); 	//使能定时器10

 
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
}
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM10CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM10CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
//定时器10中断服务程序	 
void TIM1_UP_TIM10_IRQHandler(void)
{ 		    

 	if((TIM10CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)//溢出
		{	     
			if(TIM10CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM10CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM10CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM10CH1_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM10CH1_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM10, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM10CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM10CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			  TIM10CH1_CAPTURE_VAL=TIM_GetCapture1(TIM10);//获取当前的捕获值.
	 			TIM_OC1PolarityConfig(TIM10,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM10CH1_CAPTURE_STA=0;			//清空
				TIM10CH1_CAPTURE_VAL=0;
				TIM10CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM10,DISABLE ); 	//关闭定时器5
	 			TIM_SetCounter(TIM10,0);
				TIM_Cmd(TIM10,ENABLE ); 	//使能定时器5
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM10, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}

u32 get_fre(void)
{
	   u32 temp;
	   u32 fre;
		 while(!(TIM10CH1_CAPTURE_STA&0X80)){}        //成功捕获到了一次高电平
		{
			temp=TIM10CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //溢出时间总和
			temp+=TIM10CH1_CAPTURE_VAL;		   //得到总的高电平时间
			fre = 4000000/(temp+4);
			TIM10CH1_CAPTURE_STA=0;			     //开启下一次捕获
			return fre;
		}
}

void TIM14_PWM_Init(u32 arr,u32 psc,u32 pwm)
{		 					 
	//此部分需手动修改IO口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM14); //GPIOA7复用为定时器14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr-1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
  TIM_OCInitStructure.TIM_Pulse=pwm-1;//自动重装载值为49，占空比设置为50%，则这里应赋值25
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM14, ENABLE);  //使能TIM14
 
										  
}  


void TIM2_Init(u32 arr, u16 psc)   //TIM2中断
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	                    		//使能TIM2时钟    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);                         //使能GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE12
	
	TIM_TimeBaseStructure.TIM_Period = arr-1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Update);

	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
		static int Cnt = 0;
	 if(TIM_GetITStatus(TIM2, TIM_IT_Update))
	 {
   if(gpio_get_ma == 0)
	 Cnt++;
	 if(flag_ma == 1)
	 {
		 ma_get[cnt_ma++] = gpio_get_ma;
		 if(cnt_ma == 16)
		 {
			 flag_ma = 0;
			 cnt_ma = 0;
			 Cnt = 0;
			 ma1 = (ma_get[0]<<3) +  (ma_get[1]<<2) + (ma_get[2]<<1) + (ma_get[3]);
			 ma2 = (ma_get[4]<<3) +  (ma_get[5]<<2) + (ma_get[6]<<1) + (ma_get[7]);
			 ma3 = (ma_get[8]<<3) +  (ma_get[9]<<2) + (ma_get[10]<<1) + (ma_get[11]);
			 ma4 = (ma_get[12]<<3) + (ma_get[13]<<2) + (ma_get[14]<<1) + (ma_get[15]);
		 }
	 }
	 if(gpio_get_ma == 1 && Cnt > 8)
	 {
		 Cnt = 0;
		 flag_ma = 1;
	 }
		 TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	 }
}

void TIM5_Init(u32 arr, u16 psc)   //TIM2中断
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	                    		//使能TIM2时钟    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);                         //使能GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE12
	
	TIM_TimeBaseStructure.TIM_Period = arr-1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	TIM_SelectOutputTrigger(TIM5,TIM_TRGOSource_Update);

	TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler(void)
{
	 static int Cntt = 0;
	 if(TIM_GetITStatus(TIM5, TIM_IT_Update))
	 {
	 if(ma_tr&0x80000000)
	 {
		  contr_1;
	 }
	 else
	 {
		  contr_0;
	 }
	 	 ma_tr<<=1;
	   Cntt++;
	 if(Cntt == 32)
	 {
		 ma_tr = ma;
	   Cntt = 0;
	 }
  }
		 TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
}
