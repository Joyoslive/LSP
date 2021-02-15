#pragma once
#include "Collider.h"

struct Capsule
{
	DirectX::SimpleMath::Vector3 PointA;
	DirectX::SimpleMath::Vector3 PointB;
	float radius;
};


class CapsuleCollider : public Collider
{
private:
	Capsule m_capsule;
	float m_length;

public:
	CapsuleCollider(float radius, float totalLength);
	~CapsuleCollider();

	bool collide(const Ref<Collider>& collider) override;
	void update() override;

	void initialize() override;

	const Capsule& getInternalCollider();
};

