#pragma once
#include "PointLight.h"
#include "DX/DXBuffer.h"
#include "DX\DXDevice.h"
#include <vector>
#include <memory>
#include <utility>
#include <string>


class PointLightRepo : public std::enable_shared_from_this<PointLightRepo>
{
	friend PointLight;
private:


	bool m_rebuildBuffer;
	bool m_updateBuffer;
	size_t m_latestUsedLightIndex;
	D3D11_SUBRESOURCE_DATA m_subresData;

	std::shared_ptr<DXDevice> m_dxDev;
	std::shared_ptr<DXBuffer> m_structuredBuffer;


	std::vector<std::pair<PointLight, int>> m_pointLightVector;
	std::vector<PointLight::PointLightResource> m_pointLightResourceVector;

public:
	PointLightRepo(std::shared_ptr<DXDevice> dxDev);
	~PointLightRepo();


	void addPointLight(PointLight& pointLight);

	
	

	bool findPointLight(const std::string& name);
	const PointLight& getPointLight(const std::string& name);

	std::shared_ptr<DXBuffer> getStructuredLightBuffer();


private:
	void createBuffer();
	void updateBuffer();
	void replacePointLightResource(PointLight::PointLightResource data, int index);
	void turnOnOff(bool on, int index);
};

