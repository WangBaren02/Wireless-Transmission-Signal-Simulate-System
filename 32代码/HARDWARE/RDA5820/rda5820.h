#ifndef __RDA5820_H
#define __RDA5820_H	 
#include "sys.h"
#include "myiic.h"   
 
#define RDA5820_R00		0X00  //读出16个位的ID =0X5820
#define RDA5820_R02		0X02  //寄存器地址的具体意义详见数据手册第13页
 
#define RDA5820_READ    0X23  //读RDA5820
#define RDA5820_WRITE	0X22  //写RDA5820
 
u8 RDA5820_Init(void);					//初始化		 				    
void RDA5820_WR_Reg(u8 addr,u16 val);	//写RDA5820寄存器
u16 RDA5820_RD_Reg(u8 addr);			//读RDA5820寄存器
void RDA5820_RX_Mode(void);				//设置RDA5820为RX模式
void RDA5820_TX_Mode(void);				//设置RDA5820为TX模式
u8 RDA5820_Rssi_Get(void);              //得到信号强度
void RDA5820_Mute_Set(u8 mute);         //静音设置
void RDA5820_Rssi_Set(u8 rssi);         //设置灵敏度
void RDA5820_Vol_Set(u8 vol);           //设置音量
void RDA5820_TxPAG_Set(u8 gain);        //设置TX发送功率
void RDA5820_TxPGA_Set(u8 gain);        //设置TX 输入信号增益
void RDA5820_Band_Set(u8 band);         //设置RDA5820的工作频段
void RDA5820_Space_Set(u8 spc);         //设置RDA5820的步进频率
void RDA5820_Freq_Set(u16 freq);        //设置RDA5820的频率
u16 RDA5820_Freq_Get(void);             //得到当前频率
 
#endif
