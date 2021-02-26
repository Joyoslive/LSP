#pragma once

#include "Collider.h"

class PhysicsEngine;

class OrientedBoxCollider : public Collider, public std::enable_shared_from_this<OrientedBoxCollider>
{
	friend PhysicsEngine;
private:
	DirectX::BoundingOrientedBox m_obb;
	float m_radius;
	

public:
	OrientedBoxCollider(DirectX::SimpleMath::Vector3 dimensions);
	~OrientedBoxCollider();

	bool collide(const Ref<Collider>& collider) override;
	void update() override;
	const float& getRadius() const;
	void initialize() override;

	const DirectX::BoundingOrientedBox& getInternalCollider() const;
	DirectX::SimpleMath::Vector3 closestPointOnObb(DirectX::SimpleMath::Vector3 point, DirectX::SimpleMath::Vector3& returnNormal);

private:
	template<typename T>
	bool internalCollide(const Ref<T>& collider) //ugly function to solve problem that i can't solve
	{
		return this->getInternalCollider().Intersects(collider->getInternalCollider());
	}
};

