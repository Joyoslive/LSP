#include "Graphics.h"

Graphics::Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight)
{
	m_dxDev = std::make_shared<DXDevice>(hwnd, clientWidth, clientHeight);
	m_renderer = std::make_shared<GfxRenderer>(m_dxDev);

	m_renderStrat = std::make_unique<ForwardRenderStrategy>(m_renderer);
}

Graphics::~Graphics()
{

}

void Graphics::render()
{
	m_renderStrat->render();
}
