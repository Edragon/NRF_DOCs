

#ifndef __OLED_H
#define	__OLED_H

#include "nrf.h"

#define LCD_SCL  27
#define LCD_SDA  23
#define LCD_RST  24
#define LCD_DC   26
#define LCD_CS   22
#define LCD_POWER_EN   25

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 
#define X_WIDTH 128
#define Y_WIDTH 64

void LCD_Init(void);
void LCD_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
void LCD_P8x16Str(unsigned char x, unsigned y,unsigned char ch[]);
void LCD_P16x16Ch(unsigned char x, unsigned char y,unsigned char  N);
void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);



#endif /* __OLED_H */






