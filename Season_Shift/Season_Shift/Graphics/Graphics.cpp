#include "pch.h"
#include "Graphics.h"
#include <random>
#include <imgui_impl_win32.h>

Graphics::Graphics(HWND& hwnd, UINT clientWidth, UINT clientHeight) :
	m_clientWidth(clientWidth),
	m_clientHeight(clientHeight)
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

	// Init post proc
	m_postProcessVariables.clientHeight = clientHeight;
	m_postProcessVariables.clientWidth = clientWidth;
	m_postProcessVariables.deltaTime = 0.;
	m_postProcessVariables.elapsedTime = 0.;
	m_postProcessVariables.randomNumber = 0.;
	m_postProcessVariables.speedlineRAD = 0.34;
	m_postProcessVariables.speedlineThickness = 0.00003;
	m_postProcessVariables.speedlineSpeedFactor = 1.4;
	m_postProcessVariables.motionBlurSamples = 2;

}

Graphics::~Graphics()
{

}

void Graphics::render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera, long double dt)
{
	// view frustum culling is done outside this! --> Makes use of the bounding box and camera, non-graphical components.
	
	// general post proc variables
	m_postProcessVariables.clientHeight = m_clientHeight;
	m_postProcessVariables.clientWidth = m_clientWidth;
	m_postProcessVariables.deltaTime = dt;
	m_postProcessVariables.elapsedTime += dt;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	m_postProcessVariables.randomNumber = dis(gen);
	
	m_currRenderStrat->setPostProcessVariables(m_postProcessVariables);

	m_currRenderStrat->render(models, mainCamera, dt);

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

void Graphics::setSpeedlineRadius(float radius)
{
	m_postProcessVariables.speedlineRAD = radius;
}

void Graphics::setSpeedlineSpeedFactor(float factor)
{
	m_postProcessVariables.speedlineSpeedFactor = factor;

}

void Graphics::setSpeedlineThickness(float thickness)
{
	m_postProcessVariables.speedlineThickness = thickness;

}

void Graphics::renderLine(const DirectX::SimpleMath::Vector3& startPos, const DirectX::SimpleMath::Vector3& endPos, bool shouldRender)
{
	m_currRenderStrat->setLineRenderSetttings(startPos, endPos, shouldRender);
}

void Graphics::onResize(UINT width, UINT height)
{
	m_dxDev->onResize(width, height);
	m_currRenderStrat->setUp();
}
