#pragma once
#include "Component.h"
#include "RigidBodyComponent.h"
#include <vector>
#include <memory>

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	std::vector<std::shared_ptr<Component>> m_componentVector;
	std::vector<std::shared_ptr<RigidBodyComponent>> m_v;

public:
	GameObject();
	~GameObject();

	template<typename T>
	std::shared_ptr<T> getComponentType()
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
};

