#include "OrientedBoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"


OrientedBoxCollider::OrientedBoxCollider(DirectX::SimpleMath::Vector3 dimensions)
{
	m_obb.Extents = dimensions / 2;
	m_obb.Center = m_transform->getPosition();
	m_obb.Orientation = DirectX::SimpleMath::Vector4(DirectX::XMQuaternionRotationMatrix(m_transform->getWorldMatrix()));
	
	m_componentType = ComponentEnum::ORIENTED_BOX_COLLIDER | ComponentEnum::COLLIDER;
}

OrientedBoxCollider::~OrientedBoxCollider()
{
}

void OrientedBoxCollider::initialize()
{
	m_obb.Center = m_transform->getPosition();
	m_obb.Orientation = DirectX::SimpleMath::Vector4(DirectX::XMQuaternionRotationMatrix(m_transform->getWorldMatrix()));
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

void OrientedBoxCollider::update()
{
	m_obb.Center = m_transform->getPosition();
	m_obb.Orientation = DirectX::SimpleMath::Vector4(DirectX::XMQuaternionRotationMatrix(m_transform->getWorldMatrix()));
}



DirectX::BoundingOrientedBox OrientedBoxCollider::getInternalCollider()
{
	return m_obb;
}
