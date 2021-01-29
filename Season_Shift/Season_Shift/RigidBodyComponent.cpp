#include "RigidBodyComponent.h"


RigidBodyComponent::RigidBodyComponent(Ref<GameObject> gameObject, Ref<Transform> transform) : Component(gameObject, transform)
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
