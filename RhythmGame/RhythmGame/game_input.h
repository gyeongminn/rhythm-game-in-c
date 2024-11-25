#ifndef GAME_INPUT_H
#define GAME_INPUT_H

// 외부 변수 선언
extern char keyDown[4];
extern char keyState[4];

// 키 입력 처리 함수
unsigned __stdcall keyPressHandler(void* args);

#endif
