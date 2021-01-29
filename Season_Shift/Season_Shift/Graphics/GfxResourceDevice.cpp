#include "GfxResourceDevice.h"

GfxResourceDevice::GfxResourceDevice(std::shared_ptr<DXDevice> dev) :
	m_dxDev(dev)
{
	m_resourceManager = std::make_unique<GfxResourceManager>();
}

GfxResourceDevice::~GfxResourceDevice()
{
}
