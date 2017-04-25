#include "display.h"

using namespace DirectX;

#define PI 3.14159265359f
#define R(x) (x * PI / 180.0f)

void Display::render(std::vector<Block> blocks)
{
	//updateCamera();

	//-------------------------------------
	// Clear the back buffer
	//-------------------------------------
	pDx11DeviceContext->ClearRenderTargetView(pDx11RenderTargetView, Colors::WhiteSmoke);

	

	// Render
	for(int i = 0; i < blocks.size(); i++)
	{
		renderBlock(blocks[i]);
	}

	//
	// Present our back buffer to our front buffer
	//
	pDx11SwapChain->Present(0, 0);
}

void Display::renderBlock(Block block)
{
	constantBuffer.PositionChange = XMVectorSet(block.x, block.y, block.rotation, block.size);
	constantBuffer.ScreenSize = XMVectorSet(screenWidth, screenHeight, 0, 0);
	pDx11DeviceContext->UpdateSubresource(pDx11ConstantBuffer, 0, nullptr, &constantBuffer, 0, 0);
	pDx11DeviceContext->VSSetConstantBuffers(0, 1, &pDx11ConstantBuffer);

	UINT stride = sizeof(BlockVertex);
	UINT offset = 0;

	pDx11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDx11DeviceContext->IASetVertexBuffers(0, 1, &block.pVertexBuffer, &stride, &offset);
	pDx11DeviceContext->IASetIndexBuffer(block.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDx11DeviceContext->PSSetShaderResources(0, 1, &block.pBackground);
	pDx11DeviceContext->DrawIndexed(block.iIndex, 0, 0);
}

Display::Display(HWND hwnd)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	screenWidth = width;
	screenHeight = height;


	//-------------------------------------------------------------------------
	//	Dx11 Device and Swap Chain
	//-------------------------------------------------------------------------
	// create the device describe
	DXGI_SWAP_CHAIN_DESC swapChainDescribe;
	ZeroMemory(&swapChainDescribe, sizeof(swapChainDescribe));
	swapChainDescribe.BufferCount = 1;
	swapChainDescribe.BufferDesc.Width = width;
	swapChainDescribe.BufferDesc.Height = height;
	swapChainDescribe.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescribe.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDescribe.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescribe.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescribe.OutputWindow = hwnd;
	swapChainDescribe.SampleDesc.Count = 1;
	swapChainDescribe.SampleDesc.Quality = 0;
	swapChainDescribe.Windowed = TRUE;

	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

	//	create & set the device
	if(FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&FeatureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&swapChainDescribe,
		&pDx11SwapChain,
		&pDx11Device,
		&FeatureLevelsSupported,
		&pDx11DeviceContext)))
	{
		return;
	}
	//-------------------------------------------------------------------------
	//	Render Target View
	//-------------------------------------------------------------------------
	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pDx11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if(FAILED(hr))
		return;

	hr = pDx11Device->CreateRenderTargetView(pBackBuffer, nullptr, &pDx11RenderTargetView);
	pBackBuffer->Release();
	if(FAILED(hr))
		return;

	//-------------------------------------------------------------------------
	//	Set Render Target View & Depth Stencil View
	//-------------------------------------------------------------------------
	pDx11DeviceContext->OMSetRenderTargets(1, &pDx11RenderTargetView, 0);


	//-------------------------------------------------------------------------
	//	Set View Port
	//-------------------------------------------------------------------------
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pDx11DeviceContext->RSSetViewports(1, &viewPort);

	//-------------------------------------------------------------------------
	//	Vertex Shader
	//-------------------------------------------------------------------------
	ID3DBlob* pBlob = nullptr;
	/*
	hr = CompileShaderFromFile(L"Display/vShader.hlsl", "main", "vs_4_0", &pBlob);
	if(FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}
	*/
	hr = D3DReadFileToBlob(L"../Debug/vShader.cso",&pBlob);
	if(FAILED(hr))
	{
		MessageBox(nullptr,
			L"The vShader file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}

	// Create the vertex shader
	hr = pDx11Device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pDx11VertexShader);
	if(FAILED(hr))
	{
		pBlob->Release();
		return;
	}

	// Set the vertex shader
	pDx11DeviceContext->VSSetShader(pDx11VertexShader, nullptr, 0);



	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	//-------------------------------------------------------------------------
	//	Input Layout base on Vertex Shader
	//-------------------------------------------------------------------------
	// Create the input layout
	hr = pDx11Device->CreateInputLayout(layout, ARRAYSIZE(layout), pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), &pDx11VertexLayout);
	pBlob->Release();
	if(FAILED(hr))
		return;

	// Set the input layout
	pDx11DeviceContext->IASetInputLayout(pDx11VertexLayout);

	//-------------------------------------------------------------------------
	//	Pixel Shader
	//-------------------------------------------------------------------------
	// Compile the pixel shader

	/*
	hr = CompileShaderFromFile(L"Display/pShader.hlsl", "main", "ps_4_0", &pBlob);
	if(FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}
	*/
	hr = D3DReadFileToBlob(L"../Debug/pShader.cso", &pBlob);
	if(FAILED(hr))
	{
		MessageBox(nullptr,
			L"The pShader file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}

	// Create the pixel shader
	hr = pDx11Device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pDx11PixelShader);
	pBlob->Release();
	if(FAILED(hr))
		return;

	// Set the pixel shader
	pDx11DeviceContext->PSSetShader(pDx11PixelShader, nullptr, 0);

	
	//-------------------------------------------------------------------------
	//	Vertex Texture Sampler
	//-------------------------------------------------------------------------
	D3D11_SAMPLER_DESC samplerDescribe;
	ZeroMemory(&samplerDescribe, sizeof(samplerDescribe));
	samplerDescribe.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescribe.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescribe.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescribe.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescribe.BorderColor[0] = 0;
	samplerDescribe.BorderColor[1] = 0;
	samplerDescribe.BorderColor[2] = 0;
	samplerDescribe.BorderColor[3] = 1;
	samplerDescribe.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescribe.MaxLOD = 4;
	samplerDescribe.MinLOD = 0;

	//	Create SamplerState
	pDx11Device->CreateSamplerState(&samplerDescribe, &pDx11SamplerState);

	//	Set SamplerState
	pDx11DeviceContext->PSSetSamplers(0, 1, &pDx11SamplerState);

	//-------------------------------------------------------------------------
	//	Constant Buffer
	//-------------------------------------------------------------------------
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDx11Device->CreateBuffer(&bd, nullptr, &pDx11ConstantBuffer);
	if(FAILED(hr))
		return;
	
}

Display::~Display()
{
	if(pDx11Device)						pDx11Device->Release();
	if(pDx11DeviceContext)				pDx11DeviceContext->Release();

	if(pDx11SwapChain)					pDx11SwapChain->Release();
	if(pDx11RenderTargetView)			pDx11RenderTargetView->Release();

	if(pDx11VertexShader)				pDx11VertexShader->Release();
	if(pDx11PixelShader)				pDx11PixelShader->Release();
	if(pDx11VertexLayout)				pDx11VertexLayout->Release();

	if(pDx11ConstantBuffer)				pDx11ConstantBuffer->Release();
	if(pDx11SamplerState)				pDx11SamplerState->Release();
}



//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT Display::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{

	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if(FAILED(hr))
	{
		if(pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if(pErrorBlob) pErrorBlob->Release();

	return S_OK;
}
