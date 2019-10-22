#include "dht11.h"
#include "nrf_delay.h"
 
void dht_Rst(void)	   
{                 
	 dht_IO_OUT(); 	
    dht_data_OUT_0; 	
    nrf_delay_ms(25);    
    dht_data_OUT_1; 	
	  dht_data_OUT_1; 	
	  nrf_delay_us(30);    
}
u8 dht_Check(void) 	   
{   
	u8 time=0;
	dht_IO_IN();//SET INPUT	 
  while (dht_data_IN&&time<100)
	{
		time++;
		nrf_delay_us(1);
	} 
	if(time>=100)
	{
		printf("error\r\n");
		return 1;
	}
	else time=0;
 while (!dht_data_IN&&time<100)
	{
		time++;
		nrf_delay_us(1);
 
	}
	if(time>=100)
		{
		printf("error  2\r\n");
		return 1;
	}
	return 0;
}
u8 dht_Read_Bit(void) 			 
{
 	u8 time=0;
	while(dht_data_IN&&time<100)
	{
		time++;
		nrf_delay_us(1);
	}
	time=0;
	while(!dht_data_IN&&time<100)
	{
		time++;
		nrf_delay_us(1);
	}
	nrf_delay_us(40);
	if(dht_data_IN)return 1;
	else return 0;		   
}

u8 dht_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=dht_Read_Bit();
    }						    
    return dat;
}
u8 dht_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 dat[5];
	u8 i;
	dht_Rst();
	if(dht_Check()==0)
	{
		for(i=0;i<5;i++)
		{
			dat[i]=dht_Read_Byte();
		}
		if((dat[0]+dat[1]+dat[2]+dat[3])==dat[4])
		{
			*humi=dat[0];
			*temp=dat[2];
		}
	}else return 1;
	return 0;	    
}

u8 dht_Init(void)
{
	dht_Rst();
	return dht_Check();
}
 
