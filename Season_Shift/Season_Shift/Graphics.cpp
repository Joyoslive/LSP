#include "Graphics.h"

Graphics::Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight)
{
	m_renderer = std::make_shared<GfxRenderer>(hwnd, clientWidth, clientHeight);
	m_renderStrat = std::make_unique<ForwardRenderStrategy>(m_renderer);
}

Graphics::~Graphics()
{

}

void Graphics::render()
{
	m_renderStrat->render();
}
