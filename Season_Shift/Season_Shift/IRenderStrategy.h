#pragma once
#include "GfxRenderer.h"
#include <vector>

class IRenderStrategy
{
protected:
	std::shared_ptr<GfxRenderer> m_renderer;

public:
	IRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	virtual ~IRenderStrategy();
	virtual void render() = 0;
};

