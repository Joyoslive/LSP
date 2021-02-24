#pragma once
#include "Logic.h"
#include "CameraComponent.h"

class PlayerCameraMovement : public Logic
{
private:
	Ref<CameraComponent> m_playerCamera;
	float m_pitch, m_yaw, m_roll;
	float m_frameTime;
	float m_goToRoll;
public:
	PlayerCameraMovement() = default;
	~PlayerCameraMovement() = default;

	void start() override;
	void update() override;
	//void updatePlayerCamera();
	void resetCamera();
	void updateFrameTime(const float& frameTime);

	void wallRunning(const bool& wallRunning, const DirectX::SimpleMath::Vector3& normal);
	void shake(DirectX::SimpleMath::Vector3 velocity, const DirectX::SimpleMath::Vector3& normal);
};

