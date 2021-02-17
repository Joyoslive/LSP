#include "Graphics.h"
#include <imgui_impl_win32.h>

Graphics::Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight)
{
	// API
	m_dxDev = std::make_shared<DXDevice>(hwnd, clientWidth, clientHeight);
	
	m_gfxDevice = std::make_shared<GfxResourceDevice>(m_dxDev);

	/* 
	Utilize the DirectX API in various components
	*/
	m_renderer = std::make_shared<GfxRenderer>(m_dxDev);
	//m_currRenderStrat = std::make_unique<ForwardRenderStrategy>(m_renderer);
	m_currRenderStrat = std::make_unique<DeferredRenderStrategy>(m_renderer);

	// Setup skyboxes
	m_skybox = std::make_shared<Skybox>(m_renderer);
	m_currRenderStrat->setSkybox(m_skybox);

	// Default skybox
	m_skybox->loadSkybox("Textures/Skyboxes/yokohama");
	m_skybox->setSkybox(0);

	m_dirLight = std::make_shared<DirectionalLight>();
	m_currRenderStrat->setDirLight(m_dirLight);
}

Graphics::~Graphics()
{

}

void Graphics::render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera)
{
	// view frustum culling is done outside this! --> Makes use of the bounding box and camera, non-graphical components.

	m_currRenderStrat->render(models, mainCamera);


	ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_currRenderStrat->present();
}

const std::shared_ptr<GfxResourceDevice>& Graphics::getResourceDevice()
{
	return m_gfxDevice;
}

void Graphics::loadSkybox(const std::filesystem::path directoryPath)
{
	// perhaps make some check here if the skybox is already loaded, none for now
	m_skybox->loadSkybox(directoryPath);
}

void Graphics::setSkybox(unsigned int idx)
{
	m_skybox->setSkybox(idx);
}

void Graphics::setLightDirection(const DirectX::SimpleMath::Vector3& direction)
{
	m_dirLight->setDirection(direction);
}

void Graphics::onResize(UINT width, UINT height)
{
	m_dxDev->onResize(width, height);
	m_currRenderStrat->setUp();
}
