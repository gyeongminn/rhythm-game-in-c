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

// ���� ��� �� ����� ��� ���� ����
int gameMode = NORMAL_GAME_MODE;
int debugMode = NO_DEBUG_MODE;

// ���� ���� �Լ� ����
void startGame(int songIndex)
{
	unsigned int score = 0;
	int i = 0, j, combo = 0, missingCount = 0;
	int score_p = 0, score_g = 0, score_m = 0;
	char shouldRemoveNotes[4] = {0};

	// ���� �޸� �Ҵ�: buffer[4][2200]
	const int eachBufferSize = 2200;
	char** buffer = (char**)malloc(4 * sizeof(char*));
	for (int k = 0; k < 4; k++)
	{
		buffer[k] = (char*)malloc(eachBufferSize * sizeof(char));
	}

	FILE* fp;
	if ((fp = fopen(songs[songIndex].noteFileName, "r")) == NULL)
	{
		// ������ �о����
		printf("�������� �� �� �����ϴ�.\n");
		return;
	}

	// ��Ʈ ���� �о����
	for (int k = 0; k < 4; k++)
	{
		fgets(buffer[k], eachBufferSize, fp);
	}
	fclose(fp);

	// Ű �Է� ������ ����
	unsigned int threadIDs[4];
	for (int t = 0; t < 4; t++)
	{
		_beginthreadex(NULL, 0, keyPressHandler, &t, 0, &threadIDs[t]);
		Sleep(10);
	}

	drawMap(songIndex);

	// �뷡 ���
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

		// ��Ʈ �׸��� (���� ���۸�)
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
				// �̸� ��Ʈ�� �������� �����
				backBuffer[JUDGE_LINE_Y][j] = FALSE;
				frontBuffer[JUDGE_LINE_Y][j] = FALSE;

				shouldRemoveNotes[j] = FALSE;
			}
		}

		renderBuffer();

		int miss = 0;
		changeTextColor(WHITE, BLACK);

		// Ű ���� & ���� ���
		for (int k = 0; k < 4; k++)
		{
			if (frontBuffer[JUDGE_LINE_Y][k]) miss++;
			if (keyDown[k])
			{
				if (keyState[k])
				{
					// ��Ʈ�� ���� �� ���� ���
					if (!frontBuffer[35][k] && !frontBuffer[JUDGE_LINE_Y][k])
					{
						keyState[k] = FALSE;
					}
					// �ճ�Ʈ�� ������ ���� ���
					else if (frontBuffer[34][k] && frontBuffer[35][k])
					{
						combo++;
						miss--;
						score_g++;
						score += (combo / 10 + 1) * 50; // ��������
						shouldRemoveNotes[k] = TRUE; // ���� ��Ʈ�� ����
						missingCount = 1;
						moveCursor(78, 5);
						printf("GOOD!     ");
					}
					// �ճ�Ʈ�� ���� ���
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
					// ���� ������ ���� ���
					else if (frontBuffer[35][k] && !frontBuffer[JUDGE_LINE_Y][k])
					{
						combo++;
						score_g++;
						score += (combo / 10 + 1) * 50;
						keyState[k] = FALSE;
						shouldRemoveNotes[k] = TRUE; // ���� ��Ʈ�� ����
						missingCount = 1;
						moveCursor(78, 5);
						printf("GOOD!     ");
					}
					// ��Ȯ�� ���� ���
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
			printf("                    "); // �����
		}

		moveCursor(78, 1);
		printf("SCORE : %d            ", score);

		if (i++ > songs[songIndex].endTime)
		{
			break;
		}

		// �����̰� �׻� �����ϵ��� ����
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

	// ����
	PlaySound(NULL, 0, 0);
	changeTextColor(WHITE, BLACK);
	clearWindow();
	drawScore(score, score_p, score_g, score_m);
	clearWindow();

	// �޸� ����
	for (int k = 0; k < 4; k++)
	{
		free(buffer[k]);
	}
	free(buffer);
}

// ��Ʈ ���� ��� ����
void writeNoteMode(int songIndex)
{
	int i = 0;
	DWORD real_delay;
	clock_t now, now2;

	FILE* fp;
	if ((fp = fopen("Assets/note.txt", "w")) == NULL)
	{
		// ��Ʈ ���� ����
		printf("��Ʈ ������ �� �� �����ϴ�.\n");
		return;
	}

	// Ű �Է� ������ ����
	unsigned int threadIDs[4];
	int indices[4] = {0, 1, 2, 3};
	for (int t = 0; t < 4; t++)
	{
		_beginthreadex(NULL, 0, keyPressHandler, &indices[t], 0, &threadIDs[t]);
		Sleep(10); // ������ �ε��� ���� ������ Ȯ��
	}

	drawMap(songIndex);

	// �뷡 ���
	playSong(songIndex);

	// ���� �޸� �Ҵ�: buffer[4][2200]
	const int eachBufferSize = 2200;
	char** buffer = (char**)malloc(4 * sizeof(char*));
	for (int k = 0; k < 4; k++)
	{
		buffer[k] = (char*)malloc(eachBufferSize * sizeof(char));
	}

	while (1)
	{
		now = clock();
		renderBuffer();

		// Ű ���� & ��Ʈ ���
		for (int k = 0; k < 4; k++)
		{
			buffer[k][i] = keyDown[k];
		}

		// Ű ���� ����
		for (int k = 0; k < 4; k++)
		{
			keyDown[k] = FALSE;
		}

		if (i++ > songs[songIndex].endTime)
		{
			break;
		}

		// �����̰� �׻� �����ϵ��� ����
		now2 = clock();
		real_delay = songs[songIndex].delayTime - (now2 - now);
		if (debugMode == USE_DEBUG_MODE)
		{
			changeTextColor(WHITE, BLACK);
			moveCursor(78, 5);
			printf("DELAY : %d / %d", (int)real_delay, songs[songIndex].delayTime);
		}
		Sleep(real_delay);
	}

	// ��Ʈ ���� ����
	for (int k = 0; k < 4; k++)
	{
		fputs(buffer[k], fp);
		fputs("\n", fp);
	}
	fclose(fp);

	// ����
	PlaySound(NULL, 0, 0);
	changeTextColor(WHITE, BLACK);
	clearWindow();
	moveCursor(35, 10);
	printf("��Ʈ�� ����Ǿ����ϴ�.");
	Sleep(4000);

	// �޸� ����
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
