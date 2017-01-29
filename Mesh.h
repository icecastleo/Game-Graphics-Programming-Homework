#pragma once

#include <d3d11.h>
#include "Vertex.h"



class Mesh
{
public:
	Mesh(Vertex *vertices, int vertexCount, int *indices, int indexCount, ID3D11Device *device);
	~Mesh();

	ID3D11Buffer *GetVertexBuffer();
	ID3D11Buffer *GetIndexBuffer();
	int GetIndexCount();

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int indexCount;
};

