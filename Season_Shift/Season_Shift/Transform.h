#pragma once
#include <DirectXMath.h>

class Transform
{
private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rotation;

	DirectX::XMFLOAT4X4 m_worldMatrix;
public:
	Transform();
	~Transform();

	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getScale();
	DirectX::XMFLOAT3 getRotation();
	DirectX::XMMATRIX getWorldMatrix();
};

