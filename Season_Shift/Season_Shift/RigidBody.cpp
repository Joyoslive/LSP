#include "RigidBody.h"
#include <assert.h>


RigidBody::RigidBody(float mass)
{
	m_mass = mass;
	assert(m_mass > 0);

	m_force.x = 0;
	m_force.y = 0;
	m_force.z = 0;

	m_acceleration.x = 0;
	m_acceleration.y = 0;
	m_acceleration.z = 0;
	
	m_velocity.x = 0;
	m_velocity.y = 0;
	m_velocity.z = 0;

}

void RigidBody::setMass(float mass)
{
	m_mass = mass;
	assert(m_mass > 0);
}

float RigidBody::getMass() const
{
	return m_mass;
}

void RigidBody::addForce(DirectX::SimpleMath::Vector3 force)
{
	m_force += force;
}

void RigidBody::stop()
{
	m_force.x = 0;
	m_force.y = 0;
	m_force.z = 0;

	m_acceleration.x = 0;
	m_acceleration.y = 0;
	m_acceleration.z = 0;

	m_velocity.x = 0;
	m_velocity.y = 0;
	m_velocity.z = 0;
}
