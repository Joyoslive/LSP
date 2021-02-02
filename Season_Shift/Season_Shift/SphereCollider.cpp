#include "SphereCollider.h"


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

	//auto var = typeid(collider);
	if (typeid(*collider) == typeid(SphereCollider))
	{
		if (internalCollide<SphereCollider>(std::dynamic_pointer_cast<SphereCollider>(collider)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	return 0;
}

DirectX::BoundingSphere SphereCollider::getInternalCollider()
{
	return m_sphere;
}

