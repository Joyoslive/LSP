#include "DXDevice.h"

DXDevice::DXDevice(HWND& hwnd, UINT clientWidth, UINT clientHeight) :
	m_core(nullptr)
{
	m_core = std::make_shared<DXCore>(hwnd, clientWidth, clientHeight);
}

DXDevice::~DXDevice()
{
}

void DXDevice::clearScreen(float r, float g, float b)
{
	FLOAT color[] = { r, g, b, 1.0 };
	m_core->getImmediateContext()->ClearRenderTargetView(m_core->getBackbufferRTV().Get(), color);
	m_core->getSwapChain()->Present(0, 0);
}
