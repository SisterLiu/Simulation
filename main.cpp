#include <windows.h>
#include "WinProcess/winProcess.h"
#include "Display/display.h"
#include "TextureLoader/WICTextureLoader.h"
#include "simulator/panel.h"
#include "simulator/dice.h"
#include <iostream>
#include "log.h"

class Control
{
public:
	static void render60()
	{
		static ULONGLONG Timer = 0;
		ULONGLONG cur = GetTickCount64();
		if(Timer == 0)
			Timer = cur;
		if(cur - Timer > 1000 / 120.0)
		{
			Timer = cur;
			pDisplay->render(panel);

			if(hWnd)
			{
				HDC hdc = GetDC(hWnd);
				TEXTMETRIC tm;
				GetTextMetrics(hdc, &tm);

				wchar_t* buffer;
				int count = 0;
				while((buffer = (wchar_t*)log.getLine()) != NULL)
				{
					TextOut(hdc,0,tm.tmHeight*count,buffer,wcslen(buffer));
					count++;
				}
				ValidateRect(hWnd,NULL);
				ReleaseDC(hWnd,hdc);
			}
		}
	}

	static void Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Control::hWnd = hWnd;
		PAINTSTRUCT ps;
		switch(message)
		{
			case WM_PAINT:
				BeginPaint(hWnd,&ps);
				EndPaint(hWnd,&ps);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
		}
	}

	Control(Display* pDisplay)
	{
		this->pDisplay = pDisplay;
		this->screenWidth = pDisplay->screenWidth;
		this->screenHeight = pDisplay->screenHeight;
		this->bgLoader = BackgroundLoader(pDisplay);

		//	log
		//this->pDisplay->setLog(&log);
		this->hWnd = NULL;
	}

	void initial()
	{
		bgLoader.addBackground(L"img/block.jpg");
		bgLoader.addBackground(L"img/block2.png");
		cellBuffer.CreateBuffer(pDisplay->getDevice());
		panel = Panel(10, 10);
		for(int i=0;i<panel.cxSize();i++)
			for(int j = 0; j < panel.cySize(); j++)
			{
				panel.cellAt(i,j).rotation = (i+j)%4;
				panel.cellAt(i, j).size = screenHeight/(panel.cySize() +1);
				panel.cellAt(i, j).x = i * (panel.cellAt(i, j).size +1)+100;
				panel.cellAt(i, j).y = j * (panel.cellAt(i, j).size +1);
				panel.cellAt(i, j).pBackground = bgLoader[(i+j)%2];
				panel.cellAt(i, j) = cellBuffer;
			}
		Dice dice;

		for(int i = 0; i < 5; i++)
		{
			dice.addLot(i,i);
		}

		for(int i = 0; i < 50; i++)
			log.addLog(L""+i);


	}

	static Display* pDisplay;
	BackgroundLoader bgLoader;
	CellBuffer cellBuffer;
	static Panel panel;
	static WLog log;
	static HWND hWnd;
	int screenWidth;
	int screenHeight;
};

Display* Control::pDisplay;
Panel Control::panel;
WLog Control::log;
HWND Control::hWnd;


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WinProcess winProc(hInstance, nCmdShow, 1000, 800, L"HAHA", L"Simulation");
	Display display(winProc.getHWND());
	Control control(&display);

	control.initial();

	//	PROCESSCALLBACK
	winProc.setProcessCallback(control.Proc);
	//	LOOPCALLBACK
	winProc.setLoopCallback(control.render60);
	return winProc.loop();
}



