#pragma once
#include "Logic.h"
#include "Input.h"
#include "Sound.h"
//#include "CameraComponent.h"
#include "Timer.h"
#include "PlayerCameraMovement.h"
#include "Audio.h"

class CameraComponent;
class Collider;
class CapsuleCollider;
class RigidBody;
class ISprite;
class ParticleSystemComponent;

class Player : public Logic
{
private:
	Ref<CameraComponent> m_playerCamera;
	Ref<RigidBody> m_rb;
	Ref<CapsuleCollider> m_capsuleCollider;
	Ref<Collider> m_oldCollider;
	Ref<Collider> m_oldTrampolineCollider;
	Ref<Collider> m_oldCheckpointCollider;
	Ref<PlayerCameraMovement> m_logicPlayerCamera;
	Timer m_timer;
	Timer m_goalTimer;
	Audio m_audio;
	Timer m_copyGoalTimer;
	float m_currentTime;

	DirectX::SimpleMath::Vector3 m_normal;

	DirectX::SimpleMath::Vector3 m_respawn;
	bool m_disable;
	bool m_ground;
	bool m_doubleJump;
	bool m_walljump;
	bool m_soundLoop;
	bool m_soundLoopG;
	bool m_soundLoopW;
	bool m_soundLoopA;
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
	long double m_wallTimer;
	long double m_trampolineTimer;
	bool m_hooked;
	bool m_collisionFrame;
	bool m_movObj;
	DirectX::SimpleMath::Vector3 m_movSpeed;
	float m_hookDist;
	float m_velocityY;
	float m_movPos;
	float m_movAlt;
	float m_maxYSpeed;
	bool m_trampoline;
	DirectX::SimpleMath::Vector3  m_trampolineAngle;
	float m_trampolinePower;
	//Speedlines
	float m_sLT, m_sLR, m_sLS;
	DirectX::SimpleMath::Vector3 m_deltaPos;
	DirectX::SimpleMath::Vector3 m_hookPoint;

	DirectX::SimpleMath::Vector3 m_oldMoveDirection;
	DirectX::SimpleMath::Vector3 m_oldVelocity; //velocity from last frame

	DirectX::SimpleMath::Vector3 m_hookEndPos;

	DirectX::SimpleMath::Vector3 m_playerStartPosition;


	bool m_createOnce = true;
	Ref<ParticleSystemComponent> m_playerPartSys;
	int m_landingPartEmittId;
	Ref<ParticleSystemComponent> m_playerPartSys2;

	Ref<GameObject> m_hookObject;
	Ref<ParticleSystemComponent> m_hookPartSys;
	int m_hookEmittId;

private:
	void detectDeath(float death);
	DirectX::SimpleMath::Vector3 antiMovement(DirectX::SimpleMath::Vector3 velocity, const DirectX::SimpleMath::Vector3& moveDirection, const bool& onGround);
	DirectX::SimpleMath::Vector3 checkMaxSpeed(DirectX::SimpleMath::Vector3 velocity);
	DirectX::SimpleMath::Vector3 checkYMaxSpeed(DirectX::SimpleMath::Vector3 velocity);
	DirectX::SimpleMath::Vector3 checkMinSpeed(const DirectX::SimpleMath::Vector3& velocity);
	DirectX::SimpleMath::Vector3 checkDirection(DirectX::SimpleMath::Vector3 velocity, const DirectX::SimpleMath::Vector3& moveDirection, const bool& onGround);
	void checkSpeeds(const DirectX::SimpleMath::Vector3& moveDirection);
	DirectX::SimpleMath::Vector3 jumpPlayer(DirectX::SimpleMath::Vector3 velocity);
	DirectX::SimpleMath::Vector3 dash(DirectX::SimpleMath::Vector3 velocity, DirectX::SimpleMath::Vector3 cameraLook);
	void gravityChange(const DirectX::SimpleMath::Vector3& velocity);
	void wallRunning(const DirectX::SimpleMath::Vector3& moveDirection);
	DirectX::SimpleMath::Vector3 playerFly(DirectX::SimpleMath::Vector3 velocity);
	DirectX::SimpleMath::Vector3 slowPlayer(DirectX::SimpleMath::Vector3 velocity);
	void speedLines(const DirectX::SimpleMath::Vector3& velocityXZ, const float& velocityY);
	float lerp(float a, float b, float f);
	void grappleHook(DirectX::SimpleMath::Vector3 cameraLook);
	DirectX::SimpleMath::Vector3 moveObjectCheck(DirectX::SimpleMath::Vector3 moveDirection2);
	void drawLine();
	void updateSprites(const DirectX::SimpleMath::Vector3& velocity);
public:
	Player();
	~Player();

	void start() override;
	void update() override;
	void onCollision(Ref<Collider> collider) override;
	void setRespawn(const DirectX::SimpleMath::Vector3& incomingRespawn);
	void setFrametime(long double dt);
	void setWaitForJump();
	void clearJumpFromTrigger();
	bool getOnGround();
	void getTime(std::wstring msg);
	long double goalTimerGetTime();
};
