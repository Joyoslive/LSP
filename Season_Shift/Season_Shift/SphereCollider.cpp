#include "SphereCollider.h"


SphereCollider::SphereCollider(DirectX::SimpleMath::Vector3 pos, float radius)
{
	m_sphere.Center = pos;
	m_sphere.Radius = radius;
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::collide()
{

}
