#include "GfxRenderer.h"

GfxRenderer::GfxRenderer(std::shared_ptr<DXDevice> dev) :
	m_dxDev(dev)
{

}

GfxRenderer::~GfxRenderer()
{
}

void GfxRenderer::clearScreen(float r, float g, float b)
{
	m_dxDev->clearScreen(r, g, b);
	
}
