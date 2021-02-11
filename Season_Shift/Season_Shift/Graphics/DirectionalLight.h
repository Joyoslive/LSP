#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

class DirectionalLight
{
public:
	struct alignas(16) DirLight
	{
		DirectX::SimpleMath::Vector3 direction;
		float intensity;
		DirectX::SimpleMath::Vector3 color;
	};
private:
	DirectX::SimpleMath::Vector3 m_direction;
	DirectX::SimpleMath::Vector3 m_color;
	float m_intensity;
public:
	DirectionalLight();
	DirectionalLight(DirectX::SimpleMath::Vector3 direction, DirectX::SimpleMath::Vector3 color, float intensity);
	~DirectionalLight();

	DirectX::SimpleMath::Vector3& getDirection();
	DirectX::SimpleMath::Vector3& getColor();
	float getIntensity();
	DirLight getLight();
};

