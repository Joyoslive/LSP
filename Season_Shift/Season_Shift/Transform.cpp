#include "Transform.h"

Transform::Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	setMatrixs();
}

Transform::Transform()
{
	m_position = DirectX::XMFLOAT3(0, 0, 0);
	m_scale = DirectX::XMFLOAT3(1, 1, 1);
	m_rotation = DirectX::XMFLOAT3(0, 0, 0);
	setMatrixs();
}

Transform::~Transform()
{
	
}

void Transform::setMatrixs()
{
	setPositionMatrix();
	setScaleMatrix();
	setRotationMatrix();
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

DirectX::XMFLOAT3 Transform::getRotationToDegrees() const
{
	return DirectX::XMFLOAT3(m_rotation.x * m_RADTODEG, m_rotation.y * m_RADTODEG, m_rotation.z * m_RADTODEG);
}

DirectX::XMFLOAT3 Transform::getRotationToRadians() const
{
	return DirectX::XMFLOAT3(m_rotation.x * m_DEGTORAD, m_rotation.y * m_DEGTORAD, m_rotation.z * m_DEGTORAD);
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
	m_rotation = rotation;
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
	return getRotationToDegrees();
}

DirectX::XMMATRIX Transform::getWorldMatrix() const
{
	DirectX::XMMATRIX worldMatrix = m_scaleMatrix * m_rotationMatrix * m_positionMatrix;
	return worldMatrix;
}

