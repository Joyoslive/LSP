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
	enum class ComponentEnum
	{
		Transform = 1,
		Logic = 2,
		RigidBody = 4,
		Collider = 8,
		SphereCollider = 16,
	};

	friend inline ComponentEnum operator &(ComponentEnum l, ComponentEnum r)
	{
		int lint = (int)l;
		int rint = (int)r;

		int result = lint & rint;
		return (ComponentEnum)((int)l & (int)r);
	}
	friend inline ComponentEnum operator |(ComponentEnum l, ComponentEnum r)
	{
		return (ComponentEnum)((int)l | (int)r);
	}


	Component();
	virtual ~Component() = default;
	Ref<GameObject> getGameObject() const;

	

protected:

	Ref<Transform> m_transform;
	Ref<GameObject> m_gameObject;
	ComponentEnum m_componentType;

private:
	void setTransform(Ref<Transform> transform);
	void setGameObject(Ref<GameObject> gameObject);
};

