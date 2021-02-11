#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(DirectX::SimpleMath::Vector3 direction, DirectX::SimpleMath::Vector3 color, float intensity)
	: m_direction(direction), m_color(color), m_intensity(intensity)
{

}

DirectionalLight::~DirectionalLight()
{}

DirectX::SimpleMath::Vector3& DirectionalLight::getDirection()
{
	return m_direction;
}

DirectX::SimpleMath::Vector3& DirectionalLight::getColor()
{
	return m_color;
}

float DirectionalLight::getIntensity()
{
	return m_intensity;
}
