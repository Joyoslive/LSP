#pragma once
#include "IRenderStrategy.h"
#include "GfxRenderer.h"

class ForwardRenderStrategy : public IRenderStrategy
{
public:
	ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~ForwardRenderStrategy();
	void render();
};

