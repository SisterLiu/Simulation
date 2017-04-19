#include "model.h"

using namespace DirectX;

HRESULT Block::CreateBlockBuffer(_In_ ID3D11Device* pDevice,
	_Out_ ID3D11Buffer** ppVertexBuffer, _Out_ unsigned int* pVertexCount,
	_Out_ ID3D11Buffer** ppIndexBuffer, _Out_ unsigned int* pIndexCount)
{
	if(pDevice == NULL)
		return 1;

	BlockVertex pVertexs[] =
	{
		XMFLOAT3(0, 0, 0.5f), XMFLOAT2(0, 1),
		XMFLOAT3(2, 0, 0.5f), XMFLOAT2(1, 1),
		XMFLOAT3(2, 2, 0.5f), XMFLOAT2(1, 0),
		XMFLOAT3(0, 2, 0.5f), XMFLOAT2(0, 0),
	};

	*pVertexCount = ARRAYSIZE(pVertexs);

	unsigned int pIndexs[] = 
	{
		0,2,1,
		0,3,2,
	};

	*pIndexCount = ARRAYSIZE(pIndexs);

	//------------------------------------------------------------------
	//	Create Vertex Buffer
	//------------------------------------------------------------------
	//	Set vertex buffer describe
	D3D11_BUFFER_DESC vertexBufferDescribe;
	ZeroMemory(&vertexBufferDescribe, sizeof(vertexBufferDescribe));
	vertexBufferDescribe.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDescribe.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescribe.ByteWidth = sizeof(BlockVertex) * (*pVertexCount);
	vertexBufferDescribe.CPUAccessFlags = 0;

	//	Set vertex buffer data
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = pVertexs;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	//	Create Vertex Buffer
	pDevice->CreateBuffer(&vertexBufferDescribe, &vertexBufferData, ppVertexBuffer);

	//------------------------------------------------------------------
	//	Create Index Buffer
	//------------------------------------------------------------------
	//	Set index buffer describe
	D3D11_BUFFER_DESC indexBufferDescribe;
	ZeroMemory(&indexBufferDescribe, sizeof(indexBufferDescribe));
	indexBufferDescribe.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescribe.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescribe.ByteWidth = sizeof(unsigned int) * (*pIndexCount);
	indexBufferDescribe.CPUAccessFlags = 0;

	//	Set index buffer data
	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = pIndexs;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	//	Create Vertex Buffer
	pDevice->CreateBuffer(&indexBufferDescribe, &indexBufferData, ppIndexBuffer);

	//------------------------------------------------------------------
	//	Release vertex & index buffer
	//------------------------------------------------------------------

	return 1;
}
