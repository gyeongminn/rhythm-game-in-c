#ifndef GAME_UI_H
#define GAME_UI_H

#include "constants.h"

// 외부 버퍼
extern char frontBuffer[SCREEN_HEIGHT][4];
extern char backBuffer[SCREEN_HEIGHT][4];

// 메인 메뉴 아스키아트
extern const char* rhythmGameArt[19];

// UI 관련 함수
void initializeConsole();
char* getLevelName(int level);
void drawBox(int x1, int y1, int x2, int y2);
void removeBox(int x1, int y1, int x2, int y2);
void renderBuffer(void);
void drawMainMenu(void);
void drawMap(int songIndex);
void showRecord();
void drawHelp(void);
void drawScore(unsigned int score, int score_p, int score_g, int score_m);
void printInputState(const char* message);
void printCombo(int combo);
void printScore(unsigned int score);

// 메뉴 선택 함수
int selectMainMenu(void);
void selectSongAndStartGame(void);
void showOptions(void);

#endif
