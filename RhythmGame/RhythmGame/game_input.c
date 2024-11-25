#include "game_input.h"
#include "game_ui.h"
#include <Windows.h>

// 키 코드 매핑
const int keyCodes[4] = {'D', 'F', 'J', 'K'};
char keyDown[4] = {0};
char keyState[4] = {0};

// 키 입력 처리 함수 구현
unsigned __stdcall keyPressHandler(void* args)
{
	int index = *(int*)args;
	while (1)
	{
		if (GetAsyncKeyState(keyCodes[index]) & 0x8000)
		{
			// 해당 키가 눌린 경우
			backBuffer[37][index] = TRUE;
			backBuffer[38][index] = TRUE;
			keyDown[index] = TRUE; // 키를 누른 상태
		}
		else
		{
			backBuffer[37][index] = FALSE;
			backBuffer[38][index] = FALSE;
			keyDown[index] = FALSE;
			keyState[index] = TRUE; // 꾹 누르기 방지
		}
		Sleep(1);
	}
}
