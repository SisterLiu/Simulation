#include "model.h"

using namespace DirectX;

HRESULT Block::CreateBlockBuffer(_In_ ID3D11Device* pDevice)
{
	if(pDevice == NULL)
		return 1;

	BlockVertex pVertexs[] =
	{
		XMFLOAT3(0, 0, 0.5f), XMFLOAT2(0, 0),
		XMFLOAT3(2, 0, 0.5f), XMFLOAT2(1, 0),
		XMFLOAT3(2, 2, 0.5f), XMFLOAT2(1, 1),
		XMFLOAT3(0, 2, 0.5f), XMFLOAT2(0, 1),
	};

	iVertex = ARRAYSIZE(pVertexs);

	unsigned int pIndexs[] = 
	{
		0,1,2,
		0,2,3,
	};

	iIndex = ARRAYSIZE(pIndexs);

	//------------------------------------------------------------------
	//	Create Vertex Buffer
	//------------------------------------------------------------------
	//	Set vertex buffer describe
	D3D11_BUFFER_DESC vertexBufferDescribe;
	ZeroMemory(&vertexBufferDescribe, sizeof(vertexBufferDescribe));
	vertexBufferDescribe.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDescribe.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescribe.ByteWidth = sizeof(BlockVertex) * iVertex;
	vertexBufferDescribe.CPUAccessFlags = 0;

	//	Set vertex buffer data
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = pVertexs;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	//	Create Vertex Buffer
	pDevice->CreateBuffer(&vertexBufferDescribe, &vertexBufferData, &pVertexBuffer);

	//------------------------------------------------------------------
	//	Create Index Buffer
	//------------------------------------------------------------------
	//	Set index buffer describe
	D3D11_BUFFER_DESC indexBufferDescribe;
	ZeroMemory(&indexBufferDescribe, sizeof(indexBufferDescribe));
	indexBufferDescribe.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescribe.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescribe.ByteWidth = sizeof(unsigned int) * iIndex;
	indexBufferDescribe.CPUAccessFlags = 0;

	//	Set index buffer data
	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = pIndexs;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	//	Create Vertex Buffer
	pDevice->CreateBuffer(&indexBufferDescribe, &indexBufferData, &pIndexBuffer);

	//------------------------------------------------------------------
	//	Release vertex & index buffer
	//------------------------------------------------------------------

	return 1;
}
