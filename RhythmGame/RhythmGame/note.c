#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "note.h"
#include "constants.h"

void initializeNotes(Notes* notes)
{
	for (int i = 0; i < KEY_SIZE; i++)
	{
		initialize(&notes->queues[i]);
	}
}

void releaseNotes(Notes* notes)
{
	// 혹시나 남아 있는 노트가 있다면, 모두 dequeue하여 메모리 누수를 방지
	for (int i = 0; i < KEY_SIZE; i++)
	{
		while (!isEmpty(&notes->queues[i]))
		{
			dequeue(&notes->queues[i]);
		}

		// 큐를 초기화
		initialize(&notes->queues[i]);
	}
}

// 파일에서 노트 정보 읽어오는 함수
Notes* loadNotes(const char* noteFileName)
{
	FILE* fp;

	// 맵 파일이 없는 경우, 오류를 출력하고 프로그램을 종료한다.
	if ((fp = fopen(noteFileName, "r")) == NULL)
	{
		fprintf(stderr, "맵 파일을 열 수 없습니다.\n");
		exit(1);
	}

	// 동적 할당
	Notes* notes = (Notes*)malloc(sizeof(Notes));
	initializeNotes(notes);

	int ch, step = 0;
	while ((ch = fgetc(fp)) != EOF) // 파일에서 한 문자씩 읽기
	{
		if (ch == '\n')
		{
			step++;
			continue;
		}

		// 파일 값이 1이라면 TRUE, 아니라면 FALSE
		element value = (element)((char)ch == '1');
		enqueue(&notes->queues[step], value);
	}
	fclose(fp);

	return notes; // notes 포인터 반환
}
