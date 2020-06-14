#ifndef STR_H
#define STR_H

#include "stm32f10x.h"
#include <stdio.h>

void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址
char *itoa(int value, char *string, int radix);
int atoi(const char *str);

#endif
