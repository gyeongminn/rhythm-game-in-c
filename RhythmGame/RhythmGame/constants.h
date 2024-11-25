#ifndef CONSTANTS_H
#define CONSTANTS_H

// BOOLEAN
#define TRUE 1
#define FALSE 0

// 화면 크기
#define SCR_WIDTH 100
#define SCR_HEIGHT 40
#define NUM_SONGS 3

// 색상 정의
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

// 특수키 정의
#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.
#define UP      0x48   // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN    0x50
#define LEFT    0x4B
#define RIGHT   0x4D
#define ENTER   13

// 노래 난이도 정으
#define EASY 0
#define NORMAL 1
#define HARD 2

#endif
#pragma once
