#ifndef _RECORD_H_
#define _RECORD_H_

	#ifndef uchar
	#define uchar unsigned char
	#endif

	#ifndef uint
	#define uint unsigned int
	#endif

//录音输入函数，每次记录一套数据：一个音符、这个音符的延时时间、这是第几个音符。
//即使是没有音符也会被记录
void input(uchar note, uint time, uchar item, bit flag);

//录音输出函数，把之前记录的几套数据输出
void output( bit flag);

#endif