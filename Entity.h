#pragma once

//#include "DXCore.h"
#include <DirectXMath.h>
#include <vector>

#include "Mesh.h"

//#define GETSET(type, var) \
//private: \
//	type _##var; \
//public: \
//	type Get##var() \
//	{\
//		return _##var; \
//	}\
//	void Set##var(type val) \
//	{\
//		_##var = val; \
//	}

class Entity
{
public:
	Entity(Mesh *mesh);
	~Entity();

	Mesh *getMesh();

	DirectX::XMFLOAT4X4 getWorldMatrix();
	void setWorldMatrix(const DirectX::XMFLOAT4X4 &worldMatrix);

	DirectX::XMFLOAT3 getPosition();
	void setPosition(const DirectX::XMFLOAT3 &position);

	DirectX::XMFLOAT3 getRotation();
	void setRotation(const DirectX::XMFLOAT3 &rotation);

	DirectX::XMFLOAT3 getScale();
	void setScale(const DirectX::XMFLOAT3 &scale);

	void Move(float x, float y, float z);
	//void MoveFoward(float x, float y, float z);

	// update world matrix
	void Update();

	/*GETSET(DirectX::XMFLOAT4X4, Worldmatrix)
	GETSET(DirectX::XMFLOAT3, Position)
	GETSET(DirectX::XMFLOAT3, Rotation)
	GETSET(DirectX::XMFLOAT3, Scale)*/

private:
	Mesh *_mesh;

	DirectX::XMFLOAT4X4 _worldMatrix;
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _rotation;
	DirectX::XMFLOAT3 _scale;

	bool dirty;
};


