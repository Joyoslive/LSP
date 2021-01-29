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
	setPositionMatrix();
}

void Transform::setScaleMatrix()
{
	m_scaleMatrix = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	setScaleMatrix();
}

void Transform::setRotationMatrix()
{
	m_rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	setRotationMatrix();
}

void Transform::setPosition(DirectX::XMFLOAT3 position)
{
	m_position = position;
}

void Transform::setScale(DirectX::XMFLOAT3 scale)
{
	m_scale = scale;
}

void Transform::setRotation(DirectX::XMFLOAT3 rotation)
{
	m_rotation = rotation;
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
	return m_rotation;
}

DirectX::XMMATRIX Transform::getWorldMatrix() const
{
	DirectX::XMMATRIX worldMatrix = m_scaleMatrix * m_rotationMatrix * m_positionMatrix;
	return worldMatrix;
}

