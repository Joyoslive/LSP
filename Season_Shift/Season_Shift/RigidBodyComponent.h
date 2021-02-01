#pragma once
#include "Component.h"

class RigidBodyComponent : public Component
{
private:
	float m_mass = 1;
	float m_gravity = 10;
	float m_force[3] = { 0,0,0 };
	float m_velocity[3] = { 0,0,0 };
	float m_acceleration[3] = { 0,0,0 };
	


public:
	RigidBodyComponent();
	void setMass(float mass);
	float getMass() const;

};

