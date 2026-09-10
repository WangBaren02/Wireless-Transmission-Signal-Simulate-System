#include "dac.h"

//DAC通道1输出初始化
void Dac1_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Enable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC通道1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 800);  //12位右对齐数据格式设置DAC值
}

void Dac2_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Enable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_2,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_2, ENABLE);  //使能DAC通道2
  
  DAC_SetChannel2Data(DAC_Align_12b_R, 800);  //12位右对齐数据格式设置DAC值
}
//设置通道1输出电压	0-4096
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}

void Dac2_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}

void DAC_Reset1(int AGAIN)		//DAC_SetChannelData的值为109-1287
{
	if(AGAIN<14)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((22.5*AGAIN+133)*4095/VREF));
	else if(AGAIN>=15 && AGAIN<=23)	
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((22*AGAIN+133)*4095/VREF));
	else if(AGAIN>=24 && AGAIN<=29)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((21.8*AGAIN+133)*4095/VREF));
	else if(AGAIN>=30&& AGAIN<=34)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((21.5*AGAIN+133)*4095/VREF));
	else if(AGAIN>=35&& AGAIN<=MAX_GAIN)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((21.4*AGAIN+133)*4095/VREF));
}


void DAC_Reset2(int AGAIN)		//DAC_SetChannelData的值为109-1287
{
	if(AGAIN<14)
		DAC_SetChannel2Data(DAC_Align_12b_R, (unsigned long int)((22.5*AGAIN+133)*4095/VREF));
	else if(AGAIN>=15 && AGAIN<=23)	
		DAC_SetChannel2Data(DAC_Align_12b_R, (unsigned long int)((22*AGAIN+133)*4095/VREF));
	else if(AGAIN>=24 && AGAIN<=29)
		DAC_SetChannel2Data(DAC_Align_12b_R, (unsigned long int)((21.8*AGAIN+133)*4095/VREF));
	else if(AGAIN>=30&& AGAIN<=34)
		DAC_SetChannel2Data(DAC_Align_12b_R, (unsigned long int)((21.5*AGAIN+133)*4095/VREF));
	else if(AGAIN>=35&& AGAIN<=MAX_GAIN)
		DAC_SetChannel2Data(DAC_Align_12b_R, (unsigned long int)((21.4*AGAIN+133)*4095/VREF));
}
