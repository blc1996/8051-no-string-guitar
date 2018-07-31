#include "record.h"
#include "sound.h"
#include "delay.h"
#include "display.h"

uchar xdata RNote0[50]=0;
uint  xdata RTime0[50]=0;
uchar xdata RNote1[50]=0;
uint  xdata RTime1[50]=0;


void input(uchar note, uint time, uchar item, bit flag)  //对输入进来的音符note和时长time进行记录
{
    if(flag==0)
	{
	RNote0[item] = note;
	RTime0[item] = time;
	}else
	{
	RNote1[item] = note;
	RTime1[item] = time;
	}
}

void output(bit flag)
{
	uchar i;
	uchar a;
	uint j;
	if(flag==0)
	{
	for(i=1; RTime0[i]!=0; i++)							 //模仿记录的时候的方法，把歌曲输出
	{
	    j=RTime0[i];
		while(j--)
		{
			if(RNote0[i]==0)
			{
				for(a=0; a<10; a++)
				delay(200);
			}
			else 
				Sound(RNote0[i]);
		}
	}
	}else
	{
	  for(i=1; RTime1[i]!=0; i++)
	  {
	    j=RTime1[i];
		while(j--)
		{
			if(RNote1[i]==0)
			{
				for(a=0; a<10; a++)
				delay(200);
			}
			else 
				Sound(RNote1[i]);
	     }
	   }
    }
}		   