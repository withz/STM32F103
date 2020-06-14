#ifndef OLED_H
#define OLED_H

#define OLED_ADDRESS	0x78

#include "stm32f10x.h"
#include "oledres.h"
#include "mi2c.h"
#include "gpio.h"


void OLED_Configuration(void);
void OLED_Init(void);

void OLEDCmd(u8 reg, u8 cmd);
void WriteCmd(u8 cmd);
void WriteDat(unsigned char I2C_Data);

void OLED_ON(void);
void OLED_OFF(void);

void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_SetPos(unsigned char x, unsigned char y);

void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);






#endif


