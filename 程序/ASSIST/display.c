#include "display.h"
#include "function.h"
#include "record.h"
#include "sound.h"




uchar menuflag=0;
uchar musicSpeed=0;
uchar acPointerx=0;
uchar acPointery=0;
uchar selectSong=0;

uchar mychange=1;	  //这里用全局变量来让音符动起来

extern uchar ritem;
extern bit recordFlag;

sbit blc = P3^2;  //为了方便读取是否有按键按下
sbit H = P4^5;	  //反应高音低音的按键
sbit L = P4^4;

//delay 2us
void delayus()
{ _nop_();
  _nop_();
}

//delay a*1ms
void delayMs(uchar a)
{
    uint i, j;
    for(i=a; i>0;i--)
        for(j=100;j>0;j--);
}


/*************
测试LCD是否处于忙状态
如果忙则返回0x80，否则返回0
**************/
unsigned char Lcd_CheckBusy(void)
{
    unsigned char Busy;
	LcdData=0xff;
    RS=0;
    RW=1;
    E=1;
    _nop_(); 
    Busy=LcdData&0x80;
    E=0;
    return Busy;
}

/*********************************
向LCD写入字节数据
**********************************/
void Lcd_WriteData(unsigned char Data)
{  
	while(Lcd_CheckBusy());
	RS=1;
	RW=0;
	E=0;
	_nop_();  
	_nop_(); 
	LcdData=Data;
	E=1;
	_nop_();
	_nop_();
	E=0;
}

/***********************************
从LCD中读出数据
************************************/
unsigned char Lcd_ReadData(void)
{
	unsigned char Temp;
	while(Lcd_CheckBusy());
 	LcdData=0xff;
 	RS=1;
	RW=1;
	E=1;
	_nop_();
   	Temp=LcdData;
   	E=0;
   	return Temp;
}

/*************************************
想LCD中写入指令代码
**************************************/
void Lcd_WriteCmd(unsigned char CmdCode)
{  
	while(Lcd_CheckBusy());
   	RS=0;
   	RW=0;
   	E=0;
   	_nop_();  
	_nop_();
   	LcdData=CmdCode;
   	_nop_(); 
	_nop_();
   	E=1;
   	_nop_();  
	_nop_();
   	E=0;
}

void setPosition(uchar x, uchar y)
{
    uchar p;
    switch(x%4)
    {
        case 0: p = 0x80; break; // starting address of line 1
        case 1: p = 0x90; break; // line2
        case 2: p = 0x88; break; // line3
        case 3: p = 0x98; break; // line4
    }
    p+=y;
    Lcd_WriteCmd(p);
}

/*************************************
向LCD指定起始位置写入一个字符串
*************************************/
void Lcd_WriteStr(unsigned char y,unsigned char x,unsigned char *Str)
{
	if((y>3)||(x>7))
		return;//如果指定位置不在显示区域内，则不做任何写入直接返回
	EA=0;
	switch(y)
	{
		case 0:
				Lcd_WriteCmd(0x80+x);
				break;
		case 1:
				Lcd_WriteCmd(0x90+x);
				break;				
		case 2:
				Lcd_WriteCmd(0x88+x);
				break;
		case 3:
				Lcd_WriteCmd(0x98+x);
				break;
	}
	while(*Str>0)
	{  
		Lcd_WriteData(*Str);
  		Str++;     
	}
	EA=1;
}

/**************************************
为加速逻辑运算而设置的掩码表，这是以牺牲空间而换取时间的办法
***************************************/
code unsigned int LcdMaskTab[]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
							 0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
