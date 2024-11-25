#include "game_ui.h"
#include "utils.h"

// 메인 함수
int main(void)
{
	removeCursor();
	while (1)
	{
		drawMainMenu();
		int selection = selectMainMenu();
		switch (selection)
		{
		case 0:
			selectSong();
			break;
		case 1:
			showOptions();
			break;
		case 2:
			drawHelp();
			break;
		case 3:
			clearWindow();
			changeWindowColor(WHITE, BLACK);
			return 0;
		default:
			break;
		}
	}
}
