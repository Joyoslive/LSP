#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"

class Transform : public Component
{
private:
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_oldposition;
	DirectX::SimpleMath::Vector3 m_scale;
	DirectX::SimpleMath::Vector3 m_rotation;

	DirectX::SimpleMath::Matrix m_positionMatrix;
	DirectX::SimpleMath::Matrix m_scaleMatrix;
	DirectX::SimpleMath::Matrix m_rotationMatrix;
	DirectX::SimpleMath::Matrix m_worldMatrix;

	const float m_RADTODEG = 180.0f / DirectX::XM_PI;
	const float m_DEGTORAD = DirectX::XM_PI / 180.0f;
	
private:
	void setWorldMatrix();
	void setPositionMatrix();
	void setScaleMatrix();
	void setRotationMatrix();
	DirectX::SimpleMath::Vector3 getRotationToRadians(const DirectX::SimpleMath::Vector3& degreesRotation) const;
	DirectX::SimpleMath::Vector3 getRotationToDegrees(const DirectX::SimpleMath::Vector3& radiansRotation) const;
public:
	Transform(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation);
	Transform();

	~Transform();

	const DirectX::SimpleMath::Matrix& getRotationMatrix() const;
	void setPosition(const DirectX::SimpleMath::Vector3& position);
	void setScale(const DirectX::SimpleMath::Vector3& scale);
	void setRotation(const DirectX::SimpleMath::Vector3& rotation);
	const DirectX::SimpleMath::Vector3& getPosition() const;
	DirectX::SimpleMath::Vector3 getDeltaPosition() const;
	const DirectX::SimpleMath::Vector3& getScale() const;
	DirectX::SimpleMath::Vector3 getRotation() const;
	const DirectX::SimpleMath::Matrix& getWorldMatrix() const;
};

