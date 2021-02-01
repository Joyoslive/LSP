#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"

class Transform : public Component
{
private:
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_scale;
	DirectX::SimpleMath::Vector3 m_rotation;

	DirectX::SimpleMath::Matrix m_positionMatrix;
	DirectX::SimpleMath::Matrix m_scaleMatrix;
	DirectX::SimpleMath::Matrix m_rotationMatrix;

	const float m_RADTODEG = 180.0f / DirectX::XM_PI;
	const float m_DEGTORAD = DirectX::XM_PI / 180.0f;
	
private:
	void setPositionMatrix();
	void setScaleMatrix();
	void setRotationMatrix();
	DirectX::SimpleMath::Vector3 getRotationToRadians(DirectX::SimpleMath::Vector3 degreesRotation) const;
	DirectX::SimpleMath::Vector3 getRotationToDegrees(DirectX::SimpleMath::Vector3 radiansRotation) const;
public:
	Transform(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 rotation, std::shared_ptr<GameObject> gameObject);
	Transform(std::shared_ptr<GameObject> gameObject);

	~Transform();

	void setPosition(DirectX::SimpleMath::Vector3 position);
	void setScale(DirectX::SimpleMath::Vector3 scale);
	void setRotation(DirectX::SimpleMath::Vector3 rotation);
	DirectX::SimpleMath::Vector3 getPosition() const;
	DirectX::SimpleMath::Vector3 getScale() const;
	DirectX::SimpleMath::Vector3 getRotation() const;
	DirectX::SimpleMath::Matrix getWorldMatrix() const;
};