/***************************************
向LCD指定坐标写入一个象素,象素颜色有两种，0代表白（无显示），1代表黑（有显示）
****************************************/
void Lcd_PutPixel(unsigned char x,unsigned char y,unsigned char Color)
{
	unsigned char z,w;
	unsigned int Temp;
	if(x>=128||y>=64)
		return;
	Color=Color%2;
	w=15-x%16;//确定对这个字的第多少位进行操作
	x=x/16;//确定为一行上的第几字

	if(y<32) //如果为上页
		z=0x80;
	else     //否则如果为下页
		z=0x88;

	y=y%32;
	EA=0;
	Lcd_WriteCmd(0x36);
	Lcd_WriteCmd(y+0x80);        //行地址
	Lcd_WriteCmd(x+z);     //列地址 
	Temp=Lcd_ReadData();//先空读一次
	Temp=(unsigned int)Lcd_ReadData()<<8;//再读出高8位
	Temp|=(unsigned int)Lcd_ReadData();//再读出低8位
	EA=1;
	if(Color==1) //如果写入颜色为1
		Temp|=LcdMaskTab[w];//在此处查表实现加速
	else         //如果写入颜色为0
		Temp&=~LcdMaskTab[w];//在此处查表实现加速
	EA=0;
	Lcd_WriteCmd(y+0x80);        //行地址
	Lcd_WriteCmd(x+z);     //列地址
    Lcd_WriteData(Temp>>8);//先写入高8位，再写入低8位
    Lcd_WriteData(Temp&0x00ff);
	Lcd_WriteCmd(0x30);
	EA=1;
}

/******************************************
从LCD指定坐标读取象素颜色值
*******************************************/
unsigned char Lcd_ReadPixel(unsigned char x,unsigned char y)
{
	unsigned char z,w;
	unsigned int Temp;
	if(x>=128||y>=64)
		return 0;
	w=15-x%16;//确定对这个字的第多少位进行操作
	x=x/16;//确定为一行上的第几字

	if(y<32) //如果为上页
		z=0x80;
	else     //否则如果为下页
		z=0x88;

	y=y%32;
	EA=0;
	Lcd_WriteCmd(0x36);
	Lcd_WriteCmd(y+0x80);        //行地址
	Lcd_WriteCmd(x+z);     //列地址 
	Temp=Lcd_ReadData();//先空读一次
	Temp=(unsigned int)Lcd_ReadData()<<8;//再读出高8位
	Temp|=(unsigned int)Lcd_ReadData();//再读出低8位
	EA=1;
	if((Temp&&LcdMaskTab[w])==0)
		return 0;
	else
		return 1;
}

/***************************************
向LCD指定位置画一条长度为Length的指定颜色的水平线
****************************************/
void Lcd_HoriLine(unsigned char x,unsigned char y,unsigned char Length,unsigned char Color)
{
	unsigned char i;
	if(Length==0)
		return;
	for(i=0;i<Length;i++)
	{
		Lcd_PutPixel(x+i,y,Color);
	}
}

/***************************************
向LCD指定位置画一条长度为Length的指定颜色的垂直线
****************************************/
void Lcd_VertLine(unsigned char x,unsigned char y,unsigned char Length,unsigned char Color)
{
	unsigned char i;
	if(Length==0)
		return;
	for(i=0;i<Length;i++)
	{
		Lcd_PutPixel(x,y+i,Color);
	}
}

