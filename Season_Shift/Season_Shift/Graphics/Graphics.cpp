#include "Graphics.h"

Graphics::Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight)
{
	// API
	m_dxDev = std::make_shared<DXDevice>(hwnd, clientWidth, clientHeight);
	
	m_gfxDevice = std::make_shared<GfxResourceDevice>(m_dxDev);

	/* 
	Utilize the DirectX API in various components
	*/
	m_renderer = std::make_shared<GfxRenderer>(m_dxDev);
	m_currRenderStrat = std::make_unique<ForwardRenderStrategy>(m_renderer);

}

Graphics::~Graphics()
{

}

void Graphics::render(const std::vector<std::shared_ptr<Model>>& models)
{
	// view frustum culling is done outside this! --> Makes use of the bounding box and camera, non-graphical components.
	
	m_currRenderStrat->render(models);

}

const std::shared_ptr<GfxResourceDevice>& Graphics::getResourceDevice()
{
	return m_gfxDevice;
}
