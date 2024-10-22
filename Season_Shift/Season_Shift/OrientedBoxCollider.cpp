#include "pch.h"
#include "OrientedBoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "CapsuleCollider.h"
#include "Transform.h"


using namespace DirectX::SimpleMath;
using namespace DirectX;

OrientedBoxCollider::OrientedBoxCollider(Vector3 dimensions, Vector3 offset)
{
	m_obb.Extents = dimensions / 2;
	m_radius = 0;
	m_offset = offset;
	m_componentType = ComponentEnum::ORIENTED_BOX_COLLIDER | ComponentEnum::COLLIDER;
}

OrientedBoxCollider::~OrientedBoxCollider()
{
}

void OrientedBoxCollider::initialize()
{
	//m_obb.Center = m_transform->getPosition() + (Vector3)DirectX::XMVector3Transform(m_offset, getTransform()->getRotationMatrix());
	//don't know if it is safe to cast to vector3
	DirectX::XMFLOAT3 offsetTemp;
	DirectX::XMStoreFloat3(&offsetTemp, DirectX::XMVector3Transform(m_offset, m_transform->getRotationMatrix()));
	m_obb.Center = m_transform->getPosition() + offsetTemp;

	m_obb.Orientation = DirectX::SimpleMath::Vector4(DirectX::XMQuaternionRotationMatrix(m_transform->getRotationMatrix()));

	DirectX::XMFLOAT3 corners[8];
	m_obb.GetCorners(corners);
	m_radius = (m_obb.Center - corners[0]).Length();
}

bool OrientedBoxCollider::collide(const Ref<Collider>& collider)
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
	else if ((int)(collider->getType() & ComponentEnum::CAPSULE_COLLIDER) > 0)
	{
		return std::dynamic_pointer_cast<CapsuleCollider>(collider)->collide(std::dynamic_pointer_cast<Collider>(shared_from_this())); //call capsule colliders detection function on self
	}
	return false;
}

void OrientedBoxCollider::update()
{
	//m_obb.Center = m_transform->getPosition() + (Vector3)DirectX::XMVector3Transform(m_offset, getTransform()->getRotationMatrix());
	//don't know if it is safe to cast to vector3
	DirectX::XMFLOAT3 offsetTemp;
	DirectX::XMStoreFloat3(&offsetTemp, DirectX::XMVector3Transform(m_offset, m_transform->getRotationMatrix()));
	m_obb.Center = m_transform->getPosition() + offsetTemp;

	m_obb.Orientation = DirectX::SimpleMath::Vector4(DirectX::XMQuaternionRotationMatrix(m_transform->getRotationMatrix()));
}

const float& OrientedBoxCollider::getRadius() const
{
	return m_radius;
}

const BoundingOrientedBox& OrientedBoxCollider::getInternalCollider() const
{
	return m_obb;
}

Vector3 OrientedBoxCollider::closestPointOnObb(Vector3 point, Vector3& returnNormal)
{
	//fr�n boken
	//Real time collision detection av Christer Ericson


	//obb unit vectors, fan att DirectXCollision.h bara har en quaternion i sin obb
	Vector3 unitX;
	unitX.x = m_transform->getWorldMatrix().m[0][0];
	unitX.y = m_transform->getWorldMatrix().m[0][1];
	unitX.z = m_transform->getWorldMatrix().m[0][2];
	unitX.Normalize();

	Vector3 unitY;
	unitY.x = m_transform->getWorldMatrix().m[1][0];
	unitY.y = m_transform->getWorldMatrix().m[1][1];
	unitY.z = m_transform->getWorldMatrix().m[1][2];
	unitY.Normalize();

	Vector3 unitZ;
	unitZ.x = m_transform->getWorldMatrix().m[2][0];
	unitZ.y = m_transform->getWorldMatrix().m[2][1];
	unitZ.z = m_transform->getWorldMatrix().m[2][2];
	unitZ.Normalize();

	Vector3 d = point - m_obb.Center;

	Vector3 distance;
	distance.x = unitX.Dot(d);
	distance.y = unitY.Dot(d);
	distance.z = unitZ.Dot(d);

	if (distance.x > m_obb.Extents.x) distance.x = m_obb.Extents.x;
	if (distance.x < -m_obb.Extents.x) distance.x = -m_obb.Extents.x;

	if (distance.y > m_obb.Extents.y) distance.y = m_obb.Extents.y;
	if (distance.y < -m_obb.Extents.y) distance.y = -m_obb.Extents.y;

	if (distance.z > m_obb.Extents.z) distance.z = m_obb.Extents.z;
	if (distance.z < -m_obb.Extents.z) distance.z = -m_obb.Extents.z;

	//if point is inside obb move to boundery
	bool insideObb = false;

	if (distance.x < m_obb.Extents.x && distance.x > -m_obb.Extents.x
		&& distance.y < m_obb.Extents.y && distance.y > -m_obb.Extents.y
		&& distance.z < m_obb.Extents.z && distance.z > -m_obb.Extents.z)
	{
		insideObb = true;
		float xDiff = 0.0, yDiff = 0.0, zDiff = 0.0;

		if (distance.x > 0)
			xDiff = m_obb.Extents.x - distance.x;
		else
			xDiff = m_obb.Extents.x + distance.x;

		if (distance.y > 0)
			yDiff = m_obb.Extents.y - distance.y;
		else
			yDiff = m_obb.Extents.y + distance.y;

		if (distance.z > 0)
			zDiff = m_obb.Extents.z - distance.z;
		else
			zDiff = m_obb.Extents.z + distance.z;


		if (xDiff <= yDiff && xDiff <= zDiff)
		{
			if (distance.x < 0)
				distance.x = -m_obb.Extents.x;
			else
				distance.x = m_obb.Extents.x;
		}
		else if (zDiff <= yDiff)
		{
			if (distance.z < 0)
				distance.z = -m_obb.Extents.z;
			else
				distance.z = m_obb.Extents.z;
		}
		else
		{
			if (distance.y < 0)
				distance.y = -m_obb.Extents.y;
			else
				distance.y = m_obb.Extents.y;
		}

	}

	Vector3 closestPoint = m_obb.Center;
	closestPoint += distance.x * unitX;
	closestPoint += distance.y * unitY;
	closestPoint += distance.z * unitZ;

	returnNormal = point - closestPoint;
	if (insideObb) returnNormal *= -1;
	returnNormal.Normalize();

	return closestPoint;

}