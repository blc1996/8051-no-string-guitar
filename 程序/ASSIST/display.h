#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "STC15F2K60S2.h"
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned int


//io ports
#define  LcdData P0
sbit RS = P3^6;
sbit RW = P3^5;
sbit E = P3^4;
sbit PSB = P3^3;
sbit RST = P3^1;

extern uchar menuflag;

void delayMs(uchar a);
void Lcd_WriteCmd(unsigned char CmdCode);
void Lcd_WriteData(uchar dat);
void init();
void setPosition(uchar x, uchar y);
unsigned char Lcd_CheckBusy(void);
unsigned char Lcd_ReadData(void);
void Lcd_WriteStr(unsigned char y,unsigned char x,unsigned char *Str);
void Lcd_PutPixel(unsigned char x,unsigned char y,unsigned char Color);
unsigned char Lcd_ReadPixel(unsigned char x,unsigned char y);
void Lcd_HoriLine(unsigned char x,unsigned char y,unsigned char Length,unsigned char Color);
void Lcd_VertLine(unsigned char x,unsigned char y,unsigned char Length,unsigned char Color);
void Lcd_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char Color);
void Lcd_Rectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char Color);
void CircleDot(unsigned char x,unsigned char y,char xx,char yy,unsigned char Color);
void Lcd_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char Color);
void show_Pic(uchar x, uchar y, uchar clong, uchar hight, uchar *Icon);
void Lcd_Clear(unsigned char Mode);
void Lcd_Reset();
void menuKey(uchar key);
void menu2change(uchar x);                             
void menu4();
void menu3();
void menu2();
void menu1();
void menu0();
#endif