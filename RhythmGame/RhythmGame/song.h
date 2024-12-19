#ifndef SONG_H
#define SONG_H

#include "constants.h"

typedef struct Song
{
	char titleName[32];
	char noteFileName[32];
	int bpm;
	int level;
	char genre[16];
	int endTime;
	int delayTime;
} Song;

static const Song songs[NUM_SONGS] = {
	{"������ - ���� ��", "Assets/GoodDay.txt", 128, NORMAL, "��", 1500, 144},
	{"��Ȱ - �׹��������丮", "Assets/NeverEndingStory.txt", 61, EASY, "�߶��", 1550, 156},
	{"Maroon 5 - Payphone", "Assets/Payphone.txt", 110, HARD, "POP", 2160, 105}
};

void playSong(int songIndex);
DWORD __stdcall playSongThread(LPVOID param);

#endif
