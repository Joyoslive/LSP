#pragma once
#include "Collider.h"

struct Capsule
{
	DirectX::SimpleMath::Vector3 PointA;
	DirectX::SimpleMath::Vector3 PointB;
	float radius;
};

class PhysicsEngine;

class CapsuleCollider : public Collider
{
	friend PhysicsEngine;
private:
	Capsule m_capsule;
	float m_length;

	struct CollisionInfo
	{
		DirectX::SimpleMath::Vector3 m_segmentPoint = DirectX::SimpleMath::Vector3(0, 0, 0);
		DirectX::SimpleMath::Vector3 m_collisionPoint = DirectX::SimpleMath::Vector3(0, 0, 0);
		DirectX::SimpleMath::Vector3 m_normal = DirectX::SimpleMath::Vector3(0, 0, 0);
		float m_penetration = 0;
	};
	CollisionInfo m_collisionInfo;

public:
	CapsuleCollider(float radius, float totalLength);
	~CapsuleCollider();

	bool collide(const Ref<Collider>& collider) override;
	void update() override;

	void initialize() override;

	const Capsule& getInternalCollider();
};

