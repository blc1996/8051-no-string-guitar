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
void Timer0()  interrupt 1   //采用中断0 控制节拍    
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
			delayMs(100);			//防抖动处理
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
				switch(KeyValue)			//给按键函数传入相应的参数，确定该次按键触发的动作
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
			P2=0x0f;	 //复位矩阵键盘
			delayMs(1000);
    }



void main()
{	
    uint i=0;
	Lcd_Clear(0);		//清屏
	init();			    //屏幕初始化
	kaiji1();			//开机动画
	kaiji2();			//开机动画
	quanquan();			//开机动画
	EA=1;				//中断使能
	TMOD|=0x11;		   //时钟初始化三连
	ET1=1;			  //
	PT1=1;			 //
    EX0=1;
    IT0=1;			//使能int0，下降沿触发
	P2=0x0f;		 //激活矩阵键盘
    init();			 //屏幕初始化	
	Lcd_Clear(0);//清屏
	menu0();	   //绘制主菜单

	while(1)
	{
	 switch(menuflag)		 //通过判断menuflag来决定当前的功能
	 {
	     case(2): 
		 	Function_1(); 				  //自有弹奏函数
			break;		  
		 case(3): 						 //在播放内置音乐时，通过判断selectsong来确定播放哪首歌
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
		 }							  //可以继续添加歌曲，需要和menukey结合起来修改，selectSong=3，4……
		 
		 break;
		 case(4):Function_2(); break;	 //录音函数
		 case(7):
		 	InitCpu();//初始化CPU
        	Lcd_Reset(); //初始化LCD屏
         	Lcd_Clear(0);//清屏
        	DrawBoard();//画界面
        	GamePlay();//玩游戏
        	GameOver();//游戏结束
			while(1)
			{
			if(menuflag==0)break;
			}
			init();
			Lcd_Clear(0);//清屏
			menu0();
			IE2=0x00;
			break;
		case(8):
	     	Lcd_WriteCmd(0x01);
		    Teach();				   //学习函数
			while(1)				   //结束以后还留在当前界面，按返回键再退出
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
			Lcd_WriteCmd(0x0c);						//关闭光标
			if(musicSpeed==3)						//游戏难度选择
		        musicGame2(musicSpeed);
			else
			    musicGame(musicSpeed);
			while(1)							  //游戏结束后不直接退出
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