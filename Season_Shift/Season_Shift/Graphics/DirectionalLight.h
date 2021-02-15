#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

class DirectionalLight
{
public:
	struct alignas(16) DirLight
	{
		DirectX::SimpleMath::Vector3 direction;
		float ambientIntensity;
		DirectX::SimpleMath::Vector4 color;
	};
private:
	DirectX::SimpleMath::Vector3 m_direction;
	DirectX::SimpleMath::Vector3 m_color;
	float m_ambIntensity;
public:
	DirectionalLight();
	DirectionalLight(DirectX::SimpleMath::Vector3 direction, DirectX::SimpleMath::Vector3 color = {1,1,1}, float ambientIntensity = 0.1);
	~DirectionalLight();

	const DirectX::SimpleMath::Vector3& getDirection();
	const DirectX::SimpleMath::Vector3& getColor();
	const float getIntensity();
	DirLight getLight();
};

