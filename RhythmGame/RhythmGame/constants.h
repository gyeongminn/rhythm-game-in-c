#ifndef CONSTANTS_H
#define CONSTANTS_H

// BOOLEAN
#define TRUE 1
#define FALSE 0

// ȭ�� ũ��
#define SCR_WIDTH 100
#define SCR_HEIGHT 40
#define NUM_SONGS 3

// ���� ����
typedef enum
{
	BLACK = 0,
	BLUE1 = 1,
	GREEN1 = 2,
	CYAN1 = 3,
	RED1 = 4,
	MAGENTA1 = 5,
	YELLOW1 = 6,
	GRAY1 = 7,
	GRAY2 = 8,
	BLUE2 = 9,
	GREEN2 = 10,
	CYAN2 = 11,
	RED2 = 12,
	MAGENTA2 = 13,
	YELLOW2 = 14,
	WHITE = 15
} Colors;

// Ư��Ű ����
#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.
#define UP      0x48   // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
#define DOWN    0x50
#define LEFT    0x4B
#define RIGHT   0x4D
#define ENTER   13

// �뷡 ���̵� ����
#define EASY 0
#define NORMAL 1
#define HARD 2

#endif
#pragma once
