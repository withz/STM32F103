#ifndef MSPI_H
#define MSPI_H

#include "stm32f10x.h"
#include "gpio.h"
#include "delay.h"

typedef struct MSPI_t{
	MG_PORT clk_gpio;
	MG_PORT miso_gpio;
    MG_PORT mosi_gpio;
    MG_PORT cs_gpio;
    
	MG_PIN clk_pin;
	MG_PIN miso_pin;
    MG_PIN mosi_pin;
	MG_PIN cs_pin;
    
	u8 high_first; // 0 or 1
    u8 cpol;  // 0 or 1
    u8 edge;  // 1 or 2
    u8 line;  // 1 or 2
	u16 time;
}Mspi;

#endif
