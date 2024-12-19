#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <process.h>

#include "game_logic.h"
#include "game_ui.h"
#include "game_input.h"
#include "song.h"
#include "utils.h"
#include "note.h"

char gameMode = NORMAL_GAME_MODE;
char debugMode = NO_DEBUG_MODE;
volatile char isGameRunning = FALSE;

// ���� ���� �Լ� ����
void startGame(int songIndex)
{
	unsigned int score = 0;
	int combo = 0, score_p = 0, score_g = 0, score_m = 0;
	char shouldRemoveNotes[4] = {0};

	// ��Ʈ �о����
	Notes* notes = loadNotes(songs[songIndex].noteFileName);

	// ���� ����
	isGameRunning = TRUE;
	createKeyPressThreads();
	drawMap(songIndex);

	// �뷡 ����, �뷡�� ������ isGameRunning = FALSE;
	HANDLE songThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)playSongThread, &songIndex, 0, NULL);

	// ���� �����̸� ���� ���� ���� �ð� ����
	DWORD gameStartTime = timeGetTime();

	int iteration = 0;
	while (isGameRunning)
	{
		// ���ο� ��Ʈ�� �߰�
		for (int k = 0; k < KEY_SIZE; k++)
		{
			if (!isEmpty(&notes->queues[k]))
			{
				backBuffer[NOTE_START_Y][k] = dequeue(&notes->queues[k]);
			}
			else // ť�� ��������� ��Ʈ�� ���� ������ ����
			{
				backBuffer[NOTE_START_Y][k] = FALSE;
			}
		}

		// ���۸� �� ĭ �Ʒ��� ������ ����
		for (int y = NOTE_START_Y + 1; y <= JUDGE_LINE_Y; y++)
		{
			for (int x = 0; x < KEY_SIZE; x++)
			{
				backBuffer[y][x] = frontBuffer[y - 1][x];
			}
		}

		for (int x = 0; x < KEY_SIZE; x++)
		{
			if (shouldRemoveNotes[x])
			{
				// �̸� ��Ʈ�� �������� �����
				backBuffer[JUDGE_LINE_Y][x] = FALSE;
				frontBuffer[JUDGE_LINE_Y][x] = FALSE;

				shouldRemoveNotes[x] = FALSE;
			}
		}

		renderBuffer();

		// Ű ���� & ���� ���
		int miss = 0;
		for (int k = 0; k < KEY_SIZE; k++)
		{
			if (frontBuffer[JUDGE_LINE_Y][k])
			{
				miss++;
			}

			if (!keyDown[k])
			{
				continue;
			}

			if (!keyState[k])
			{
				keyDown[k] = FALSE;
				continue;
			}

			// ��Ʈ�� ���� �� ���� ���
			if (!frontBuffer[JUDGE_LINE_Y - 1][k] && !frontBuffer[JUDGE_LINE_Y][k])
			{
				keyState[k] = FALSE;
			}
			// �ճ�Ʈ�� ������ ���� ���
			else if (frontBuffer[JUDGE_LINE_Y - 2][k] && frontBuffer[JUDGE_LINE_Y - 1][k] &&
				!frontBuffer[JUDGE_LINE_Y][k])
			{
				combo++;
				miss--;
				score_g++;
				score += (combo / 10 + 1) * 50; // ���� ��� ��
				shouldRemoveNotes[k] = TRUE; // ���� ��Ʈ�� ����
				printInputState("GOOD!     ");
			}
			// �ճ�Ʈ�� ��Ȯ�� ���� ���
			else if (frontBuffer[JUDGE_LINE_Y - 1][k] && frontBuffer[JUDGE_LINE_Y][k])
			{
				combo++;
				miss--;
				score_p++;
				score += (combo / 10 + 1) * 100;
				printInputState("EXCELLENT!");
			}
			// ���� ������ ���� ���
			else if (frontBuffer[JUDGE_LINE_Y - 1][k] && !frontBuffer[JUDGE_LINE_Y][k])
			{
				combo++;
				score_g++;
				score += (combo / 10 + 1) * 50;
				keyState[k] = FALSE;
				shouldRemoveNotes[k] = TRUE; // ���� ��Ʈ�� ����
				printInputState("GOOD!     ");
			}
			// ��Ȯ�� ���� ���
			else if (frontBuffer[JUDGE_LINE_Y][k])
			{
				combo++;
				miss--;
				score_p++;
				score += (combo / 10 + 1) * 100;
				keyState[k] = FALSE;
				printInputState("EXCELLENT!");
			}
		}

		if (miss > 0)
		{
			score_m++;
			combo = 0;
		}

		if (combo == 0)
		{
			printInputState("          ");
		}

		printCombo(combo);
		printScore(score);

		// ���� ������ ���
		DWORD songDelay = (DWORD)songs[songIndex].delayTime;
		DWORD estimatedDuration = songDelay * ++iteration;
		DWORD realCurrentDuration = timeGetTime() - gameStartTime;
		DWORD dynamicDelay = estimatedDuration > realCurrentDuration ? estimatedDuration - realCurrentDuration : 0;

		// �����̰� �ʹ� ��ų� ���� �ʵ��� ���� (�뷡 �������� ��50%)
		dynamicDelay = MIN(dynamicDelay, songDelay * 2);
		dynamicDelay = MAX(dynamicDelay, songDelay / 2);

		if (debugMode == USE_DEBUG_MODE)
		{
			moveCursor(78, 7);
			printf("iteration : %d", iteration);
			moveCursor(78, 8);
			printf("DELAY : %lu / %d", dynamicDelay, songs[songIndex].delayTime);
			moveCursor(78, 9);
			printf("realTime : %lu", realCurrentDuration);
			moveCursor(78, 10);
			printf("estTime : %lu", estimatedDuration);
		}

		Sleep(dynamicDelay);
	}
	
	// �޸� ���� �� ������ ����
	releaseNotes(notes);
	CloseHandle(songThread);

	// ���� ���
	changeTextColor(WHITE, BLACK);
	clearWindow();
	drawScore(score, score_p, score_g, score_m);

	// ��� ���� �� TOP 10 ��� ǥ��
    showRecord();
	clearWindow();
}

