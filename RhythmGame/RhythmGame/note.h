#ifndef NOTE_H
#define NOTE_H

#include "queue.h"

typedef struct Notes // 노트 정의
{
	LinkedQueueType queues[4];
} Notes;

void initializeNotes(Notes*);
void releaseNotes(Notes*);
Notes* loadNotes(const char* noteFileName);

#endif
