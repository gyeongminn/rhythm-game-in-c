#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define NORMAL_GAME_MODE 0
#define WRITE_NOTE_MODE 1
extern int gameMode;

#define NO_DEBUG_MODE 0
#define USE_DEBUG_MODE 1
extern int debugMode;

void startGame(int songIndex);
void writeNoteMode(int songIndex);
void playSong(int songIndex);

#endif
