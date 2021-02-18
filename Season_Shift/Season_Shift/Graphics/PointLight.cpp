#include "PointLight.h"
#include <assert.h>

using namespace DirectX::SimpleMath;

int PointLight::s_nameNumber = 0;
std::vector<std::string> PointLight::s_names = std::vector<std::string>();

void PointLight::generateName()
{
	bool unique = true;
	this->m_name = "pointLight" + std::to_string(s_nameNumber++);

	for (auto& str : s_names)
	{
		if (str == m_name)
		{
			unique = false;
		}
	}
	if(!unique) generateName();
}

PointLight::PointLight(Vector3 position, Vector3 color, float strength, std::string name) : PointLight(position, name)
{
	m_color = color;
	m_strength = strength;
}

PointLight::PointLight(Vector3 position, std::string name)
{
	m_color = Vector3(1, 1, 1);
	m_strength = 1.0f;
	m_position = position;
	m_name = name;

	if (name == "")
	{
		generateName();
	}
	for (auto& str : s_names)
	{
		assert(str != m_name);
	}
	
	s_names.push_back(m_name);
}
