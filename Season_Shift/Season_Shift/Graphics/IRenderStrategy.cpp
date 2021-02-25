#include "pch.h"
#include "IRenderStrategy.h"

IRenderStrategy::IRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	m_renderer(renderer)
{
}

IRenderStrategy::~IRenderStrategy()
{
}
