#include "RigidBodyComponent.h"


using std::shared_ptr;

RigidBodyComponent::RigidBodyComponent(shared_ptr<GameObject> gameObject, shared_ptr<Transform> transform) : Component(gameObject, transform)
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
