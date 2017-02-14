#pragma once

#include <DirectXMath.h>
#include <vector>
#include "Mesh.h"
#include "Material.h"

class Entity
{
public:
	Entity(Mesh *mesh, Material *material);
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

	void Update();
	void PrepareMaterial(DirectX::XMFLOAT4X4 _viewMatrix, DirectX::XMFLOAT4X4 _projectionMatrix);

private:
	Mesh *mesh;
	Material *material;

	DirectX::XMFLOAT4X4 _worldMatrix;
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _rotation;
	DirectX::XMFLOAT3 _scale;

	bool dirty;
};


