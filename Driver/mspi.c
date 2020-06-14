#include "mspi.h"

void MSPI_Init(Mspi *m)
{
    MG_RCC(m->clk_gpio);
    MG_RCC(m->cs_gpio);

    MG_Output(m->clk_gpio, m->clk_pin);
	MG_Output(m->cs_gpio, m->cs_pin);

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
    
    if(m->cpol==1){
        MG_Set(m->clk_gpio, m->clk_pin, 1);
    }else{
        MG_Set(m->clk_gpio, m->clk_pin, 0);
    }
}

void MSPI_Start(Mspi *m)
{
    MG_Set(m->cs_gpio, m->cs_pin, 0);
}    

void MSPI_End(Mspi *m)
{
    MG_Set(m->cs_gpio, m->cs_pin, 1);
}  

void MSPI_Up(Mspi *m)
{
    MG_Set(m->clk_gpio, m->clk_pin, 1);
} 

void MSPI_Down(Mspi *m)
{
    MG_Set(m->clk_gpio, m->clk_pin, 1);
} 

void MSPI_In(Mspi *m)
{
    MG_Input(m->mosi_gpio, m->mosi_pin, 1);
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
    if(m->line==1){
        return MG_Get(m->mosi_gpio, m->mosi_pin);
    }else{
        return MG_Get(m->miso_gpio, m->miso_pin);
    }
}

void MSPI_WriteByte(Mspi *m, u8 byte)
{
    u8 i;
    u8 data=byte;
    if(m->high_first==1){
        data=0;
        for(i=0;i<8;i++){
            data=data>>1;
            data|=((byte&(1<<(7-i)))<<i);
        }
    }
    for(i=0;i<8;i++){
        if(m->cpol==1){
            if(m->edge==1){
                MSPI_Down(m);
                MSPI_Set(m, (data&0x01));
                MSPI_Wait(m);
                MSPI_Up(m);
                MSPI_Wait(m);
            }else{
                MSPI_Down(m);
                MSPI_Wait(m);
                MSPI_Up(m);
                MSPI_Set(m, (data&0x01));
                MSPI_Wait(m);
            }
        }else{
            if(m->edge==1){
                MSPI_Up(m);
                MSPI_Set(m, (data&0x01));
                MSPI_Wait(m);
                MSPI_Down(m);
                MSPI_Wait(m);
            }else{
                MSPI_Up(m);
                MSPI_Wait(m);
                MSPI_Down(m);
                MSPI_Set(m, (data&0x01));
                MSPI_Wait(m);
            }
        }
        data=data>>1;
    }
}

u8 MSPI_ReadByte(Mspi *m)
{
    u8 i;
    u8 data=0;
    u8 byte=0;
    
    
    for(i=0;i<8;i++){
        if(m->cpol==1){
            if(m->edge==1){
                MSPI_Down(m);
                byte|=MSPI_Get(m)&0x80;
                MSPI_Wait(m);
                MSPI_Up(m);
                MSPI_Wait(m);
            }else{
                MSPI_Down(m);
                MSPI_Wait(m);
                MSPI_Up(m);
                byte|=MSPI_Get(m)&0x80;
                MSPI_Wait(m);
            }
        }else{
            if(m->edge==1){
                MSPI_Up(m);
                byte|=MSPI_Get(m)&0x80;
                MSPI_Wait(m);
                MSPI_Down(m);
                MSPI_Wait(m);
            }else{
                MSPI_Up(m);
                MSPI_Wait(m);
                MSPI_Down(m);
                byte|=MSPI_Get(m)&0x80;
                MSPI_Wait(m);
            }
        }
        byte=byte>>1;
    }
    
    data=byte;
    if(m->high_first==1){
        data=0;
        for(i=0;i<8;i++){
            data=data>>1;
            data|=((byte&(1<<(7-i)))<<i);
        }
    }
    return data;
}

u8 MSPI_ReadWriteByte(Mspi *m, u8 byte)
{
    u8 i;
    u8 data=0;
    u8 temp=0;
    
    if(m->high_first==1){
        data=0;
        for(i=0;i<8;i++){
            data=data>>1;
            data|=((byte&(1<<(7-i)))<<i);
        }
    }
    for(i=0;i<8;i++){
        if(m->cpol==1){
            if(m->edge==1){
                MSPI_Down(m);
                temp|=MSPI_Get(m)&0x80;
                MSPI_Set(m, (data&0x01));
                MSPI_Wait(m);
                MSPI_Up(m);
                MSPI_Wait(m);
            }else{
                MSPI_Down(m);
                MSPI_Wait(m);
                MSPI_Up(m);
                temp|=MSPI_Get(m)&0x80;
                MSPI_Set(m, (data&0x01));
                MSPI_Wait(m);
            }
        }else{
            if(m->edge==1){
                MSPI_Up(m);
                temp|=MSPI_Get(m)&0x80;
                MSPI_Set(m, (data&0x01));
                MSPI_Wait(m);
                MSPI_Down(m);
                MSPI_Wait(m);
            }else{
                MSPI_Up(m);
                MSPI_Wait(m);
                MSPI_Down(m);
                temp|=MSPI_Get(m)&0x80;
                MSPI_Set(m, (data&0x01));
                MSPI_Wait(m);
            }
        }
        temp=temp>>1;
        data=data<<1;
    }
    
    data=temp;
    if(m->high_first==1){
        data=0;
        for(i=0;i<8;i++){
            data=data>>1;
            data|=((temp&(1<<(7-i)))<<i);
        }
    }
    return data;
}

void MSPI_WriteCmd(Mspi *m, u8 cmd)
{
    MSPI_Start(m);
    MSPI_Out(m);
    MSPI_WriteByte(m, cmd);
    MSPI_End(m);
}

void MSPI_WriteReg(Mspi *m, u8 addr, u8 data)
{
    MSPI_Start(m);
    MSPI_Out(m);
    MSPI_WriteByte(m, addr);
    MSPI_WriteByte(m, data);
    MSPI_End(m);
}

u8 MSPI_ReadReg(Mspi *m, u8 addr)
{
    u8 data=0;
    MSPI_Start(m);
    MSPI_Out(m);
    MSPI_WriteByte(m, addr);
    MSPI_In(m);
    data=MSPI_ReadByte(m);
    MSPI_End(m);
    return data;
}

void MSPI_WriteMultiReg(Mspi *m, u8 addr, u32 len, u8 *data)
{
    MSPI_Start(m);
    MSPI_Out(m);
    MSPI_WriteByte(m, addr);
    while(len--){
        MSPI_WriteByte(m, *data);
        data++;
    }
    MSPI_End(m);
}

void MSPI_ReadMultiReg(Mspi *m, u8 addr, u32 len, u8 *data)
{
    MSPI_Start(m);
    MSPI_Out(m);
    MSPI_WriteByte(m, addr);
    MSPI_In(m);
    while(len--){
        *data=MSPI_ReadByte(m);
        data++;
    }
    MSPI_End(m);
}

