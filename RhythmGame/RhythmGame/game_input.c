#include "game_input.h"
#include "game_ui.h"
#include <Windows.h>

#include "game_logic.h"

// Ű �ڵ� ����
const int keyCodes[4] = {'D', 'F', 'J', 'K'};
char keyDown[4] = {0};
char keyState[4] = {0};

// Ű �Է� ó�� �Լ� ����
unsigned __stdcall keyPressHandler(void* args)
{
	int index = *(int*)args;
	while (1)
	{
		if (!isGameRunning)
		{
			 // ������ ����Ǿ��� �� ������ ����
			backBuffer[JUDGE_LINE_Y + 1][index] = FALSE;
			backBuffer[JUDGE_LINE_Y + 2][index] = FALSE;
			return 0;
		}

		if (GetAsyncKeyState(keyCodes[index]) & 0x8000)
		{
			// �ش� Ű�� ���� ���
			backBuffer[JUDGE_LINE_Y + 1][index] = TRUE;
			backBuffer[JUDGE_LINE_Y + 2][index] = TRUE;
			keyDown[index] = TRUE; // Ű�� ���� ����
		}
		else
		{
			backBuffer[JUDGE_LINE_Y + 1][index] = FALSE;
			backBuffer[JUDGE_LINE_Y + 2][index] = FALSE;
			keyDown[index] = FALSE;
			keyState[index] = TRUE; // �� ������ ����
		}
		Sleep(1);
	}
}
