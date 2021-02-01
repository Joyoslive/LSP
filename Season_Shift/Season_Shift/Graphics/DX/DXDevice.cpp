#include "DXDevice.h"

DXDevice::DXDevice(HWND& hwnd, UINT clientWidth, UINT clientHeight) :
	m_core(DXCore(hwnd, clientWidth, clientHeight))
{

}

DXDevice::~DXDevice()
{
}

std::shared_ptr<DXBuffer> DXDevice::createVertexBuffer(unsigned int elementSize, unsigned int elementStride, bool dynamic, bool cpuUpdates, bool streamOut, D3D11_SUBRESOURCE_DATA* subres)
{
	D3D11_BUFFER_DESC desc = { };
	desc.ByteWidth = elementSize * elementStride;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (streamOut)
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	else
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (dynamic && cpuUpdates)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (dynamic)
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}
	else
	{
		if (subres == nullptr)	assert(false);	// Immutable but no initial data!?!?

		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}

	std::shared_ptr<DXBuffer> toRet = nullptr;

	//HRCHECK(m_core.getDevice()->CreateBuffer(&desc, subres, )
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
