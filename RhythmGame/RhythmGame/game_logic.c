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

// 게임 시작 함수 구현
void startGame(int songIndex)
{
	unsigned int score = 0;
	int combo = 0, score_p = 0, score_g = 0, score_m = 0;
	char shouldRemoveNotes[4] = {0};

	// 노트 읽어오기
	Notes* notes = loadNotes(songs[songIndex].noteFileName);

	// 게임 시작
	isGameRunning = TRUE;
	createKeyPressThreads();
	drawMap(songIndex);

	// 노래 시작, 노래가 끝나면 isGameRunning = FALSE;
	HANDLE songThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)playSongThread, &songIndex, 0, NULL);

	// 동적 딜레이를 위한 게임 시작 시간 측정
	DWORD gameStartTime = timeGetTime();

	int iteration = 0;
	while (isGameRunning)
	{
		// 새로운 노트를 추가
		for (int k = 0; k < KEY_SIZE; k++)
		{
			if (!isEmpty(&notes->queues[k]))
			{
				backBuffer[NOTE_START_Y][k] = dequeue(&notes->queues[k]);
			}
			else // 큐가 비어있으면 노트가 없는 것으로 간주
			{
				backBuffer[NOTE_START_Y][k] = FALSE;
			}
		}

		// 버퍼를 한 칸 아래로 내리며 복사
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
				// 미리 노트를 눌렀으면 지우기
				backBuffer[JUDGE_LINE_Y][x] = FALSE;
				frontBuffer[JUDGE_LINE_Y][x] = FALSE;

				shouldRemoveNotes[x] = FALSE;
			}
		}

		renderBuffer();

		// 키 판정 & 점수 계산
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

			// 노트가 없을 때 누른 경우
			if (!frontBuffer[JUDGE_LINE_Y - 1][k] && !frontBuffer[JUDGE_LINE_Y][k])
			{
				keyState[k] = FALSE;
			}
			// 롱노트를 빠르게 누른 경우
			else if (frontBuffer[JUDGE_LINE_Y - 2][k] && frontBuffer[JUDGE_LINE_Y - 1][k] &&
				!frontBuffer[JUDGE_LINE_Y][k])
			{
				combo++;
				miss--;
				score_g++;
				score += (combo / 10 + 1) * 50; // 점수 계산 식
				shouldRemoveNotes[k] = TRUE; // 지울 노트를 저장
				printInputState("GOOD!     ");
			}
			// 롱노트를 정확히 누른 경우
			else if (frontBuffer[JUDGE_LINE_Y - 1][k] && frontBuffer[JUDGE_LINE_Y][k])
			{
				combo++;
				miss--;
				score_p++;
				score += (combo / 10 + 1) * 100;
				printInputState("EXCELLENT!");
			}
			// 조금 빠르게 누른 경우
			else if (frontBuffer[JUDGE_LINE_Y - 1][k] && !frontBuffer[JUDGE_LINE_Y][k])
			{
				combo++;
				score_g++;
				score += (combo / 10 + 1) * 50;
				keyState[k] = FALSE;
				shouldRemoveNotes[k] = TRUE; // 지울 노트를 저장
				printInputState("GOOD!     ");
			}
			// 정확히 누른 경우
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

		// 동적 딜레이 계산
		DWORD songDelay = (DWORD)songs[songIndex].delayTime;
		DWORD estimatedDuration = songDelay * ++iteration;
		DWORD realCurrentDuration = timeGetTime() - gameStartTime;
		DWORD dynamicDelay = estimatedDuration > realCurrentDuration ? estimatedDuration - realCurrentDuration : 0;

		// 딜레이가 너무 길거나 작지 않도록 조정 (노래 딜레이의 ±50%)
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
	
	// 메모리 해제 및 쓰레드 종료
	releaseNotes(notes);
	CloseHandle(songThread);

	// 점수 출력
	changeTextColor(WHITE, BLACK);
	clearWindow();
	drawScore(score, score_p, score_g, score_m);

	// 기록 저장 및 TOP 10 기록 표시
    showRecord();
	clearWindow();
}

// 노트 쓰기 모드 구현
void writeNoteMode(int songIndex)
{
	FILE* fp;
	if ((fp = fopen("Assets/note.txt", "w")) == NULL)
	{
		printf("노트 파일을 열 수 없습니다.\n");
		return;
	}

	// 동적 메모리 할당: buffer[4][3000]
	const int eachBufferSize = 3000;
	char** buffer = (char**)malloc(4 * sizeof(char*));
	for (int k = 0; k < 4; k++)
	{
		buffer[k] = (char*)malloc(eachBufferSize * sizeof(char));
	}

	// 게임 시작
	isGameRunning = TRUE;
	createKeyPressThreads();
	drawMap(songIndex);

	// 노래 시작, 노래가 끝나면 isGameRunning = FALSE;
	HANDLE songThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)playSongThread, &songIndex, 0, NULL);

	// 동적 딜레이를 위한 게임 시작 시간 측정
	DWORD gameStartTime = timeGetTime();

	int iteration = 0;
	while (isGameRunning)
	{
		renderBuffer();

		// 키 판정 & 노트 기록
		for (int k = 0; k < 4; k++)
		{
			buffer[k][iteration] = keyDown[k];
		}

		// 키 상태 리셋
		for (int k = 0; k < 4; k++)
		{
			keyDown[k] = FALSE;
		}

		// 동적 딜레이 계산
		DWORD songDelay = (DWORD)songs[songIndex].delayTime;
		DWORD estimatedDuration = songDelay * ++iteration;
		DWORD realCurrentDuration = timeGetTime() - gameStartTime;
		DWORD dynamicDelay = estimatedDuration > realCurrentDuration ? estimatedDuration - realCurrentDuration : 0;

		// 딜레이가 너무 길거나 작지 않도록 조정 (노래 딜레이의 ±50%)
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

	// 노트 파일 저장
	for (int k = 0; k < 4; k++)
	{
		fputs(buffer[k], fp);
		fputs("\n", fp);
	}
	fclose(fp);

	// 종료
	changeTextColor(WHITE, BLACK);
	clearWindow();
	moveCursor(35, 10);
	printf("노트가 저장되었습니다.");
	Sleep(4000);
	CloseHandle(songThread);

	// 메모리 해제
	for (int k = 0; k < 4; k++)
	{
		free(buffer[k]);
	}
	free(buffer);
}

// 키보드 입력 쓰레드 생성 함수
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
