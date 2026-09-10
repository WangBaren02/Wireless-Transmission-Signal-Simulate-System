#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "oled.h"
//#include "dip.h"
//#include "adc.h"
#include "dac.h"
#include "myiic.h"
#include "FPGA_SPI.h"
#include "arm_math.h"
#include "timer.h"
#include "hmi.h"
#include "pwm.h"

extern u8  TIM10CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM10CH1_CAPTURE_VAL;	  //输入捕获值  

//extern u16 ADC1_DMA[1024];//ADC采集数据的缓存
//extern u16 ADC2_DMA[1024];//ADC采集数据的缓存
//extern u16 ADC3_DMA[1024];//ADC采集数据的缓存
//extern u16 ADC_value[1000];

//uint32_t fft_res[4096];
u16 Ac_CTRL[110]={		109,165,258,320,370,410,438,474,521,558,
											109,194,278,354,398,438,466,502,539,586,
											109,222,306,334,370,418,454,494,578,614,
											109,241,325,390,438,474,510,539,595,642,
											109,253,354,410,454,494,533,578,623,670,
											138,278,362,426,474,522,567,595,651,698,
											166,306,390,446,502,539,583,623,670,698,
											166,314,418,474,522,578,606,651,690,754,
											194,334,438,494,539,586,634,670,718,754,
											222,362,446,510,578,606,651,690,735,782,
											222,390,466,530,586,634,670,718,774,810};
uint32_t PWMshuaijian[11]={7500,6810,6120,5430,4740,4050,3360,2670,1980,1290,600};
/************************************************************************************/
void Ac_control1(uint32_t a,uint32_t f)
{
	DAC_SetChannel1Data(DAC_Align_12b_R,Ac_CTRL[(f-30)*10+a/10-1]);
	DAC_SetChannel2Data(DAC_Align_12b_R,Ac_CTRL[(f-30)*10+a/10-1]);
}

void PWMconDC(uint32_t shuai)
{
	TIM_SetCompare2(TIM9,PWMshuaijian[shuai/2]);
	TIM_SetCompare2(TIM9,PWMshuaijian[shuai/2]);
}

/***********************************************************************************/
int main(void)
{
	u16 A_4dB=1,A_step=200,PWM_step=690;								// 默认步进增益4dB
	u16 PWM_MAX=7500,PWM_MIN=600;
	int  gain1=-2,gain2=-2,PWM1=600,PWM2=600;								// 默认初始增益值为0dB
//	u8 temp=0;
	int i;
	
	uint32_t FPGA_data=0,Acdata=0,ttt;
	uint32_t Ac=0,fc=0,alpha=0,Dtime=0,theta=0,ma=0;	//载波幅值,载波频率，衰减系数，时延，相位，调制度
	
	u8 KeyValue=0xff;											// 按键值
	
	delay_init(168);
	LED_Init();
	KEY_Init();
	OLED_Init();
	OLED_Clear();
	TIM5_Init(20000,84);
	TIM2_Init(20000,84);
	TIM9_CH2_PWM_Init(20000,72);
	FPGA_SPI_Init();
	Dac1_Init();
	Dac2_Init();
	uart1_init(9600);
	uart3_init(9600);
	uart6_init(9600);
	HMISendstart();
	DAC_Reset1(-2);
	DAC_Reset2(-2);
	TIM_SetCompare1(TIM9,PWM_MIN);
	TIM_SetCompare2(TIM9,PWM_MIN);
	while(1)
	{
		KeyValue = KEY_Scan(0);        			// 得到键值
		switch(KeyValue)          					// 判断
		{
			case 1:
				if(gain1+A_4dB< MAX_GAIN)				// 判断增益是否超过最大值
				{
					gain1+=A_4dB;									// 加步进4dB
					DAC_Reset1(gain1);							// 输出相应增益的模拟信号
					DAC_Reset2(gain1);
				}
				else
				{
					gain1=MAX_GAIN;								// 设置最大增益
					DAC_Reset1(gain1);							// 输出相应增益的模拟信号
					DAC_Reset2(gain1);
				}
				break; 
			case 2: 
				if(gain1-A_4dB>=MIN_GAIN)				// 判断增益是否超过最小值
				{
					gain1-=A_4dB;									// 减步进4dB
					DAC_Reset1(gain1);							// 输出相应增益的模拟信号
					DAC_Reset2(gain1);
				}
				else
				{
					gain1 = MIN_GAIN;							// 设置最小增益
					DAC_Reset1(gain1);							// 输出相应增益的模拟信号
					DAC_Reset2(gain1);
				}
				break;
			case 3:
				break; 
			case 4:															//疑似损坏
				break;
			case 5:
				if(PWM1+PWM_step<=PWM_MAX)
					PWM1+=PWM_step;
				else PWM1=PWM_MIN;
				TIM_SetCompare1(TIM9,PWM1);
				break;
			case 6:
				if(PWM2+PWM_step<=PWM_MAX)
					PWM2+=PWM_step;
				else PWM2=PWM_MIN;
				TIM_SetCompare2(TIM9,PWM2);
				break;
			default: KeyValue=0xff;	break;		// 重新赋值未按按键的键值
		}
//		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))LED2=0;
//		else LED2=0;
		if(USART_RX_BUF1[6]==0x53)
		{
			LED1=0;
			/***************OLED提取参数*****************/
			fc=USART_RX_BUF1[0];
			theta=USART_RX_BUF1[1];
			alpha=USART_RX_BUF1[2];
			ma=USART_RX_BUF1[3];
			Dtime=USART_RX_BUF1[4];
			Ac=USART_RX_BUF1[5];
			/**************************************************/
//			FPGA_data=(ma*10-30)+Ac/10;							//调制度+载波信息
			FPGA_data=USART_RX_BUF1[3];								//调制度
			FPGA_data=FPGA_data<<8;
			FPGA_data=FPGA_data+USART_RX_BUF1[0];		//载波频率
			FPGA_data=FPGA_data<<8;
			FPGA_data=FPGA_data+USART_RX_BUF1[4];		//时延
			FPGA_data=FPGA_data<<8;
			FPGA_data=FPGA_data+USART_RX_BUF1[1];		//相位
			FPGA_data=FPGA_data & 0x0fffffff;
			spi_send32_down(FPGA_data);
			spi_send32_up(FPGA_data);
			/**************************************************/
//			printf("%d\n",FPGA_data);
			Ac_control1(Ac,fc);
//			PWMconDC(alpha);
//			ttt=((fc-30)*10+Ac/10-1);			
			OLED_ShowNumber(0,0,fc,2,12);					//改变X，Y不变 是同一行
			OLED_ShowNumber(24,0,Ac*10,3,12);
			OLED_ShowNumber(48,0,ma,2,12);
			OLED_ShowNumber(0,10,theta,3,12);
			OLED_ShowNumber(24,10,Dtime,3,12);
			OLED_ShowNumber(48,10,alpha,2,12);
			/**************************************************/
			for(i=0;i<12;i++)
			{
				USART_RX_BUF1[i]=0;
			}
			delay_ms(200);
			LED1=1;
		}
	}
}
