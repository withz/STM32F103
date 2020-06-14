#include "spi1.h"

SPI_InitTypeDef spi1;

//const u32 spi1_tx_buf_size = 128;
//const u32 spi1_rx_buf_size = 128;

//u8 spi1_tx_buf[spi1_tx_buf_size];
//u8 spi1_rx_buf[spi1_rx_buf_size];

void SPI1M_Configuration()
{
    GPIO_InitTypeDef gpio;
    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio);
    
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &gpio);
    
    spi1.SPI_Direction = SPI_Direction_1Line_Tx;
    spi1.SPI_Mode = SPI_Mode_Master;
    spi1.SPI_DataSize = SPI_DataSize_8b;
    spi1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    spi1.SPI_CPHA = SPI_CPHA_2Edge;
    spi1.SPI_CPOL = SPI_CPOL_High;
    spi1.SPI_FirstBit = SPI_FirstBit_LSB;
    spi1.SPI_CRCPolynomial = 7;
    spi1.SPI_NSS = SPI_NSS_Soft;
    SPI_Init(SPI1, &spi1);
    
    SPI_Cmd(SPI1, ENABLE);
}

void SPI1_SetSpeed(u8 speed)
{
	SPI_Cmd(SPI1, DISABLE);
	spi1.SPI_BaudRatePrescaler = speed;
	SPI_Init(SPI1,&spi1);
	SPI_Cmd(SPI1,ENABLE);
}

void SPI1_HighFirst(u8 h)
{
	if(h!=0){
		spi1.SPI_FirstBit = SPI_FirstBit_MSB  ;
	}else{
		spi1.SPI_FirstBit = SPI_FirstBit_LSB  ;
	}
	SPI_Init(SPI1,&spi1);
	SPI_Cmd(SPI1,ENABLE);
}

void SPI1_SetDoubleMode(void)
{
	spi1.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Init(SPI1,&spi1);
	SPI_Cmd(SPI1,ENABLE);
}

void SPI1_SetSingleMode(void)
{
	spi1.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_Init(SPI1,&spi1);
	SPI_Cmd(SPI1,ENABLE);
}

void SPI1_Input()
{
    SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Rx);
}

void SPI1_Output()
{
    SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Tx);
}

u8 SPI1_ReadWriteByte(u8 w)
{
	u8 data=0;
    u32 wait=200;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET){
        if(wait<10) break;
        wait--;
    }
    SPI_I2S_SendData(SPI1, w);
    
    wait=200;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET){
        if(wait<10) break;
        wait--;
    }
    data= SPI_I2S_ReceiveData(SPI1);
    wait=200;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)==RESET){
        if(wait<10) break;
        wait--;
    }
    return data;
}

u8 SPI1_ReadByte(void)
{
    u32 wait=200;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET){
        if(wait<10) break;
        wait--;
    }
    return SPI_I2S_ReceiveData(SPI1);
}

void SPI1_WriteByte(u8 w)
{
    u32 wait=200;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET){
        if(wait<10) break;
        wait--;
    }
    SPI_I2S_SendData(SPI1, w);
}


//void SPI1S_Configuration()
//{
//    GPIO_InitTypeDef gpio;
//    SPI_InitTypeDef spi;
//    NVIC_InitTypeDef nvic;
//    
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
//    
//    gpio.GPIO_Speed = GPIO_Speed_50MHz;
//    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
//    gpio.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
//    GPIO_Init(GPIOA, &gpio);
//    
//    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    gpio.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_7;
//    GPIO_Init(GPIOA, &gpio);
//    
//    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//    spi.SPI_Mode = SPI_Mode_Slave;
//    spi.SPI_DataSize = SPI_DataSize_8b;
//    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
//    spi.SPI_CPHA = SPI_CPHA_2Edge;
//    spi.SPI_CPOL = SPI_CPOL_High;
//    spi.SPI_FirstBit = SPI_FirstBit_MSB;
//    spi.SPI_NSS = SPI_NSS_Hard;
//    spi.SPI_CRCPolynomial = 7;
//    SPI_Init(SPI1, &spi);
//    
//    nvic.NVIC_IRQChannel = SPI1_IRQn;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    nvic.NVIC_IRQChannelPreemptionPriority = 1;
//    nvic.NVIC_IRQChannelSubPriority = 1;
//    NVIC_Init(&nvic);
//    
////    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
////    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
////    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_OVR, ENABLE);
//    
//    SPI_Cmd(SPI1, ENABLE);
//}


//void SPI1_IRQHandler(void)
//{
//    u8 recv=0;
//    if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE)!=RESET){
//        recv=SPI1->DR;
//        SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
//    }
//    if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE)!=RESET){
//        SPI1->DR=recv;
//        SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
//    }
//    if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_OVR)!=RESET){
//        
//        SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_OVR);
//    }
//    
//}

