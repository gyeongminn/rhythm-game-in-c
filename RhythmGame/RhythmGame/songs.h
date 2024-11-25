#ifndef SONGS_H
#define SONGS_H

#include "constants.h"

typedef struct
{
	char titleName[25];
	char noteFileName[23];
	int bpm;
	int level;
	char genre[10];
	int endTime;
	int delayTime;
} Song;

static const Song songs[NUM_SONGS] = {
	{"������ - ���� ��", "Assets/GoodDay.txt", 128, NORMAL, "��", 1500, 144},
	{"��Ȱ - �׹��������丮", "Assets/NeverEndingStory.txt", 61, EASY, "�߶��", 1550, 156},
	{"Maroon 5 - Payphone", "Assets/Payphone.txt", 110, HARD, "POP", 2160, 105}
};

#endif
