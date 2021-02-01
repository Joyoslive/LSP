#pragma once
#include "Component.h"
#include "RigidBodyComponent.h"
#include "Transform.h"
#include "Logic.h"
#include "Collider.h"
#include <vector>
#include <memory>
#include <string>

class Transform;

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	std::vector<Ref<Component>> m_componentVector;
	Ref<Transform> m_transform;
	std::string m_name;

public:
	GameObject(std::string name = "");
	~GameObject();

	std::string getName() const;

	template<typename T>
	Ref<T> getComponentType()
	{
		if (typeid(T) == typeid(Transform))
		{
			return std::dynamic_pointer_cast<T>(m_transform);
		}
		else
		{
			for (auto& c : m_componentVector)
			{
				if (typeid(T) == typeid(*c))
				{
					return std::dynamic_pointer_cast<T>(c);
				}
			}
			return nullptr;
		}
	}

	template<typename T>
	std::vector<Ref<T>> getMultipleComponentType()
	{	
		std::vector<Ref<T>> vec;
		for (auto& c : m_componentVector)
		{
			if (typeid(T) == typeid(*c))
			{
				vec.push_back(std::dynamic_pointer_cast<T>(c));
			}
		}
		return vec;
		
	}

	void start();
	void update();

	int AddComponent(Ref<Component> component);
	Ref<Transform> getTransform();
};

