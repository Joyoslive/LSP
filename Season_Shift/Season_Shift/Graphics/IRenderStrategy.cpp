#include "IRenderStrategy.h"

IRenderStrategy::IRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	m_renderer(renderer)
{
	m_clientWidth = renderer->getDXDevice()->getClientWidth();
	m_clientHeight = renderer->getDXDevice()->getClientHeight();
}

IRenderStrategy::~IRenderStrategy()
{
}
