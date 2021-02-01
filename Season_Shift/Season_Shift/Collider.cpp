#include "Collider.h"
#include "Transform.h"

using namespace DirectX;

Collider::Collider(DirectX::SimpleMath::Vector3 position)
{
	m_sphere.Center = position;
	m_sphere.Radius = 2;
}

Collider::~Collider()
{
}

BoundingSphere Collider::getCollider()
{
	return m_sphere;
}

void Collider::update()
{
	m_sphere.Center = m_transform->getPosition();
}
