#pragma once

#include<memory>

class GameObject;
class Transform;

template<typename T>
using Ref = std::shared_ptr<T>;

class Component
{
	friend GameObject;
public:
	Component();
	virtual ~Component() = default;
	Ref<GameObject> getGameObject() const;

protected:

	Ref<Transform> m_transform;
	Ref<GameObject> m_gameObject;

private:
	void setTransform(Ref<Transform> transform);
	void setGameObject(Ref<GameObject> gameObject);
};

