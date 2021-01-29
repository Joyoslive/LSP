#include "GameObject.h"
#include <typeinfo>

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}


template<typename T>
std::optional<T&> GameObject::getComponentType()
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



void GameObject::start()
{
}

void GameObject::update()
{
}

void GameObject::testAdd()
{
}



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