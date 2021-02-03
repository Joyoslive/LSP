#pragma once
#include "Camera.h"
#include "Input.h"

class DebugCamera {
private:
	Camera camera
public:
	void Move();
	void Rotate();
};