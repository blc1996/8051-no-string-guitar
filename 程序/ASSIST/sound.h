#ifndef _SOUND_H_
#define _SOUND_H_

	#ifndef uchar
	#define uchar unsigned char
	#endif

	#ifndef uint
	#define uint unsigned int
	#endif

//蜂鸣器发声函数，当输入相应的1 2 3 4 5 6 7
//发出声音非常短的时间周期（一个时间周期人耳无法识别），需要用循环来延长发生时间
extern void Sound(uchar i);

#endif