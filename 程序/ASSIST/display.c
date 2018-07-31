#include "display.h"
#include "function.h"
#include "record.h"
#include "sound.h"




uchar menuflag=0;
uchar musicSpeed=0;
uchar acPointerx=0;
uchar acPointery=0;
uchar selectSong=0;

uchar mychange=1;	  //������ȫ�ֱ�����������������

extern uchar ritem;
extern bit recordFlag;

sbit blc = P3^2;  //Ϊ�˷����ȡ�Ƿ��а�������
sbit H = P4^5;	  //��Ӧ���������İ���
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
����LCD�Ƿ���æ״̬
���æ�򷵻�0x80�����򷵻�0
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
��LCDд���ֽ�����
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
��LCD�ж�������
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
��LCD��д��ָ�����
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
��LCDָ����ʼλ��д��һ���ַ���
*************************************/
void Lcd_WriteStr(unsigned char y,unsigned char x,unsigned char *Str)
{
	if((y>3)||(x>7))
		return;//���ָ��λ�ò�����ʾ�����ڣ������κ�д��ֱ�ӷ���
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
Ϊ�����߼���������õ�����������������ռ����ȡʱ��İ취
***************************************/
code unsigned int LcdMaskTab[]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
							 0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
/***************************************
��LCDָ������д��һ������,������ɫ�����֣�0����ף�����ʾ����1����ڣ�����ʾ��
****************************************/
void Lcd_PutPixel(unsigned char x,unsigned char y,unsigned char Color)
{
	unsigned char z,w;
	unsigned int Temp;
	if(x>=128||y>=64)
		return;
	Color=Color%2;
	w=15-x%16;//ȷ��������ֵĵڶ���λ���в���
	x=x/16;//ȷ��Ϊһ���ϵĵڼ���

	if(y<32) //���Ϊ��ҳ
		z=0x80;
	else     //�������Ϊ��ҳ
		z=0x88;

	y=y%32;
	EA=0;
	Lcd_WriteCmd(0x36);
	Lcd_WriteCmd(y+0x80);        //�е�ַ
	Lcd_WriteCmd(x+z);     //�е�ַ 
	Temp=Lcd_ReadData();//�ȿն�һ��
	Temp=(unsigned int)Lcd_ReadData()<<8;//�ٶ�����8λ
	Temp|=(unsigned int)Lcd_ReadData();//�ٶ�����8λ
	EA=1;
	if(Color==1) //���д����ɫΪ1
		Temp|=LcdMaskTab[w];//�ڴ˴����ʵ�ּ���
	else         //���д����ɫΪ0
		Temp&=~LcdMaskTab[w];//�ڴ˴����ʵ�ּ���
	EA=0;
	Lcd_WriteCmd(y+0x80);        //�е�ַ
	Lcd_WriteCmd(x+z);     //�е�ַ
    Lcd_WriteData(Temp>>8);//��д���8λ����д���8λ
    Lcd_WriteData(Temp&0x00ff);
	Lcd_WriteCmd(0x30);
	EA=1;
}

/******************************************
��LCDָ�������ȡ������ɫֵ
*******************************************/
unsigned char Lcd_ReadPixel(unsigned char x,unsigned char y)
{
	unsigned char z,w;
	unsigned int Temp;
	if(x>=128||y>=64)
		return 0;
	w=15-x%16;//ȷ��������ֵĵڶ���λ���в���
	x=x/16;//ȷ��Ϊһ���ϵĵڼ���

	if(y<32) //���Ϊ��ҳ
		z=0x80;
	else     //�������Ϊ��ҳ
		z=0x88;

	y=y%32;
	EA=0;
	Lcd_WriteCmd(0x36);
	Lcd_WriteCmd(y+0x80);        //�е�ַ
	Lcd_WriteCmd(x+z);     //�е�ַ 
	Temp=Lcd_ReadData();//�ȿն�һ��
	Temp=(unsigned int)Lcd_ReadData()<<8;//�ٶ�����8λ
	Temp|=(unsigned int)Lcd_ReadData();//�ٶ�����8λ
	EA=1;
	if((Temp&&LcdMaskTab[w])==0)
		return 0;
	else
		return 1;
}

/***************************************
��LCDָ��λ�û�һ������ΪLength��ָ����ɫ��ˮƽ��
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
��LCDָ��λ�û�һ������ΪLength��ָ����ɫ�Ĵ�ֱ��
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
��LCDָ����ʼ����ͽ�������֮�仭һ��ָ����ɫ��ֱ��
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
��LCDָ�����Ͻ���������½����껭һ��ָ����ɫ�ľ���
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
�ԳƷ���Բ��8�������
*****************************************/
void CircleDot(unsigned char x,unsigned char y,char xx,char yy,unsigned char Color)//�ڲ��������ԳƷ���Բ��8�������
{
	Lcd_PutPixel((x+yy),(y+xx),Color);//�� 1 �� 8 ��Բ
	Lcd_PutPixel((x+xx),(y+yy),Color);//�� 2 �� 8 ��Բ
	Lcd_PutPixel((x-xx),(y+yy),Color);//�� 3 �� 8 ��Բ
	Lcd_PutPixel((x-yy),(y+xx),Color);//�� 4 �� 8 ��Բ
	Lcd_PutPixel((x-yy),(y-xx),Color);//�� 5 �� 8 ��Բ
	Lcd_PutPixel((x-xx),(y-yy),Color);//�� 6 �� 8 ��Բ
	Lcd_PutPixel((x+xx),(y-yy),Color);//�� 7 �� 8 ��Բ
	Lcd_PutPixel((x+yy),(y-xx),Color);//�� 8 �� 8 ��Բ
}

/******************************************
��LCDָ��Բ�����껭һ���뾶Ϊr��ָ����ɫ��Բ
*******************************************/
void Lcd_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char Color)//�е㷨��Բ
{//�е㷨��Բ
	unsigned char xx,yy;
	char deltax,deltay,d;
	xx=0;
	yy=r;
	deltax=3;
	deltay=2-r-r;
	d=1-r;
	CircleDot(x,y,xx,yy,Color);//�ԳƷ���Բ��8�������
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
		CircleDot(x,y,xx,yy,Color);//�ԳƷ���Բ��8�������
	}
}

