#include "function.h"
#include "sound.h"
#include "delay.h"
#include "piano.h"
#include "record.h"
#include "display.h"

extern void menu2change(uchar x);
uchar ritem=0;//����ȫ�ֱ������ڶ��������м�¼
bit recordFlag = 0;


//��ͨ�����ٺ���
//���ܣ�����ͬ�����һᷢ����Ӧ������
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


//¼�������ٺ���
//���ܣ���������ͨ�����ٹ��ܣ����¼������
extern void Function_2(void)
{
	uchar x,rnote;//x���ڵõ�Piano������ֵ��note���ڼ�¼����
	uint rtime;	 //time���ڼ�¼ÿ���������ֶ��ٸ�ʱ�����ڣ�����İ���û����������û����������ʱ��Ҳ���¼��Ӧʱ��
	uchar a;
	//��Ҫ��������������
	x = Piano();
/*	if(x==27)	//���������������Ὺʼ����¼��
		{
			output(recordFlag);
			x = Piano();
			item = 0;		 //������¼��֮��Բ��Ž��г�ʼ��
		}
		 else
		{	 */
			rnote = x;
			rtime = 0;
			if(x==0)//�����0�Ͳ������ļ���
				{

					while((x==0)&&(menuflag==4))	  
					{
						for(a=0; a<10; a++)
							delay(200);		 //�����delay������Ϊ����Sound������ʱ����Ӧ��࣬��ֹtime̫��
						x = Piano();
						rtime++;
						
					}
					input(rnote,rtime,ritem,recordFlag);
					ritem++;
				}
			else//�������0��������Ȼ�����
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