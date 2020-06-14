#ifndef STR_H
#define STR_H

#include "stm32f10x.h"
#include <stdio.h>

void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ
char *itoa(int value, char *string, int radix);
int atoi(const char *str);

#endif
