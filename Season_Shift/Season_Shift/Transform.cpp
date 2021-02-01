#include "Transform.h"

Transform::Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotation)
{
	setPosition(position);
	setPosition(scale);
	setPosition(rotation);
}

Transform::Transform()
{
	setPosition(DirectX::XMFLOAT3(0, 0, 0));
	setPosition(DirectX::XMFLOAT3(1, 1, 1));
	setPosition(DirectX::XMFLOAT3(0, 0, 0));
}

Transform::~Transform()
{
	
}

void Transform::setPositionMatrix()
{
	m_positionMatrix = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}

void Transform::setScaleMatrix()
{
	m_scaleMatrix = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
}

void Transform::setRotationMatrix()
{
	m_rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
}

DirectX::XMFLOAT3 Transform::getRotationToDegrees(DirectX::XMFLOAT3 radiansRotation) const
{
	return DirectX::XMFLOAT3(radiansRotation.x * m_RADTODEG, radiansRotation.y * m_RADTODEG, radiansRotation.z * m_RADTODEG);
}

DirectX::XMFLOAT3 Transform::getRotationToRadians(DirectX::XMFLOAT3 degreesRotation) const
{
	return DirectX::XMFLOAT3(degreesRotation.x * m_DEGTORAD, degreesRotation.y * m_DEGTORAD, degreesRotation.z * m_DEGTORAD);
}

void Transform::setPosition(DirectX::XMFLOAT3 position)
{
	m_position = position;
	setPositionMatrix();
}

void Transform::setScale(DirectX::XMFLOAT3 scale)
{
	m_scale = scale;
	setScaleMatrix();
}

void Transform::setRotation(DirectX::XMFLOAT3 rotation)
{
	m_rotation = getRotationToRadians(rotation);
	setRotationMatrix();
}

DirectX::XMFLOAT3 Transform::getPosition() const
{
	return m_position;
}

DirectX::XMFLOAT3 Transform::getScale() const
{
	return m_scale;
}

DirectX::XMFLOAT3 Transform::getRotation() const
{
	return getRotationToDegrees(m_rotation);
}

DirectX::XMMATRIX Transform::getWorldMatrix() const
{
	DirectX::XMMATRIX worldMatrix = m_scaleMatrix * m_rotationMatrix * m_positionMatrix;
	return worldMatrix;
}

