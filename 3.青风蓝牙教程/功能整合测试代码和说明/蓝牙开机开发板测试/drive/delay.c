#include "delay.h"
void delay_ms(unsigned long ms)    //ÑÓÊ±º¯Êý
{                         
	unsigned long a;
	while(ms)
	{
		a=2585;
		while(a--);
		ms--;
	}
}

void delay_us(unsigned long ms)    
{                         
	unsigned long a;
	while(ms)
	{
		a=1;
		while(a--);
		ms--;
	}
}

