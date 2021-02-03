#include "Component.h"


Component::Component()
{
	m_componentType = (ComponentEnum)0;
}

Ref<GameObject> Component::getGameObject() const
{
	return m_gameObject;
}

Component::ComponentEnum Component::getType() const
{
	return m_componentType;
}

void Component::setTransform(Ref<Transform> transform)
{
	m_transform = transform;
}

void Component::setGameObject(Ref<GameObject> gameObject)
{
	m_gameObject = gameObject;
}
