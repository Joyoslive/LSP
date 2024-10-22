#pragma once
#include "Logic.h"
#include <d3d11.h>
#include <SimpleMath.h>
//#include "CameraComponent.h"
class CameraComponent;

class PlayerCameraMovement : public Logic
{
private:
	Ref<CameraComponent> m_playerCamera;
	float m_pitch, m_yaw, m_roll;
	float m_frameTime;
	float m_goToRoll;
	float m_runRoll;
	float m_direction;
	float m_camMoveDirection;
	bool m_stop;
	bool m_landShake;
	bool m_rollLandShake;
	bool m_runShake;
	bool m_rollSecondTime;

	bool m_runMoveY;
	float m_goToY;
	float m_baseCamPosY;
	float m_camPosY;
	bool m_moveSecondTime;
	bool m_moveStop;
	bool m_moveLandShake;
	
	float m_cameraFov;
	float m_baseCameraFov;

	float m_velocityY;
	float temp = 50.0f;
private:
	void setDirection(const float& roll, const bool& camMove = false);
	void setGoToRoll(const bool& firstTime);
	void setRunRoll(const bool& firstTime);
	void setRunMoveY(const bool& firstTime, float changeYPos = 0.2f);
	void landShake();
	void runMoveY(const DirectX::SimpleMath::Vector3& moveDirection, const bool& onGround, const bool& wallRunning, const float& speed, const float& maxSpeed);
public:
	PlayerCameraMovement() = default;
	~PlayerCameraMovement() = default;

	void start() override;
	void update() override;
	//void updatePlayerCamera();
	void resetCamera();
	void updateFrameTime(const float& frameTime);

	void changeFOV(const DirectX::SimpleMath::Vector3& velocity, const float& maxSpeedXZ, const float& maxSpeedY);
	void wallRunning(const bool& wallRunning, const DirectX::SimpleMath::Vector3& normal);
	bool shake(DirectX::SimpleMath::Vector3 velocity, const DirectX::SimpleMath::Vector3& normal);
	bool land(DirectX::SimpleMath::Vector3 velocity, const DirectX::SimpleMath::Vector3& normal);
	void runShake(const DirectX::SimpleMath::Vector3& moveDirection, const bool& onGround, const bool& wallRunning, const float& speed, const float& maxSpeed);
};

