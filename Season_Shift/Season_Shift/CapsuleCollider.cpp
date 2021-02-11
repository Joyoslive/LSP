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

Vector3 closestPointOnSegmentFromPoint(Vector3 segmentEndPoint1, Vector3 segmentEndPoint2, Vector3 point)
{
    //closest point on linesegment to point
    //från boken Real time collision detection

    Vector3 ab = segmentEndPoint1 - segmentEndPoint2;
    float t = ab.Dot(point - segmentEndPoint2) / ab.Dot(ab);

    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    return segmentEndPoint2 + t * ab;
}

bool CapsuleCollider::collide(const Ref<Collider>& collider)
{
    update();
    if (!((int)(collider->getType() & Component::ComponentEnum::ORIENTED_BOX_COLLIDER) > 0))    return false;   //only support collision vs obb


    Ref<OrientedBoxCollider> obb = std::dynamic_pointer_cast<OrientedBoxCollider>(collider);

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

    Vector3 planeNormal1;
    Vector3 planeNormal2;
    Vector3 planeNormal3;
    Vector3 planePoint1;
    Vector3 planePoint2;
    Vector3 planePoint3;

    if (obbToCapsule.Dot(obbUnitX) > 0)
    {
        planeNormal1 = obbUnitX;
        planePoint1 = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.x * planeNormal1;
    }
    else
    {
        planeNormal1 = -obbUnitX;
        planePoint1 = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.x * planeNormal1;
    }
    if (obbToCapsule.Dot(obbUnitY) > 0)
    {
        planeNormal2 = obbUnitY;
        planePoint2 = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.y * planeNormal2;
    }
    else
    {
        planeNormal2 = -obbUnitY;
        planePoint2 = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.y * planeNormal2;
    }
    if (obbToCapsule.Dot(obbUnitZ) > 0)
    {
        planeNormal3 = obbUnitZ;
        planePoint3 = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.z * planeNormal3;
    }
    else
    {
        planeNormal3 = -obbUnitZ;
        planePoint3 = obb->getInternalCollider().Center + obb->getInternalCollider().Extents.z * planeNormal3;
    }
    
    /*Logger::getLogger().debugLog(std::to_string(planeNormal.x) +
        ", " + std::to_string(planeNormal.y) +
        ", " + std::to_string(planeNormal.z) + "\n");*/

    DirectX::XMVECTOR closestPlane1 = DirectX::XMPlaneFromPointNormal(planePoint1, planeNormal1);
    DirectX::XMVECTOR closestPlane2 = DirectX::XMPlaneFromPointNormal(planePoint2, planeNormal2);
    DirectX::XMVECTOR closestPlane3 = DirectX::XMPlaneFromPointNormal(planePoint3, planeNormal3);

    //find point on plane, OBS! this does not always give "right" result

    DirectX::XMVECTOR planeIntersectionPoint1 = DirectX::XMPlaneIntersectLine(closestPlane1, m_capsule.PointA, m_capsule.PointB);
    DirectX::XMVECTOR planeIntersectionPoint2 = DirectX::XMPlaneIntersectLine(closestPlane2, m_capsule.PointA, m_capsule.PointB);
    DirectX::XMVECTOR planeIntersectionPoint3 = DirectX::XMPlaneIntersectLine(closestPlane3, m_capsule.PointA, m_capsule.PointB);



    //closeset point on plane to obb, OBS! 
    Vector3 trash;
    Vector3 closestPoint1 = obb->closestPointOnObb(planeIntersectionPoint1, trash);
    Vector3 closestPoint2 = obb->closestPointOnObb(planeIntersectionPoint2, trash);
    Vector3 closestPoint3 = obb->closestPointOnObb(planeIntersectionPoint3, trash);


    //closest point on linesegment to point
        

    closestPoint1 = closestPointOnSegmentFromPoint(m_capsule.PointB, m_capsule.PointA, closestPoint1);
    closestPoint2 = closestPointOnSegmentFromPoint(m_capsule.PointB, m_capsule.PointA, closestPoint2);
    closestPoint3 = closestPointOnSegmentFromPoint(m_capsule.PointB, m_capsule.PointA, closestPoint3);

    Vector3 collisionNormal1;
    Vector3 collisionNormal2;
    Vector3 collisionNormal3;

    Vector3 closestPointOnObb1 = obb->closestPointOnObb(closestPoint1, collisionNormal1);
    Vector3 closestPointOnObb2 = obb->closestPointOnObb(closestPoint2, collisionNormal2);
    Vector3 closestPointOnObb3 = obb->closestPointOnObb(closestPoint3, collisionNormal3);
    
    float pen = 0;
    if (m_capsule.radius - (closestPoint1 - closestPointOnObb2).Length() > pen)
    {
        m_collisionInfo.m_collisionPoint = closestPointOnObb1;
        m_collisionInfo.m_segmentPoint = closestPoint1;
        m_collisionInfo.m_normal = collisionNormal1;
        m_collisionInfo.m_penetration = m_capsule.radius - (closestPoint1 - closestPointOnObb1).Length();
        pen = m_capsule.radius - (closestPoint1 - closestPointOnObb1).Length();
    }
    if (m_capsule.radius - (closestPoint2 - closestPointOnObb2).Length() > pen)
    {
        m_collisionInfo.m_collisionPoint = closestPointOnObb2;
        m_collisionInfo.m_segmentPoint = closestPoint2;
        m_collisionInfo.m_normal = collisionNormal2;
        m_collisionInfo.m_penetration = m_capsule.radius - (closestPoint2 - closestPointOnObb2).Length();
        pen = m_capsule.radius - (closestPoint2 - closestPointOnObb2).Length();
    }
    if (m_capsule.radius - (closestPoint3 - closestPointOnObb3).Length() > pen)
    {
        m_collisionInfo.m_collisionPoint = closestPointOnObb3;
        m_collisionInfo.m_segmentPoint = closestPoint3;
        m_collisionInfo.m_normal = collisionNormal3;
        m_collisionInfo.m_penetration = m_capsule.radius - (closestPoint3 - closestPointOnObb3).Length();
        pen = m_capsule.radius - (closestPoint3 - closestPointOnObb3).Length();
    }
    if (pen > 0)
    {
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