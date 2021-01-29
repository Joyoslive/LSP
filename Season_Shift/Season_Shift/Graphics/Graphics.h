#pragma once
#include "DXDevice.h"
#include "GfxRenderer.h"
#include "GfxResourceDevice.h"
#include "ForwardRenderStrategy.h"

#include <vector>


/*

Works as the single interface to Graphics.
- Takes in data to be rendered
- Offers high level creational interface for graphics resources
- User of this class is expected to use the 'GfxResourceDevice' in order to create graphics resources

*/
class Graphics
{
public:
	enum class RenderType
	{
		FORWARD		= 0,		// Temporary setup forward to begin game development iteration faster
		DEFERRED	= 1			// Primary rendering setup to develop techniques in
	};

private:
	/*
	
	Since we are only working with DirectX 11, the API will be utilized directly without an abstraction layer that represents non-API bound graphics resources.
	DX - Refers to API specific components
	Gfx - Refers to a higher level components that 'utilize' the specific graphics API

	*/

	// Graphics API Device
	std::shared_ptr<DXDevice> m_dxDev;
	
	// High Level Graphics Resource Creator
	std::shared_ptr<GfxResourceDevice> m_gfxDevice;

	// Rendering components
	std::shared_ptr<GfxRenderer> m_renderer;
	std::unique_ptr<IRenderStrategy> m_currRenderStrat;

public:
	Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~Graphics();

	/*
	To-add: Data to be rendered! (Mesh, Material, Shader) (Assumes all the data is to be rendered, meaning culling has already been done prior to passing it for rendering)
	*/
	void render();

	/*
	
	void setMainRenderCamera();
	void setDirectionalLightCamera();

	void setRenderStrategy(ENUM);
	
	void resize(unsigned int newClientWidth, unsigned int newClientHeight);

	*/

	/*
	Expose creational interface for graphics resources
	*/
	GfxResourceDevice* const getResourceDevice();

};

