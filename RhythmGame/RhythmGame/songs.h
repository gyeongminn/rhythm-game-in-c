#ifndef SONGS_H
#define SONGS_H

#include "constants.h"

typedef struct
{
	char title[25];
	char file[23];
	int bpm;
	char level[10];
	char genre[10];
	int endTime;
	int delayTime;
} Song;

// 노래 정보 초기화
static const Song songs[NUM_SONGS] = {
	{"아이유 - 좋은 날", "Assets/GoodDay.txt", 128, "NORMAL", "댄스", 1500, 144},
	{"부활 - 네버엔딩스토리", "Assets/NeverEndingStory.txt", 61, "EASY", "발라드", 1550, 156},
	{"Maroon 5 - Payphone", "Assets/Payphone.txt", 110, "HARD", "POP", 2160, 105}
};

#endif
