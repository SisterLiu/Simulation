#pragma once
#include <vector>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "../Display/model.h"

typedef class CellBuffer
{
	public:
	struct CellVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texture;
	};
	ID3D11Buffer*	pVertexBuffer;
	ID3D11Buffer*	pIndexBuffer;
	unsigned int iVertex;
	unsigned int iIndex;

	D3D_PRIMITIVE_TOPOLOGY layout = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	HRESULT CreateBuffer(_In_ ID3D11Device* pDevice);

} CELLBUFFER, *PCELLBUFFER;

typedef class Cell : public Block
{
	public:
	unsigned int val;
	unsigned int type;

	Cell();
	bool operator != (const Cell& b);
	Cell& operator = (const CellBuffer& cb);
	
	void setCellBuffer(const CellBuffer& cb);
	CellBuffer getCellBuffer();

	static const int BLACK	= 0x0001;
	static const int WHITE	= 0x0002;
	static const int YELLOW = 0x0004;
	static const int BLUE	= 0x0008;
	static const int RED	= 0x0010;
	static const int GREEN	= 0x0011;

	private:

} CELL, *PCELL;

typedef class Panel
{
	public:
	Cell& cellAt(int x, int y);
	int cxSize();
	int cySize();
	Panel(int x = 5, int y = 5);
	bool resize(int x, int y);
	Panel getPanel(int x, int y, int cx, int cy);
	int operator == (const Panel& p);

	//Panel(const Panel& copy);

	private:
	std::vector<Cell> cells;
	int xSize;
	int ySize;


} PANEL, *PPANEL;

