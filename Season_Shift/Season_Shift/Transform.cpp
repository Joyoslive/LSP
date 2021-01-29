#include "Transform.h"

Transform::Transform()
{

}

Transform::~Transform()
{
	
}

DirectX::XMMATRIX Transform::getWorldMatrix()
{
	return DirectX::XMLoadFloat4x4(&m_worldMatrix);
}