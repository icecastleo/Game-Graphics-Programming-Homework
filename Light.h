#pragma once

#include <DirectXMath.h>

struct DirectionalLight
{
	DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Direction;
	float padding;
};

struct PointLight
{
	DirectX::XMFLOAT4 PointLightColor;
	DirectX::XMFLOAT3 PointLightPosition;
	float padding;
	DirectX::XMFLOAT3 CameraPosition;
};

//class Light
//{
//public:
//	Light();
//	~Light();
//};
//
