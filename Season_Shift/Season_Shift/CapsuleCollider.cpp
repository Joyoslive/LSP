#include "CapsuleCollider.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;


CapsuleCollider::CapsuleCollider(float radius, float totalLength)
{
    m_length = totalLength - 2 * radius;
    m_capsule.radius = radius;
}

CapsuleCollider::~CapsuleCollider()
{
}



void CapsuleCollider::initialize()
{
    Vector3 unitY;
    unitY.x = m_transform->getWorldMatrix().m[1][0];
    unitY.y = m_transform->getWorldMatrix().m[1][1];
    unitY.z = m_transform->getWorldMatrix().m[1][2];
    unitY.Normalize();

    m_capsule.PointB = m_transform->getPosition();
    m_capsule.PointA = m_transform->getPosition() + m_length * unitY;
}

void CapsuleCollider::update()
{
    Vector3 unitY;
    unitY.x = m_transform->getWorldMatrix().m[1][0];
    unitY.y = m_transform->getWorldMatrix().m[1][1];
    unitY.z = m_transform->getWorldMatrix().m[1][2];
    unitY.Normalize();

    m_capsule.PointB = m_transform->getPosition();
    m_capsule.PointA = m_transform->getPosition() + m_length * unitY;
}


bool CapsuleCollider::collide(const Ref<Collider>& collider)
{
    if (!((int)(collider->getType() & Component::ComponentEnum::ORIENTED_BOX_COLLIDER) > 0))    return false;   //only support collision vs obb


    Ref<OrientedBoxCollider> obb = std::dynamic_pointer_cast<OrientedBoxCollider>(collider);

    Vector3 normalA;
    Vector3 pointA = obb->closestPointOnObb(m_capsule.PointA, normalA);

    Vector3 normalB;
    Vector3 pointB = obb->closestPointOnObb(m_capsule.PointB, normalB);

    if (normalA == normalB) // capsule can't have collided with two faces
    {
        if ((m_capsule.PointA - pointA).Length() < m_capsule.radius || (m_capsule.PointB - pointB).Length() < m_capsule.radius)
        {
            return true;
        }
    }


    //obb->closestPointOnObb()

    //printa ut obb hörnen så att du ser hur du kan göra 12 trianglar av dem
}


const Capsule& CapsuleCollider::getInternalCollider()
{
    return m_capsule;
}
