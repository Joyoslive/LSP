#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"

class Collider : public Component
{
private:
	DirectX::BoundingSphere m_sphere;

public:
	Collider(DirectX::SimpleMath::Vector3 position);
	~Collider();

	DirectX::BoundingSphere getCollider();
	void update();
};

