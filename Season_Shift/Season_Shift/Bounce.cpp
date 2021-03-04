#include "pch.h"
#include "Bounce.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "OrientedBoxCollider.h"
#include "CapsuleCollider.h"
#include "Transform.h"
#include "Input.h"

Bounce::Bounce()
{
}

Bounce::Bounce(DirectX::SimpleMath::Vector3 angle, float power)
{
	m_angle = angle;
	m_power = power;
}

Bounce::~Bounce()
{
}

DirectX::SimpleMath::Vector3 Bounce::getAngle() {
	return m_angle;
}

float Bounce::getPower() {
	return m_power;
}