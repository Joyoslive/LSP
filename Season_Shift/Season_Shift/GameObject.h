#pragma once
#include "Component.h"
#include <vector>
#include <optional>
#include <memory>

class GameObject : std::enable_shared_from_this<GameObject>
{
private:
	std::vector<std::shared_ptr<Component>> m_componentVector;

public:
	GameObject();
	~GameObject();
	
	template<typename T>
	std::optional<T&> getComponentType();

	void start();
	void update();

	void testAdd();
};

