#include "ForwardRenderStrategy.h"

ForwardRenderStrategy::ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	m_renderer(renderer)
{
	OutputDebugStringW(L"Using forward render.\n");
}

ForwardRenderStrategy::~ForwardRenderStrategy()
{
}

void ForwardRenderStrategy::render()
{
	m_renderer->clearScreen(0.0, 0.0, 0.0);
}
