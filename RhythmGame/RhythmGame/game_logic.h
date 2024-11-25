#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define NORMAL_GAME_MODE 0
#define WRITE_NOTE_MODE 1
extern int gameMode;

#define NO_DEBUG_MODE 0
#define USE_DEBUG_MODE 1
extern int debugMode;

void drawScore(int score, int score_p, int score_g, int score_m);
void startGame(int songIndex);
void writeNoteMode(int songIndex);

#endif
