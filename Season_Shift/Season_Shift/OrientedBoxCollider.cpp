#include "OrientedBoxCollider.h"
#include "SphereCollider.h"

OrientedBoxCollider::OrientedBoxCollider()
{
	m_componentType = ComponentEnum::ORIENTED_BOX_COLLIDER | ComponentEnum::COLLIDER;
}

OrientedBoxCollider::~OrientedBoxCollider()
{
}

bool OrientedBoxCollider::collide(Ref<Collider> collider)
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

DirectX::BoundingOrientedBox OrientedBoxCollider::getInternalCollider()
{
	return m_obb;
}
