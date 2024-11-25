#include "game_input.h"
#include "game_ui.h"
#include <Windows.h>

#include "game_logic.h"

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
		if (!isGameRunning)
		{
			 // 게임이 종료되었을 시 쓰레드 종료
			backBuffer[JUDGE_LINE_Y + 1][index] = FALSE;
			backBuffer[JUDGE_LINE_Y + 2][index] = FALSE;
			return 0;
		}

		if (GetAsyncKeyState(keyCodes[index]) & 0x8000)
		{
			// 해당 키가 눌린 경우
			backBuffer[JUDGE_LINE_Y + 1][index] = TRUE;
			backBuffer[JUDGE_LINE_Y + 2][index] = TRUE;
			keyDown[index] = TRUE; // 키를 누른 상태
		}
		else
		{
			backBuffer[JUDGE_LINE_Y + 1][index] = FALSE;
			backBuffer[JUDGE_LINE_Y + 2][index] = FALSE;
			keyDown[index] = FALSE;
			keyState[index] = TRUE; // 꾹 누르기 방지
		}
		Sleep(1);
	}
}