/*******************************************
向LCD指定起始坐标和结束坐标之间画一条指定颜色的直线
********************************************/
void Lcd_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char Color)
{
	unsigned int x,y; 
	unsigned int d_x,d_y;//d_x=x2-x1;d_y=y2-y1;
	int err=0;
	unsigned char temp=0;
	if(y2<y1)
	{
		x=x1;
		y=y1;
		x1=x2;
		y1=y2;
		x2=x;
		y2=y;
	}
	d_y=y2-y1;
	if (d_y==0)
	{
		if (x1>x2) 
		{
			x=x1;
			x1=x2;
			x2=x;
		}
		for (x=x1;x<=x2;x++) 
			Lcd_PutPixel(x,y1,Color);
	}

	else
	{
		if(x2>=x1)
		{
			temp=1;
			d_x=x2-x1;
		}
		else 
			d_x=x1-x2;
		x=x1;
		y=y1;
		Lcd_PutPixel(x,y,1);

		if(temp&&(d_y<=d_x))
			while(x!=x2)
			{
				if(err<0)
				{
					x=x+1;
					err=err+(y2-y);
				}
				else 
				{
					x=x+1;
					y=y+1;
					err=err+(y2-y)-(x2-x);
				}
				Lcd_PutPixel(x,y,Color);
			}

		else if(temp&&(d_y>d_x))
			while(y!=y2)
			{
				d_x=x2-x;
				d_y=y2-y; 
				if(err<0)
				{
					x=x+1;
					y=y+1;
					err=err+d_y-d_x;
				}
				else 
				{
					y=y+1;
					err=err-d_x;
				}
				Lcd_PutPixel(x,y,Color);

			}

		else if(!temp&&(d_y<=d_x))
			while(x!=x2)
			{
				d_x=x-x2;
				d_y=y2-y; 
				if(err<0)
				{
					x=x-1;
					err=err+d_y;
				}
				else 
				{
					x=x-1;
					y=y+1;
					err=err+d_y-d_x;
				}
				Lcd_PutPixel(x,y,Color);
			}

		else if(!temp &&(d_y>d_x))
			while(y!=y2)
			{
				d_x=x-x2;
				d_y=y2-y; 
				if(err<0)
				{
					x=x-1;
					y=y+1;
					err=err+d_y-d_x;
				}
				else 
				{
					y=y+1;
					err=err-d_x;
				}
				Lcd_PutPixel(x,y,Color);
			}
	}
}

/*******************************************
向LCD指定左上角坐标和右下角坐标画一个指定颜色的矩形
********************************************/
void Lcd_Rectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char Color)
{
	unsigned char Temp;
	if(x0>x1)
	{
		Temp=x0;
		x0=x1;
		x1=Temp;
	}	
	if(y0>y1)
	{
		Temp=y0;
		y0=y1;
		y1=Temp;
	}
	Lcd_VertLine(x0,y0,y1-y0+1,Color);
	Lcd_VertLine(x1,y0,y1-y0+1,Color);
	Lcd_HoriLine(x0,y0,x1-x0+1,Color);
	Lcd_HoriLine(x0,y1,x1-x0+1,Color);	
}

/****************************************
对称法画圆的8个镜像点
*****************************************/
void CircleDot(unsigned char x,unsigned char y,char xx,char yy,unsigned char Color)//内部函数，对称法画圆的8个镜像点
{
	Lcd_PutPixel((x+yy),(y+xx),Color);//第 1 个 8 分圆
	Lcd_PutPixel((x+xx),(y+yy),Color);//第 2 个 8 分圆
	Lcd_PutPixel((x-xx),(y+yy),Color);//第 3 个 8 分圆
	Lcd_PutPixel((x-yy),(y+xx),Color);//第 4 个 8 分圆
	Lcd_PutPixel((x-yy),(y-xx),Color);//第 5 个 8 分圆
	Lcd_PutPixel((x-xx),(y-yy),Color);//第 6 个 8 分圆
	Lcd_PutPixel((x+xx),(y-yy),Color);//第 7 个 8 分圆
	Lcd_PutPixel((x+yy),(y-xx),Color);//第 8 个 8 分圆
}

/******************************************
向LCD指定圆心坐标画一个半径为r的指定颜色的圆
*******************************************/
void Lcd_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char Color)//中点法画圆
{//中点法画圆
	unsigned char xx,yy;
	char deltax,deltay,d;
	xx=0;
	yy=r;
	deltax=3;
	deltay=2-r-r;
	d=1-r;
	CircleDot(x,y,xx,yy,Color);//对称法画圆的8个镜像点
	while (xx<yy)
	{
		if (d<0)
		{
			d+=deltax;
			deltax+=2;
			xx++;
		}
		else
		{
			d+=deltax+deltay;
			deltax+=2;
			deltay+=2;
			xx++;
			yy--;
		}
		CircleDot(x,y,xx,yy,Color);//对称法画圆的8个镜像点
	}
}

