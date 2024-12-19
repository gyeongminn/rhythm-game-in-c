#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#include "song.h"
#include "game_logic.h"

DWORD WINAPI playSongThread(LPVOID param)
{
	int songIndex = *(int*)param;

	playSong(songIndex);

	isGameRunning = FALSE;
	return 0;
}

void playSong(int songIndex)
{
	LPCWSTR songFilePath;

	switch (songIndex)
	{
	case 0:
		songFilePath = TEXT("Assets\\GoodDay.wav");
		break;
	case 1:
		songFilePath = TEXT("Assets\\NeverEndingStory.wav");
		break;
	case 2:
	default:
		songFilePath = TEXT("Assets\\Payphone.wav");
		break;
	}

	// ���� �۾� ���丮 Ȯ��
	WCHAR fullPath[MAX_PATH];
	if (!GetFullPathName(songFilePath, MAX_PATH, fullPath, NULL))
	{
		fprintf(stderr, "���� ��θ� Ȯ���� �� �����ϴ�: %ws\n", songFilePath);
		exit(1);
	}

	// MCI�� ���� �뷡 ���
	MCI_OPEN_PARMS openParams;
	openParams.lpstrDeviceType = L"waveaudio";
	openParams.lpstrElementName = fullPath;

	if (mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&openParams) != 0)
	{
		fprintf(stderr, "�뷡 ������ �� �� �����ϴ�: %ws\n", fullPath);
		exit(1);
	}

	MCI_PLAY_PARMS playParams;
	mciSendCommand(openParams.wDeviceID, MCI_PLAY, MCI_WAIT, (DWORD_PTR)&playParams);

	// ����� ������ MCI ��ġ �ݱ�
	mciSendCommand(openParams.wDeviceID, MCI_CLOSE, 0, 0);
}
