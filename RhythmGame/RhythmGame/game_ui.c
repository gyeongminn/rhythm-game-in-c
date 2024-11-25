#include "game_logic.h"
#include "game_ui.h"
#include "utils.h"
#include "songs.h"
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

// 외부 변수
char frontBuffer[SCR_HEIGHT][4] = {0};
char backBuffer[SCR_HEIGHT][4] = {0};

// 메인 메뉴 아스키아트
const char* rhythmGameArt[19] = {
	"          88888888ba   88                                 88                                             ",
	"          88      \"8b  88                          ,d     88                                            ",
	"          88      ,8P  88                          88     88                                             ",
	"          88aaaaaa8P\'  88,dPPYba,   8b       d8  MM88MMM  88,dPPYba,   88,dPYba,,adPYba,                ",
	"          88\"\"\"\"88\'    88P\'    \"8a  `8b     d8\'    88     88P\'    \"8a  88P\'   \"88\"    \"8a  ",
	"          88    `8b    88       88   `8b   d8\'     88     88       88  88      88      88               ",
	"          88     `8b   88       88    `8b,d8\'      88,    88       88  88      88      88               ",
	"          88      `8b  88       88      Y88\'       \"Y888  88       88  88      88      88              ",
	"                                        d8\'                                                             ",
	"                                       d8\'                                                              ",
	"                                                                                                         ",
	"                       ,ad8888ba,                                                                        ",
	"                      d8\"\'    `\"8b                                                                    ",
	"                     d8\'                                                                                ",
	"                     88             ,adPPYYba,  88,dPYba,,adPYba,    ,adPPYba,                           ",
	"                     88      88888  \"\"     `Y8  88P\'   \"88\"    \"8a  a8P     `8b                    ",
	"                     Y8,        88  ,adPPPPP88  88      88      88  8PP8888888P                          ",
	"                      Y8a.    .a88  88,    ,88  88      88      88  \"8b,    ,aa                         ",
	"                       `\"Y88888P\"   `\"8bbdP\"Y8  88      88      88   `\"Ybbd88\"\'                   "
};

// 박스 그리기
void drawBox(int x1, int y1, int x2, int y2)
{
	for (int x = x1; x <= x2; x++)
	{
		moveCursor(x, y1);
		printf("─");
		moveCursor(x, y2);
		printf("─");
	}
	for (int y = y1; y <= y2; y++)
	{
		moveCursor(x1, y);
		printf("│");
		moveCursor(x2, y);
		printf("│");
	}
	moveCursor(x1, y1);
	printf("┌");
	moveCursor(x2, y1);
	printf("┐");
	moveCursor(x1, y2);
	printf("└");
	moveCursor(x2, y2);
	printf("┘");
}

// 박스 제거
void removeBox(int x1, int y1, int x2, int y2)
{
	for (int x = x1; x <= x2; x += 2)
	{
		moveCursor(x, y1);
		printf("  ");
		moveCursor(x, y2);
		printf("  ");
	}
	for (int y = y1; y <= y2; y++)
	{
		moveCursor(x1, y);
		printf("  ");
		moveCursor(x2, y);
		printf("  ");
	}
}

// 더블 버퍼 렌더링
void renderBuffer(void)
{
	for (int y = 2; y < SCR_HEIGHT; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			// backBuffer와 frontBuffer가 다를 때만 노트를 그려 화면 렌더링을 최적화
			if (backBuffer[y][x] == frontBuffer[y][x])
			{
				continue;
			}

			// 노트 판정선 부분은 렌더링하지 않음
			if (y == 36)
			{
				continue;
			}

			// 노트 지우기
			moveCursor(x * 12 + 27, y);
			if (!backBuffer[y][x])
			{
				changeTextColor(WHITE, BLACK);
				printf("          ");
				continue;
			}

			// 노트 그리기
			switch (x)
			{
			case 0: changeTextColor(WHITE, RED1);
				break;
			case 1: changeTextColor(WHITE, BLUE1);
				break;
			case 2: changeTextColor(WHITE, GREEN1);
				break;
			case 3: changeTextColor(WHITE, YELLOW1);
				break;
			default: changeTextColor(WHITE, BLACK);
				break;
			}
			printf("          ");
		}
	}

	// 백버퍼을 프론트버퍼로 복사하고 백버퍼 초기화
	for (int y = 2; y < SCR_HEIGHT; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			frontBuffer[y][x] = backBuffer[y][x];
		}
	}
}

