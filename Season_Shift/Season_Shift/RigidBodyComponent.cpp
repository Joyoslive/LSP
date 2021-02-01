#include "RigidBodyComponent.h"


RigidBodyComponent::RigidBodyComponent()
{

}

void RigidBodyComponent::setMass(float mass)
{
	m_mass = mass;
}

float RigidBodyComponent::getMass() const
{
	return m_mass;
}
