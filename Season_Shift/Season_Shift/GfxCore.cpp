#include "GfxCore.h"

GfxCore::GfxCore(HWND& hwnd, UINT clientWidth, UINT clientHeight) :
	m_dxDev(nullptr)
{
	m_dxDev = std::make_shared<DXDevice>(hwnd, clientWidth, clientHeight);
}

GfxCore::~GfxCore()
{
}

void GfxCore::clearScreen(float r, float g, float b)
{
	m_dxDev->clearScreen(r, g, b);
}
