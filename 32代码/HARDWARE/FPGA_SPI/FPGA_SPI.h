//
// Created by 86180 on 2023-07-25.
//

#ifndef INC_2023_FPGA_SPI_H
#define INC_2023_FPGA_SPI_H

#include "sys.h"
#define WIDTH_BYTE_DATA 2
#define FPGA_SPI_CS2(x)      (PAout(15)) = (x)
#define FPGA_SPI_SCL2(x)     (PCout(10)) = (x)
#define FPGA_SPI_MOSI2(x)    (PCout(12)) = (x)
#define FPGA_SPI_MISO2()     (PDin(3))
/*******************  reg setting end  **********/

void FPGA_SPI_Init(void);
void spi_rece8(uint32_t *data);
void spi_rece16(uint32_t *data);
void spi_rece32(uint32_t *data);
void spi_send32_down(uint32_t data);
void spi_send32_up(uint32_t data);
void spi_send16(uint16_t data);
void SPI_SEND16(uint16_t data);

void      FPGA_SPI_Send_Data(uint16_t data);
uint32_t  FPGA_SPI_Rece_Data(void);

#endif //INC_2023_FPGA_SPI_H
