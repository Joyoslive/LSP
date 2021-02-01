#include "GameObject.h"
#include <typeinfo>
#include "RigidBodyComponent.h"


class Transform;
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

void GameObject::testAdd()
{
	m_componentVector.emplace_back(std::make_shared<RigidBodyComponent>());
	std::dynamic_pointer_cast<RigidBodyComponent>(m_componentVector[0])->setMass(42.0f);
}

int GameObject::AddComponent(Ref<Component> component)
{
	//if (typeid(component) == typeid(m_transform))
	//{
	//	if (m_transform == nullptr)
	//	{
	//		m_transform = std::dynamic_pointer_cast<Transform>(component);
	//	}
	//	else
	//	{
	//		return -1; // only one transform is allowed
	//	}
	//}
	//else
	//{
	//	//m_componentVector.push_back(component);
	//}
	component->setGameObject(shared_from_this());
	component->setTransform(m_transform);
	return 0;
}

Ref<Transform> GameObject::getTransform()
{
	return m_transform;
}
