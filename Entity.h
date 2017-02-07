#pragma once

//#include "DXCore.h"
#include <DirectXMath.h>
#include <vector>

#include "Mesh.h"

#define GETSET(type, var) \
private: \
	type _##var; \
public: \
	type Get##var() \
	{\
		return _##var; \
	}\
	void Set##var(type val) \
	{\
		_##var = val; \
	}

class Entity
{
public:
	Entity(Mesh *mesh);
	~Entity();

	GETSET(DirectX::XMFLOAT4X4, WorldMatrix)

	GETSET(DirectX::XMFLOAT3, Position)
	GETSET(DirectX::XMFLOAT3, Rotation)
	GETSET(DirectX::XMFLOAT3, Scale)

};


