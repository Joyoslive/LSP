#pragma once
#include <vector>
#include "GfxRenderer.h"
#include "ForwardRenderStrategy.h"

class RenderData;

class Graphics
{
private:
	std::shared_ptr<GfxRenderer> m_renderer;
	std::unique_ptr<IRenderStrategy> m_renderStrat;

public:
	Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight);
	~Graphics();

	void render();
	//void setRenderStrategy();


};

