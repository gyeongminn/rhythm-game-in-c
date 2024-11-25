#define _CRT_SECURE_NO_WARNINGS

#include "game_logic.h"
#include "game_ui.h"
#include "game_input.h"
#include "songs.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#include <process.h>

#pragma comment(lib, "winmm.lib")

char gameMode = NORMAL_GAME_MODE;
char debugMode = NO_DEBUG_MODE;
volatile char isGameRunning = FALSE;

// 게임 시작 함수 구현
void startGame(int songIndex)
{
	unsigned int score = 0;
	int i = 0, j, combo = 0, missingCount = 0;
	int score_p = 0, score_g = 0, score_m = 0;
	char shouldRemoveNotes[4] = {0};

	// 동적 메모리 할당: buffer[4][2200]
	const int eachBufferSize = 2200;
	char** buffer = (char**)malloc(4 * sizeof(char*));
	for (int k = 0; k < 4; k++)
	{
		buffer[k] = (char*)malloc(eachBufferSize * sizeof(char));
	}

	// 노트 파일 읽어오기
	FILE* fp;
	if ((fp = fopen(songs[songIndex].noteFileName, "r")) == NULL)
	{
		printf("맵파일을 열 수 없습니다.\n");
		return;
	}
	for (int k = 0; k < 4; k++)
	{
		fgets(buffer[k], eachBufferSize, fp);
	}
	fclose(fp);

	// 게임 시작
	isGameRunning = TRUE;
	createKeyPressThreads();
	drawMap(songIndex);
	playSong(songIndex);

	clock_t now = clock();
	while (1)
	{
		if (missingCount > 0)
		{
			missingCount++;
			if (missingCount > 5)
			{
				missingCount = 0;
				moveCursor(78, 5);
				printf("          ");
			}
		}

		// 노트 그리기 (더블 버퍼링)
		for (j = i; j >= 0; j--)
		{
			if (j >= JUDGE_LINE_Y)
			{
				j = JUDGE_LINE_Y;
			}
			for (int k = 0; k < 4; k++)
			{
				backBuffer[j][k] = buffer[k][i - j] == '1';
			}
		}

		for (j = 0; j < 4; j++)
		{
			if (shouldRemoveNotes[j])
			{
				// 미리 노트를 눌렀으면 지우기
				backBuffer[JUDGE_LINE_Y][j] = FALSE;
				frontBuffer[JUDGE_LINE_Y][j] = FALSE;

				shouldRemoveNotes[j] = FALSE;
			}
		}

		renderBuffer();

		int miss = 0;
		changeTextColor(WHITE, BLACK);

		// 키 판정 & 점수 계산
		for (int k = 0; k < 4; k++)
		{
			if (frontBuffer[JUDGE_LINE_Y][k]) miss++;
			if (keyDown[k])
			{
				if (keyState[k])
				{
					// 노트가 없을 때 누른 경우
					if (!frontBuffer[35][k] && !frontBuffer[JUDGE_LINE_Y][k])
					{
						keyState[k] = FALSE;
					}
					// 롱노트를 빠르게 누른 경우
					else if (frontBuffer[34][k] && frontBuffer[35][k])
					{
						combo++;
						miss--;
						score_g++;
						score += (combo / 10 + 1) * 50; // 점수계산식
						shouldRemoveNotes[k] = TRUE; // 지울 노트를 저장
						missingCount = 1;
						moveCursor(78, 5);
						printf("GOOD!     ");
					}
					// 롱노트를 누른 경우
					else if (frontBuffer[35][k] && frontBuffer[JUDGE_LINE_Y][k])
					{
						combo++;
						miss--;
						score_p++;
						score += (combo / 10 + 1) * 100;
						missingCount = 1;
						moveCursor(78, 5);
						printf("EXCELLENT!");
					}
					// 조금 빠르게 누른 경우
					else if (frontBuffer[35][k] && !frontBuffer[JUDGE_LINE_Y][k])
					{
						combo++;
						score_g++;
						score += (combo / 10 + 1) * 50;
						keyState[k] = FALSE;
						shouldRemoveNotes[k] = TRUE; // 지울 노트를 저장
						missingCount = 1;
						moveCursor(78, 5);
						printf("GOOD!     ");
					}
					// 정확히 누른 경우
					else if (frontBuffer[JUDGE_LINE_Y][k])
					{
						combo++;
						miss--;
						score_p++;
						score += (combo / 10 + 1) * 100;
						keyState[k] = FALSE;
						missingCount = 1;
						moveCursor(78, 5);
						printf("EXCELLENT!");
					}
				}
				keyDown[k] = FALSE;
			}
		}

		if (miss > 0)
		{
			score_m++;
			combo = 0;
		}

		if (combo)
		{
			moveCursor(78, 3);
			printf("COMBO! x %d            ", combo);
		}
		else
		{
			moveCursor(78, 3);
			printf("                    "); // 지우기
		}

		moveCursor(78, 1);
		printf("SCORE : %d            ", score);

		if (i++ > songs[songIndex].endTime)
		{
			break;
		}

		// 딜레이가 항상 일정하도록 설정
		clock_t now2 = clock();
		DWORD real_delay = songs[songIndex].delayTime - (now2 - now);
		if (debugMode == USE_DEBUG_MODE)
		{
			moveCursor(78, 5);
			printf("DELAY : %d / %d", (int)real_delay, songs[songIndex].delayTime);
		}
		Sleep(real_delay);
		now = clock();
	}

	// 종료
	isGameRunning = FALSE;
	stopSong();
	changeTextColor(WHITE, BLACK);
	clearWindow();
	drawScore(score, score_p, score_g, score_m);
	clearWindow();

	// 메모리 해제
	for (int k = 0; k < 4; k++)
	{
		free(buffer[k]);
	}
	free(buffer);
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

	// 게임 시작
	isGameRunning = TRUE;
	createKeyPressThreads();
	drawMap(songIndex);
	playSong(songIndex);

	// 동적 메모리 할당: buffer[4][2200]
	const int eachBufferSize = 2200;
	char** buffer = (char**)malloc(4 * sizeof(char*));
	for (int k = 0; k < 4; k++)
	{
		buffer[k] = (char*)malloc(eachBufferSize * sizeof(char));
	}

	int i = 0;
	while (1)
	{
		clock_t now = clock();
		renderBuffer();

		// 키 판정 & 노트 기록
		for (int k = 0; k < 4; k++)
		{
			buffer[k][i] = keyDown[k];
		}

		// 키 상태 리셋
		for (int k = 0; k < 4; k++)
		{
			keyDown[k] = FALSE;
		}

		if (i++ > songs[songIndex].endTime)
		{
			break;
		}

		// 딜레이가 항상 일정하도록 설정
		clock_t now2 = clock();
		DWORD real_delay = songs[songIndex].delayTime - (now2 - now);
		if (debugMode == USE_DEBUG_MODE)
		{
			changeTextColor(WHITE, BLACK);
			moveCursor(78, 5);
			printf("DELAY : %d / %d", (int)real_delay, songs[songIndex].delayTime);
		}
		Sleep(real_delay);
	}

	// 노트 파일 저장
	for (int k = 0; k < 4; k++)
	{
		fputs(buffer[k], fp);
		fputs("\n", fp);
	}
	fclose(fp);

	// 종료
	isGameRunning = FALSE;
	stopSong();
	changeTextColor(WHITE, BLACK);
	clearWindow();
	moveCursor(35, 10);
	printf("노트가 저장되었습니다.");
	Sleep(4000);

	// 메모리 해제
	for (int k = 0; k < 4; k++)
	{
		free(buffer[k]);
	}
	free(buffer);
}

void playSong(int songIndex)
{
	switch (songIndex)
	{
	case 0:
		PlaySound(TEXT("Assets/GoodDay.wav"), NULL, SND_ASYNC);
		break;
	case 1:
		PlaySound(TEXT("Assets/NeverEndingStory.wav"), NULL, SND_ASYNC);
		break;
	case 2:
		PlaySound(TEXT("Assets/Payphone.wav"), NULL, SND_ASYNC);
		break;
	default:
		break;
	}
}

void stopSong()
{
	PlaySound(NULL, 0, 0);
}

// 키보드 입력 쓰레드 생성 함수
void createKeyPressThreads()
{
    int indices[4];
    for (int t = 0; t < 4; t++)
    {
        indices[t] = t;
        if (_beginthreadex(NULL, 0, keyPressHandler, &indices[t], 0, NULL) == 0)
        {
            perror("Failed to create thread");
        }
        Sleep(10); // 스레드 생성 안정성을 위해 딜레이
    }
}