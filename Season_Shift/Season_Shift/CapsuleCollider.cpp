#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider()
{
    m_capsule.radius = 1.0f;
}

CapsuleCollider::~CapsuleCollider()
{
}



void CapsuleCollider::update()
{
}

void CapsuleCollider::initialize()
{
}

bool CapsuleCollider::collide(const Ref<Collider>& collider)
{
    return false;
}


const Capsule& CapsuleCollider::getInternalCollider()
{
    return m_capsule;
}
