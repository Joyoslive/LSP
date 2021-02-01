#include "Component.h"

Component::Component(Ref<GameObject> gameObject, Ref<Transform> transform)
{
	m_gameObject = gameObject;
}

void Component::setTransform(Ref<Transform> transform)
{

}

void Component::setGameObject(Ref<GameObject> gameObject)
{
}