/*****************************************
清除Lcd全屏，如果清除模式Mode为0，则为全屏清除为颜色0（无任何显示）
否则为全屏清除为颜色1(全屏填充显示)
******************************************/
void Lcd_Clear(unsigned char Mode)
{
	unsigned char x,y,ii;
	unsigned char Temp;
	if(Mode%2==0)
		Temp=0x00;
	else
		Temp=0xff;
	Lcd_WriteCmd(0x36);//扩充指令 绘图显示
	for(ii=0;ii<9;ii+=8)   
		for(y=0;y<0x20;y++)     
			for(x=0;x<8;x++)
			{ 	
				EA=0;
				Lcd_WriteCmd(y+0x80);        //行地址
				Lcd_WriteCmd(x+0x80+ii);     //列地址     
				Lcd_WriteData(Temp); //写数据 D15－D8 
				Lcd_WriteData(Temp); //写数据 D7－D0 
				EA=1;
			}
	Lcd_WriteCmd(0x30);
}

/****************************************
LCD初始化
*****************************************/
void Lcd_Reset()
{  
	PSB=1;
	Lcd_WriteCmd(0x30);       //选择基本指令集
	Lcd_WriteCmd(0x0c);       //开显示(无游标、不反白)
	Lcd_WriteCmd(0x01);       //清除显示，并且设定地址指针为00H
	Lcd_WriteCmd(0x06);       //指定在资料的读取及写入时，设定游标的移动方向及指定显示的移位
}

/****************************************
//设置显示屏GDRAM位置
*****************************************/
void Lcd_Cursor(uchar a, uchar b)
{
	Lcd_WriteCmd(0x34);			  //关闭图形显示	
	Lcd_WriteCmd(b);			  //先写垂直地址
	Lcd_WriteCmd(a);			  //再写水平地址
}

/****************************************
//显示图片函数
*****************************************/
void show_Pic(uchar x, uchar y, uchar clong, uchar hight, uchar *Icon)
{                                        
	uchar i, j;
	for(i=0; i<hight; i++)		 //字模按行一次写入
	{
		if(y+i<32)				  //判断上下屏
			{Lcd_Cursor(0x80+x/16,0x80+y+i);}			
		else
			{Lcd_Cursor(0x88+x/16,0x80-32+y+i);}	

		for(j=0; j<clong; j++)
			{Lcd_WriteData(Icon[clong*i+j]);}	
	
	}
		Lcd_WriteCmd(0x36);

}
//initialize the LCD
void init()
{
    PSB = 1; //parallel
    Lcd_WriteCmd(0x30); //basic commands
    delayMs(10);
    Lcd_WriteCmd(0x0f); // display on, ac on
    delayMs(10);
    Lcd_WriteCmd(0x01); // clear screen
    delayMs(10);
}

void menu2change(uchar x)
{
    uchar key,note;
	key=(x-1)/7;
	note=(x-1)%7;
	mychange=~mychange;
 
  
   	if(x==0)
		{
		 /*writeString(data2no);
		 setPosition(3,3);
		 writeString(data2no);
		 setPosition(3,5);*/
		}
	else{
	if(key==1)
	    Lcd_WriteStr(2,3,"中");
	else if(key==2)
	    Lcd_WriteStr(2,3,"高");
	else if(key==0)
		Lcd_WriteStr(2,3,"低");
		if(note==0)
		 	Lcd_WriteStr(3,3,"Do");
		else if(note==1)
		 	Lcd_WriteStr(3,3,"Re");
		else if(note==2)
		 	Lcd_WriteStr(3,3,"Mi");
		else if(note==3)
		 	Lcd_WriteStr(3,3,"Fa");
		else if(note==4)
		 	Lcd_WriteStr(3,3,"So");
		else if(note==5)
		 	Lcd_WriteStr(3,3,"La");
		else if(note==6)
		 	Lcd_WriteStr(3,3,"Si");

	if(mychange==1)
		Lcd_WriteStr(7,3,0x0d0e);//这里尝试加音符
	else
		Lcd_WriteStr(7,3,0x0e0d);
	
	}	   
	//	setPosition(0,0);										   
}


void menu9()
{
	Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"请选择游戏难度");
	Lcd_WriteStr(1,0,"* 困难");
	Lcd_WriteStr(2,0,"* 中等");
	Lcd_WriteStr(3,0,"* 容易");
	setPosition(1,0);
	acPointery=1;
}

