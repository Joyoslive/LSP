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
		DirectX::SimpleMath::Vector4 ambColor;
		DirectX::SimpleMath::Vector4 difColor;
	};
private:
	DirectX::SimpleMath::Vector3 m_direction;
	DirectX::SimpleMath::Vector3 m_ambColor;
	DirectX::SimpleMath::Vector3 m_difColor;
	float m_intensity;
public:
	DirectionalLight();
	DirectionalLight(DirectX::SimpleMath::Vector3 direction, DirectX::SimpleMath::Vector3 ambColor, DirectX::SimpleMath::Vector3 difColor, float intensity);
	~DirectionalLight();

	DirectX::SimpleMath::Vector3& getDirection();
	DirectX::SimpleMath::Vector3& getAmbColor();
	DirectX::SimpleMath::Vector3& getDifColor();
	float getIntensity();
	DirLight getLight();
};

