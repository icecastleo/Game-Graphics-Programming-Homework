#include "Entity.h"

using namespace DirectX;

Entity::Entity(Mesh * mesh)
{
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&_WorldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	_Position = XMFLOAT3(0, 0, 0);
	_Rotation = XMFLOAT3(0, 0, 0);
	_Scale = XMFLOAT3(0, 0, 0);
}

Entity::~Entity()
{
	
}
