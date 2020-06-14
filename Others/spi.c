#include "spi.h"

SPI_InitTypeDef  SPI_InitStructure;
u32 speed_wait = 1;

void SPI_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;       
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;       
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;      
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource3,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource2,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
}

void SPI_SetSpeed(u8 speed)
{
	SPI_Cmd(SPI2, DISABLE);
	SPI_InitStructure.SPI_BaudRatePrescaler = speed;
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE); 
	speed_wait = (speed >> 3)&0xF;
}

void SPI_HighFirst(u8 h)
{
	if(h!=0){
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB  ;
	}else{
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB  ;
	}
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
}

void SPI_SetSingleMode(void)
{
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
}

void SPI_DirectionSwitch(u8 in)
{
	if(in!=0){
		SPI_BiDirectionalLineConfig(SPI2,SPI_Direction_Rx);
	}else{
		SPI_BiDirectionalLineConfig(SPI2,SPI_Direction_Tx);
	}
}

void SPI_SetDoubleMode(void)
{
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
}

void SPI_SetByte(void)
{
	SPI_Cmd(SPI2, DISABLE);
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}

void SPI_SetHalfWord(void)
{
	SPI_Cmd(SPI2, DISABLE);
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}

u8 SPI_ReadWrite(u16 data)
{
	u32 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){
		retry++;
		if(retry>200)return 0;
	}
	SPI_I2S_SendData(SPI2, data);
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){
		retry++;
		if(retry>200)return 0;
	}
	return SPI_I2S_ReceiveData(SPI2);
}

void SPI_Write(u16 data)
{
	u32 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){
		retry++;
		if(retry>200)return;
	}
	SPI_I2S_SendData(SPI2, data);
}

u8 SPI_Read(void)
{
	u32 retry=0;	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){
		retry++;
		if(retry>200)return 0;
	}
	return SPI_I2S_ReceiveData(SPI2);
}

void SPI_Wait(void)
{
	u32 wait=(0x10<<speed_wait);
	while(wait--);
}
