#include "sound.h"
#include "reg51.h"
//#include "delay.h"

sbit beep = P3^0;



//uint code note[] = {0xf8, 0x89, 0xf9, 0x59, 0xfa, 0x13, 0xfa, 0x68, 0xfb, 0x05, 0xfb, 0x90, 0xfc, 0x0c,
//					0xfc, 0x44, 0xfc, 0xad, 0xfd, 0x0a, 0xfd, 0x34, 0xfd, 0x83, 0xfd, 0xc8, 0xfe, 0x06,	  
//					0xfe, 0x23, 0xfe, 0x57, 0xfe, 0x85, 0xfe, 0x9a, 0xfe, 0xc2, 0xfe, 0xe4, 0xff, 0x02};

uint code note[] = {0xf8, 0x89, 0xf9, 0x59, 0xfa, 0x13, 0xfa, 0x68, 0xfb, 0x05, 0xfb, 0x90, 0xfc, 0x0c,
					0xfc, 0x44, 0xfc, 0xad, 0xfd, 0x0a, 0xfd, 0x34, 0xfd, 0x83, 0xfd, 0xc8, 0xfe, 0x06,	  
					0xfe, 0x23, 0xfe, 0x57, 0xfe, 0x85, 0xfe, 0x9a, 0xfe, 0xc2, 0xfe, 0xe4, 0xff, 0x02};


/*uint code note[] = 	{63625,63833,64019,64104,64260,64400,64524,
					 64581,64685,64778,64820,64898,64968,65030,
					 65058,65111,65157,65178,65217,65252,65283 }; */

uchar t ;

void Timer1() interrupt 3
{
	 t=0;
	 beep=~beep;
	 
}

void Sound(uchar x)
{
//	TMOD|=0x11;
//	ET1=1;
//	PT1=1;
	t = 1;												
	TH1 = note[x*2-2];
	TL1 = note[x*2-1];
	TR1=1;
	while(t)
	{}
	TR1=0;
}		
