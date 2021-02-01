#include "Component.h"

Component::Component()
{

}

Ref<GameObject> Component::getGameObject() const
{
	return m_gameObject;
}

void Component::setTransform(Ref<Transform> transform)
{
	m_transform = transform;
}

void Component::setGameObject(Ref<GameObject> gameObject)
{
	m_gameObject = gameObject;
}
