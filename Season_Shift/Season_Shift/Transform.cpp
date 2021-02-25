#include "pch.h"
#include "Transform.h"

Transform::Transform(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation)
{
	setPosition(position);
	setScale(scale);
	setRotation(rotation);

	m_componentType = ComponentEnum::TRANSFORM;
}

Transform::Transform()
{
	setPosition(DirectX::SimpleMath::Vector3(0, 0, 0));
	setScale(DirectX::SimpleMath::Vector3(1, 1, 1));
	setRotation(DirectX::SimpleMath::Vector3(0, 0, 0));

	m_componentType = ComponentEnum::TRANSFORM;
}

Transform::~Transform()
{
	
}

void Transform::setWorldMatrix()
{
	m_worldMatrix = m_scaleMatrix * m_rotationMatrix * m_positionMatrix;
}

void Transform::setPositionMatrix()
{
	m_positionMatrix = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	setWorldMatrix();
}

void Transform::setScaleMatrix()
{
	m_scaleMatrix = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	setWorldMatrix();
}

void Transform::setRotationMatrix()
{
	m_rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	setWorldMatrix();
}

const DirectX::SimpleMath::Vector3& Transform::getRotationToDegrees(const DirectX::SimpleMath::Vector3& radiansRotation) const
{
	return DirectX::SimpleMath::Vector3(radiansRotation.x * m_RADTODEG, radiansRotation.y * m_RADTODEG, radiansRotation.z * m_RADTODEG);
}

const DirectX::SimpleMath::Vector3& Transform::getRotationToRadians(const DirectX::SimpleMath::Vector3& degreesRotation) const
{
	return DirectX::SimpleMath::Vector3(degreesRotation.x * m_DEGTORAD, degreesRotation.y * m_DEGTORAD, degreesRotation.z * m_DEGTORAD);
}

const DirectX::SimpleMath::Matrix& Transform::getRotationMatrix() const
{
	return m_rotationMatrix;
}

void Transform::setPosition(const DirectX::SimpleMath::Vector3& position)
{
	m_position = position;
	setPositionMatrix();
}

void Transform::setScale(const DirectX::SimpleMath::Vector3& scale)
{
	m_scale = scale;
	setScaleMatrix();
}

void Transform::setRotation(const DirectX::SimpleMath::Vector3& rotation)
{
	m_rotation = getRotationToRadians(rotation);
	setRotationMatrix();
}

const DirectX::SimpleMath::Vector3& Transform::getPosition() const
{
	return m_position;
}

const DirectX::SimpleMath::Vector3& Transform::getScale() const
{
	return m_scale;
}

const DirectX::SimpleMath::Vector3& Transform::getRotation() const
{
	return getRotationToDegrees(m_rotation);
}

const DirectX::SimpleMath::Matrix& Transform::getWorldMatrix() const
{
	return m_worldMatrix;
}

