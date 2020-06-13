#ifndef SPI1_H
#define SPI1_H

#include "stm32f10x.h"

void SPI1M_Configuration(void);

u8 SPI1_ReadWriteByte(u8 w);
void SPI1_Input(void);
void SPI1_Output(void);
void SPI1_WriteByte(u8 w);
u8 SPI1_ReadByte(void);

#endif
