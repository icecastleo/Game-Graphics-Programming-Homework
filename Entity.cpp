#include "Entity.h"
#include "SimpleShader.h"

using namespace DirectX;

Entity::Entity(Mesh *mesh, Material *material) :mesh(mesh), material(material)
{
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
	return mesh;
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
	// update world matrix
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

void Entity::PrepareMaterial(DirectX::XMFLOAT4X4 _viewMatrix, DirectX::XMFLOAT4X4 _projectionMatrix)
{
	SimpleVertexShader *vertexShader = material->getVertexShader();
	SimplePixelShader *pixelShader = material->getPixelShader();

	vertexShader->SetMatrix4x4("world", _worldMatrix);
	vertexShader->SetMatrix4x4("view", _viewMatrix);
	vertexShader->SetMatrix4x4("projection", _projectionMatrix);

	pixelShader->SetShaderResourceView("diffuseTexture", material->getShaderResourceView());
	pixelShader->SetSamplerState("basicSampler", material->getSamplerState());

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();
	pixelShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();
}
