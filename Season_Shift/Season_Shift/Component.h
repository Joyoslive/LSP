#pragma once

#include<memory>

class GameObject;
class Transform;

class Component
{
public:
	Component(std::shared_ptr<GameObject> gameObject);

protected:

	std::shared_ptr<Transform> m_transform;
	std::shared_ptr<GameObject> m_gameObject;
};

