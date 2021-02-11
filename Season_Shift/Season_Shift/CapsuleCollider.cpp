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

    //Vector3 normalA;
    //Vector3 pointA = obb->closestPointOnObb(m_capsule.PointA, normalA);

    //Vector3 normalB;
    //Vector3 pointB = obb->closestPointOnObb(m_capsule.PointB, normalB);

    //if (normalA == normalB) // capsule can't have collided with two faces
    //{
    //    if ((m_capsule.PointA - pointA).Length() < m_capsule.radius || (m_capsule.PointB - pointB).Length() < m_capsule.radius)
    //    {
    //        return true;   
    //    }
    //}
    


    //--------------------------------

    //unit vectors from obb
    Vector3 obbUnitX;
    obbUnitX.x = obb->getTransform()->getWorldMatrix().m[0][0];
    obbUnitX.y = obb->getTransform()->getWorldMatrix().m[0][1];
    obbUnitX.z = obb->getTransform()->getWorldMatrix().m[0][2];
    obbUnitX.Normalize();

    Vector3 obbUnitY;
    obbUnitY.x = obb->getTransform()->getWorldMatrix().m[1][0];
    obbUnitY.y = obb->getTransform()->getWorldMatrix().m[1][1];
    obbUnitY.z = obb->getTransform()->getWorldMatrix().m[1][2];
    obbUnitY.Normalize();

    Vector3 obbUnitZ;
    obbUnitZ.x = obb->getTransform()->getWorldMatrix().m[2][0];
    obbUnitZ.y = obb->getTransform()->getWorldMatrix().m[2][1];
    obbUnitZ.z = obb->getTransform()->getWorldMatrix().m[2][2];
    obbUnitZ.Normalize();
   
    //get closest plane
    Vector3 capsuleMiddlePoint = (m_capsule.PointA + m_capsule.PointB) / 2;
    Vector3 obbToCapsule = capsuleMiddlePoint - obb->getInternalCollider().Center;

    Vector3 planeNormal;
    Vector3 planePoint;
    float dot = 0;
    float tempDot = 0;
    tempDot = obbToCapsule.Dot(obbUnitX);
    if (tempDot > dot)
    {
        dot = tempDot;
        planeNormal = obbUnitX;
        planePoint = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.x * planeNormal;
    }
    tempDot = obbToCapsule.Dot(-obbUnitX);
    if (tempDot > dot)
    {
        dot = tempDot;
        planeNormal = -obbUnitX;
        planePoint = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.x * planeNormal;
    }
    tempDot = obbToCapsule.Dot(obbUnitY);
    if (tempDot > dot)
    {
        dot = tempDot;
        planeNormal = obbUnitY;
        planePoint = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.y * planeNormal;
    }
    tempDot = obbToCapsule.Dot(-obbUnitY);
    if (tempDot > dot)
    {
        dot = tempDot;
        planeNormal = -obbUnitY;
        planePoint = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.y * planeNormal;
    }
    tempDot = obbToCapsule.Dot(obbUnitZ);
    if (tempDot > dot)
    {
        dot = tempDot;
        planeNormal = obbUnitZ;
        planePoint = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.z * planeNormal;
    }
    tempDot = obbToCapsule.Dot(-obbUnitZ);
    if (tempDot > dot)
    {
        dot = tempDot;
        planeNormal = -obbUnitZ;
        planePoint = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.z * planeNormal;
    }
    
    /*Logger::getLogger().debugLog(std::to_string(planeNormal.x) +
        ", " + std::to_string(planeNormal.y) +
        ", " + std::to_string(planeNormal.z) + "\n");*/

    DirectX::XMVECTOR closestPlane = DirectX::XMPlaneFromPointNormal(planePoint, planeNormal);

    //find point on plane, OBS! this does not always give "right" result

    DirectX::XMVECTOR planeIntersectionPoint = DirectX::XMPlaneIntersectLine(closestPlane, m_capsule.PointA, m_capsule.PointB);



    //closeset point on plane to obb, OBS! 
    Vector3 trash;
    Vector3 closestPoint = obb->closestPointOnObb(planeIntersectionPoint, trash);


    //closest point on linesegment to point
    //från boken Real time collision detection
    
    Vector3 ab = m_capsule.PointB - m_capsule.PointA;
    float t = ab.Dot(closestPoint - m_capsule.PointA) / ab.Dot(ab);

    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    closestPoint = m_capsule.PointA + t * ab;

    Vector3 collisionNormal;
    Vector3 closestPointOnObb = obb->closestPointOnObb(closestPoint, collisionNormal);
    if ((closestPoint - closestPointOnObb).Length() <= m_capsule.radius)
    {
        m_collisionInfo.m_collisionPoint = closestPointOnObb;
        m_collisionInfo.m_segmentPoint = closestPoint;
        m_collisionInfo.m_normal = collisionNormal;
        m_collisionInfo.m_penetration = m_capsule.radius - (closestPoint - closestPointOnObb).Length();

        return true;
    }
    else
    {
        return false;
    }
}


const Capsule& CapsuleCollider::getInternalCollider()
{
    return m_capsule;
}





//Vector3 inputTest = Vector3(-1, 0, 0);
//Vector3 resultTest2;
//
//Vector3 unitX;
//unitX.x = obb->getTransform()->getWorldMatrix().m[0][0];
//unitX.y = obb->getTransform()->getWorldMatrix().m[0][1];
//unitX.z = obb->getTransform()->getWorldMatrix().m[0][2];
//unitX.Normalize();
//
//Vector3 unitY;
//unitY.x = obb->getTransform()->getWorldMatrix().m[1][0];
//unitY.y = obb->getTransform()->getWorldMatrix().m[1][1];
//unitY.z = obb->getTransform()->getWorldMatrix().m[1][2];
//unitY.Normalize();
//
//Vector3 unitZ;
//unitZ.x = obb->getTransform()->getWorldMatrix().m[2][0];
//unitZ.y = obb->getTransform()->getWorldMatrix().m[2][1];
//unitZ.z = obb->getTransform()->getWorldMatrix().m[2][2];
//unitZ.Normalize();
//
//
//resultTest2.x = unitX.Dot(inputTest - obb->getInternalCollider().Center);
//resultTest2.y = unitY.Dot(inputTest - obb->getInternalCollider().Center);
//resultTest2.z = unitZ.Dot(inputTest - obb->getInternalCollider().Center);