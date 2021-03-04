#include "pch.h"
#include "GameObject.h"
#include "Logic.h"
#include "Transform.h"
#include <typeinfo>
#include <assert.h>


GameObject::GameObject(std::string name, Ref<Scene> scene)
{
	m_name = name;
	if (scene == nullptr)
		assert(false);
	m_myScene = scene;
	setIsVisible(true);
}

GameObject::~GameObject()
{
}


const std::string& GameObject::getName() const
{
	return m_name;
}

const Ref<Scene>& GameObject::getScene() const
{
	return m_myScene;
}

void GameObject::clearGameObject()
{
	for (int i = 0; i < m_componentVector.size(); ++i)
	{
		m_componentVector[i]->clearComponent();
		m_componentVector[i]->setGameObject(nullptr);
		m_componentVector[i]->setTransform(nullptr);
	}
	m_componentVector.clear();
	m_transform = nullptr;
}

void GameObject::setIsVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

bool GameObject::getIsVisible() const
{
	return m_isVisible;
}

void GameObject::start()
{
	for (auto& logic : getMultipleComponentType<Logic>(Component::ComponentEnum::LOGIC))
	{
		logic->start();
	}
}

void GameObject::update()
{
	std::dynamic_pointer_cast<Component>(m_transform)->update();
	for (auto& component : m_componentVector)
	{
		component->update();
	}
}

const Ref<Component>& GameObject::AddComponent(Ref<Component> component)
{
	if (typeid(*component) == typeid(Transform))
	{
		if (m_transform == nullptr)
		{
			m_transform = std::dynamic_pointer_cast<Transform>(component);
			if (m_componentVector.size() < 0)
			{
				for (auto &c : m_componentVector)
				{
					c->setTransform(m_transform);
				}
			}
		}
		else
		{
			assert(false); // only one transform is allowed
		}
	}
	m_componentVector.push_back(component);
	
	component->setGameObject(shared_from_this());
	component->setTransform(m_transform);

	component->initialize();
	return component;
}

const Ref<Transform>& GameObject::getTransform() const
{
	return m_transform;
}
