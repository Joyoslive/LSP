#include "DirectionalLight.h"

using namespace DirectX::SimpleMath;

DirectionalLight::DirectionalLight()
	: m_direction(Vector3(0, 0, 1)), m_ambColor(Vector3(0.1, 0.1, 0.1)), m_difColor(Vector3(1,1,1)),  m_intensity(1)
{}

DirectionalLight::DirectionalLight(Vector3 direction, Vector3 ambColor, Vector3 difColor, float intensity)
	: m_direction(direction), m_ambColor(ambColor), m_difColor(difColor), m_intensity(intensity)
{}

DirectionalLight::~DirectionalLight()
{}

const DirectX::SimpleMath::Vector3& DirectionalLight::getDirection()
{
	return m_direction;
}

const DirectX::SimpleMath::Vector3& DirectionalLight::getAmbColor()
{
	return m_ambColor;
}

const DirectX::SimpleMath::Vector3& DirectionalLight::getDifColor()
{
	return m_difColor;
}

const float DirectionalLight::getIntensity()
{
	return m_intensity;
}

DirectionalLight::DirLight DirectionalLight::getLight()
{
	Vector4 ambient = Vector4(m_ambColor.x, m_ambColor.y, m_ambColor.z, 1);
	Vector4 diffuse = Vector4(m_difColor.x, m_difColor.y, m_difColor.z, 1);

	return {m_direction, m_intensity, ambient, diffuse};
}
