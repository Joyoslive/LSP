#pragma once
#include "Logic.h"
#include "Input.h"
#include "CameraComponent.h"

class Player : public Logic
{
private:
	Ref<Input> m_input;
	Ref<CameraComponent> m_playerCamera;
	Ref<RigidBody> m_rb;
	float m_pitch, m_yaw, m_roll;
public:
	Player(const Ref<Input>& input);
	~Player();

	void start() override;
	void update() override;
	void onCollision(Ref<Collider> collider) override;
	void lookAround();
};
