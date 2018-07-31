#ifndef _MUSICGAME_H_
#define _MUSICGAME_H_

void drawPiano(void);
void showStatus(unsigned char status);
void discore(unsigned char value, unsigned char roll);
void showScore(unsigned char score);
void showNote(unsigned char note, unsigned char row);
void rushNote(unsigned char note);
void musicGameEnd(unsigned char score);
unsigned char checkKey(unsigned char note,unsigned char flag);
void musicGame( unsigned char speed);
void musicGame2(unsigned char speed);

#endif
