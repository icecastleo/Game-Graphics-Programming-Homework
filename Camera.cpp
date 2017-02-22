#include "Camera.h"
#include <Windows.h>
#include <algorithm>

using namespace DirectX;

Camera::Camera()
{
	_position = XMFLOAT3(1.5f, 0, -10.0f);
	_direction = XMFLOAT3(0, 0, 1);
	_up = XMFLOAT3(0, 1, 0);

	_rotationX = 0.0f;
	_rotationY = 0.0f;
}

Camera::~Camera()
{
}

DirectX::XMFLOAT4X4 Camera::getViewMatrix()
{
	return _viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::getProjectionMatrix()
{
	return _projectionMatrix;
}

DirectX::XMFLOAT3 Camera::getPosition()
{
	return _position;
}

void Camera::setPosition(const DirectX::XMFLOAT3 & position)
{
	_position = position;
}

DirectX::XMFLOAT3 Camera::getDirection()
{
	return _direction;
}

void Camera::setDirection(const DirectX::XMFLOAT3 & direction)
{
	_direction = direction;
}

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
	return clamp(v, lo, hi, std::less<>());
}

template<class T, class Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp)
{
	return assert(!comp(hi, lo)),
		comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

void Camera::rotateByMouse(float x, float y)
{
	rotateX(y / 1000);
	rotateY(x / 1000);
}

void Camera::rotateX(float x)
{
	_rotationX = clamp<float>(_rotationX + x, -0.4f, 0.4f);
}

void Camera::rotateY(float y)
{
	_rotationY = clamp<float>(_rotationY + y, -0.8f, 0.8f);
}

void Camera::setAspectRatio(float ratio)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		ratio,						// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&_projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Camera::update(float deltaTime, float totalTime)
{
	XMVECTOR pos = XMLoadFloat3(&_position);

	//XMMATRIX rotate = XMMatrixRotationX(_rotationX) * XMMatrixRotationY(_rotationY);
	//XMVECTOR dir = XMVector3Transform(XMLoadFloat3(&_direction), rotate);
	XMVECTOR dir = XMVector3Rotate(XMLoadFloat3(&_direction), XMQuaternionRotationRollPitchYaw(_rotationX, _rotationY, 0));
	XMVECTOR up = XMLoadFloat3(&_up);

	// Move camera first
	if (GetAsyncKeyState('W') & 0x8000) {
		pos = XMVectorAdd(pos, dir * deltaTime);
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		pos = XMVectorAdd(pos, -dir * deltaTime);
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		pos = XMVectorAdd(pos, XMVector3Cross(dir, up) * deltaTime);
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		pos = XMVectorAdd(pos, -XMVector3Cross(dir, up) * deltaTime);
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		pos = XMVectorAdd(pos, XMVector3Cross(dir, up) * deltaTime);
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		pos = XMVectorAdd(pos, -XMVector3Cross(dir, up) * deltaTime);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		pos = XMVectorAdd(pos, up * deltaTime);
		//MessageBox(NULL, "Spacebar pressed!", "TEST", MB_OK);
	}

	if (GetAsyncKeyState('X') & 0x8000) {
		pos = XMVectorAdd(pos, -up * deltaTime);
	}

	XMStoreFloat3(&_position, pos);

	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&_viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
}