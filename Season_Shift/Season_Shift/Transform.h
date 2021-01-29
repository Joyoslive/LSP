#pragma once
#include <DirectXMath.h>
#include "Component.h"

class Transform : public Component
{
private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rotation;

	DirectX::XMMATRIX m_rotationMatrix;
	DirectX::XMMATRIX m_positionMatrix;
	DirectX::XMMATRIX m_scaleMatrix;

	const float m_RADTODEG = 180.0f / DirectX::XM_PI;
	const float m_DEGTORAD = DirectX::XM_PI / 180.0f;
	
private:
	void setMatrixs();
	void setPositionMatrix();
	void setScaleMatrix();
	void setRotationMatrix();
	DirectX::XMFLOAT3 getRotationToRadians(DirectX::XMFLOAT3 degreesRotation) const;
	DirectX::XMFLOAT3 getRotationToDegrees(DirectX::XMFLOAT3 radiansRotation) const;
public:
	Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotation, std::shared_ptr<GameObject> gameObject);
	Transform(std::shared_ptr<GameObject> gameObject);

	~Transform();

	void setPosition(DirectX::XMFLOAT3 position);
	void setScale(DirectX::XMFLOAT3 scale);
	void setRotation(DirectX::XMFLOAT3 rotation);
	DirectX::XMFLOAT3 getPosition() const;
	DirectX::XMFLOAT3 getScale() const;
	DirectX::XMFLOAT3 getRotation() const;
	DirectX::XMMATRIX getWorldMatrix() const;
};

