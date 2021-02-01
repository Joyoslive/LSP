#include "Component.h"

Component::Component()
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
