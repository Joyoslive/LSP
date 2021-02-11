#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>

class DirectionalLight
{
private:
	DirectX::SimpleMath::Vector3 m_direction;
	DirectX::SimpleMath::Vector3 m_color;
	float m_intensity;
public:
	DirectionalLight(DirectX::SimpleMath::Vector3 direction, DirectX::SimpleMath::Vector3 color, float intensity);
	~DirectionalLight();

	DirectX::SimpleMath::Vector3& getDirection();
	DirectX::SimpleMath::Vector3& getColor();
	float getIntensity();
};

