#include "Collider.h"
#include "Transform.h"

using namespace DirectX;

Collider::Collider()
{

	m_componentType = ComponentEnum::COLLIDER;
}

Collider::~Collider()
{
}

//bool Collider::collide(Ref<Collider> collider)
//{
//	return false;
//}

