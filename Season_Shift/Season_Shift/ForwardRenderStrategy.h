#pragma once
#include "IRenderStrategy.h"
#include "GfxRenderer.h"

class ForwardRenderStrategy : public IRenderStrategy
{
private:
	std::shared_ptr<GfxRenderer> m_renderer;

public:
	ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~ForwardRenderStrategy();
	void render();
};

