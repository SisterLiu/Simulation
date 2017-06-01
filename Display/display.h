#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include "../Simulator/panel.h"
#include "../TextureLoader/WICTextureLoader.h"

using namespace DirectX;


struct ConstantBuffer
{
	DirectX::XMVECTOR PositionChange;
	DirectX::XMVECTOR ScreenSize;
};


typedef class Display
{
public:
	void render(Panel);// render a frame
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

	unsigned int				screenWidth;
	unsigned int				screenHeight;
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
	
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	
}DX11DISPLAYER, *PDX11DISPLAYER;


typedef class BackgroundLoader
{
	public:
	BackgroundLoader();
	BackgroundLoader(PDX11DISPLAYER pDisplay);
	bool addBackground(const wchar_t*);
	void removeBackground(int i);
	void clear();
	void setDisplayer(PDX11DISPLAYER pDisplay);

	ID3D11ShaderResourceView*& operator[] (int index);
	
	private:
	struct BackgroundResource
	{
		ID3D11Resource* pBackground;
		ID3D11ShaderResourceView* pBackgroundView;
	};

	PDX11DISPLAYER pDisplayer;
	std::vector<BackgroundResource> backgrounds;
} BACKGROUNDLOADER, *PBACKGROUNDLOADER;
