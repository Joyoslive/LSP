#include "GfxResourceManager.h"

GfxResourceManager::GfxResourceManager(std::shared_ptr<DXResourceManager> resourceManager) :
	m_dxResourceManager(resourceManager)
{

}

GfxResourceManager::~GfxResourceManager()
{
}
