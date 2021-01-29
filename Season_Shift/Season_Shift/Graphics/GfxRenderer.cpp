#include "GfxRenderer.h"

GfxRenderer::GfxRenderer(std::shared_ptr<DXDevice> dev) :
	m_dxDev(dev)
{

}

GfxRenderer::~GfxRenderer()
{
}

DXDevice* const GfxRenderer::getDXDevice()
{
	return m_dxDev.get();
}
