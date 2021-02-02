#include "GfxResourceDevice.h"

GfxResourceDevice::GfxResourceDevice(std::shared_ptr<DXDevice> dev) :
	m_dxDev(dev)
{
	
}

GfxResourceDevice::~GfxResourceDevice()
{
}

std::shared_ptr<Mesh> GfxResourceDevice::createMesh(const std::string& meshID, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	D3D11_SUBRESOURCE_DATA subres;
	ZeroMemory(&subres, sizeof(D3D11_SUBRESOURCE_DATA));
	subres.pSysMem = vertices.data();
	auto vb = m_dxDev->createVertexBuffer(vertices.size(), sizeof(Vertex), false, false, false, &subres);

	subres.pSysMem = indices.data();
	auto ib = m_dxDev->createIndexBuffer(indices.size() * sizeof(uint32_t), false, &subres);

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vb, ib);
	return mesh;
}