// ��Ʈ ���� ��� ����
void writeNoteMode(int songIndex)
{
	FILE* fp;
	if ((fp = fopen("Assets/note.txt", "w")) == NULL)
	{
		printf("��Ʈ ������ �� �� �����ϴ�.\n");
		return;
	}

	// ���� �޸� �Ҵ�: buffer[4][3000]
	const int eachBufferSize = 3000;
	char** buffer = (char**)malloc(4 * sizeof(char*));
	for (int k = 0; k < 4; k++)
	{
		buffer[k] = (char*)malloc(eachBufferSize * sizeof(char));
	}

	// ���� ����
	isGameRunning = TRUE;
	createKeyPressThreads();
	drawMap(songIndex);

	// �뷡 ����, �뷡�� ������ isGameRunning = FALSE;
	HANDLE songThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)playSongThread, &songIndex, 0, NULL);

	// ���� �����̸� ���� ���� ���� �ð� ����
	DWORD gameStartTime = timeGetTime();

	int iteration = 0;
	while (isGameRunning)
	{
		renderBuffer();

		// Ű ���� & ��Ʈ ���
		for (int k = 0; k < 4; k++)
		{
			buffer[k][iteration] = keyDown[k];
		}

		// Ű ���� ����
		for (int k = 0; k < 4; k++)
		{
			keyDown[k] = FALSE;
		}

		// ���� ������ ���
		DWORD songDelay = (DWORD)songs[songIndex].delayTime;
		DWORD estimatedDuration = songDelay * ++iteration;
		DWORD realCurrentDuration = timeGetTime() - gameStartTime;
		DWORD dynamicDelay = estimatedDuration > realCurrentDuration ? estimatedDuration - realCurrentDuration : 0;

		// �����̰� �ʹ� ��ų� ���� �ʵ��� ���� (�뷡 �������� ��50%)
		dynamicDelay = MIN(dynamicDelay, songDelay * 2);
		dynamicDelay = MAX(dynamicDelay, songDelay / 2);

		if (debugMode == USE_DEBUG_MODE)
		{
			moveCursor(78, 7);
			printf("iteration : %d", iteration);
			moveCursor(78, 8);
			printf("DELAY : %lu / %d", dynamicDelay, songs[songIndex].delayTime);
			moveCursor(78, 9);
			printf("realTime : %lu", realCurrentDuration);
			moveCursor(78, 10);
			printf("estTime : %lu", estimatedDuration);
		}

		Sleep(dynamicDelay);
	}

	// ��Ʈ ���� ����
	for (int k = 0; k < 4; k++)
	{
		fputs(buffer[k], fp);
		fputs("\n", fp);
	}
	fclose(fp);

	// ����
	changeTextColor(WHITE, BLACK);
	clearWindow();
	moveCursor(35, 10);
	printf("��Ʈ�� ����Ǿ����ϴ�.");
	Sleep(4000);
	CloseHandle(songThread);

	// �޸� ����
	for (int k = 0; k < 4; k++)
	{
		free(buffer[k]);
	}
	free(buffer);
}

// Ű���� �Է� ������ ���� �Լ�
void createKeyPressThreads()
{
	unsigned int threadIDs[4];
	int indices[4] = {0, 1, 2, 3};
	for (int t = 0; t < 4; t++)
	{
		_beginthreadex(NULL, 0, keyPressHandler, &indices[t], 0, &threadIDs[t]);
		Sleep(10);
	}
}
