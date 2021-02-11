#pragma once
#include "Logic.h"
#include "Input.h"
#include "CameraComponent.h"

class Player : public Logic
{
private:
	Ref<CameraComponent> m_playerCamera;
	Ref<RigidBody> m_rb;
	float m_pitch, m_yaw, m_roll;

	DirectX::SimpleMath::Vector3 respawn;
	bool m_disable;
	bool m_ground;
	bool m_doubleJump;
	long double m_frameTime;
	float m_speed;
	float m_maxSpeed, m_minSpeed;
	float m_maxAntiMoveSize, m_minAntiMoveSize;
	float m_jetPackFuel;

private:
	void lookAround();
	void detectDeath(float death);
	const DirectX::SimpleMath::Vector3& antiMovement(DirectX::SimpleMath::Vector3 velocity);
	const DirectX::SimpleMath::Vector3& checkMaxSpeed(DirectX::SimpleMath::Vector3 velocity, const float& velocityY);

	const DirectX::SimpleMath::Vector3& antiMovement(DirectX::SimpleMath::Vector3 velocity, const DirectX::SimpleMath::Vector3& moveDirection);
	const DirectX::SimpleMath::Vector3& checkMaxSpeed(DirectX::SimpleMath::Vector3 velocity);
	const DirectX::SimpleMath::Vector3& checkMinSpeed(const DirectX::SimpleMath::Vector3& velocity);
public:
	Player();
	~Player();

	void start() override;
	void update() override;
	void onCollision(Ref<Collider> collider) override;
	void setRespawn(DirectX::SimpleMath::Vector3 incomingRespawn);
	void setFrametime(long double dt);
};
