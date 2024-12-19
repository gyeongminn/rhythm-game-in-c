#ifndef UTILS_H
#define UTILS_H

// 최대, 최소 매크로 함수
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// 커서 이동
void moveCursor(int x, int y);

// 커서 숨기기
void removeCursor(void);

// 텍스트 색상 변경
void changeTextColor(int fgColor, int bgColor);

// 화면 색상 변경
void changeWindowColor(int text_color, int bg_color);

// 화면 클리어
void clearWindow();

#endif