// 메인 메뉴 그리기
void drawMainMenu(void)
{
	for (int j = 0; j < 19; j++)
	{
		moveCursor(0, j + 2);
		printf("%s", rhythmGameArt[j]);
	}

	moveCursor(46, 24);
	printf("시작\n");
	moveCursor(46, 27);
	printf("설정\n");
	moveCursor(45, 30);
	printf("도움말\n");
	moveCursor(45, 33);
	printf("나가기\n");
	drawBox(30, 23, 64, 25);
	moveCursor(85, 38);
	printf("2171333 이경민");
}

// 맵 그리기
void drawMap(int songIndex)
{
	moveCursor(2, 1);
	printf("%s\n\n  템포(BPM) : %d\n\n  난이도 : %s", songs[songIndex].titleName, songs[songIndex].bpm,
	       getLevelName(songIndex));
	changeTextColor(WHITE, BLACK);
	drawBox(24, 36, 74, 39);
	drawBox(24, 0, 74, 39);
	moveCursor(32, 38);
	printf("D           F           J           K");
}

// 점수 출력
void drawScore(int score, int score_p, int score_g, int score_m)
{
	int percent[3];
	percent[0] = (int)((double)score_p / ((double)score_p + score_g + score_m) * 100);
	percent[1] = (int)((double)score_g / ((double)score_p + score_g + score_m) * 100);
	percent[2] = (int)((double)score_m / ((double)score_p + score_g + score_m) * 100);

	moveCursor(0, 5);
	if (percent[0] + percent[1] > 90)
	{
		printf("                              d8888      8888888b.                    888\n");
		printf("                             d88888      888   Y88b                   888\n");
		printf("                            d88P888      888    888                   888\n");
		printf("                           d88P 888      888   d88P  8888b.  88888b.  888  888\n");
		printf("                          d88P  888      8888888P\"      \"88b 888 \"88b 888 .88P\n");
		printf("                         d88P   888      888 T88b   .d888888 888  888 888888K\n");
		printf("                        d8888888888      888  T88b  888  888 888  888 888 \"88b\n");
		printf("                       d88P     888      888   T88b \"Y888888 888  888 888  888");
		moveCursor(23, 20);
		printf("Rank : A");
	}
	else if (percent[0] + percent[1] > 70)
	{
		printf("                       888888b.        8888888b.                    888\n");
		printf("                       888  \"88b       888   Y88b                   888\n");
		printf("                       888  .88P       888    888                   888\n");
		printf("                       8888888K.       888   d88P  8888b.  88888b.  888  888\n");
		printf("                       888  \"Y88b      8888888P\"      \"88b 888 \"88b 888 .88P\n");
		printf("                       888    888      888 T88b   .d888888 888  888 888888K\n");
		printf("                       888   d88P      888  T88b  888  888 888  888 888 \"88b\n");
		printf("                       8888888P\"       888   T88b \"Y888888 888  888 888  888");
		moveCursor(23, 20);
		printf("Rank : B");
	}
	else if (percent[0] + percent[1] > 50)
	{
		printf("                        .d8888b.       8888888b.                    888\n");
		printf("                       d88P  Y88b      888   Y88b                   888\n");
		printf("                       888    888      888    888                   888\n");
		printf("                       888             888   d88P  8888b.  88888b.  888  888\n");
		printf("                       888             8888888P\"      \"88b 888 \"88b 888 .88P\n");
		printf("                       888    888      888 T88b   .d888888 888  888 888888K\n");
		printf("                       Y88b  d88P      888  T88b  888  888 888  888 888 \"88b\n");
		printf("                        \"Y8888P\"       888   T88b \"Y888888 888  888 888  888");
		moveCursor(23, 20);
		printf("Rank : C");
	}
	else
	{
		printf("                       8888888b.       8888888b.                    888\n");
		printf("                       888  \"Y88b      888   Y88b                   888\n");
		printf("                       888    888      888    888                   888\n");
		printf("                       888    888      888   d88P  8888b.  88888b.  888  888\n");
		printf("                       888    888      8888888P\"      \"88b 888 \"88b 888 .88P \n");
		printf("                       888    888      888 T88b.   d888888 888  888 888888K\n");
		printf("                       888.  d88P      888  T88b  888  888 888  888 888 \"88b \n");
		printf("                       8888888P\"       888   T88b \"Y888888 888  888 888  888");
		moveCursor(23, 20);
		printf("Rank : D");
	}
	Sleep(100);
	for (int i = 0; i <= score; i += 2)
	{
		moveCursor(23, 22);
		printf("SCORE : %d", i);
	}
	Sleep(100);
	for (int i = 0; i <= score_p; i++)
	{
		moveCursor(23, 24);
		printf("Perfect : %d", i);
		Sleep(1);
	}
	Sleep(100);
	for (int i = 0; i <= score_g; i++)
	{
		moveCursor(23, 26);
		printf("Good : %d", i);
		Sleep(1);
	}
	Sleep(100);
	for (int i = 0; i <= score_m; i++)
	{
		moveCursor(23, 28);
		printf("Miss : %d", i);
		Sleep(2);
	}
	Sleep(300);
	moveCursor(53, 28);
	printf("Perfect    Good    Miss");
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j <= percent[i] / 10; j++)
		{
			moveCursor(55 + 9 * i + i % 2, 26 - j);
			printf("■");
			Sleep(300);
		}
	}
	Sleep(10000);
}

