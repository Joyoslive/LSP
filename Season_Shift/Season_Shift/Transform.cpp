#include "Transform.h"

Transform::Transform(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 rotation)
{
	setPosition(position);
	setPosition(scale);
	setPosition(rotation);

	m_componentType = ComponentEnum::Transform;
}

Transform::Transform()
{
	setPosition(DirectX::SimpleMath::Vector3(0, 0, 0));
	setScale(DirectX::SimpleMath::Vector3(1, 1, 1));
	setRotation(DirectX::SimpleMath::Vector3(0, 0, 0));

	m_componentType = ComponentEnum::Transform;
}

Transform::~Transform()
{
	
}

void Transform::setPositionMatrix()
{
	m_positionMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
}

void Transform::setScaleMatrix()
{
	m_scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
}

void Transform::setRotationMatrix()
{
	m_rotationMatrix = DirectX::SimpleMath::Matrix::CreateRotationX(m_rotation.x);
	m_rotationMatrix *= DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.y);
	m_rotationMatrix *= DirectX::SimpleMath::Matrix::CreateRotationZ(m_rotation.z);
}

DirectX::SimpleMath::Vector3 Transform::getRotationToDegrees(DirectX::SimpleMath::Vector3 radiansRotation) const
{
	return DirectX::SimpleMath::Vector3(radiansRotation.x * m_RADTODEG, radiansRotation.y * m_RADTODEG, radiansRotation.z * m_RADTODEG);
}

DirectX::SimpleMath::Vector3 Transform::getRotationToRadians(DirectX::SimpleMath::Vector3 degreesRotation) const
{
	return DirectX::SimpleMath::Vector3(degreesRotation.x * m_DEGTORAD, degreesRotation.y * m_DEGTORAD, degreesRotation.z * m_DEGTORAD);
}

void Transform::setPosition(DirectX::SimpleMath::Vector3 position)
{
	m_position = position;
	setPositionMatrix();
}

void Transform::setScale(DirectX::SimpleMath::Vector3 scale)
{
	m_scale = scale;
	setScaleMatrix();
}

void Transform::setRotation(DirectX::SimpleMath::Vector3 rotation)
{
	m_rotation = getRotationToRadians(rotation);
	setRotationMatrix();
}

DirectX::SimpleMath::Vector3 Transform::getPosition() const
{
	return m_position;
}

DirectX::SimpleMath::Vector3 Transform::getScale() const
{
	return m_scale;
}

DirectX::SimpleMath::Vector3 Transform::getRotation() const
{
	return getRotationToDegrees(m_rotation);
}

DirectX::SimpleMath::Matrix Transform::getWorldMatrix() const
{
	DirectX::SimpleMath::Matrix worldMatrix = m_scaleMatrix * m_rotationMatrix * m_positionMatrix;
	return worldMatrix;
}

