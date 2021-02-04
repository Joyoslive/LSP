#pragma once
#include "Camera.h"
#include "Input.h"
#include <SimpleMath.h>

class DebugCamera {
private:
	Camera camera;
	Input input;
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR forward;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR up;
	DirectX::SimpleMath::Vector2 mousePos;
	float speed;
	float pitch, yaw, roll;
public:
	DebugCamera(HWND wndHandle);
	~DebugCamera();


	void Move();
	void Rotate();
};