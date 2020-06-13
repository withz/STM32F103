#include "mspi.h"

void MSPI_Init(Mspi *m)
{
    MG_RCC(m->clk_gpio);
    MG_RCC(m->cs_gpio);

    MG_Output(m->clk_gpio, m->clk_pin);
	MG_Output(m->cs_gpio, m->cs_pin);

    MG_Set(m->clk_gpio, m->clk_pin, 1);
    MG_Set(m->cs_gpio, m->cs_pin, 1);
    
    if(m->line==1){
        MG_RCC(m->mosi_gpio);
        MG_Output(m->mosi_gpio, m->mosi_pin);
        MG_Set(m->mosi_gpio, m->mosi_pin, 1);
    }else{
        MG_RCC(m->miso_gpio);
        MG_RCC(m->mosi_gpio);
        MG_Output(m->miso_gpio, m->miso_pin);
        MG_Output(m->mosi_gpio, m->mosi_pin);
        MG_Set(m->miso_gpio, m->miso_pin, 1);
        MG_Set(m->mosi_gpio, m->mosi_pin, 1);
    }
    
}

void MSPI_Start(Mspi *m)
{
    if(m->cpol==1){
        MG_Set(m->cs_gpio, m->cs_pin, 0);
    }else{
        MG_Set(m->cs_gpio, m->cs_pin, 1);
    }
}    

void MSPI_End(Mspi *m)
{
    if(m->cpol==1){
        MG_Set(m->cs_gpio, m->cs_pin, 1);
    }else{
        MG_Set(m->cs_gpio, m->cs_pin, 0);
    }
}    

void MSPI_In(Mspi *m)
{
    if(m->cpol==1){
        MG_Input(m->mosi_gpio, m->mosi_pin, 1);
    }else{
        MG_Input(m->mosi_gpio, m->mosi_pin, 0);
    }
}

void MSPI_Out(Mspi *m)
{
    MG_Output(m->mosi_gpio, m->mosi_pin);
}

void MSPI_Wait(Mspi *m)
{
    delayus(m->time);
}

void MSPI_Set(Mspi *m, u8 val)
{
    MG_Set(m->mosi_gpio, m->mosi_pin, val);
}

u8 MSPI_Get(Mspi *m)
{
    return MG_Get(m->mosi_gpio, m->mosi_pin);
}

void MSPI_WriteByte(Mspi *m, u8 byte)
{
    u8 i;
    if(m->cpol==1){
        if(m->edge==1){
            
        }
    }else{
        
    }
}

u8 MSPI_ReadByte(Mspi *m)
{
    
}

u8 MSPI_ReadWriteByte(Mspi *m, u8 byte)
{
    
}

