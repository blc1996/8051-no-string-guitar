#ifndef __GAME_H__
#define __GAME_H__

struct Jimu
{
	unsigned int dat;
 	char x;
	unsigned char y;
	unsigned char type;
	unsigned char change;
};
void InitCpu(void);
void DrawBoard(void);
void GameOver(void);
void GamePlay(void);
#endif