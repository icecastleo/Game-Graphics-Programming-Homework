#pragma once

#include <d3d11.h>
#include "Vertex.h"

class Mesh
{
public:
	Mesh(Vertex *vertices, int vertexCount, UINT *indices, int indexCount, ID3D11Device *device);
	Mesh(char* objFile, ID3D11Device * device);
	~Mesh();

	ID3D11Buffer *GetVertexBuffer();
	ID3D11Buffer *GetIndexBuffer();
	int GetIndexCount();

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int indexCount;

	void init(Vertex *vertices, int vertexCount, UINT *indices, int indexCount, ID3D11Device *device);
};

