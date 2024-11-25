#ifndef GAME_UI_H
#define GAME_UI_H

#include "constants.h"

// �ܺ� ����
extern char frontBuffer[SCR_HEIGHT][4];
extern char backBuffer[SCR_HEIGHT][4];

// ���� �޴� �ƽ�Ű��Ʈ
extern const char* rhythmGameArt[19];

// UI ���� �Լ�
void initializeConsole();
char* getLevelName(int level);
void drawBox(int x1, int y1, int x2, int y2);
void removeBox(int x1, int y1, int x2, int y2);
void renderBuffer(void);
void drawMainMenu(void);
void drawMap(int songIndex);
void drawHelp(void);
void drawScore(int score, int score_p, int score_g, int score_m);

// �޴� ���� �Լ�
int selectMainMenu(void);
void selectSong(void);
void showOptions(void);

#endif
