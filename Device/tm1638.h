#ifndef TM1638_H
#define TM1638_H

#include "stm32f10x.h"
#include "spi1.h"
#include "gpio.h"
#include "delay.h"


void TM1638_Init(void);

void TM1638_On(void);
void TM1638_Off(void);
void TM1638_DisplayBit(u8 p, u8 n);
void TM1638_Clear(void);
u32 TM1638_ReadKey(void);

#endif
