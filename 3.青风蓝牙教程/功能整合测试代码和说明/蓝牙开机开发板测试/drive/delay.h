#ifndef __DELAY_H
#define	__DELAY_H

#include "nrf.h"
#include <stdio.h>
#include "nrf_gpio.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
 

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

void delay_us(unsigned long ms);    //ÑÓÊ±º¯Êý
void delay_ms(unsigned long ms);

#endif /* __DELAY_H */
