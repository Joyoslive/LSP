#include "GameObject.h"
#include <typeinfo>
#include "RigidBodyComponent.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}




void GameObject::start()
{
}

void GameObject::update()
{
}

void GameObject::testAdd() // need to fix this templated
{
	m_componentVector.emplace_back(std::make_shared<RigidBodyComponent>(shared_from_this(), nullptr));
	std::dynamic_pointer_cast<RigidBodyComponent>(m_componentVector[0])->setMass(42.0f);
}