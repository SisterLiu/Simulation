#include "winProcess.h"


PROCESSCALLBACK WinProcess::processCallback;

WinProcess::WinProcess(HINSTANCE hInstance, int nCmdShow, int screenX, int screenY, LPCWSTR className, LPCWSTR titleName)
{
	this->clientScreenX = screenX;
	this->clientScreenY = screenY;
	this->loopCallback = nullptr;
	processCallback = nullptr;

	// Register class
	WNDCLASSEX wcex = {0};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, (LPCTSTR)IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(nullptr, (LPCTSTR)IDI_APPLICATION);
	if(!RegisterClassEx(&wcex))
		return;

	// Create window
	RECT rc = {0, 0, screenX, screenY};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	hWnd = CreateWindowEx(0, className, titleName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if(hWnd)
		ShowWindow(hWnd, nCmdShow);
}

LRESULT CALLBACK WinProcess::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(processCallback != nullptr)
		(*processCallback)(hWnd, message, wParam, lParam);
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT WinProcess::loop()
{
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if(loopCallback != nullptr)
				(*loopCallback)();
		}
	}
	return (int)msg.wParam;
	
}

LRESULT WinProcess::setLoopCallback(LOOPCALLBACK funCallback)
{
	this->loopCallback = funCallback;
	return 0;
}

LRESULT WinProcess::setProcessCallback(PROCESSCALLBACK funCallback)
{
	processCallback = funCallback;
	return 0;
}

HWND WinProcess::getHWND()
{
	return hWnd;
}
