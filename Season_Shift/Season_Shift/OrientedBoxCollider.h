#pragma once

#include "Collider.h"
class OrientedBoxCollider : public Collider
{
private:
	DirectX::BoundingOrientedBox m_obb;

	

public:
	OrientedBoxCollider(DirectX::SimpleMath::Vector3 dimensions);
	~OrientedBoxCollider();

	bool collide(Ref<Collider> collider) override;

	DirectX::BoundingOrientedBox getInternalCollider();


private:
	template<typename T>
	bool internalCollide(Ref<T> collider) //ugly function to solve problem that i can't solve
	{
		return this->getInternalCollider().Intersects(collider->getInternalCollider());
	}
};

