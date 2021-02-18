#pragma once
#include "Logic.h"
#include "Input.h"
#include "CameraComponent.h"
#include "Timer.h"

class Player : public Logic
{
private:
	Ref<CameraComponent> m_playerCamera;
	Ref<RigidBody> m_rb;
	Ref<CapsuleCollider> m_capsuleCollider;
	Timer m_timer;
	float m_pitch, m_yaw, m_roll;
	DirectX::SimpleMath::Vector3 m_normal;

	DirectX::SimpleMath::Vector3 respawn;
	bool m_disable;
	bool m_ground;
	bool m_doubleJump;
	bool m_walljump;
	bool m_fly;
	long double m_frameTime;
	float m_speed;
	float m_maxSpeed, m_maxGroundSpeed, m_maxFlySpeed, m_minSpeed, m_groundSpeed, m_flySpeed, m_jumpSpeed, m_doubleJumpSpeed;
	float m_dashSpeed;
	float m_baseFlySpeed, m_baseGroundSpeed;
	float m_maxSpeedRetardation;
	float m_maxAntiMoveSize, m_minAntiMoveSize;
	float m_jetPackFuel;
	float m_jetPackFuelMax;
	float m_jetPackSpeed;
	float m_cooldownDash;
	bool m_waitForJump, m_checkCollideJump, m_jumpWhenLanding;
	long double m_oldFrameTime;

	DirectX::SimpleMath::Vector3 m_oldMoveDirection;
	float m_lerp;

private:
	void lookAround();
	void detectDeath(float death);
	DirectX::SimpleMath::Vector3 antiMovement(DirectX::SimpleMath::Vector3 velocity, const DirectX::SimpleMath::Vector3& moveDirection, const bool& onGround);
	DirectX::SimpleMath::Vector3 checkMaxSpeed(DirectX::SimpleMath::Vector3 velocity);
	DirectX::SimpleMath::Vector3 checkMinSpeed(const DirectX::SimpleMath::Vector3& velocity);
	DirectX::SimpleMath::Vector3 checkDirection(DirectX::SimpleMath::Vector3 velocity, const DirectX::SimpleMath::Vector3& moveDirection, const bool& onGround);
	void checkSpeeds(const DirectX::SimpleMath::Vector3& moveDirection);
	DirectX::SimpleMath::Vector3 jumpPlayer(DirectX::SimpleMath::Vector3 velocity);
	DirectX::SimpleMath::Vector3 dash(DirectX::SimpleMath::Vector3 velocity, DirectX::SimpleMath::Vector3 cameraLook);
	void gravityChange(const DirectX::SimpleMath::Vector3& velocity);
	void getTime(std::wstring msg);
	void wallRunning(const DirectX::SimpleMath::Vector3& moveDirection);
	DirectX::SimpleMath::Vector3 playerFly(DirectX::SimpleMath::Vector3 velocity);
	DirectX::SimpleMath::Vector3 slowPlayer(DirectX::SimpleMath::Vector3 velocity);
public:
	Player();
	~Player();

	void start() override;
	void update() override;
	void onCollision(Ref<Collider> collider) override;
	void setRespawn(DirectX::SimpleMath::Vector3 incomingRespawn);
	void setFrametime(long double dt);
	void setWaitForJump();
	bool getOnGround();
};
