#ifndef MI2C_H
#define MI2C_H

#include "stm32f10x.h"
#include "gpio.h"
#include "delay.h"

typedef struct MI2C_t{
	MG_PORT scl_gpio;
	MG_PORT sda_gpio;
	MG_PIN scl_pin;
	MG_PIN sda_pin;
	u8 high_first;
	u16 time;
}Mi2c;

void MI2C_Init(Mi2c* m);
void MI2C_Out(Mi2c* m);
void MI2C_In(Mi2c* m);
void MI2C_Set(Mi2c* m,u8 i);
u8 MI2C_Get(Mi2c* m);
void MI2C_Start(Mi2c* m);
void MI2C_End(Mi2c* m);
u8 MI2C_Read(Mi2c* m);
void MI2C_Write(Mi2c* m,u8 cmd);
void MI2C_NoAck(Mi2c* m);
void MI2C_SetNoAck(Mi2c* m);
void MI2C_SetAck(Mi2c* m);
u8 MI2C_GetAck(Mi2c* m);

u8 MI2C_WriteReg(Mi2c* m, u8 device, u8 reg, u8 cmd);
u8 MI2C_ReadReg(Mi2c* m, u8 device, u8 reg);
u8 MI2C_WriteMiltiReg(Mi2c* m, u8 device, u8 reg, u32 len, u8 *buf);
u8 MI2C_ReadMiltiReg(Mi2c* m, u8 device, u8 reg, u32 len, u8 *buf);




#endif
