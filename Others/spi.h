#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f4xx.h"
#include "sys.h"

void SPI_Configuration(void);
u8 SPI_ReadWrite(u16 data);
void SPI_SetByte(void);
void SPI_SetHalfWord(void);
void SPI_SetDoubleMode(void);
void SPI_SetSingleMode(void);
void SPI_SetSpeed(u8 speed);
void SPI_HighFirst(u8 h);
void SPI_DirectionSwitch(u8 in);
void SPI_Write(u16 data);
u8 SPI_Read(void);
void SPI_Wait(void);

#endif


