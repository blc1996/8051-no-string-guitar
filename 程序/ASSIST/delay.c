#include "delay.h"			

//延时函数，延时约为 n us

void delay(uchar n)
{
	while(--n);
}