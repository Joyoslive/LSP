#pragma once
#include <utility>
#include "DX/DXDevice.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceRepository.h"
#include "Model.h"
#include "AssimpLoader.h"

/*

Works as a creator for high level graphics resources. Holds repositories for currently loaded graphics resources.

Adds (Assembles them with DXResources), removes, manages existing
- Models
- Meshes
- Material
*/


// Put this under Graphics namespace!
enum class GfxShader
{
	DEFAULT
};

class GfxResourceDevice
{
private:
	std::shared_ptr<DXDevice> m_dxDev;
	std::unique_ptr<AssimpLoader> m_assimpLoader;

	ResourceRepository<std::size_t, Material::ShaderSet> m_shaderSetRepo;
	ResourceRepository<std::size_t, std::shared_ptr<Material>> m_materialRepo;			
	ResourceRepository<std::string, std::shared_ptr<Mesh>> m_meshRepo;

	
private:
	std::shared_ptr<DXTexture> loadTexture(std::string filepath);
	std::pair<std::size_t, Material::ShaderSet> loadShader(GfxShader shader);


public:
	GfxResourceDevice(std::shared_ptr<DXDevice> dev);
	~GfxResourceDevice();

	/*
	Create material to use for Model assembly
	*/
	std::shared_ptr<Material> createMaterial(GfxShader shader, const std::string& difPath, const std::string& specPath, const std::string& normPath);

	/*
	Create a mesh, store it with an ID for Model assembly
	*/
	std::shared_ptr<Mesh> createMesh(const std::string& meshID, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	/*
	Assumes you have created a mesh
	*/
	std::shared_ptr<Model> assembleModel(const std::string& meshID, std::shared_ptr<Material> material);

	/*
	Create single model (one mesh) from file
	*/
	std::shared_ptr<Model> createModel(const std::string& fileName);


};

