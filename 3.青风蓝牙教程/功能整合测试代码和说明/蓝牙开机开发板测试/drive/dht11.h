#ifndef __dht11_H
#define __dht11_H 

#include "delay.h"
 							  
 
#define   ds1802          11

#define dht_IO_IN()   nrf_gpio_cfg_input(ds1802,NRF_GPIO_PIN_PULLUP);   /*!< Configures SDA pin as input  */
#define dht_IO_OUT()  do { NRF_GPIO->DIRSET = (1UL << ds1802);  } while(0)   /*!< Configures SDA pin as output */
 
									   
#define	dht_data_OUT_0  nrf_gpio_pin_clear(ds1802) 
#define	dht_data_OUT_1  nrf_gpio_pin_set(ds1802)  
#define	dht_data_IN     ((NRF_GPIO->IN >> ds1802) & 0x1UL)    

u8 dht_Init(void);
u8 dht_Read_Data(u8 *temp,u8 *humi);  
#endif















