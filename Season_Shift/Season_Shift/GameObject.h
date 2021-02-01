#pragma once
#include "Component.h"
#include "RigidBodyComponent.h"
#include <vector>
#include <memory>

class Transform;

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	std::vector<Ref<Component>> m_componentVector;
	Ref<Transform> m_transform;

public:
	GameObject();
	~GameObject();

	template<typename T>
	Ref<T> getComponentType()
	{
		for (auto &c : m_componentVector)
		{
			if (typeid(T) == typeid(*c))
			{
				return std::dynamic_pointer_cast<T>(c);
			}
		}
		return nullptr;
	}

	void start();
	void update();

	void testAdd();
	int AddComponent(Ref<Component> component);
	Ref<Transform> getTransform();
};

