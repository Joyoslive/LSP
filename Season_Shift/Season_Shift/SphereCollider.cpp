#include "SphereCollider.h"
#include "OrientedBoxCollider.h"
#include "GameObject.h"


SphereCollider::SphereCollider(float radius)
{
	
	m_sphere.Radius = radius;
	m_componentType = ComponentEnum::SPHERE_COLLIDER | ComponentEnum::COLLIDER;
}

SphereCollider::~SphereCollider()
{
	
}

void SphereCollider::initialize()
{
	m_sphere.Center = m_transform->getPosition();
}

bool SphereCollider::collide(const Ref<Collider>& collider)
{
	if (collider.get() == this) // check for selfCollison
	{
		return 0;
	}
	update();
	if ((int)(collider->getType() & ComponentEnum::SPHERE_COLLIDER) > 0)
	{
		return internalCollide<SphereCollider>(std::dynamic_pointer_cast<SphereCollider>(collider));
	}
	else if ((int)(collider->getType() & ComponentEnum::ORIENTED_BOX_COLLIDER) > 0)
	{
		return internalCollide<OrientedBoxCollider>(std::dynamic_pointer_cast<OrientedBoxCollider>(collider));
	}


	return 0;
}

void SphereCollider::update()
{
	m_sphere.Center = m_transform->getPosition();
}



const DirectX::BoundingSphere& SphereCollider::getInternalCollider()
{
	return m_sphere;
}

