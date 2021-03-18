#include "pch.h"
#include "DirectionalLight.h"
#include <stdexcept>

using namespace DirectX::SimpleMath;

DirectionalLight::DirectionalLight()
	: m_direction(Vector3(0, 0, 1)), m_color(Vector3(1, 1, 1)), m_ambIntensity(0.1f)
{}

DirectionalLight::DirectionalLight(Vector3 direction, Vector3 color, float ambientIntensity)
	: m_direction(direction), m_color(color), m_ambIntensity(ambientIntensity)
{
	m_direction.Normalize();
}

DirectionalLight::~DirectionalLight()
{}

const DirectX::SimpleMath::Vector3& DirectionalLight::getDirection()
{
	return m_direction;
}

const DirectX::SimpleMath::Vector3& DirectionalLight::getColor()
{
	return m_color;
}

const float DirectionalLight::getIntensity()
{
	return m_ambIntensity;
}

DirectionalLight::DirLight DirectionalLight::getLight()
{
	Vector4 col = { m_color.x, m_color.y, m_color.z, 1 };
	return { m_direction, m_ambIntensity, col };
}

void DirectionalLight::setDirection(const Vector3& direction)
{
	if (direction.Length() > 0.00001)
		m_direction = direction;
	else
		throw std::invalid_argument("Length of direction vector must be greater than 0");
}
