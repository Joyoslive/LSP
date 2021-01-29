#pragma once
#include "Component.h"
#include <vector>
#include <typeinfo>
#include <optional>

class GameObject
{
private:
	std::vector<std::shared_ptr<Component>> m_componentVector;

public:
	
	/*template<typename T>
	std::shared_ptr<T> getComponentType()
	{
		for (auto &c : m_componentVector)
		{
			if (typeid(T) == typeid(*c))
			{
				return c;
			}
		}
		return nullptr;
	}*/
	template<typename T>
	std::optional<T&> getComponentType()
	{
		for (auto& c : m_componentVector)
		{
			if (typeid(T) == typeid(*c))
			{
				return *c;
			}
		}
		return std::nullopt;
	}

	void start()
	{

	}
	void update()
	{

	}

};

