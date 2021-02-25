#include "pch.h"
#include "PointLight.h"
#include "PointLightRepo.h"
#include <assert.h>

using namespace DirectX::SimpleMath;

int PointLight::s_nameNumber = 0;

void PointLight::generateName()
{
	this->m_name = "pointLight" + std::to_string(s_nameNumber++);
}

void PointLight::linkToRepo(std::shared_ptr<PointLightRepo> repo)
{
	if (m_repo == nullptr) m_repo = repo;
}

void PointLight::updateRepo()
{
	if (m_repo != nullptr)
	{
		m_repo->replacePointLightResource(m_PointLightData, m_internalRepoIndex);
	}
}

PointLight::PointLight(const Vector3& position, const Vector3& color, float strength, const std::string& name) : PointLight(position, name)
{
	m_PointLightData.m_color = color;
	m_PointLightData.m_strength = strength;
}

PointLight::PointLight(Vector3 position, std::string name)
{
	m_PointLightData.m_color = Vector3(1, 1, 1);
	m_PointLightData.m_strength = 1.0f;
	m_PointLightData.m_position = Vector4(position.x, position.y, position.z, 1.0f);
	m_lit = true;
	m_name = name;
	m_repo = nullptr;
	m_internalRepoIndex = -1;

	if (name == "")
	{
		generateName();
	}
}

void PointLight::setColor(float r, float g, float b)
{	
	m_PointLightData.m_color = Vector3(r, g, b);
	updateRepo();
}

void PointLight::setColor(const DirectX::SimpleMath::Vector3& rgb)
{
	m_PointLightData.m_color = rgb;
	updateRepo();
}

void PointLight::setColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_PointLightData.m_color = Vector3(r / 255.0f, g / 255.0f, b / 255.0f);
	updateRepo();
}

void PointLight::setPosition(const DirectX::SimpleMath::Vector3& position)
{
	m_PointLightData.m_position = Vector4(position.x, position.y, position.z, 1.0f);
	updateRepo();
}

void PointLight::setPosition(float x, float y, float z)
{
	m_PointLightData.m_position = Vector4(x, y, z, 1.0f);
	updateRepo();
}

void PointLight::setStrength(float strength)
{
	m_PointLightData.m_strength = strength;
	updateRepo();
}

void PointLight::setOnOff(bool on)
{
	m_lit = on;

	if (m_repo != nullptr)
	{
		m_repo->turnOnOff(on, m_internalRepoIndex);
	}
}