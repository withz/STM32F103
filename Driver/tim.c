#include "tim.h"



void TIM2_Configuration(void)		//¶¨Ê±Æ÷
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = INTER_TIM2_PRE;
    nvic.NVIC_IRQChannelSubPriority = INTER_TIM2_SUB;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 36000-1;    //APB1 is 36MHz
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 1000;
    TIM_TimeBaseInit(TIM2,&tim);
}

void TIM2_Start(void)
{
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	TIM_Cmd(TIM2, ENABLE);
}



void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    nvic.NVIC_IRQChannel = TIM3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = INTER_TIM3_PRE;
    nvic.NVIC_IRQChannelSubPriority = INTER_TIM3_SUB;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 36-1;    //APB1 is 36MHz
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 5*1000;
    TIM_TimeBaseInit(TIM3,&tim);
}

void TIM3_Start(void)
{
    TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);	
	TIM_Cmd(TIM3, ENABLE);
}




