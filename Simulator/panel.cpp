#include "panel.h"
//	class CellBuffer

using namespace DirectX;

HRESULT CellBuffer::CreateBuffer(_In_ ID3D11Device* pDevice)
{
	if(pDevice == NULL)
		return 1;

	CellVertex pVertexs[] =
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
	vertexBufferDescribe.ByteWidth = sizeof(CellVertex) * iVertex;
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

//	class Cell
Cell::Cell()
{
	this->val = 0;
	this->type = 0;
}

bool Cell::operator!=(const Cell& b)
{
	if(this->val != b.val)return false;
	if(this->type != b.type)return false;
	return true;
}

Cell& Cell::operator=(const CellBuffer& cb)
{
	setCellBuffer(cb);
	return *this;
}

void Cell::setCellBuffer(const CellBuffer& cb)
{
	this->iIndex = cb.iIndex;
	this->iVertex = cb.iVertex;
	this->layout = cb.layout;
	this->pIndexBuffer = cb.pIndexBuffer;
	this->pVertexBuffer = cb.pVertexBuffer;
}

CellBuffer Cell::getCellBuffer()
{
	CellBuffer cb;
	cb.iIndex = this->iIndex;
	cb.iVertex = this->iVertex;
	cb.layout = this->layout;
	cb.pIndexBuffer = this->pIndexBuffer;
	cb.pVertexBuffer = this->pVertexBuffer;
	return cb;
}

//	class Panel
Panel::Panel(int x, int y)
{
	cells.clear();
	cells.resize(x*y);
	xSize = x;
	ySize = y;
}

Cell& Panel::cellAt(int x, int y)
{
	return cells[y*xSize + x];
}

int Panel::cxSize()
{
	return xSize;
}

int Panel::cySize()
{
	return ySize;
}

bool Panel::resize(int x, int y)
{
	bool result = false;
	cells.clear();
	cells.resize(x * y);
	xSize = x;
	ySize = y;
	return true;
}

Panel Panel::getPanel(int x, int y, int cx, int cy)
{
	Panel result(cx,cy);
	for(int i = 0; i < cx; i++)
		for(int j = 0; j < cy; j++)
		{
			int xThisPanel = x - cx / 2 + i;
			int yThisPanel = y - cy / 2 + j;
			if(xThisPanel >= 0 && xThisPanel < xSize &&\
				yThisPanel >= 0 && yThisPanel < ySize)
			{
				result.cellAt(i, j) = this->cellAt(xThisPanel, yThisPanel);
			}
		}
	return result;
}

int Panel::operator==(const Panel& p)
{
	if(this->xSize != p.xSize || this->ySize != p.ySize)
		return -1;
	for(int i = 0; i < this->cells.size(); i++)
	{
		if(this->cells[i] != p.cells[i])
			return -2;
	}
	return 1;
}


