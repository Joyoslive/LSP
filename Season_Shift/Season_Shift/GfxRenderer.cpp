#include "GfxRenderer.h"

GfxRenderer::GfxRenderer(HWND& hwnd, UINT clientWidth, UINT clientHeight) :
	m_gfxCore(nullptr)
{
	m_gfxCore = std::make_shared<GfxCore>(hwnd, clientWidth, clientHeight);
}

GfxRenderer::~GfxRenderer()
{
}

void GfxRenderer::clearScreen(float r, float g, float b)
{
	m_gfxCore->clearScreen(r, g, b);
	
}
