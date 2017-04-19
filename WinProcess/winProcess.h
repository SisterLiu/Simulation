#pragma once
#include <windows.h>

class WinProcess;
typedef void(*LOOPCALLBACK)();
typedef void(*PROCESSCALLBACK)(HWND, UINT, WPARAM, LPARAM);

class WinProcess
{
public:
	WinProcess(HINSTANCE hInstance, int nCmdShow, int screenX, int screenY, LPCWSTR className, LPCWSTR titleName);
	LRESULT static CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT setLoopCallback(LOOPCALLBACK);
	LRESULT setProcessCallback(PROCESSCALLBACK);
	LRESULT loop();

	HWND getHWND();

private:
	HWND hWnd;
	int clientScreenX;
	int clientScreenY;
	LOOPCALLBACK loopCallback;
	static PROCESSCALLBACK processCallback;
};


