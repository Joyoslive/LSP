#pragma once
#include "Collider.h"
#include <typeinfo>
class SphereCollider : public Collider
{
private:
	DirectX::BoundingSphere m_sphere;

	

public:
	SphereCollider(DirectX::SimpleMath::Vector3 pos, float radius);
	~SphereCollider();

	bool collide(Ref<Collider> collider) override;

	DirectX::BoundingSphere getInternalCollider();


public:
	template<typename T>
	bool internalCollide(Ref<T> collider) //ugly function to solve problem that i can't solve
	{
		return this->getInternalCollider().Intersects(collider->getInternalCollider());
	}
};
