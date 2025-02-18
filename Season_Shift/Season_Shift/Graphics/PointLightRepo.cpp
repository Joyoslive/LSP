#include "pch.h"
#include "PointLightRepo.h"

PointLightRepo::PointLightRepo(std::shared_ptr<DXDevice> dxDev)//cosnt ref
{
	m_latestUsedLightIndex = 0;
	m_rebuildBuffer = true;
	m_updateBuffer = false;
	m_dxDev = dxDev;
	ZeroMemory(&m_subresData, sizeof(D3D11_SUBRESOURCE_DATA));
}

PointLightRepo::~PointLightRepo()
{

}

void PointLightRepo::addPointLight(PointLight& pointLight)
{
	if (findPointLight(pointLight.m_name)) return; //can't have same


	m_pointLightVector.push_back(std::pair(pointLight, -1)); // hope pair() stores a copy of pointlight
	m_latestUsedLightIndex = m_pointLightVector.size() - 1;
	m_rebuildBuffer = true;
	pointLight.linkToRepo(shared_from_this()); // use non const refrence to link repo
	pointLight.m_internalRepoIndex = static_cast<int>(m_pointLightVector.size()) - 1;
}

bool PointLightRepo::findPointLight(const std::string& name)
{
	for (int i = 0; i < m_pointLightVector.size(); ++i)
	{
		if (name.compare(m_pointLightVector[i].first.m_name))
		{
			m_latestUsedLightIndex = i;
			return true;
		}
	}
	return false;

}

const PointLight& PointLightRepo::getPointLight(const std::string& name)
{
	if (name.compare(m_pointLightVector[m_latestUsedLightIndex].first.m_name)) return m_pointLightVector[m_latestUsedLightIndex].first; // fast check, if someone called findPointLight beforehand

	for (int i = 0; i < m_pointLightVector.size(); ++i)
	{
		if (name.compare(m_pointLightVector[i].first.m_name))
		{
			m_latestUsedLightIndex = i;
			return m_pointLightVector[i].first;
		}
	}
	assert(false);
	return m_pointLightVector[0].first; // will never return
}

std::shared_ptr<DXBuffer> PointLightRepo::getStructuredLightBuffer()
{
	if (m_rebuildBuffer)
	{
		createBuffer();
	}
	else if(m_updateBuffer)
	{
		updateBuffer();
	}

	return m_structuredBuffer;
}


void PointLightRepo::createBuffer()
{
	m_pointLightResourceVector.clear();
	m_pointLightResourceVector.reserve(m_pointLightVector.size());

	for (int i = 0; i < m_pointLightVector.size(); ++i)
	{
		m_pointLightVector[i].second = -1; // reset index to resourceVector
		if (m_pointLightVector[i].first.m_lit)
		{
			m_pointLightResourceVector.push_back(m_pointLightVector[i].first.m_PointLightData);
			m_pointLightVector[i].second = static_cast<int>(m_pointLightResourceVector.size()) - 1; // save index so we can map into resourceVector later
		}
	}

	//create structerd buffer
	m_subresData.pSysMem = m_pointLightResourceVector.data();
	unsigned int count = static_cast<unsigned int>(m_pointLightResourceVector.size());
	m_structuredBuffer = m_dxDev->createStructuredBuffer(count, sizeof(PointLight::PointLightResource), true, false, &m_subresData);

	m_rebuildBuffer = false;
	m_updateBuffer = false;
}

void PointLightRepo::updateBuffer()
{
	m_structuredBuffer->updateMapUnmap(m_pointLightResourceVector.data(), static_cast<unsigned int>(
		m_pointLightResourceVector.size() * sizeof(PointLight::PointLightResource)));
	m_updateBuffer = false;
}

void PointLightRepo::replacePointLightResource(const PointLight::PointLightResource& data, int index)
{
	m_pointLightVector[index].first.m_PointLightData = data; // update PointLight
	if (m_pointLightVector[index].second != -1) // update resource vector if it has owns a copy of the resource
	{
		m_pointLightResourceVector[m_pointLightVector[index].second] = data;
		m_updateBuffer = true;
	}
}

void PointLightRepo::turnOnOff(bool on, int index)
{
	if (m_pointLightVector[index].first.m_lit == on) return; // no change, early exit

	m_pointLightVector[index].first.m_lit = on;
	m_rebuildBuffer = true;
}