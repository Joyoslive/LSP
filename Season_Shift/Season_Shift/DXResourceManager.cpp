#include "DXResourceManager.h"

DXResourceManager::DXResourceManager(std::shared_ptr<DXDevice> dev) :
	m_dxDev(dev)
{
}

DXResourceManager::~DXResourceManager()
{
}
