#include "display.h"
#include "teach.h"
#include "delay.h"
#include "piano.h"
#include "sound.h"
#include "music.h"

uchar code Music_Teach[] = {3,2,3,2,4,2,5,2,5,2,4,2,3,2,2,2,
						    1,2,1,2,2,2,3,2,3,3,2,1,2,1,0,0,0,0,0};


void Yin1(uchar o)
{
	switch(o)
	{
		case 0: Lcd_WriteStr(2,6,"    ");break;
		case 1: Lcd_WriteStr(2,6,"��Do");break;
		case 2: Lcd_WriteStr(2,6,"��Re");break;
		case 3: Lcd_WriteStr(2,6,"��Mi");break;
		case 4: Lcd_WriteStr(2,6,"��Fa");break;
		case 5: Lcd_WriteStr(2,6,"��So");break;
		case 6: Lcd_WriteStr(2,6,"��La");break;
		case 7: Lcd_WriteStr(2,6,"��Si");break;
	}
}
void Yin2(uchar p)
{
	switch(p)
	{
		case 0: Lcd_WriteStr(2,3,"    ");break;
		case 1: Lcd_WriteStr(2,3,"��Do");break;
		case 2: Lcd_WriteStr(2,3,"��Re");break;
		case 3: Lcd_WriteStr(2,3,"��Mi");break;
		case 4: Lcd_WriteStr(2,3,"��Fa");break;
		case 5: Lcd_WriteStr(2,3,"��So");break;
		case 6: Lcd_WriteStr(2,3,"��La");break;
		case 7: Lcd_WriteStr(2,3,"��Si");break;
	}
}
void Yin3(uchar q)
{
	switch(q)
	{
		case 0: Lcd_WriteStr(2,0,"    ");break;
		case 1: Lcd_WriteStr(2,0,"��Do");break;
		case 2: Lcd_WriteStr(2,0,"��Re");break;
		case 3: Lcd_WriteStr(2,0,"��Mi");break;
		case 4: Lcd_WriteStr(2,0,"��Fa");break;
		case 5: Lcd_WriteStr(2,0,"��So");break;
		case 6: Lcd_WriteStr(2,0,"��La");break;
		case 7: Lcd_WriteStr(2,0,"��Si");break;
	}
}

void Yinchang(uchar time_o)
{
	switch(time_o)
	{
		case 0:	Lcd_WriteStr(3,6,"0");break;
		case 1:	Lcd_WriteStr(3,6,"1");break;
		case 2:	Lcd_WriteStr(3,6,"2");break;
		case 3:	Lcd_WriteStr(3,6,"3");break;
		case 4:	Lcd_WriteStr(3,6,"4");break;
	}
}

void Board_first(void)
{
	unsigned char n;
	for(n=0;n<=42;n++)
	{
		Lcd_Rectangle(3*n,0,3*n+2,2,1);
		Lcd_Rectangle(3*n,60,3*n+2,62,1);
	}
	for(n=0;n<21;n++)
	{
		Lcd_Rectangle(90,3*n,92,3*n+2,1);		
	}
}

void Board_second(void)
{
	unsigned char n;
	for(n=0;n<=42;n++)
	{
		Lcd_Rectangle(3*n,0,3*n+2,2,1);
		Lcd_Rectangle(3*n,60,3*n+2,62,1);
	}
}

void arrow(void)
{
	uchar n;
	for(n=6;n<=21;n++)
	   Lcd_Rectangle(3*n,3*6,3*n+2,3*6+2,1);
	for(n=19;n<=20;n++)
	{
		Lcd_Rectangle(3*n,3*5,3*n+2,3*5+2,1);
		Lcd_Rectangle(3*n,3*7,3*n+2,3*7+2,1);
	}
	for(n=18;n<=19;n++)
	{
	Lcd_Rectangle(3*n,3*4,3*n+2,3*4+2,1);
	Lcd_Rectangle(3*n,3*8,3*n+2,3*8+2,1);
	}
	for(n=17;n<=18;n++)
	{
	Lcd_Rectangle(3*n,3*3,3*n+2,3*3+2,1);
	Lcd_Rectangle(3*n,3*9,3*n+2,3*9+2,1);
	}
	for(n=16;n<=17;n++)
	{
	Lcd_Rectangle(3*n,3*2,3*n+2,3*2+2,1);
	Lcd_Rectangle(3*n,3*10,3*n+2,3*10+2,1);
	}
}								 



void Teach(void)
{
	uchar iter_Teach = 0;
	uchar o,p,q,time_o;
	uchar x;
	uchar i;
	uchar set_time;

	Board_first();
	arrow();
	Lcd_WriteStr(1,6,"��ǰ");
	o = Music_Teach[0];
	p = Music_Teach[2];
	q = Music_Teach[4];
	time_o = Music_Teach[1];
	TMOD|=0x11;    
	ET0=1;
	EA = 1;
//	PT0=1;
//	PT1=0;
	set_time=200;
	n = set_time;
	TR0 = 1;
	while((o!=0)&&(menuflag==8))
	{
		 Yin1(o);
		 Yin2(p);
		 Yin3(q);
		 Yinchang(time_o);
		 if(time_o==0)
		 {
		 	iter_Teach = iter_Teach+2;
			o = p;
			p = q;
			q = Music_Teach[iter_Teach+4];
			time_o = Music_Teach[iter_Teach+1];
		 }
		 
		 if(n==0)
		{n = set_time;time_o--;}
		 
		 x = Piano();
		 if(x!=0)
		 	{
			for(i=0;i<150;i++)
				{
					if(n==0)
					 {n = set_time;time_o--; break;}
					 Sound(x);
				}
			}	

	}	 
   	
	Lcd_Clear(0);
    Lcd_WriteStr(3,6,"  ");
	Board_second();
	Lcd_WriteStr(1,0,"��ֻ�����ţ�    ");
	Lcd_WriteStr(2,0,"��ǿ��Ҫ����ϰŶ");  
}