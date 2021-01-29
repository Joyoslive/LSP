#pragma once
#include "GfxRenderer.h"
#include <vector>

/*

Works as an interface to be able to change between different rendering modes should different implementations be done (e.g forward or deferred)

*/
class IRenderStrategy
{
protected:
	std::shared_ptr<GfxRenderer> m_renderer;

public:
	IRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	virtual ~IRenderStrategy();
	virtual void render() = 0;
};

