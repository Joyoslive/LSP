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
		TRANSFORM = 1,
		LOGIC = 2,
		RIGID_BODY = 4,
		COLLIDER = 8,
		SPHERE_COLLIDER = 16,
		ORIENTED_BOX_COLLIDER = 32,
		MODEL = 64,
		CAMERA = 128,
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
	Ref<Transform> getTransform() const;
	ComponentEnum getType() const;
	virtual void update();

protected:

	Ref<Transform> m_transform;
	Ref<GameObject> m_gameObject;
	ComponentEnum m_componentType;
	virtual void initialize();
	

private:
	void setTransform(Ref<Transform> transform);
	void setGameObject(Ref<GameObject> gameObject);
};

