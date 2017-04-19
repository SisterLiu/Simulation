#pragma once

#include "d3d11_1.h"
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

struct BlockVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texture;
};

typedef class Block
{
public:
	ID3D11Buffer*				pVertexBuffer;
	ID3D11Buffer*				pIndexBuffer;
	unsigned int iVertex;
	unsigned int iIndex;
	ID3D11ShaderResourceView*	pBackground;
	D3D_PRIMITIVE_TOPOLOGY layout = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	HRESULT CreateBlockBuffer(_In_ ID3D11Device* pDevice, 
		_Out_ ID3D11Buffer** ppVertexBuffer, _Out_ unsigned int* pVertexCount,
		_Out_ ID3D11Buffer** ppIndexBuffer, _Out_ unsigned int* pIndexCount);
private:

}BLOCK;

