#include "RigidBody.h"
#include <assert.h>

using namespace DirectX::SimpleMath;

RigidBody::RigidBody(float mass)
{
	m_mass = mass;
	assert(m_mass > 0);

	m_force = Vector3(0,0,0);
	m_acceleration = Vector3(0, 0, 0);
	m_velocity = Vector3(0, 0, 0);
	m_pendelLength = 0;
	m_pendelMotion = false;
	m_pendelPoint = Vector3(0, 0, 0);

	m_componentType = ComponentEnum::RIGID_BODY;
}

void RigidBody::setMass(float mass)
{
	m_mass = mass;
	assert(m_mass > 0);
}

void RigidBody::setGravity(float g)
{
	m_gravity = g;
}

float RigidBody::getMass() const
{
	return m_mass;
}

float RigidBody::getGravity() const
{
	return m_gravity;
}

const DirectX::SimpleMath::Vector3& RigidBody::getForce() const
{
	return m_force;
}

void RigidBody::addForce(DirectX::SimpleMath::Vector3 force)
{
	m_force += force;
}

void RigidBody::setVelocity(const DirectX::SimpleMath::Vector3& velocity)
{
	m_velocity = velocity;
}

const DirectX::SimpleMath::Vector3& RigidBody::getVelocity() const
{
	return m_velocity;
}

void RigidBody::removeForces()
{
	m_force = Vector3(0, 0, 0);
}

void RigidBody::stop()
{
	/*m_force = Vector3(0, 0, 0);
	m_acceleration = Vector3(0, 0, 0);
	m_velocity = Vector3(0, 0, 0);*/

	m_force = Vector3(0, 0, 0);
	m_acceleration = Vector3(0, 0, 0);
	m_velocity = Vector3(m_velocity.x, 0, m_velocity.z);
}

void RigidBody::startPendelMotion(const DirectX::SimpleMath::Vector3& pendelPoint, float length)
{
	m_pendelMotion = true;
	m_pendelPoint = pendelPoint;
	m_pendelLength = length;
}

void RigidBody::stopPendelMotion()
{
	m_pendelMotion = false;
}

void RigidBody::update()
{
	m_force = Vector3(0, 0, 0);
}