// 도움말 그리기
void drawHelp(void)
{
	clearWindow();
	moveCursor(15, 10);
	printf("노트가 판정선에 떨어질 때 D, F, J, K 키를 이용해서 노트를 누르면 됩니다.");
	moveCursor(15, 13);
	printf("디버깅 모드 : 현재 딜레이를 보여줍니다.");
	moveCursor(15, 16);
	printf("노트 찍기 모드 : 노트를 찍는 모드입니다. \"note.txt\" 파일에 저장됩니다.");
	moveCursor(15, 18);
	printf("노트가 내려오지 않고, 노래만 재생됩니다.");
	moveCursor(15, 20);
	printf("파일 내용을 복사 후 노래 파일에 붙혀넣기한 뒤, 시작 타이밍을 맞춰주면 됩니다.");
	moveCursor(15, 23);
	printf("제작자 : 2171333 이경민");
	_getch();
	clearWindow();
	drawMap(0); // 기본 맵 그리기
	moveCursor(76, 36);
	printf("← 여기가 판정선입니다.");
	_getch();
	clearWindow();
}

// 메뉴 선택 함수 구현
int selectMainMenu(void)
{
	int n = 0;
	unsigned char ch = 0;
	while (1)
	{
		if (_kbhit())
		{
			// 키보드가 눌려져 있으면
			ch = _getch();
			if (ch == SPECIAL1 || ch == SPECIAL2)
			{
				// 특수키
				ch = _getch();
				removeBox(30, 23 + n * 3, 64, 25 + n * 3);
				if (ch == UP)
				{
					n = (n - 1 + 4) % 4; // 한칸 위로
				}
				else if (ch == DOWN)
				{
					n = (n + 1) % 4; // 한칸 아래로
				}
				drawBox(30, 23 + n * 3, 64, 25 + n * 3);
			}
			else if (ch == ENTER)
			{
				changeTextColor(WHITE, BLACK);
				clearWindow();
				return n % 4; // 엔터키 입력하면 위치를 리턴
			}
		}
	}
}

char* getLevelName(int level)
{
	static const char* levelNames[] = {"EASY", "NORMAL", "HARD"};
	return levelNames[level];
}

