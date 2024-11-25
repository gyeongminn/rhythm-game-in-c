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
	char buf1[2200] = {0}; // ��Ʈ ���� ����
	char buf2[2200] = {0};
	char buf3[2200] = {0};
	char buf4[2200] = {0};
	FILE* fp;

	if ((fp = fopen(songs[songIndex].noteFileName, "r")) == NULL)
	{
		// ������ �о����
		printf("�������� �� �� �����ϴ�.\n");
		return;
	}

	// ��Ʈ ���� �о����
	fgets(buf1, sizeof(buf1), fp);
	fgets(buf2, sizeof(buf2), fp);
	fgets(buf3, sizeof(buf3), fp);
	fgets(buf4, sizeof(buf4), fp);
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
			if (j >= 36)
			{
				j = 36;
			}
			backBuffer[j][0] = buf1[i - j] == '1';
			backBuffer[j][1] = buf2[i - j] == '1';
			backBuffer[j][2] = buf3[i - j] == '1';
			backBuffer[j][3] = buf4[i - j] == '1';
		}

		for (j = 0; j < 4; j++)
		{
			if (shouldRemoveNotes[j])
			{
				// �̸� ��Ʈ�� �������� �����
				backBuffer[36][j] = FALSE;
				frontBuffer[36][j] = FALSE;

				shouldRemoveNotes[j] = FALSE;
			}
		}

		renderBuffer();

		int miss = 0;
		changeTextColor(WHITE, BLACK);

		// Ű ���� & ���� ���
		for (int k = 0; k < 4; k++)
		{
			if (frontBuffer[36][k]) miss++;
			if (keyDown[k])
			{
				if (keyState[k])
				{
					// ��Ʈ�� ���� �� ���� ���
					if (!frontBuffer[35][k] && !frontBuffer[36][k])
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
					else if (frontBuffer[35][k] && frontBuffer[36][k])
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
					else if (frontBuffer[35][k] && !frontBuffer[36][k])
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
					else if (frontBuffer[36][k])
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
}

// ��Ʈ ���� ��� ����
void writeNoteMode(int songIndex)
{
	int i = 0;
	char buf1[2200] = {0}; // ��Ʈ ���� ����
	char buf2[2200] = {0};
	char buf3[2200] = {0};
	char buf4[2200] = {0};
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

	while (1)
	{
		now = clock();
		renderBuffer();

		// Ű ���� & ��Ʈ ���
		buf1[i] = keyDown[0];
		buf2[i] = keyDown[1];
		buf3[i] = keyDown[2];
		buf4[i] = keyDown[3];

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
	fputs(buf1, fp);
	fputs("\n", fp);
	fputs(buf2, fp);
	fputs("\n", fp);
	fputs(buf3, fp);
	fputs("\n", fp);
	fputs(buf4, fp);
	fclose(fp);

	// ����
	PlaySound(NULL, 0, 0);
	changeTextColor(WHITE, BLACK);
	clearWindow();
	moveCursor(35, 10);
	printf("��Ʈ�� ����Ǿ����ϴ�.");
	Sleep(4000);
}
