#include "piano.h"
#include "STC15F2K60S2.h"
#include "delay.h"

sbit k1 = P1^1;
sbit k2 = P1^2;
sbit k3 = P1^3;
sbit k4 = P1^4;
sbit k5 = P1^5;
sbit k6 = P1^6;
sbit k7 = P1^7;
sbit k0 = P1^0;
sbit high = P4^5;
sbit low  = P4^4;

//琴弦函数，判断是那根琴弦按下
//对于不同的高低音返回不一样的值，最终是能返回高中低各7音
uchar Piano(void)
{
	if(k1==0||k2==0||k3==0||k4==0||k5==0||k6==0||k7==0||k0==0)
	{

			   if(k0==0)return 27;
			   if(k1==0)return  (8+7*high-7*low);
			   if(k2==0)return  (9+7*high-7*low);
			   if(k3==0)return (10+7*high-7*low);
			   if(k4==0)return (11+7*high-7*low);
			   if(k5==0)return (12+7*high-7*low);
			   if(k6==0)return (13+7*high-7*low);
			   if(k7==0)return (14+7*high-7*low);
		   
	}  
	return 0;
}

