#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <string>
#include <vector>

class PointLightRepo;

class PointLight
{
	friend PointLightRepo;
private:

	struct PointLightResource
	{
		DirectX::SimpleMath::Vector4 m_position;
		DirectX::SimpleMath::Vector3 m_color;
		float m_strength;
	} m_PointLightData;
 
	bool m_lit;
	std::shared_ptr<PointLightRepo> m_repo;
	int m_internalRepoIndex;
	std::string m_name;

	static int s_nameNumber;

private:
	void generateName();
	void linkToRepo(std::shared_ptr<PointLightRepo> repo);
	void updateRepo();
public:

	PointLight(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& color, float strength, const std::string& name = "");
	PointLight(DirectX::SimpleMath::Vector3 position, std::string name = "");
	~PointLight() = default;

	

	void setColor(float r, float g, float b);
	void setColor(const DirectX::SimpleMath::Vector3& rgb);
	void setColor(unsigned char r, unsigned char g, unsigned char b);

	void setPosition(const DirectX::SimpleMath::Vector3& position);
	void setPosition(float x, float y, float z);

	void setStrength(float strength);

	void setOnOff(bool on);

	//getters??
	
};

