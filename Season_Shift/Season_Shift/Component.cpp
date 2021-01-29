#include "Component.h"

Component::Component(std::shared_ptr<GameObject> gameObject)
{
	m_gameObject = gameObject;
}
