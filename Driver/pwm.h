#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"

void PWMA_Configuration(void);
void PWMB_Configuration(void);

void SetPWMA1(unsigned int x);
void SetPWMA2(unsigned int x);
void SetPWMA3(unsigned int x);
void SetPWMA4(unsigned int x);

void SetPWMB1(unsigned int x);
void SetPWMB2(unsigned int x);
void SetPWMB3(unsigned int x);

#define PWM1 TIM1->CCR1
#define PWM2 TIM1->CCR2

#endif
