#include <windows.h>
#include "WinProcess/winProcess.h"
#include "Display/display.h"
#include "TextureLoader/WICTextureLoader.h"

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
			pDisplay->render(blocks);
		}
	}
	static Display* pDisplay;
	static std::vector<Block> blocks;
	static ID3D11Resource* pResource;
	static ID3D11ShaderResourceView* pResourceView;
};

Display* Control::pDisplay;
std::vector<Block> Control::blocks;
ID3D11Resource* Control::pResource;
ID3D11ShaderResourceView* Control::pResourceView;

void Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch(message)
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd,&ps);
			EndPaint(hWnd,&ps);
	}
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WinProcess winProc(hInstance, nCmdShow, 1600, 900, L"HAHA", L"Simulation");
	HWND hwnd = winProc.getHWND();
	Display display(hwnd);
	Control control;
	//CreateWICTextureFromFile(display.getDevice(), display.getContext(), L"block.jpg", &control.pResource, &control.pResourceView);
	Block block;
	block.CreateBlockBuffer(display.getDevice(), &block.pVertexBuffer, &block.iVertex, &block.pIndexBuffer, &block.iIndex);
	//block.pBackground = control.pResourceView;
	control.blocks.push_back(block);
	control.pDisplay = &display;

	//	PROCESSCALLBACK
	winProc.setProcessCallback(Proc);
	//	LOOPCALLBACK
	winProc.setLoopCallback(control.render60);
	return winProc.loop();
}



