#ifndef _RECORD_H_
#define _RECORD_H_

	#ifndef uchar
	#define uchar unsigned char
	#endif

	#ifndef uint
	#define uint unsigned int
	#endif

//¼�����뺯����ÿ�μ�¼һ�����ݣ�һ�������������������ʱʱ�䡢���ǵڼ���������
//��ʹ��û������Ҳ�ᱻ��¼
void input(uchar note, uint time, uchar item, bit flag);

//¼�������������֮ǰ��¼�ļ����������
void output( bit flag);

#endif