#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <string>
#include <vector>

class PointLight
{
private:
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_color;
	float m_strength;
	bool m_lit;
	std::string m_name;

	//static std::vector<std::string> s_names;
	static int s_nameNumber;

private:
	void generateName();
public:

	PointLight(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 color, float strength, std::string name = "");
	PointLight(DirectX::SimpleMath::Vector3 position, std::string name = "");
	~PointLight() = default;

	void setColor(float r, float g, float b);
	void setColor(const DirectX::SimpleMath::Vector3& rgb);
	void setColor(unsigned char r, unsigned char g, unsigned char b);

	void setPosition(const DirectX::SimpleMath::Vector3& position);
	void setPosition(float x, float y, float z);

	void setStrength(float strength);

	void setOnOff(bool on);
};

