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

	static void Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		HDC hdc;
		PAINTSTRUCT ps;
		switch(message)
		{
			case WM_PAINT:
				hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
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
	}

	void initial()
	{
		ID3D11Resource* pResource;
		ID3D11ShaderResourceView* pResourceView;
		CreateWICTextureFromFile(pDisplay->getDevice(), pDisplay->getContext(), L"block.jpg", &pResource, &pResourceView);
		resources.push_back(pResource);
		resourceViews.push_back(pResourceView);
		Block block;
		block.CreateBlockBuffer(pDisplay->getDevice());
		block.pBackground = resourceViews[0];
		int c = 100;
		for(int i=0;i<c;i++)
			for(int j = 0; j < c; j++)
			{
				block.rotation = (i+j)%4;
				block.size = screenHeight/(c+1);
				block.x = i * (block.size+1) + 350;
				block.y = j * (block.size+1);
				blocks.push_back(block);
			}
	}

	static Display* pDisplay;
	static std::vector<Block> blocks;
	std::vector<ID3D11Resource*> resources;
	std::vector<ID3D11ShaderResourceView*> resourceViews;
	int screenWidth;
	int screenHeight;
};

Display* Control::pDisplay;
std::vector<Block> Control::blocks;



int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WinProcess winProc(hInstance, nCmdShow, 1500, 800, L"HAHA", L"Simulation");
	Display display(winProc.getHWND());
	Control control(&display);

	control.initial();

	//	PROCESSCALLBACK
	winProc.setProcessCallback(control.Proc);
	//	LOOPCALLBACK
	winProc.setLoopCallback(control.render60);
	return winProc.loop();
}



