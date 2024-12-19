#include "game_ui.h"
#include "utils.h"

int main(void)
{
    initializeConsole();
	while (1)
	{
		drawMainMenu();
		int selection = selectMainMenu();
		switch (selection)
		{
		case 0:
			selectSongAndStartGame();
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