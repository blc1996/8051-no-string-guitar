#include "music.h"
#include "delay.h"
#include "sound.h"
#include "display.h"

uchar n = 0;
uchar code Music_TabM1[] = {3,2,3,2,4,2,5,2,5,2,4,2,3,2,2,2,
						   1,2,1,2,2,2,3,2,3,3,2,1,2,4,
						   3,2,3,2,4,2,5,2,5,2,4,2,3,2,2,2,
						   1,2,1,2,2,2,3,2,2,3,1,1,1,4,
						   2,2,2,2,3,2,1,2,2,2,3,1,4,1,3,2,1,2,
						   2,2,3,1,4,1,3,2,2,2,1,2,2,2,-2,4,
						   3,2,3,2,4,2,5,2,5,2,4,2,3,2,2,2,
						   1,2,1,2,2,2,3,2,2,3,1,1,1,4,0};		//欢乐颂

uchar code Music_TabM2[] = {6,1,7,1,
						   8,3,7,1,8,2,10,2,7,6,3,2,
						   6,3,5,1,6,2,8,2,5,6,3,2,
						   4,3,3,1,4,1,8,1,8,2,3,6,8,2,
						   7,3,4,1,4,2,7,2,7,4,0}	 ;			//天空之城

/*void Timer0()  interrupt 1   //采用中断0 控制节拍    
{  
	TH0=0xd8;   
	TL0=0xef;   
	n--;  
} */

void music1(void)
{
	uchar p,m;   //m为频率常数变量    
	uchar i=0;    	  
	TMOD|=0x11;    
	ET0=1; 		 //为了不影响别的使能位
	while(menuflag==3)
	{
		TH0=0xd8;TL0=0xef;
		p = Music_TabM1[i]+7;
		menu2change(p);
		if(p==0)break;
		else
		{
			m = p; 
			i++;
			n = Music_TabM1[i]*20;		 //改变这个可以调整每个音的时间长短
			TR0 = 1;
			while(n!=0)Sound(m);
			TR0 = 0;
			delay(100);	
		}
		i++;
	}
}

void music2(void)
{
	uchar p,m;   //m为频率常数变量    
	uchar i=0;    	  
	TMOD|=0x11;    
	ET0=1; 		 //为了不影响别的使能位
	while(menuflag==3)
	{
		TH0=0xd8;TL0=0xef;
		p = Music_TabM2[i]+7;
		menu2change(p);
		if(p==0)break;
		else
		{
			m = p; 
			i++;
			n = Music_TabM2[i]*30;		 //改变这个可以调整每个音的时间长短
			TR0 = 1;
			while(n!=0)Sound(m);
			TR0 = 0;
			delay(100);	
		}
		i++;
	}
}