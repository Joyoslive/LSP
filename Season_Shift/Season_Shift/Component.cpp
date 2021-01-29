#include "Component.h"

using std::shared_ptr;

Component::Component(shared_ptr<GameObject> gameObject, shared_ptr<Transform> transform)
{
	m_gameObject = gameObject;
}
