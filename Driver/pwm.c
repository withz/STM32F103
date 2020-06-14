#include "pwm.h"

void PWMA_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	TIM_OCInitTypeDef        oc;
	GPIO_InitTypeDef         gpio;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);	
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	
	gpio.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &gpio);   
	gpio.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &gpio);
	gpio.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &gpio);
	gpio.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &gpio);
	
	tim.TIM_Period=20000-1;
	tim.TIM_Prescaler=72-1;
	tim.TIM_ClockDivision=0;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &tim);

	oc.TIM_OCMode = TIM_OCMode_PWM2;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_Pulse = 980;
	oc.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &oc);
	TIM_OC2Init(TIM1, &oc);
	TIM_OC3Init(TIM1, &oc);
	TIM_OC4Init(TIM1, &oc);
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);

	TIM_ARRPreloadConfig(TIM1, ENABLE);

	TIM_CtrlPWMOutputs(TIM1,ENABLE);

	TIM_Cmd(TIM1, ENABLE);
}

void PWMB_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	TIM_OCInitTypeDef        oc;
	GPIO_InitTypeDef         gpio;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	
	gpio.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &gpio);   
	gpio.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &gpio);
	gpio.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB, &gpio);

	tim.TIM_Period=200-1;
	tim.TIM_Prescaler=7200-1;
	tim.TIM_ClockDivision=0;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &tim);

	oc.TIM_OCMode = TIM_OCMode_PWM2;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_Pulse = 10;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM4, &oc);
	TIM_OC2Init(TIM4, &oc);
	TIM_OC3Init(TIM4, &oc);
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	//TIM_CtrlPWMOutputs(TIM4,ENABLE);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void SetPWMA1(unsigned int x)
{
	TIM_SetCompare1(TIM1,x);
}

void SetPWMA2(unsigned int x)
{
	TIM_SetCompare2(TIM1,x);
}

void SetPWMA3(unsigned int x)
{
	TIM_SetCompare3(TIM1,x);
}

void SetPWMA4(unsigned int x)
{
	TIM_SetCompare4(TIM1,x);
}

void SetPWMB1(unsigned int x)
{
	TIM_SetCompare1(TIM4,x);
}

void SetPWMB2(unsigned int x)
{
	TIM_SetCompare2(TIM4,x);
}

void SetPWMB3(unsigned int x)
{
	TIM_SetCompare3(TIM4,x);
}

