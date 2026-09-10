//
// Created by 86180 on 2023-07-25.
//

#include "FPGA_SPI.h"
#include "delay.h"
#define FPGA_SPI_CS(x)      (PBout(12)) = (x)
#define FPGA_SPI_SCL(x)     (PBout(13)) = (x)
#define FPGA_SPI_MOSI(x)    (PBout(15)) = (x)
#define FPGA_SPI_MISO()     (PBin(14))	

#define FPGA_SPI_CS_UP(x)      (PAout(15)) = (x)
#define FPGA_SPI_SCL_UP(x)     (PCout(10)) = (x)
#define FPGA_SPI_MOSI_UP(x)    (PCout(12)) = (x)
#define FPGA_SPI_MISO_UP()     (PCin(11))




void FPGA_SPI_Init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
//////DOWN
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;//NSS,SCK,MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
    GPIO_ResetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15);//拉低


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//MISO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
////////UP
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;//SCK,MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
    GPIO_ResetBits(GPIOC,GPIO_Pin_10 | GPIO_Pin_12);//拉低

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//MISO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//NSS
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
    GPIO_ResetBits(GPIOA,GPIO_Pin_15);//拉低
	
}

void spi_send8(uint8_t data)
{
    uint32_t i;
    for(i = 0; i < 8; ++i)
    {
        FPGA_SPI_MOSI((data &(0x80))) ?1:0;
        data <<= 1;
        FPGA_SPI_SCL(1);
        FPGA_SPI_SCL(0);
    }
}

void spi_send16(uint16_t data)
{
    uint32_t i;
    FPGA_SPI_CS(0);
    delay_us(3);
    for(i = 0; i < 16; ++i)
    {
        FPGA_SPI_SCL(0);
        FPGA_SPI_MOSI((data &(0x8000)) ?1 :0);///变
        data <<= 1;
        delay_us(3);
        FPGA_SPI_SCL(1);/////等待采
        delay_us(3);
    }
    delay_us(3);
    FPGA_SPI_CS(1);
}
void spi_send32_up(uint32_t data1)
{
    uint32_t i;
    FPGA_SPI_CS(0);
    delay_us(3);
    for(i = 0; i < 32; ++i)
    {
        FPGA_SPI_SCL(0);
        FPGA_SPI_MOSI((data1 &(0x80000000)) ?1 :0);///变
        data1 <<= 1;
        delay_us(3);
        FPGA_SPI_SCL(1);/////等待采
        delay_us(3);
    }
    delay_us(3);
    FPGA_SPI_CS(1);
}
void spi_send32_down(uint32_t data)
{
    uint32_t i;
    FPGA_SPI_CS_UP(0);
    delay_us(3);
    for(i = 0; i < 32; ++i)
    {
        FPGA_SPI_SCL_UP(0);
        FPGA_SPI_MOSI_UP((data &(0x80000000)) ?1 :0);///变
        data <<= 1;
        delay_us(3);
        FPGA_SPI_SCL_UP(1);/////等待采
        delay_us(3);
    }
    delay_us(3);
    FPGA_SPI_CS_UP(1);
}
//void SPI_SEND16(uint16_t data)
//{
//		int i;
//    FPGA_SPI_CS2(0);
//    delay_us(3);
//    for(i = 0; i < 16; ++i)
//    {
//        FPGA_SPI_MOSI2((data &(0x8000)) ?1 :0);
//        data <<= 1;
//        FPGA_SPI_SCL2(1);
//        FPGA_SPI_SCL2(0);
//    }
//    delay_us(3);
//    FPGA_SPI_CS2(1);
//}
//void spi_send32(uint32_t data)
//{
//    uint32_t i;
//    FPGA_SPI_CS(0);
//    for(i = 0; i < 32; ++i)
//    {
//        FPGA_SPI_MOSI((data &(0x80000000)) ?1 :0);//PB15
//        data <<= 1;
//        FPGA_SPI_SCL(1);
//        FPGA_SPI_SCL(0);
//    }
//    FPGA_SPI_CS(1);
//}

void spi_rece8(uint32_t *data)
{
    uint32_t i;
    *data = 0;
    FPGA_SPI_CS(0);
    delay_us(1);
    for(i = 0; i < 8; ++i)
    {
        FPGA_SPI_SCL(1);
        delay_us(1);
        *data <<= 1;
        *data = (*data) | (FPGA_SPI_MISO());
        FPGA_SPI_SCL(0);
        delay_us(1);
    }
    FPGA_SPI_CS(1);
}

void spi_rece16(uint32_t *data)
{
    uint32_t i;
    *data = 0;
    FPGA_SPI_CS(0);
    delay_us(1);
    for(i = 0; i < 16; ++i)
    {
        FPGA_SPI_SCL(1);
        delay_us(1);
        *data <<= 1;
        *data = (*data) | (FPGA_SPI_MISO());
        FPGA_SPI_SCL(0);
        delay_us(1);
    }
    if((*data&0x00008000) != 0)
    {
        *data ^= 0xffff;
        *data = -(*data) - 1;
    }
}

void spi_rece32(uint32_t *data)
{
    uint32_t i;
    *data = 0;
    FPGA_SPI_CS(0);
    delay_us(1);
    for(i = 0; i < 32; ++i)
    {
        FPGA_SPI_SCL(1);
        delay_us(1);
        *data <<= 1;
        *data = (*data) | (FPGA_SPI_MISO());
        FPGA_SPI_SCL(0);
        delay_us(1);
    }
}


void FPGA_SPI_Send_Data(uint16_t data)
{
    spi_send16(data);
}

uint32_t FPGA_SPI_Rece_Data(void)
{
    uint32_t receData = 0;
#if   (WIDTH_BYTE_DATA == 1)
    spi_rece8(&receData);
#elif (WIDTH_BYTE_DATA == 2)
    spi_rece16(&receData);
#elif (WIDTH_BYTE_DATA == 4)
    spi_rece32(&receData);
#endif
    return receData;
}