void menu6()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"* 录音一");
	Lcd_WriteStr(1,0,"* 录音二");
	setPosition(0,0);
	acPointery=0;
}


//矩阵键盘菜单
void menu5()
{
    Lcd_WriteCmd(0x01);  
    Lcd_WriteStr(0,0,"现在开始弹奏吧");
	Lcd_WriteStr(1,0,"按左上角按键退出");
	setPosition(3,7);
	Lcd_WriteData(0x0E);
	setPosition(3,7);
}

//录音菜单
void menu4()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"请随意弹奏吧！");
	Lcd_WriteStr(1,0,"按左上角按键完成");
	setPosition(3,7);
	Lcd_WriteData(0x0E);
	setPosition(3,7);
}

//选择内置歌曲菜单
void menu3()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"* 欢乐颂");
	Lcd_WriteStr(1,0,"* 天空之城");
	setPosition(0,0);
	acPointerx=0;
	acPointery=0;
}

//歌曲正在播放
void menu31()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"歌曲播放中");
    Lcd_WriteStr(1,0,"请稍后操作……");
	setPosition(3,7);
	Lcd_WriteData(0x0E);
	setPosition(3,7);
}

//弹奏菜单
void menu2()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"现在开始弹奏吧");
	Lcd_WriteStr(1,0,"按左上角按键退出");
	Lcd_WriteStr(2,0,"音阶：  ");
	setPosition(3,7);
	Lcd_WriteData(0x0E);
//	writeString(data23);
//	setPosition(4,0);
}

//子菜单选择，并非真的菜单;通过判断menuflag来决定进入哪一层菜单
void menu1()
{
	 menuflag++; //初始是0，要++
     switch(menuflag)
	 {
		case(2): menu2(); break;
		case(3): menu3(); break;
		case(4): menu4(); break;
		case(5): menu5(); break;
		case(6): menu6(); break;
		case(9): menu9(); break;
	 }

}

//主菜单
void menu0()
{
    if(acPointery<4)
	{
    Lcd_WriteCmd(0x01);
    Lcd_WriteStr(0,0,"* 自己弹奏音乐");
	Lcd_WriteStr(1,0,"* 播放现有歌曲");
	Lcd_WriteStr(2,0,"* 我想要录音！");
	Lcd_WriteStr(3,0,"* 矩阵键盘弹奏");
	Lcd_WriteCmd(0x80);
	}else
	{
	Lcd_WriteCmd(0x01);
    Lcd_WriteStr(0,0,"* 播放已有录音");
	Lcd_WriteStr(1,0,"* 俄罗斯方块");
	Lcd_WriteStr(2,0,"* 学习功能");
	Lcd_WriteStr(3,0,"* 节奏大师！");
	}
}

