#pragma once
#include "Collider.h"
class SphereCollider : public Collider
{
private:
	DirectX::BoundingSphere m_sphere;
	
public:
	SphereCollider(DirectX::SimpleMath::Vector3 pos, float radius);
	~SphereCollider();

	void collide() override;

};

