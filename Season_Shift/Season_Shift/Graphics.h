#pragma once
#include "DXDevice.h"
#include "DXResourceManager.h"
#include "GfxRenderer.h"
#include "GfxResourceManager.h"
#include "GfxResourceDevice.h"
#include "ForwardRenderStrategy.h"

#include <vector>


/*

Works as the single interface to Graphics.
- Takes in data to be rendered
- Offers creational interface
..

*/
class Graphics
{
private:

	// Specific API (Used for dependency injection)
	std::shared_ptr<DXDevice> m_dxDev;
	
	// High Level Graphics Resource Creator
	std::shared_ptr<GfxResourceDevice> m_gfxDevice;

	// Repositories
	std::shared_ptr<DXResourceManager> m_dxResourceManager;
	std::unique_ptr<GfxResourceManager> m_resourceManager;

	// Rendering components
	std::shared_ptr<GfxRenderer> m_renderer;						
	std::unique_ptr<IRenderStrategy> m_renderStrat;		


public:
	Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~Graphics();

	void render();
	//void setRenderStrategy();


};

