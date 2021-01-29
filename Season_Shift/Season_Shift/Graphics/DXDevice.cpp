#include "DXDevice.h"

DXDevice::DXDevice(HWND& hwnd, UINT clientWidth, UINT clientHeight) :
	m_core(nullptr)
{
	m_core = std::make_shared<DXCore>(hwnd, clientWidth, clientHeight);
	m_dxResourceManager = std::make_unique<DXResourceManager>();
}

DXDevice::~DXDevice()
{
}

void DXDevice::clearScreen()
{
	FLOAT color[] = { 0.0, 0.0, 0.0, 1.0 };
	m_core->getImmediateContext()->ClearRenderTargetView(m_core->getBackbufferRTV().Get(), color);
}

void DXDevice::present()
{
	m_core->getSwapChain()->Present(0, 0);
}
