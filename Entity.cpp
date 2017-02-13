#include "Entity.h"

using namespace DirectX;

Entity::Entity(Mesh *mesh)
{
	_mesh = mesh;

	XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity());

	_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	dirty = false;
}

Entity::~Entity()
{
	
}

Mesh * Entity::getMesh()
{
	return _mesh;
}

DirectX::XMFLOAT4X4 Entity::getWorldMatrix()
{
	return _worldMatrix;
}

void Entity::setWorldMatrix(const XMFLOAT4X4 &worldMatrix)
{
	_worldMatrix = worldMatrix;
}

DirectX::XMFLOAT3 Entity::getPosition()
{
	return _position;
}

void Entity::setPosition(const XMFLOAT3 &position)
{
	_position = position;
	dirty = true;
}

DirectX::XMFLOAT3 Entity::getRotation()
{
	return _rotation;
}

void Entity::setRotation(const XMFLOAT3 &rotation)
{
	_rotation = rotation;
	dirty = true;
}

DirectX::XMFLOAT3 Entity::getScale()
{
	return _scale;
}

void Entity::setScale(const XMFLOAT3 &scale)
{
	_scale = scale;
	dirty = true;
}

void Entity::Move(float x, float y, float z)
{
	XMStoreFloat3(&_position, XMLoadFloat3(&_position) + XMVectorSet(x, y, z, 0));
	dirty = true;
}

//void Entity::MoveFoward(float x, float y, float z)
//{
//	XMStoreFloat3(&_Position, XMLoadFloat3(&_Position) *  + XMVectorSet(x, y, z, 0));
//	dirty = true;
//}

void Entity::Update()
{
	if (dirty) {
		XMMATRIX trans = XMMatrixTranslationFromVector(XMLoadFloat3(&_position));
		XMMATRIX rot = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_rotation));
		XMMATRIX scale = XMMatrixScalingFromVector(XMLoadFloat3(&_scale));

		// Combine together
		// Same as calling XMMatrixMultiply twice
		XMMATRIX world = scale * rot * trans;

		XMStoreFloat4x4(&_worldMatrix, XMMatrixTranspose(world)); // Transpose for HLSL!

		dirty = false;
	}
}
