#include "GfxResourceDevice.h"

GfxResourceDevice::GfxResourceDevice(std::shared_ptr<DXDevice> dev) :
	m_dxDev(dev)
{
	
}

GfxResourceDevice::~GfxResourceDevice()
{
}
