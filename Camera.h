#pragma once

#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMFLOAT4X4 getViewMatrix();
	DirectX::XMFLOAT4X4 getProjectionMatrix();

	DirectX::XMFLOAT3 getPosition();
	void setPosition(const DirectX::XMFLOAT3 &position);

	DirectX::XMFLOAT3 getDirection();
	void setDirection(const DirectX::XMFLOAT3 &direction);

	void rotateByMouse(float x, float y);
	void rotateX(float x);
	void rotateY(float y);

	void setAspectRatio(float ratio);

	void update(float deltaTime, float totalTime);

private:

	DirectX::XMFLOAT4X4 _viewMatrix;
	DirectX::XMFLOAT4X4 _projectionMatrix;
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _direction;
	DirectX::XMFLOAT3 _up;

	float _rotationX;
	float _rotationY;
};

