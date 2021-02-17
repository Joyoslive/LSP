#pragma once
#include "Camera.h"
#include "Input.h"
#include <SimpleMath.h>

class DebugCamera {
private:
	std::shared_ptr<Camera> m_camera;
	//Camera camera;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_up;
	DirectX::SimpleMath::Vector2 mousePos;
	float m_speed;
	float m_pitch, m_yaw, m_roll;
	bool m_freecamMode;
	bool m_lock;
	long double m_frameTime;
public:
	DebugCamera(std::shared_ptr<Camera> incomingCamera);
	~DebugCamera();


	void move(long double dt);
	void rotate();
};