//按键以后对菜单的动作函数
void menuKey(uchar key)
{	  
       if(menuflag==0)	 //通过综合判断menuflag和当前按下的键来确定进一步的动作
      {
    	switch(key)
			{
			//	case(6): acPointerx++; setPosition(acPointery, acPointerx); break;
				case(11): 
				if((acPointery>3)&&(acPointery<=7))
				{
				acPointery--;
				menu0();
				setPosition(acPointery, acPointerx); break;
				}
				if(acPointery>0)				 //防止菜单越级的处理
				acPointery=acPointery-1; setPosition(acPointery, acPointerx); break;	  //acpointer是指示当前光标位置的全局变量
				case(10): menuflag++; menuflag+=acPointery; setPosition(0, 0);menu1(); break;
				case(9):
				if((acPointery>=3)&&(acPointery<7))
				{
				acPointery++;
				menu0();
				setPosition(acPointery, acPointerx); break;
				}
				if(acPointery<3)
			    acPointery++; setPosition(acPointery, acPointerx); break;
			//	case(14): acPointerx=acPointerx-1; setPosition(acPointery, acPointerx); break;
			}
	  }	else if(menuflag==2)
	  {
	      	 if(key==15)		 //退出到主菜单
			 {
			     menuflag=0;
				 menu0();
				 setPosition(0,0);
				 acPointerx=0;
	             acPointery=0;
			 }
	  }	else if(menuflag==3)
	  {
	     switch(key)
			{			//这里使用了selectsong这个全局变量来帮助判断要播放哪首歌，一开始为0，选择了以后会变成相应的歌的编号
			//	case(6): acPointerx++; setPosition(acPointery, acPointerx); break;	  
				case(11): 
				if(acPointery>0)acPointery=acPointery-1; setPosition(acPointery, acPointerx); break;
				case(10): selectSong++; selectSong+=acPointery; menu31(); break;
				case(9): 
				if(acPointery<1)acPointery++; setPosition(acPointery, acPointerx); break;
				case(15):menuflag=0; menu0();acPointerx=0; acPointery=0; selectSong==0;break;
			//	case(14): acPointerx=acPointerx-1; setPosition(acPointery, acPointerx); break;
			} 
	  }	else if(menuflag==4)			//录音
	  {
	      if(key==15)
			 {
			     menuflag=0;
				 menu0();
				 acPointerx=0;
	             acPointery=0;
				 recordFlag=~recordFlag;		 //录一首，标志位反转
				 ritem=0;
			 }
	  } else if (menuflag==5)	//矩阵键盘弹奏
	  {
	      switch(key)			//中断中直接算出键值，发声
		  {
		      case(15): menuflag=0; menu0();acPointerx=0; acPointery=0; break;
			  case(0): menu2change(14+7*H-7*L);while(blc==0){Sound(14+7*H-7*L);P2=0x0f;}; break;
			  case(1): menu2change(13+7*H-7*L);while(blc==0){Sound(13+7*H-7*L);P2=0x0f;}; break;
			  case(2): menu2change(12+7*H-7*L);while(blc==0){Sound(12+7*H-7*L);P2=0x0f;}; break;
			  case(3): menu2change(11+7*H-7*L);while(blc==0){Sound(11+7*H-7*L);P2=0x0f;}; break;
			  case(4): menu2change(10+7*H-7*L);while(blc==0){Sound(10+7*H-7*L);P2=0x0f;}; break;
			  case(5): menu2change(9+7*H-7*L);	while(blc==0){Sound(9+7*H-7*L);P2=0x0f;}; break;
			  case(6): menu2change(8+7*H-7*L);	while(blc==0){Sound(8+7*H-7*L);P2=0x0f;}; break;
		  }
	  }else if(menuflag==6)			 //录音播放
	  {
	      	switch(key)
			{
			    case(15): menuflag=0; acPointerx=0; acPointery=0; menu0(); break;
				case(11): acPointery=0; setPosition(acPointery, acPointerx); break;
				case(10): menu31();output(acPointery);menu6(); break;
				case(9): acPointery=1; setPosition(acPointery, acPointerx); break;
			}

	  }else if(menuflag==7)		  //俄罗斯方块
	  {
			switch(key)
			{
			    case(15): menuflag=0; acPointerx=0; acPointery=0; break;
			}	 
	  }else if(menuflag==8)			//学习功能
	  {
	        if(key==15)
			{
			    menuflag=0; acPointerx=0; acPointery=0; 
			}
	  }else if(menuflag==9)		  //节奏大师
	  {
			switch(key)
			{			//这里使用了musicSPeed这个全局变量来帮助判断游戏速度，一开始为0，选择了以后会变成相应的歌的速度
			//	case(6): acPointerx++; setPosition(acPointery, acPointerx); break;	  
				case(11): 
				if(acPointery>1)acPointery=acPointery-1; setPosition(acPointery, acPointerx); break;
				case(10): musicSpeed=acPointery; break;
				case(9): 
				if(acPointery<3)acPointery++; setPosition(acPointery, acPointerx); break;
				case(15):menuflag=0; menu0();acPointerx=0; acPointery=0; selectSong==0;break;
			//	case(14): acPointerx=acPointerx-1; setPosition(acPointery, acPointerx); break;
			}
	  }
}
