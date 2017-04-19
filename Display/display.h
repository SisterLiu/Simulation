#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include "model.h"

using namespace DirectX;

struct ConstantBuffer
{
	DirectX::XMVECTOR PositionChange;
	DirectX::XMVECTOR ScreenSize;
};


typedef class Display
{
public:
	void render(std::vector<Block>);// render a frame
	void renderBlock(Block);

	Display(HWND);
	~Display();

	ID3D11Device*			getDevice()
	{
		return pDx11Device;
	}
	ID3D11DeviceContext*	getContext()
	{
		return pDx11DeviceContext;
	}


private:
	ID3D11Device*				pDx11Device;
	ID3D11DeviceContext*		pDx11DeviceContext;

	IDXGISwapChain*				pDx11SwapChain;
	ID3D11RenderTargetView*		pDx11RenderTargetView;

	ID3D11VertexShader*			pDx11VertexShader;
	ID3D11PixelShader*			pDx11PixelShader;
	ID3D11InputLayout*			pDx11VertexLayout;


	ID3D11Buffer*				pDx11ConstantBuffer;
	ID3D11SamplerState*			pDx11SamplerState;
	ConstantBuffer				constantBuffer;

	D3D11_VIEWPORT				viewPort;

	unsigned int				screenWidth;
	unsigned int				screenHeight;

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	
}DX11DISPLAYER, *PDX11DISPLAYER;