/*****************************************
���Lcdȫ����������ģʽModeΪ0����Ϊȫ�����Ϊ��ɫ0�����κ���ʾ��
����Ϊȫ�����Ϊ��ɫ1(ȫ�������ʾ)
******************************************/
void Lcd_Clear(unsigned char Mode)
{
	unsigned char x,y,ii;
	unsigned char Temp;
	if(Mode%2==0)
		Temp=0x00;
	else
		Temp=0xff;
	Lcd_WriteCmd(0x36);//����ָ�� ��ͼ��ʾ
	for(ii=0;ii<9;ii+=8)   
		for(y=0;y<0x20;y++)     
			for(x=0;x<8;x++)
			{ 	
				EA=0;
				Lcd_WriteCmd(y+0x80);        //�е�ַ
				Lcd_WriteCmd(x+0x80+ii);     //�е�ַ     
				Lcd_WriteData(Temp); //д���� D15��D8 
				Lcd_WriteData(Temp); //д���� D7��D0 
				EA=1;
			}
	Lcd_WriteCmd(0x30);
}

/****************************************
LCD��ʼ��
*****************************************/
void Lcd_Reset()
{  
	PSB=1;
	Lcd_WriteCmd(0x30);       //ѡ�����ָ�
	Lcd_WriteCmd(0x0c);       //����ʾ(���αꡢ������)
	Lcd_WriteCmd(0x01);       //�����ʾ�������趨��ַָ��Ϊ00H
	Lcd_WriteCmd(0x06);       //ָ�������ϵĶ�ȡ��д��ʱ���趨�α���ƶ�����ָ����ʾ����λ
}

/****************************************
//������ʾ��GDRAMλ��
*****************************************/
void Lcd_Cursor(uchar a, uchar b)
{
	Lcd_WriteCmd(0x34);			  //�ر�ͼ����ʾ	
	Lcd_WriteCmd(b);			  //��д��ֱ��ַ
	Lcd_WriteCmd(a);			  //��дˮƽ��ַ
}

/****************************************
//��ʾͼƬ����
*****************************************/
void show_Pic(uchar x, uchar y, uchar clong, uchar hight, uchar *Icon)
{                                        
	uchar i, j;
	for(i=0; i<hight; i++)		 //��ģ����һ��д��
	{
		if(y+i<32)				  //�ж�������
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
	    Lcd_WriteStr(2,3,"��");
	else if(key==2)
	    Lcd_WriteStr(2,3,"��");
	else if(key==0)
		Lcd_WriteStr(2,3,"��");
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
		Lcd_WriteStr(7,3,0x0d0e);//���ﳢ�Լ�����
	else
		Lcd_WriteStr(7,3,0x0e0d);
	
	}	   
	//	setPosition(0,0);										   
}


void menu9()
{
	Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"��ѡ����Ϸ�Ѷ�");
	Lcd_WriteStr(1,0,"* ����");
	Lcd_WriteStr(2,0,"* �е�");
	Lcd_WriteStr(3,0,"* ����");
	setPosition(1,0);
	acPointery=1;
}

void menu6()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"* ¼��һ");
	Lcd_WriteStr(1,0,"* ¼����");
	setPosition(0,0);
	acPointery=0;
}


//������̲˵�
void menu5()
{
    Lcd_WriteCmd(0x01);  
    Lcd_WriteStr(0,0,"���ڿ�ʼ�����");
	Lcd_WriteStr(1,0,"�����Ͻǰ����˳�");
	setPosition(3,7);
	Lcd_WriteData(0x0E);
	setPosition(3,7);
}

