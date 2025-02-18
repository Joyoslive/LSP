#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <SimpleMath.h>

class PhysicsEngine;

class RigidBody : public Component
{
	friend PhysicsEngine;
private:
	float m_mass = 1;
	float m_gravity = 10;
	DirectX::SimpleMath::Vector3 m_force;
	DirectX::SimpleMath::Vector3 m_velocity;
	DirectX::SimpleMath::Vector3 m_acceleration;
	DirectX::SimpleMath::Vector3 m_pendelPoint;
	bool m_pendelMotion;
	float m_pendelLength;
	
private:
	void update() override;

public:
	RigidBody(float mass = 1);
	void setMass(float mass);
	void setGravity(float g);
	float getMass() const;
	float getGravity() const;
	const DirectX::SimpleMath::Vector3& getForce() const;
	void addForce(DirectX::SimpleMath::Vector3 force);
	void setVelocity(const DirectX::SimpleMath::Vector3& velocity);
	const DirectX::SimpleMath::Vector3& getVelocity() const;
	void removeForces();
	void stop();
	void startPendelMotion(const DirectX::SimpleMath::Vector3& pendelPoint, float length);
	void stopPendelMotion();
	
};

