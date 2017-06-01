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
	int x;
	int y;
	int rotation;
	int size=100;
	ID3D11ShaderResourceView*	pBackground;
	D3D_PRIMITIVE_TOPOLOGY layout = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	HRESULT CreateBlockBuffer(_In_ ID3D11Device* pDevice);
private:

}BLOCK;


