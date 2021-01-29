#pragma once
#include <vector>
#include "GfxRenderer.h"
#include "ForwardRenderStrategy.h"

class RenderData;

class Graphics
{
private:

	// Specific API
	std::shared_ptr<DXDevice> m_dxDev;

	// Works with dependency injection (DXDevice)
	std::shared_ptr<GfxRenderer> m_renderer;
	std::unique_ptr<IRenderStrategy> m_renderStrat;

	// Game Resource Creators (Mesh, Model, Material)

public:
	Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~Graphics();

	void render();
	//void setRenderStrategy();


};

