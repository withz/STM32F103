#include "bstring.h"


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
	while((USART1->SR&0X40)==0); //ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

char *itoa(int value, char *string, int radix)
{
	int i, d;
	int flag = 0;
	char *ptr = string;

	if (radix != 10){
		*ptr = 0;
		return string;
	}
	if (!value){
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}
	if (value < 0){
		*ptr++ = '-';
		value *= -1;
	}

	for (i = 10000; i > 0; i /= 10){
		d = value / i;
		if (d || flag){
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}
	*ptr = 0;

	return string;
}
 

int atoi(const char *str)
{
	int s=0;
	unsigned char falg=0;
	
	while(*str==' '){
		str++;
	}
	if(*str=='-'||*str=='+'){
		if(*str=='-'){
			falg=1;
		}
		str++;
	}
 
	while(*str>='0'&&*str<='9'){
		s=s*10+*str-'0';
		str++;
		if(s<0){
			s=2147483647;
			break;
		}
	}
	return s*(falg?-1:1);
}

