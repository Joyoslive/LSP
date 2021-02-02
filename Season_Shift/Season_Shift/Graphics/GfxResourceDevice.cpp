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
		static_cast<UINT>(texData.width),
		static_cast<UINT>(texData.height),
		1,
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
		texData.width * sizeof(uint32_t),
		0,
	};


	auto tex = m_dxDev->createTexture(desc, &initData);

	free(texData.data);

	return tex;
}

std::shared_ptr<Mesh> GfxResourceDevice::createMesh(const std::string& meshID, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	if (m_meshRepo.exists(meshID)) assert(false);		// Mesh with same ID already exists!

	D3D11_SUBRESOURCE_DATA subres;
	ZeroMemory(&subres, sizeof(D3D11_SUBRESOURCE_DATA));
	subres.pSysMem = vertices.data();
	auto vb = m_dxDev->createVertexBuffer(vertices.size(), sizeof(Vertex), false, false, false, &subres);

	subres.pSysMem = indices.data();
	auto ib = m_dxDev->createIndexBuffer(indices.size() * sizeof(uint32_t), false, &subres);

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vb, ib);

	m_meshRepo.add(meshID, mesh);

	return mesh;
}

std::shared_ptr<Model> GfxResourceDevice::assembleModel(const std::string& meshID, std::shared_ptr<Material> material)
{
	if (!m_meshRepo.exists(meshID)) assert(false);		// Mesh ID doesn't exist!
	auto mesh = m_meshRepo.find(meshID);			// Get mesh

	std::shared_ptr<Model> modToAdd = std::make_shared<Model>(mesh, material);

	return modToAdd;			// NO REPO FOR NOW FOR MODELS!

}

std::shared_ptr<Material> GfxResourceDevice::createMaterial(const std::string& vsFileName, const std::string& psFileName, const std::string& ambPath, const std::string& difPath, const std::string& specPath, const std::string& normPath)
{

	// Load shaders

	// Check hash first if this shaderset is already loaded!
	std::string shdPathToHash = vsFileName + psFileName;
	size_t shdHash = std::hash<std::string>{}(shdPathToHash);

	Material::ShaderSet shaders;
	if (!m_shaderSetRepo.exists(shdHash))
	{
		auto vs = m_dxDev->createShader(vsFileName, DXShader::Type::VS);
		auto ps = m_dxDev->createShader(psFileName, DXShader::Type::PS);

		shaders = { vs, ps };
	};
	
	shaders = m_shaderSetRepo.add(shdHash, shaders);  // If hash already exists, the existing shader will be returned and old one will be discarded.

	// Load textures
	Material::PhongMaps maps;

	// Check hash first before loading texture!
	std::string texPathToHash = ambPath + difPath + specPath + normPath;
	size_t texHash = std::hash<std::string>{}(texPathToHash);

	std::shared_ptr<Material> matToAdd = nullptr;
	if (!m_materialRepo.exists(texHash))
	{
		maps.ambient = loadTexture(ambPath);
		maps.diffuse = loadTexture(difPath);
		maps.specular = loadTexture(specPath);
		maps.normal = loadTexture(normPath);

		matToAdd = std::make_shared<Material>(shaders, maps, texHash, shdHash);
	}

	std::shared_ptr<Material> mat = m_materialRepo.add(texHash, matToAdd);			// If hash already exists, the existing mat will be returned and matToAdd will be discarded

	return mat;
}
