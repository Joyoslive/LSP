#include "ForwardRenderStrategy.h"

ForwardRenderStrategy::ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	IRenderStrategy(renderer)
{
	OutputDebugStringW(L"Using forward render.\n");
}

ForwardRenderStrategy::~ForwardRenderStrategy()
{
}

void ForwardRenderStrategy::render()
{
	auto dxDev = m_renderer->getDXDevice();

	dxDev->clearScreen();

	// Do lots of rendering stuff with the help of Renderer and various Binder/Technique abstractions in mind! etc.





	dxDev->present();
}
