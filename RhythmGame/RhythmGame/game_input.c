#include "game_input.h"
#include "game_ui.h"
#include <Windows.h>

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
		if (GetAsyncKeyState(keyCodes[index]) & 0x8000)
		{
			// �ش� Ű�� ���� ���
			backBuffer[37][index] = '1';
			backBuffer[38][index] = '1';
			keyDown[index] = '1'; // Ű�� ���� ����
		}
		else
		{
			keyDown[index] = '\0';
			keyState[index] = '1'; // �� ������ ����
		}
		Sleep(1);
	}
}
