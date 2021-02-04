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
		TRANSFORM			  = 1 << 0,
		LOGIC				  = 1 << 1,
		RIGID_BODY			  = 1 << 2,
		COLLIDER			  = 1 << 3,
		SPHERE_COLLIDER		  = 1 << 4,
		ORIENTED_BOX_COLLIDER = 1 << 5,
		CAMERA				  = 1 << 6,
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
	ComponentEnum getType() const;
	

protected:

	Ref<Transform> m_transform;
	Ref<GameObject> m_gameObject;
	ComponentEnum m_componentType;
	virtual void initialize();
	virtual void update();

private:
	void setTransform(Ref<Transform> transform);
	void setGameObject(Ref<GameObject> gameObject);
};

