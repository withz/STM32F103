#ifndef DELAY_H
#define DELAY_H

#include "stm32f10x.h"

void delay_init(void);

void delay_us(unsigned int nus);
void delay_ms(unsigned int nms);

void delayms(unsigned int t);
void delayus(unsigned int t);
#endif
