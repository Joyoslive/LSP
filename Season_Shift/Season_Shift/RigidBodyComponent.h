#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <SimpleMath.h>

class RigidBodyComponent : public Component
{
private:
	float m_mass = 1;
	float m_gravity = 10;
	DirectX::SimpleMath::Vector3 m_force;
	DirectX::SimpleMath::Vector3 m_velocity;
	DirectX::SimpleMath::Vector3 m_acceleration;
	


public:
	RigidBodyComponent(float mass = 1);
	void setMass(float mass);
	float getMass() const;

};

