#include "CapsuleCollider.h"
#include "GameObject.h"
#include "Logger.h"

using namespace DirectX::SimpleMath;


CapsuleCollider::CapsuleCollider(float radius, float totalLength)
{
    m_length = totalLength - 2 * radius;
    m_capsule.radius = radius;
    m_componentType = ComponentEnum::CAPSULE_COLLIDER | ComponentEnum::COLLIDER;
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
    update();
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
    


    //--------------------------------


    Vector3 inputTest = Vector3(-1, 0, 0);
    Vector3 resultTest2;

    Vector3 unitX;
    unitX.x = obb->getTransform()->getWorldMatrix().m[0][0];
    unitX.y = obb->getTransform()->getWorldMatrix().m[0][1];
    unitX.z = obb->getTransform()->getWorldMatrix().m[0][2];
    unitX.Normalize();

    Vector3 unitY;
    unitY.x = obb->getTransform()->getWorldMatrix().m[1][0];
    unitY.y = obb->getTransform()->getWorldMatrix().m[1][1];
    unitY.z = obb->getTransform()->getWorldMatrix().m[1][2];
    unitY.Normalize();

    Vector3 unitZ;
    unitZ.x = obb->getTransform()->getWorldMatrix().m[2][0];
    unitZ.y = obb->getTransform()->getWorldMatrix().m[2][1];
    unitZ.z = obb->getTransform()->getWorldMatrix().m[2][2];
    unitZ.Normalize();


    resultTest2.x = unitX.Dot(inputTest - obb->getInternalCollider().Center);
    resultTest2.y = unitY.Dot(inputTest - obb->getInternalCollider().Center);
    resultTest2.z = unitZ.Dot(inputTest - obb->getInternalCollider().Center);

}


const Capsule& CapsuleCollider::getInternalCollider()
{
    return m_capsule;
}