//¼���˵�
void menu4()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"�����ⵯ��ɣ�");
	Lcd_WriteStr(1,0,"�����Ͻǰ������");
	setPosition(3,7);
	Lcd_WriteData(0x0E);
	setPosition(3,7);
}

//ѡ�����ø����˵�
void menu3()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"* ������");
	Lcd_WriteStr(1,0,"* ���֮��");
	setPosition(0,0);
	acPointerx=0;
	acPointery=0;
}

//�������ڲ���
void menu31()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"����������");
    Lcd_WriteStr(1,0,"���Ժ��������");
	setPosition(3,7);
	Lcd_WriteData(0x0E);
	setPosition(3,7);
}

//����˵�
void menu2()
{
    Lcd_WriteCmd(0x01);  
	Lcd_WriteStr(0,0,"���ڿ�ʼ�����");
	Lcd_WriteStr(1,0,"�����Ͻǰ����˳�");
	Lcd_WriteStr(2,0,"���ף�  ");
	setPosition(3,7);
	Lcd_WriteData(0x0E);
//	writeString(data23);
//	setPosition(4,0);
}

//�Ӳ˵�ѡ�񣬲�����Ĳ˵�;ͨ���ж�menuflag������������һ��˵�
void menu1()
{
	 menuflag++; //��ʼ��0��Ҫ++
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

//���˵�
void menu0()
{
    if(acPointery<4)
	{
    Lcd_WriteCmd(0x01);
    Lcd_WriteStr(0,0,"* �Լ���������");
	Lcd_WriteStr(1,0,"* �������и���");
	Lcd_WriteStr(2,0,"* ����Ҫ¼����");
	Lcd_WriteStr(3,0,"* ������̵���");
	Lcd_WriteCmd(0x80);
	}else
	{
	Lcd_WriteCmd(0x01);
    Lcd_WriteStr(0,0,"* ��������¼��");
	Lcd_WriteStr(1,0,"* ����˹����");
	Lcd_WriteStr(2,0,"* ѧϰ����");
	Lcd_WriteStr(3,0,"* �����ʦ��");
	}
}

//�����Ժ�Բ˵��Ķ�������
void menuKey(uchar key)
{	  
       if(menuflag==0)	 //ͨ���ۺ��ж�menuflag�͵�ǰ���µļ���ȷ����һ���Ķ���
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
				if(acPointery>0)				 //��ֹ�˵�Խ���Ĵ���
				acPointery=acPointery-1; setPosition(acPointery, acPointerx); break;	  //acpointer��ָʾ��ǰ���λ�õ�ȫ�ֱ���
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
	      	 if(key==15)		 //�˳������˵�
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
			{			//����ʹ����selectsong���ȫ�ֱ����������ж�Ҫ�������׸裬һ��ʼΪ0��ѡ�����Ժ������Ӧ�ĸ�ı��
			//	case(6): acPointerx++; setPosition(acPointery, acPointerx); break;	  
				case(11): 
				if(acPointery>0)acPointery=acPointery-1; setPosition(acPointery, acPointerx); break;
				case(10): selectSong++; selectSong+=acPointery; menu31(); break;
				case(9): 
				if(acPointery<1)acPointery++; setPosition(acPointery, acPointerx); break;
				case(15):menuflag=0; menu0();acPointerx=0; acPointery=0; selectSong==0;break;
			//	case(14): acPointerx=acPointerx-1; setPosition(acPointery, acPointerx); break;
			} 
	  }	else if(menuflag==4)			//¼��
	  {
	      if(key==15)
			 {
			     menuflag=0;
				 menu0();
				 acPointerx=0;
	             acPointery=0;
				 recordFlag=~recordFlag;		 //¼һ�ף���־λ��ת
				 ritem=0;
			 }
	  } else if (menuflag==5)	//������̵���
	  {
	      switch(key)			//�ж���ֱ�������ֵ������
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
	  }else if(menuflag==6)			 //¼������
	  {
	      	switch(key)
			{
			    case(15): menuflag=0; acPointerx=0; acPointery=0; menu0(); break;
				case(11): acPointery=0; setPosition(acPointery, acPointerx); break;
				case(10): menu31();output(acPointery);menu6(); break;
				case(9): acPointery=1; setPosition(acPointery, acPointerx); break;
			}

	  }else if(menuflag==7)		  //����˹����
	  {
			switch(key)
			{
			    case(15): menuflag=0; acPointerx=0; acPointery=0; break;
			}	 
	  }else if(menuflag==8)			//ѧϰ����
	  {
	        if(key==15)
			{
			    menuflag=0; acPointerx=0; acPointery=0; 
			}
	  }else if(menuflag==9)		  //�����ʦ
	  {
			switch(key)
			{			//����ʹ����musicSPeed���ȫ�ֱ����������ж���Ϸ�ٶȣ�һ��ʼΪ0��ѡ�����Ժ������Ӧ�ĸ���ٶ�
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
