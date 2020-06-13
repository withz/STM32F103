#include "mi2c.h"

GPIO_TypeDef* MI2C_GPIO(u8 g);
u16 MI2C_GPIN(u16 gpin);
void MI2C_RCC(u8 g);

#define MI2C_DELAY 		delayus(m->time)
#define MI2C_CLKD		MG_Set(m->scl_gpio, m->scl_pin, 0)
#define MI2C_CLKU		MG_Set(m->scl_gpio, m->scl_pin, 1)

void MI2C_Init(Mi2c* m)
{
    MG_RCC(m->scl_gpio);
    MG_RCC(m->sda_gpio);
    
	MG_Output(m->scl_gpio, m->scl_pin);
	MG_Output(m->sda_gpio, m->sda_pin);
    
    MG_Set(m->scl_gpio, m->scl_pin, 1);
    MG_Set(m->sda_gpio, m->sda_pin, 1);
}

void MI2C_Out(Mi2c* m)
{
	MG_Output(m->sda_gpio, m->sda_pin);
}

void MI2C_In(Mi2c* m)
{
	MG_Input(m->sda_gpio, m->sda_pin, 1);
}

void MI2C_Set(Mi2c* m,u8 i)
{
	if(i==0)
	{
		MG_Set(m->sda_gpio, m->sda_pin, 0);
	}else{
		MG_Set(m->sda_gpio, m->sda_pin, 1);
	}
}

u8 MI2C_Get(Mi2c* m)
{
	return MG_Get(m->sda_gpio, m->sda_pin);
}

void MI2C_Start(Mi2c* m)
{
	MI2C_Out(m);
	MG_Set(m->scl_gpio, m->scl_pin, 1);
	MG_Set(m->sda_gpio, m->sda_pin, 1);
	MI2C_DELAY;
    
	MG_Set(m->sda_gpio, m->sda_pin, 0);
	MI2C_DELAY;
    
	MG_Set(m->scl_gpio, m->scl_pin, 0);
	MI2C_DELAY;
}

void MI2C_End(Mi2c* m)
{
	MI2C_Out(m);
	MG_Set(m->sda_gpio, m->sda_pin, 0);
	MG_Set(m->scl_gpio, m->scl_pin, 0);
	MI2C_DELAY;
    
	MG_Set(m->scl_gpio, m->scl_pin, 1);
	MI2C_DELAY;
    
	MG_Set(m->sda_gpio, m->sda_pin, 1);
	MI2C_DELAY;
}

u8 MI2C_Read(Mi2c* m)
{
	u8 i;
	u8 data=0;
	data=0;
	MI2C_In(m);
	if(m->high_first==0)
	{
		
		for(i=0;i<8;i++)
		{
			data|=(MI2C_Get(m)<<i);
			MI2C_DELAY;
			MI2C_CLKU;
			MI2C_DELAY;
			MI2C_CLKD;
		}
	}else{
		for(i=0;i<8;i++)
		{
			data|=(MI2C_Get(m)<<(7-i));
			MI2C_DELAY;
			MI2C_CLKU;
			MI2C_DELAY;
			MI2C_CLKD;
		}
	}
	return data;
}

void MI2C_Write(Mi2c* m,u8 cmd)
{
	u8 i;
	MI2C_Out(m);
	if(m->high_first==0)
	{
		for(i=0;i<8;i++)
		{
			MI2C_Set(m,(cmd&0X01));
			cmd=cmd>>1;
			MI2C_DELAY;
			MI2C_CLKU;
			MI2C_DELAY;
			MI2C_CLKD;
		}
	}else{
		for(i=0;i<8;i++)
		{
			MI2C_Set(m,(cmd&0X80));
			cmd=cmd<<1;
			MI2C_DELAY;
			MI2C_CLKU;
			MI2C_DELAY;
			MI2C_CLKD;
		}
	}
	
}

void MI2C_NoAck(Mi2c* m)
{
	MI2C_Out(m);
	MI2C_Set(m,1);
    delayus(1);	   
	MI2C_CLKU;
    delayus(1);
	MI2C_DELAY;
	MI2C_CLKD;
}

void MI2C_SetNoAck(Mi2c* m)
{
	MI2C_CLKD;
	MI2C_Out(m);
	MI2C_Set(m,1);
	MI2C_DELAY;
	MI2C_CLKU;
	MI2C_DELAY;
	MI2C_CLKD;
}

void MI2C_SetAck(Mi2c* m)
{
	MI2C_CLKD;
	MI2C_Out(m);
	MI2C_Set(m,0);
	MI2C_DELAY;
	MI2C_CLKU;
	MI2C_DELAY;
	MI2C_CLKD;
}

u8 MI2C_GetAck(Mi2c* m)
{
	u8 ucErrTime=0;
	MI2C_In(m);
	MI2C_Set(m,1);
    delayus(2);	   
	MI2C_CLKU;
    delayus(2);	 
	while(MI2C_Get(m))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			return 1;
		}
	}
	MI2C_CLKD; 
	return 0;
}

u8 MI2C_WriteReg(Mi2c* m, u8 device, u8 reg, u8 cmd)
{
    u8 re=0;
    MI2C_Start(m);
    
    MI2C_Write(m, (device<<1)|0);
    re+=MI2C_GetAck(m);
    
    MI2C_Write(m, reg);
    re+=MI2C_GetAck(m);
    
    MI2C_Write(m, cmd);
    re+=MI2C_GetAck(m);
    
    MI2C_End(m);
    return re;
}

u8 MI2C_ReadReg(Mi2c* m, u8 device, u8 reg)
{
    u8 re=0;
    u8 data=0;
    MI2C_Start(m);
    
    MI2C_Write(m, (device<<1)|0);
    re+=MI2C_GetAck(m);
    
    MI2C_Write(m, reg);
    re+=MI2C_GetAck(m);
    
    MI2C_Start(m);
    
    MI2C_Write(m, (device<<1)|1);
    re+=MI2C_GetAck(m);
    
    data=MI2C_Read(m);
    MI2C_SetNoAck(m);
    
    MI2C_End(m);
    return data;
}

u8 MI2C_WriteMiltiReg(Mi2c* m, u8 device, u8 reg, u32 len, u8 *buf)
{
    u8 re=0;
    u32 i=0;
    MI2C_Start(m);
    
    MI2C_Write(m, (device<<1)|0);
    re+=MI2C_GetAck(m);
    
    MI2C_Write(m, reg);
    re+=MI2C_GetAck(m);
    
    for(i=0;i<len;i++){
        MI2C_Write(m, buf[i]);
        re+=MI2C_GetAck(m);
    }

    MI2C_End(m);
    return re;
}

u8 MI2C_ReadMiltiReg(Mi2c* m, u8 device, u8 reg, u32 len, u8 *buf)
{
    u8 re=0;
    u8 data=0;
    MI2C_Start(m);
    
    MI2C_Write(m, (device<<1)|0);
    re+=MI2C_GetAck(m);
    
    MI2C_Write(m, reg);
    re+=MI2C_GetAck(m);
    
    MI2C_Start(m);
    
    MI2C_Write(m, (device<<1)|1);
    re+=MI2C_GetAck(m);
    
    while(len){
        *buf=MI2C_Read(m);
        if(len==1){
            MI2C_SetNoAck(m);
        }else{
            MI2C_SetAck(m);
        }
        len--;
        buf++;
    }
    
    MI2C_End(m);
    return data;
}
