#ifndef _PIANO_H_
#define _PIANO_H_

	#ifndef uchar
	#define uchar unsigned char
	#endif

//音符函数，返回值是按下的音符
//对应7个音符的 1 2 3 4 5 6 7
//对应不同的高低音位还可以有对应的高低音 1 2 3 4 5 6 7
uchar Piano(void);


#endif