#pragma once
#include "Camera.h"
#include "Input.h"

class DebugCamera {
private:
	Camera camera;
public:
	DebugCamera();
	~DebugCamera();


	void Move();
	void Rotate();
};