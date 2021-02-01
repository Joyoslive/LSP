#include "DXDevice.h"

DXDevice::DXDevice(HWND& hwnd, UINT clientWidth, UINT clientHeight) :
	m_core(DXCore(hwnd, clientWidth, clientHeight))
{

}

DXDevice::~DXDevice()
{
}

void DXDevice::clearScreen()
{
	FLOAT color[] = { 0.0, 0.0, 0.0, 1.0 };
	m_core.getImmediateContext()->ClearRenderTargetView(m_core.getBackbufferRTV().Get(), color);
}

void DXDevice::present()
{
	m_core.getSwapChain()->Present(0, 0);
}
