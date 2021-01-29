#pragma once

#include<memory>

class GameObject;
class Transform;

class Component
{
public:
	Component(std::shared_ptr<GameObject> gameObject, std::shared_ptr<Transform> transform);
	virtual ~Component() = default;

protected:

	std::shared_ptr<Transform> m_transform;
	std::shared_ptr<GameObject> m_gameObject;
};

