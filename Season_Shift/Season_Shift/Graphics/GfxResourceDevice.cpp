#include "GfxResourceDevice.h"
#include <functional>

GfxResourceDevice::GfxResourceDevice(std::shared_ptr<DXDevice> dev) :
	m_dxDev(dev)
{
	
}

GfxResourceDevice::~GfxResourceDevice()
{

}

std::shared_ptr<DXTexture> GfxResourceDevice::loadTexture(std::string filepath)
{
	auto texData = loadFileToTexture(filepath);
	DXTexture::Desc desc{
		DXTexture::Type::TEX2D,
	};
	desc.desc2D = {
		texData.width,
		texData.height,
		0,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		{1, 0},
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
	};

	D3D11_SUBRESOURCE_DATA initData{
		texData.data,
		texData.width * texData.channels,
		0,
	};

	free(texData.data);

	return m_dxDev->createTexture(desc, &initData);
}

std::shared_ptr<Mesh> GfxResourceDevice::createMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
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

std::shared_ptr<Material> GfxResourceDevice::createMaterial(std::string& ambPath, std::string& difPath, std::string& specPath, std::string& normPath)
{
	Material::PhongMaps maps;
	maps.ambient  = loadTexture(ambPath);
	maps.diffuse  = loadTexture(difPath);
	maps.specular = loadTexture(specPath);
	maps.normal   = loadTexture(normPath);

	std::string toBeHashed = ambPath + difPath + specPath + normPath;
	size_t hash = std::hash<std::string>{}(toBeHashed);

	std::make_shared<Material>(Material(maps, hash));
	return std::shared_ptr<Material>();
}
