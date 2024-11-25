#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// 커서 이동
void moveCursor(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 커서 숨기기
void removeCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

// 텍스트 색상 변경
void changeTextColor(int fgColor, int bgColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fgColor | (bgColor << 4));
}

// 화면 색상 변경
void changeWindowColor(int text_color, int bg_color)
{
	char cmd[100];
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}

// 화면 클리어
void clearWindow()
{
	system("cls");
}