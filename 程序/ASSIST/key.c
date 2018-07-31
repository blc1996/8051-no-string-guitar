#include "STC15F2K60S2.h"
#include "Key.h"
#define OS_LONG_KEY_EN 1//如果应用中需要处理长按键动作，则定义为1，否则定义为0（如果应用中不需要处理长按动作，则建议定义为0，以节省代码空间）

/***********************************************
功能说明：按键驱动扫描
入口参数：无
出口参数：扫描一次键盘以获得按键句柄
注：交OSReadKey（）函数调用
需要根据实际硬件的键盘接口修改
***********************************************/
unsigned char OSScanKey(void)
{
	unsigned char Temp;
	unsigned char i;
	P1=0xff;
	Temp=P1;
	for(i=0;i<8;i++)
	{
		if((Temp&(1<<i))==0)
			break;
	}
	if(i<8)
		return i+1;
	else
		return	0;
}

/**********************************************
功能说明：读取按键动作
入口参数：无
出口参数：返回按键动作
注：没有按键动作，则返回0，1号按键动作，返回1-4
2号按键动作，返回5-8，如此类推
返回1、5、..：确认短按按下
返回2、6、..：确认长按按下
返回3、7、..：确认短按松开
返回4、8、..：确认长按松开
***********************************************/
unsigned char OSReadKey(void)
{
	static unsigned char KeyEventCnt=0;
	static unsigned char KeySampleCnt=0;
	static unsigned char KeyBuffer=0;
#define SHORT_ON_DITHERING_COUNTER 3//定义短按按下去抖时间
#define SHORT_OFF_DITHERING_COUNTER 3//定义短按松开去抖时间，一般与短按按下去抖时间相同

#if OS_LONG_KEY_EN>0
	static unsigned int LongKeySampleCnt=0;
	#define LONG_ON_DITHERING_COUNTER 250//定义长按按下确认需要的时间，如果是每1MS调用一次OSReadKey()，则1000意味着这个时间为1S
	#define LONG_OFF_DITHERING_COUNTER 3//定义长按松开去抖时间，一般和短按去抖时间相同
#endif
	unsigned char KeyTemp;
	KeyTemp=OSScanKey();
	switch(KeyEventCnt)
	{
		case 0:
			if(KeyTemp!=0)
			{
				KeySampleCnt=0;
				KeyBuffer=KeyTemp;
				KeyEventCnt=1;     
			}
			return 0;//no key on,return 0
			break;
		
		#if OS_LONG_KEY_EN>0	
		case 1:
			if(KeyTemp!=KeyBuffer)
			{
				KeyEventCnt=0;
				return 0;//is dithering,return 0
			}
			else
			{
				if(++KeySampleCnt>SHORT_ON_DITHERING_COUNTER)
				{
					KeySampleCnt=0;
					KeyEventCnt=2;
					LongKeySampleCnt=0;
					return ((KeyBuffer-1)<<2)+1;//sure that key on,return (KeyBuffer-1)<<2+1
				}
				else
					return 0;//not sure that key on,return 0                 
			}
			break;
				
		case 2:
			if(++LongKeySampleCnt>LONG_ON_DITHERING_COUNTER)
			{
				KeySampleCnt=0;
				KeyEventCnt=3;
				return ((KeyBuffer-1)<<2)+2; //sure that key long on,return (KeyBuffer-1)<<2+2
			}
			else
			{
				if(KeyTemp!=KeyBuffer)
				{
					if(++KeySampleCnt>SHORT_OFF_DITHERING_COUNTER)
					{
						KeyEventCnt=0;
						return ((KeyBuffer-1)<<2)+3;//after short on to off,(KeyBuffer-1)<<2+3
					}
					else
						return 0;
				}
				else
				{
					KeySampleCnt=0;
					return 0;
				}
			}
			break;
		
		case 3:
			if(KeyTemp!=KeyBuffer)
			{
				if(++KeySampleCnt>LONG_OFF_DITHERING_COUNTER)
				{
					KeyEventCnt=0;
					return ((KeyBuffer-1)<<2)+4;  //after long key on turn to off,(KeyBuffer-1)<<2+4
				}
				else
					return 0;
			}
			else
			{
				KeySampleCnt=0;
				return 0;
			}
			break;
		
		#else
		case 1:
			if(KeyTemp!=KeyBuffer)
			{
				KeyEventCnt=0;
				return 0;//is dithering,return 0
			}
			else
			{
				if(++KeySampleCnt>=SHORT_ON_DITHERING_COUNTER)
				{
					KeySampleCnt=0;
					KeyEventCnt=2;
					return ((KeyBuffer-1)<<2)+1;//sure that key on,return (KeyBuffer-1)<<2+1
				}
				else
					return 0;//not sure that key on,return 0                 
			}
			break;
				
		case 2:
			if(KeyTemp!=KeyBuffer)
			{
				if(++KeySampleCnt>=SHORT_OFF_DITHERING_COUNTER)
				{
					KeyEventCnt=0;
					return ((KeyBuffer-1)<<2)+3;//after short on to off,(KeyBuffer-1)<<2+3
				}
				else
					return 0;
			}
			else
			{
				KeySampleCnt=0;
				return 0;
			}
			break;
		#endif

		default:break;
	}
	return 0;
}
/***************************************************************/
