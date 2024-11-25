#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// Ŀ�� �̵�
void moveCursor(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Ŀ�� �����
void removeCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

// �ؽ�Ʈ ���� ����
void changeTextColor(int fgColor, int bgColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fgColor | (bgColor << 4));
}

// ȭ�� ���� ����
void changeWindowColor(int text_color, int bg_color)
{
	char cmd[100];
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}

// ȭ�� Ŭ����
void clearWindow()
{
	system("cls");
}