#include "pch.h"
#include "Component.h"


Component::Component()
{
	m_componentType = (ComponentEnum)0;
}

Ref<GameObject> Component::getGameObject() const
{
	return m_gameObject;
}

Ref<Transform> Component::getTransform() const
{
	return m_transform;
}

Component::ComponentEnum Component::getType() const
{
	return m_componentType;
}

void Component::initialize()
{
}

void Component::update()
{

}

void Component::clearComponent()
{
}

void Component::setTransform(Ref<Transform> transform)
{
	m_transform = transform;
}

void Component::setGameObject(Ref<GameObject> gameObject)
{
	m_gameObject = gameObject;
}
