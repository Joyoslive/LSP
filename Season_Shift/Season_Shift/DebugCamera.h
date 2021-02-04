#pragma once
#include "Camera.h"
#include "Input.h"
#include <SimpleMath.h>

class DebugCamera {
private:
	std::shared_ptr<Camera> camera;
	//Camera camera;
	Input input;
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR up;
	DirectX::SimpleMath::Vector2 mousePos;
	float speed;
	float pitch, yaw, roll;
public:
	DebugCamera(HWND wndHandle, std::shared_ptr<Camera> incomingCamera);
	~DebugCamera();


	void Move();
	void Rotate();
};