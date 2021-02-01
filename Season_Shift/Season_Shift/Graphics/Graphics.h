#pragma once
#include "DX/DXDevice.h"
#include "GfxRenderer.h"
#include "GfxResourceDevice.h"
#include "ForwardRenderStrategy.h"

#include <vector>



/*

Works as the single interface to Graphics.
- Takes in data to be rendered
- Offers high level creational interface for graphics resources
- User of this class is expected to use the 'GfxResourceDevice' in order to create graphics resources
To-do --> Also expose 'GfxResourceManager' to flush/ask for/remove existing resources

*/
class Graphics
{
public:
	enum class RenderType
	{
		FORWARD		= 0,		// Temporary setup forward to begin game development iteration faster
		DEFERRED	= 1			// Primary rendering setup to develop techniques in
	};

	/*
	
	enum class GameShaders
	{
		...
		...
	}
	
	*/

private:
	/*
	
	Since we are only working with DirectX 11, the API will be utilized directly without an abstraction layer that represents non-API bound graphics resources.
	DX - Refers to API specific components
	Gfx - Refers to a higher level components that 'utilize' the specific graphics API
	
	This means that implementations in RenderStrategies will directly use D3D11 resources and their respective thin-wrappers
	*/

	// Graphics API Device
	std::shared_ptr<DXDevice> m_dxDev;
	
	// High Level Graphics Resource Creator
	std::shared_ptr<GfxResourceDevice> m_gfxDevice;

	// Model Loader --> Mesh, Amb, Diff, Spec, Nor
	// AssimpLoader

	// TextureFileReader -- we can just make function
	// Reads textures and converts to raw data: JPG/PNG + others through STB Image

	// Rendering components
	std::shared_ptr<GfxRenderer> m_renderer;
	std::unique_ptr<IRenderStrategy> m_currRenderStrat;

public:
	Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~Graphics();

	/*
	To-add: Data to be rendered! (Mesh, Material, Shader) (Assumes all the data is to be rendered, meaning culling has already been done prior to passing it for rendering)
	*/
	void render();   //

	/*
	
	void setMainRenderCamera();
	void setDirectionalLightCamera();

	void setRenderStrategy(ENUM);
	
	void resize(unsigned int newClientWidth, unsigned int newClientHeight);

	*/

	/*
	Expose creational interface for graphics resources
	*/
	/*
	
	Create Mesh (vbDat, ibDat)							--> Create mesh manually
	Load Material (vector<fileName>, shaderENUM)		--> Load material manually
	Assemble Model (mesh, material)						--> Assemble custom model from custom mesh and material

	Load Model (fileName, shaderENUM)					--> Model file		--> Mesh (VB, IB) + Material (Assumes VS/PS combo + Textures and CBs held with std::pair)		--> Requires ModelLoader
		

	*/

	// Expose shader data upload MAYBE???		--> Remember that we don't want to be uploading to GPU Buffer unless we KNOW that we will use it to render!
	//											--> Above is best alleviated with data living in Material..
	//											--> But it also means we have to store shader data (like a union) which stores the largest data.
	//											--> Above works as long as we have some variable keeping track of which one is active and its size (to copy properly to GPU)



	// We don't have time to create a proper Material System so extendability is not taken into account!
	// We need to know internals while at the same time exposing a common interface?? Doesn't make sense!
	/*

	void UploadShaderData(data*, mtl);		--> e.g (struct defaultShaderData, defaultMaterial);
											--> e.g (struct iceShaderData, iceMaterial1)		// if there are two different materials.
											--> e.g (struct iceShaderData, iceMaterial2)

	// Upload post processing effects
	void UploadShaderData(struct postprocessData);



	*/

};

