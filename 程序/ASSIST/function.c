#include "function.h"
#include "sound.h"
#include "delay.h"
#include "piano.h"
#include "record.h"
#include "display.h"

extern void menu2change(uchar x);
uchar ritem=0;//建立全局变量用于对音符进行记录
bit recordFlag = 0;


//普通无弦琴函数
//功能：按不同的琴弦会发出相应的声音
void Function_1(void)
{
	uchar x;
	x = Piano();
	menu2change(x);
	while(x!=0)
	{
		Sound(x);
		x = Piano();		
	}
}


//录音无弦琴函数
//功能：包含了普通无弦琴功能，外加录音功能
extern void Function_2(void)
{
	uchar x,rnote;//x用于得到Piano函数的值，note用于记录音符
	uint rtime;	 //time用于记录每个音符保持多少个时间周期，这里的包括没有音符（即没有声音）的时候也会记录对应时间
	uchar a;
	//需要添加数组清零操作
	x = Piano();
/*	if(x==27)	//如果按下这个按键会开始播放录音
		{
			output(recordFlag);
			x = Piano();
			item = 0;		 //播放完录音之后对播放进行初始化
		}
		 else
		{	 */
			rnote = x;
			rtime = 0;
			if(x==0)//如果是0就不发声的计数
				{

					while((x==0)&&(menuflag==4))	  
					{
						for(a=0; a<10; a++)
							delay(200);		 //这里的delay函数是为了与Sound函数的时长对应差不多，防止time太大
						x = Piano();
						rtime++;
						
					}
					input(rnote,rtime,ritem,recordFlag);
					ritem++;
				}
			else//如果不是0就响声，然后计数
				{
					menu2change(x);
					while(x!=0)		
					{
						Sound(x);
						x = Piano();
						rtime++;
					}
					input(rnote,rtime,ritem,recordFlag);
					ritem++;
				}


		
}