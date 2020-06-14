#include "dac.h"

u32 DAC_Ref_Voltage = 3110;

void DAC_Configuration(void)
{
	DAC_InitTypeDef dac;
	GPIO_InitTypeDef gpio;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_4;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);
	
	dac.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	dac.DAC_Trigger = DAC_Trigger_Software;
	dac.DAC_WaveGeneration = DAC_WaveGeneration_None;
	dac.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits11_0;
	DAC_Init(DAC_Channel_1,&dac);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
}

void DAC_Value(u32 val)
{
	val = val * 4095 / DAC_Ref_Voltage;
	DAC_SetChannel1Data(DAC_Align_12b_R, val);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}
