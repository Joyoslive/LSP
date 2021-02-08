#pragma once

#include "Collider.h"

class PhysicsEngine;

class OrientedBoxCollider : public Collider
{
	friend PhysicsEngine;
private:
	DirectX::BoundingOrientedBox m_obb;

	

public:
	OrientedBoxCollider(DirectX::SimpleMath::Vector3 dimensions);
	~OrientedBoxCollider();

	bool collide(const Ref<Collider>& collider) override;
	void update() override;

	void initialize() override;

	const DirectX::BoundingOrientedBox& getInternalCollider();
	DirectX::SimpleMath::Vector3 closestPointOnObb(DirectX::SimpleMath::Vector3 point, DirectX::SimpleMath::Vector3& returnNormal);

private:
	template<typename T>
	bool internalCollide(const Ref<T>& collider) //ugly function to solve problem that i can't solve
	{
		return this->getInternalCollider().Intersects(collider->getInternalCollider());
	}
};

