#include "function.h"
#include "display.h"
#include "music.h"
#include "sound.h"
#include "game.h"
#include "teach.h"
#include "musicGame.h"

#define uchar unsigned char
#define uint  unsigned int

//extern uchar menuflag;
extern uchar selectSong;
extern uchar acPointerx;
extern uchar acPointery;
extern uchar musicSpeed;
//extern uchar item;


//sbit gao = P4^5;
//sbit di  = P4^4;
void Timer0()  interrupt 1   //�����ж�0 ���ƽ���    
{  
	TH0=0xd8;   
	TL0=0xef;   
	n--;  
}

void Int0int(void) interrupt 0
{
		uint KeyValue=16;
		uchar a=0;
    	P2=0X0F;
			if(P2!=0x0f)
			{
			delayMs(100);			//����������
			//test columns
				switch(P2)
				{
					case(0X07):	KeyValue=0;break;
					case(0X0b):	KeyValue=1;break;
					case(0X0d): KeyValue=2;break;
					case(0X0e):	KeyValue=3;break;
				}
				//test rows
				P2=0XF0;
				switch(P2)
				{
					case(0X70):	KeyValue=KeyValue;break;
					case(0Xb0):	KeyValue=KeyValue+4;break;
					case(0Xd0): KeyValue=KeyValue+8;break;
					case(0Xe0):	KeyValue=KeyValue+12;break;
				}
				while((a<50)&&(P2!=0xf0))	 //monitor if still pressing
				{
					delayMs(10);
					a++;			     
				}
				switch(KeyValue)			//����������������Ӧ�Ĳ�����ȷ���ôΰ��������Ķ���
				{
					case(0): menuKey(0); break;
					case(1): menuKey(1); break;
					case(2): menuKey(2); break;
					case(3): menuKey(3); break;
					case(4): menuKey(4); break;
					case(5): menuKey(5); break;
					case(6): menuKey(6); break;
					case(7): menuKey(7); break;
					case(8): menuKey(8); break;
					case(9): menuKey(9); break;
					case(10): menuKey(10); break;
					case(11): menuKey(11); break;
					case(12): menuKey(12); break;
					case(13): menuKey(13); break;
					case(14): menuKey(14); break;
					case(15): menuKey(15); break;
		
				}
			}
			P2=0x0f;	 //��λ�������
			delayMs(1000);
    }



void main()
{	
    uint i=0;
	Lcd_Clear(0);		//����
	init();			    //��Ļ��ʼ��
	kaiji1();			//��������
	kaiji2();			//��������
	quanquan();			//��������
	EA=1;				//�ж�ʹ��
	TMOD|=0x11;		   //ʱ�ӳ�ʼ������
	ET1=1;			  //
	PT1=1;			 //
    EX0=1;
    IT0=1;			//ʹ��int0���½��ش���
	P2=0x0f;		 //����������
    init();			 //��Ļ��ʼ��	
	Lcd_Clear(0);//����
	menu0();	   //�������˵�

	while(1)
	{
	 switch(menuflag)		 //ͨ���ж�menuflag��������ǰ�Ĺ���
	 {
	     case(2): 
		 	Function_1(); 				  //���е��ຯ��
			break;		  
		 case(3): 						 //�ڲ�����������ʱ��ͨ���ж�selectsong��ȷ���������׸�
		 if(selectSong==1)
		 {
		     music1();
			 selectSong=0;
			 menuflag=0;
			 acPointerx=0;
			 acPointery=0;
			 menu0();
		 }else if(selectSong==2)
		 {
		     music2();
			 selectSong=0;
			 menuflag=0;
			 acPointerx=0;
			 acPointery=0;
			 menu0();
		 }							  //���Լ�����Ӹ�������Ҫ��menukey��������޸ģ�selectSong=3��4����
		 
		 break;
		 case(4):Function_2(); break;	 //¼������
		 case(7):
		 	InitCpu();//��ʼ��CPU
        	Lcd_Reset(); //��ʼ��LCD��
         	Lcd_Clear(0);//����
        	DrawBoard();//������
        	GamePlay();//����Ϸ
        	GameOver();//��Ϸ����
			while(1)
			{
			if(menuflag==0)break;
			}
			init();
			Lcd_Clear(0);//����
			menu0();
			IE2=0x00;
			break;
		case(8):
	     	Lcd_WriteCmd(0x01);
		    Teach();				   //ѧϰ����
			while(1)				   //�����Ժ����ڵ�ǰ���棬�����ؼ����˳�
			{
			if(menuflag==0)break;
			}
			init();
			menu0();
			break;
		case(9):
			if(musicSpeed!=0)
			{
			Lcd_WriteCmd(0x01);
			Lcd_Clear(0);
			Lcd_WriteCmd(0x0c);						//�رչ��
			if(musicSpeed==3)						//��Ϸ�Ѷ�ѡ��
		        musicGame2(musicSpeed);
			else
			    musicGame(musicSpeed);
			while(1)							  //��Ϸ������ֱ���˳�
			{
			if(menuflag==0)
			{
			 musicSpeed=0;
			 break;
			 }
			}
			init();
			menu0();
			}
			break;
	 }
	}			 

}			   