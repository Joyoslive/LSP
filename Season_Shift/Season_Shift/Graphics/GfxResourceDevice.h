#pragma once
#include "DX/DXDevice.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceRepository.h"
#include "Model.h"

/*

Works as a creator for high level graphics resources. Holds repositories for currently loaded graphics resources.

Adds (Assembles them with DXResources), removes, manages existing
- Models
- Meshes
- Material
*/
class GfxResourceDevice
{
private:
	std::shared_ptr<DXDevice> m_dxDev;

	ResourceRepository<std::size_t, Material::ShaderSet> m_shaderSetRepo;
	ResourceRepository<std::size_t, std::shared_ptr<Material>> m_materialRepo;			
	ResourceRepository<std::string, std::shared_ptr<Mesh>> m_meshRepo;

	
private:
	std::shared_ptr<DXTexture> loadTexture(std::string filepath);


public:
	GfxResourceDevice(std::shared_ptr<DXDevice> dev);
	~GfxResourceDevice();

	std::shared_ptr<Material> createMaterial(const std::string& vsFileName, const std::string& psFileName, const std::string& ambPath, const std::string& difPath, const std::string& specPath, const std::string& normPath);

	std::shared_ptr<Mesh> createMesh(const std::string& meshID, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	/*
	Assumes you have created a mesh prior
	*/
	std::shared_ptr<Model> assembleModel(const std::string& meshID, std::shared_ptr<Material> material);





	/*

	<-> Game object specific shaders
	shaderEnum: 
	- Default					(phong material)
	- WallShader
	- GroundShader			
	- SomeSpecificObjectShader
	- IceShader					(phong material + PS depth shader illusion?)
	...

	Create Mesh (vbDat, ibDat)							--> Create mesh manually
	Load Material (vector<fileName>, shaderENUM)		--> Load material manually
	Assemble Model (mesh, material)						--> Assemble custom model from custom mesh and material

	Load Model (fileName, shaderENUM)					--> Model file		--> Mesh (VB, IB) + Material (Assumes VS/PS combo + Textures and CBs held with std::pair)

	Model
	{
		vector<Transform*> instanceData

		Mesh
		Material

		struct renderPriority: (bitfielded struct --> uint16/32) ________ _____		(shaders left then material right)
	}

	--> When you add Model Component to Game Object:
	--> In GO constructor.. given that you have constructed the Transform component!.. model.addInstance(&transform);
	--> This lets us utillize instancing and gives the renderer complete information!

	--> std::sort(modelsToRender.begin(), modelsToRender.end());	: Let it sort by checking renderPriority (operator overloading?)


	ShaderSet (Specific Game Object Shaders if any --> Theres a DEFAULT shader if no specific shaders)
	{
		id (ENUM) [0, 31]
		
		vs
		ps
		-- can be extended to incorporate more stages
	}

	Material
	{
		id [0, 1023] 

		vector<pair<slot, std::shared_ptr<DXTexture>>> textures;			--> Depending on shader, we can vary the slot placement of various textures/buffers
		or
		vector<pair<slot, std::shared_ptr<TextureHndl>>> textures;			--> Depending on shader, we can vary the slot placement of various textures/buffers

		ShaderSet

		
		// Large single buffers to store CPU variables to manipulate vertex shader or pixel shader
		std::shared_ptr<DXBuffer> psCB;										
		std::shared_ptr<DXBuffer> vsCB;		

		std::shared_ptr<BufferHdnl> vsCB;

		std::vector<std::pair<"variableName", 


		-- can be extended to incorporate more stages

	}

	--> GameObject and their scripts can do:
	matData = mat->getMaterialDataRef();
	in loop..:
		matData.uvOffset = vec2(sinf(time), cosf(time));


	Shaders and Constant Buffers to be used are loaded beforehand! The only 'dynamic' resource that is created and loaded in multiple points of the program are Textures and VB/IBs




	*/
};

