#include "SphereCollider.h"
#include "OrientedBoxCollider.h"


SphereCollider::SphereCollider(DirectX::SimpleMath::Vector3 pos, float radius)
{
	m_sphere.Center = pos;
	m_sphere.Radius = radius;

	m_componentType = ComponentEnum::SPHERE_COLLIDER | ComponentEnum::COLLIDER;
}

SphereCollider::~SphereCollider()
{
	
}

bool SphereCollider::collide(Ref<Collider> collider)
{
	if ((collider->getType() & ComponentEnum::SPHERE_COLLIDER) == ComponentEnum::SPHERE_COLLIDER)
	{
		return internalCollide<SphereCollider>(std::dynamic_pointer_cast<SphereCollider>(collider));
	}
	else if ((collider->getType() & ComponentEnum::ORIENTED_BOX_COLLIDER) == ComponentEnum::ORIENTED_BOX_COLLIDER)
	{
		return internalCollide<OrientedBoxCollider>(std::dynamic_pointer_cast<OrientedBoxCollider>(collider));
	}


	return 0;
}

DirectX::BoundingSphere SphereCollider::getInternalCollider()
{
	return m_sphere;
}

