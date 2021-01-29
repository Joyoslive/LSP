#pragma once
#include <DirectXMath.h>

class Transform
{
private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rotation;

	//DirectX::XMFLOAT4X4 m_worldMatrix;
	DirectX::XMMATRIX m_rotationMatrix;
	DirectX::XMMATRIX m_positionMatrix;
	DirectX::XMMATRIX m_scaleMatrix;
	
private:
	void setMatrixs();
	void setPositionMatrix();
	void setScaleMatrix();
	void setRotationMatrix();
public:
	Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotation);
	Transform();

	~Transform();

	void setPosition(DirectX::XMFLOAT3 position);
	void setScale(DirectX::XMFLOAT3 scale);
	void setRotation(DirectX::XMFLOAT3 rotation);
	DirectX::XMFLOAT3 getPosition() const;
	DirectX::XMFLOAT3 getScale() const;
	DirectX::XMFLOAT3 getRotation() const;
	DirectX::XMMATRIX getWorldMatrix() const;
};