// 노래 선택 창 구현
void selectSong(void)
{
	// 애니메이션 효과
	int i, j, n = 0;
	unsigned char ch = 0;
	for (i = 0; i < 19; i++)
	{
		for (j = 0; j < 19 - i; j++)
		{
			moveCursor(0, j + 3);
			printf("                                                                                             ");
			moveCursor(0, j + 2);
			printf("%s", rhythmGameArt[j + i]);
		}
		Sleep(50);
	}
	moveCursor(0, 2);
	printf("                                                                                             ");
	Sleep(300);
	moveCursor(39, 5);
	printf("노래를 선택해 주세요");
	moveCursor(44, 30);
	printf("뒤로가기");
	for (i = 0; i < 3; i++)
	{
		for (j = 5; j > 0; j--)
		{
			moveCursor(i * 31, 14);
			moveCursor(8 + i * 30 + i / 2 * 3 - j, 14);
			printf("  %s", songs[i].titleName);
			moveCursor(8 + i * 30 + i / 2 * 3 - j, 16);
			printf("  난이도 : %s", getLevelName(i));
			moveCursor(8 + i * 30 + i / 2 * 3 - j, 18);
			printf("  템포(BPM) : %d", songs[i].bpm);
			moveCursor(8 + i * 30 + i / 2 * 3 - j, 20);
			printf("  장르 : %s", songs[i].genre);
			Sleep(70);
		}
	}
	drawBox(3, 13, 29, 21);
	int selected = 1;
	while (1)
	{
		if (_kbhit())
		{
			// 키보드가 눌려져 있으면
			ch = _getch();
			if (ch == SPECIAL1 || ch == SPECIAL2)
			{
				// 특수키
				ch = _getch();
				// 박스 제거
				if (selected <= 3)
				{
					removeBox(3 + (selected - 1) * 32, 13, 29 + (selected - 1) * 32, 21);
				}
				else
				{
					removeBox(30, 29, 64, 31);
				}

				if (ch == LEFT)
				{
					selected = (selected - 1 + 4) % 4;
				}
				else if (ch == RIGHT)
				{
					selected = (selected + 1) % 4;
				}
				else if (ch == UP || ch == DOWN)
				{
					// 상하 이동 없이 좌우만 이동
				}

				// 박스 다시 그리기
				if (selected >= 1 && selected <= 3)
				{
					drawBox(3 + (selected - 1) * 32, 13, 29 + (selected - 1) * 32, 21);
				}
				else
				{
					drawBox(30, 29, 64, 31);
				}
			}
			else if (ch == ENTER)
			{
				changeTextColor(WHITE, BLACK);
				clearWindow();

				if (selected == 4)
				{
					return;
				}

				if (gameMode == WRITE_NOTE_MODE)
				{
					writeNoteMode((selected - 1) % 3);
				}
				else
				{
					startGame((selected - 1) % 3);
				}
			}
		}
	}
}

// 옵션 메뉴 구현
void showOptions(void)
{
	int n = 0;
	unsigned char ch = 0;
	clearWindow();
	moveCursor(48, 10);
	printf("옵션");
	moveCursor(40, 21);
	printf("      뒤로가기");
	drawBox(37, 14, 61, 16);
	if (gameMode == WRITE_NOTE_MODE)
	{
		moveCursor(40, 15);
		printf("노트 찍기 모드   ON ");
	}
	else
	{
		moveCursor(40, 15);
		printf("노트 찍기 모드   OFF");
	}
	if (debugMode == USE_DEBUG_MODE)
	{
		moveCursor(40, 18);
		printf("   디버깅 모드   ON ");
	}
	else
	{
		moveCursor(40, 18);
		printf("   디버깅 모드   OFF");
	}
	while (1)
	{
		if (_kbhit())
		{
			// 키보드가 눌려져 있으면
			ch = _getch();
			if (ch == SPECIAL1 || ch == SPECIAL2)
			{
				// 특수키
				ch = _getch();
				removeBox(37, 14 + n * 3, 61, 16 + n * 3);
				if (ch == UP)
				{
					n = (n - 1 + 2) % 2;
				}
				else if (ch == DOWN)
				{
					n = (n + 1) % 2;
				}
				drawBox(37, 14 + n * 3, 61, 16 + n * 3);
			}
			else if (ch == ENTER)
			{
				changeTextColor(WHITE, BLACK);
				if (n == 0)
				{
					// 노트 찍기 모드
					gameMode = (gameMode + 1) % 2;
					moveCursor(40, 15);
					printf("노트 찍기 모드   %s", gameMode == WRITE_NOTE_MODE ? "ON " : "OFF");
				}
				else if (n == 1)
				{
					// 디버깅 모드
					debugMode = (debugMode + 1) % 2;
					moveCursor(40, 18);
					printf("   디버깅 모드   %s", debugMode == USE_DEBUG_MODE ? "ON " : "OFF");
				}
				else
				{
					// 뒤로가기
					clearWindow();
					return;
				}
			}
		}
	}
}

void initializeConsole()
{
	removeCursor();
	SetConsoleTitleA("Rhythm Game - Gyeongmin Lee");
}
