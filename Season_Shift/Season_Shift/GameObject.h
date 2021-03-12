#pragma once
#include "Component.h"
#include "Transform.h"
//#include "RigidBody.h"
//#include "Logic.h"
//#include "Collider.h"
//#include "SphereCollider.h"
//#include "OrientedBoxCollider.h"
//#include "CapsuleCollider.h"
//#include "Sound.h"
//#include "Graphics/Model.h"
#include "Scene.h"
#include <vector>
#include <memory>
#include <string>

class Transform;
class Scene;

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	std::vector<Ref<Component>> m_componentVector;
	Ref<Transform> m_transform;
	std::string m_name;
	//Ref<Scene> m_myScene;
	Scene* m_myScene;
	bool m_isVisible;

public:
	GameObject(std::string name = "", Ref<Scene> scene = nullptr);
	~GameObject();

	template<typename T>
	Ref<T> getComponentType(Component::ComponentEnum componentType)
	{
		if ((int)(componentType & Component::ComponentEnum::TRANSFORM))
		{
			return std::dynamic_pointer_cast<T>(m_transform);
		}
		else
		{
			for (auto& c : m_componentVector)
			{
				if ((int)(componentType & c->m_componentType))
				{
					return std::dynamic_pointer_cast<T>(c);
				}
			}
			return nullptr;
		}
	}

	template<typename T>
	std::vector<Ref<T>> getMultipleComponentType(Component::ComponentEnum componentType)
	{	
		std::vector<Ref<T>> vec;
		for (auto& c : m_componentVector)
		{
			if ((int)(componentType & c->m_componentType))
			{
				vec.push_back(std::dynamic_pointer_cast<T>(c));
			}
		}
		return vec;
		
	}

	void start();
	void update();

	void clearGameObject();
	void setIsVisible(bool isVisible);
	bool getIsVisible() const;
	const Ref<Transform>& getTransform() const;
	const std::string& getName() const;
	Scene* getScene() const;

	const Ref<Component>& AddComponent(Ref<Component> component);
};

