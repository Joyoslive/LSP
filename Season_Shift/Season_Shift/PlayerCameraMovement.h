#pragma once
#include "Logic.h"
#include "CameraComponent.h"

class PlayerCameraMovement : public Logic
{
private:
	Ref<CameraComponent> m_playerCamera;
	float m_pitch, m_yaw, m_roll;
public:
	PlayerCameraMovement() = default;
	~PlayerCameraMovement() = default;

	void start() override;
	void update() override;
	//void updatePlayerCamera();
	void resetCamera();

	void wallRunning(const bool& wallRunning, const DirectX::SimpleMath::Vector3& normal, const float& frameTime);
};

