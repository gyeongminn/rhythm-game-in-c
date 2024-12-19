#ifndef UTILS_H
#define UTILS_H

// �ִ�, �ּ� ��ũ�� �Լ�
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Ŀ�� �̵�
void moveCursor(int x, int y);

// Ŀ�� �����
void removeCursor(void);

// �ؽ�Ʈ ���� ����
void changeTextColor(int fgColor, int bgColor);

// ȭ�� ���� ����
void changeWindowColor(int text_color, int bg_color);

// ȭ�� Ŭ����
void clearWindow();

#endif