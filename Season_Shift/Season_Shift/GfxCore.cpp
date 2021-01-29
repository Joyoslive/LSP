#include "GfxCore.h"

GfxCore::GfxCore(std::shared_ptr<DXDevice> dev) :
	m_dxDev(dev)
{

}

GfxCore::~GfxCore()
{
}

void GfxCore::clearScreen(float r, float g, float b)
{
	m_dxDev->clearScreen(r, g, b);
}
