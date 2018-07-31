#include "STC15F2K60S2.h"
#include "Key.h"
#define OS_LONG_KEY_EN 1//���Ӧ������Ҫ������������������Ϊ1��������Ϊ0�����Ӧ���в���Ҫ���������������鶨��Ϊ0���Խ�ʡ����ռ䣩

/***********************************************
����˵������������ɨ��
��ڲ�������
���ڲ�����ɨ��һ�μ����Ի�ð������
ע����OSReadKey������������
��Ҫ����ʵ��Ӳ���ļ��̽ӿ��޸�
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
����˵������ȡ��������
��ڲ�������
���ڲ��������ذ�������
ע��û�а����������򷵻�0��1�Ű�������������1-4
2�Ű�������������5-8���������
����1��5��..��ȷ�϶̰�����
����2��6��..��ȷ�ϳ�������
����3��7��..��ȷ�϶̰��ɿ�
����4��8��..��ȷ�ϳ����ɿ�
***********************************************/
unsigned char OSReadKey(void)
{
	static unsigned char KeyEventCnt=0;
	static unsigned char KeySampleCnt=0;
	static unsigned char KeyBuffer=0;
#define SHORT_ON_DITHERING_COUNTER 3//����̰�����ȥ��ʱ��
#define SHORT_OFF_DITHERING_COUNTER 3//����̰��ɿ�ȥ��ʱ�䣬һ����̰�����ȥ��ʱ����ͬ

#if OS_LONG_KEY_EN>0
	static unsigned int LongKeySampleCnt=0;
	#define LONG_ON_DITHERING_COUNTER 250//���峤������ȷ����Ҫ��ʱ�䣬�����ÿ1MS����һ��OSReadKey()����1000��ζ�����ʱ��Ϊ1S
	#define LONG_OFF_DITHERING_COUNTER 3//���峤���ɿ�ȥ��ʱ�䣬һ��Ͷ̰�ȥ��ʱ����ͬ
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
