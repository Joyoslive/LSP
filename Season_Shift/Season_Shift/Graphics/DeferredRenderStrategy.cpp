#include "pch.h"
#include "DeferredRenderStrategy.h"
#include "Skybox.h"
#include <random>
#include "LineDrawer.h"
#include "../Camera.h"
#include "../Input.h"
#include "2D/Text.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

DeferredRenderStrategy::DeferredRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	IRenderStrategy(renderer)
{
	setupGeometryPass();
	setupPostProcessPass();
	setupLightPass();
	setupUIRenderer();

	m_lineDrawer = std::make_shared<LineDrawer>(renderer);

	m_shadowCascades = { {}, {}, {} };
	m_shadowMapper = std::make_shared<ShadowMapper>(renderer);
	m_cascadeBuffer = renderer->getDXDevice()->createConstantBuffer(sizeof(CascadeBuffer), true, true);
}

void DeferredRenderStrategy::render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera, long double dt)
{
	auto dev = m_renderer->getDXDevice();
	dev->clearScreen();

	m_gbuffers.clear(dev);

	//Update the matrices of the previous frame for motionblur
	DirectX::XMMATRIX prevMatrices[2] = {m_gpMatrices[1], m_gpMatrices[2]};
	m_prevMatrices->updateMapUnmap(&prevMatrices, sizeof(DirectX::XMMATRIX) * 2);

	m_gpMatrices[0] = {};
	m_gpMatrices[1] = mainCamera->getViewMatrix();
	m_gpMatrices[2] = mainCamera->getProjectionMatrix();

	// Set shadow mapper settings (hardcoded to three cascades)		// [0.1, 100] --> [100, 500] --> [500, 1000]
	m_shadowCascades[0] = { mainCamera->getNearPlane() + 100, 4096 * 2 };
	m_shadowCascades[1] = { (mainCamera->getNearPlane() + mainCamera->getFarPlane()) / 2, 4096 };
	m_shadowCascades[2] = { mainCamera->getFarPlane(), 2048 };
	m_shadowMapper->setCascadeSettings(m_shadowCascades);

	// Generate shadow map
	Matrix lightView = DirectX::XMMatrixLookAtLH(Vector3(0.0, 0.0, 0.0), m_dirLight->getDirection(), Vector3(0.0, 1.0, 0.0));
	auto cascades = m_shadowMapper->generateCascades(models, mainCamera, lightView);

	m_geometryPassSolid->bind(dev);
	dev->bindDepthStencilState(m_gDss);
	m_geometryPassSolid->clearAttachedDepthTarget(dev);

	for (auto& mod : models)
	{
		for (auto& mat : mod->getSubsetsMaterial())
		{
			mat.material->bindShader(dev);
			mat.material->bindTextures(dev);
			mat.material->bindBuffers(dev);

			m_gpMatrices[0] = mod->getTransform()->getWorldMatrix();

			m_gpMatrixBuffer->updateMapUnmap(m_gpMatrices, sizeof(m_gpMatrices), 0, D3D11_MAP_WRITE_DISCARD, 0);

			dev->bindShaderConstantBuffer(DXShader::Type::VS, 0, m_gpMatrixBuffer);

			dev->bindDrawIndexedBuffer(mod->getMesh()->getVB(), mod->getMesh()->getIB(), 0, 0);
			dev->drawIndexed(mat.indexCount, mat.indexStart, mat.vertexStart);
		}
	}

	if (m_skybox != nullptr)
		m_skybox->draw(mainCamera);

	if (m_lineDrawer != nullptr)
		m_lineDrawer->draw(mainCamera);

	for (auto& p : m_partSysVec)
	{
		p->simulateAndDraw(mainCamera->getViewMatrix(), mainCamera->getProjectionMatrix(), dt);
	}


	dev->bindRenderTargets({nullptr, nullptr, nullptr, nullptr}, nullptr);

	auto lightData = m_dirLight->getLight();
	m_dirLightBuffer->updateMapUnmap(&lightData, sizeof(lightData));

	CameraBuffer camBuf = { mainCamera->getPosition() };
	m_cameraBuffer->updateMapUnmap(&camBuf, sizeof(camBuf));
	
	CascadeBuffer cascBuf;
	cascBuf.nearCascadeEnd = cascades[0].cascadeEnd; 
	cascBuf.midCascadeEnd = cascades[1].cascadeEnd;
	cascBuf.farCascadeEnd = cascades[2].cascadeEnd;
	cascBuf.mainViewMatrix = mainCamera->getViewMatrix();
	cascBuf.lightViewMatrix = lightView;
	cascBuf.lightNearProjection = cascades[0].projMat;
	cascBuf.lightMidProjection = cascades[1].projMat;
	cascBuf.lightFarProjection = cascades[2].projMat;
	m_cascadeBuffer->updateMapUnmap(&cascBuf, sizeof(cascBuf));
	
	// Bind light pass
	m_lightPass->bind(dev);

	// Send cascade starts
	dev->bindShaderConstantBuffer(DXShader::Type::PS, 7, m_cascadeBuffer);	// Note: Slot 7

	// Send shadow maps
	dev->bindShaderTexture(DXShader::Type::PS, 6, cascades[0].texture);
	dev->bindShaderTexture(DXShader::Type::PS, 7, cascades[1].texture);
	dev->bindShaderTexture(DXShader::Type::PS, 8, cascades[2].texture);

	dev->bindDrawIndexedBuffer(m_fsQuad.getVB(), m_fsQuad.getIB(), 0, 0);
	dev->drawIndexed(6, 0, 0);

	//dev->present();

	dev->bindShaderTexture(DXShader::Type::PS, 0, nullptr);
	dev->bindShaderTexture(DXShader::Type::PS, 1, nullptr);
	dev->bindShaderTexture(DXShader::Type::PS, 2, nullptr);
	dev->bindShaderTexture(DXShader::Type::PS, 3, nullptr);

	dev->bindShaderTexture(DXShader::Type::PS, 6, nullptr);
	dev->bindShaderTexture(DXShader::Type::PS, 7, nullptr);
	dev->bindShaderTexture(DXShader::Type::PS, 8, nullptr);

	dev->bindRenderTargets({ dev->getBackbuffer() }, nullptr);

	if (m_usePostProcessing)
	{
		m_postProcessVariableBuffer->updateMapUnmap(&m_postProcessVariables, sizeof(m_postProcessVariables));

		m_postProcessPass->bind(dev);
		dev->bindDrawIndexedBuffer(m_fsQuad.getVB(), m_fsQuad.getIB(), 0, 0);
		dev->drawIndexed(6, 0, 0);
	}

	dev->bindShaderTexture(DXShader::Type::PS, 0, nullptr);
	dev->bindShaderTexture(DXShader::Type::PS, 1, nullptr);

	if (m_viewUI)
	{
		for (auto& s : m_sprites) 
		{
			if (s->getShow())
			{
				if (m_sprites.size() == 0)
					break;
				s->checkForClick(Input::getInput().mousePos().x, Input::getInput().mousePos().y, Input::getInput().mousePressed(Input::LeftButton));
				s->checkForRelease(Input::getInput().mousePos().x, Input::getInput().mousePos().y, Input::getInput().mouseReleased(Input::LeftButton));
				m_spriteRenderer->queueDraw(s);
			}
		}
		if (Input::getInput().mouseReleased(Input::LeftButton))
		{
			for (auto& s : m_sprites)
			{
				if (s->getShow())
				{
					if (m_sprites.size() == 0)
						break;
					s->globalRelease();
				}
			}
		}
		
		
		m_spriteRenderer->drawQueued(dev);
	}
}

void DeferredRenderStrategy::setSkybox(std::shared_ptr<Skybox> skybox)
{
	m_skybox = skybox;
}

void DeferredRenderStrategy::setDirLight(std::shared_ptr<DirectionalLight> light)
{
	m_dirLight = light;
}

void DeferredRenderStrategy::setPostProcessVariables(PostProcessVariables ppVar)
{
	m_postProcessVariables = ppVar;
}

void DeferredRenderStrategy::setLineRenderSetttings(const LineVariables& settings)
{
	m_lineDrawer->setPoints(settings.startPos, settings.endPos);
	m_lineDrawer->setRenderState(true);
	m_lineDrawer->setOffset(settings.offset);
	m_lineDrawer->setColor(settings.color);
	m_lineDrawer->setThickness(settings.thickness);
}

void DeferredRenderStrategy::addParticleSystem(std::shared_ptr<ParticleSystem> particleSystem)
{
	m_partSysVec.push_back(particleSystem);
}

void DeferredRenderStrategy::removeParticleSystem(const std::shared_ptr<ParticleSystem>& particleSystem)
{
	if (particleSystem == nullptr) return;
	int index = -1;
	for (int i = 0; i < m_partSysVec.size(); i++)
	{
		if (m_partSysVec[i].get() == particleSystem.get())
		{
			index = i;
			break;
		}
	}
	if (index == -1) return;
	m_partSysVec.erase(m_partSysVec.begin() + index);
}

void DeferredRenderStrategy::addToSpriteBatch(std::shared_ptr<ISprite> sprite)
{
	m_sprites.push_back(sprite);
}

void DeferredRenderStrategy::clearSpriteBatch()
{
	m_sprites.clear();
}

void DeferredRenderStrategy::setUp()
{
	setupGeometryPass();
	setupPostProcessPass(); // This before because it generates the prePostTexture
	setupLightPass();
}

void DeferredRenderStrategy::present()
{
	m_renderer->getDXDevice()->present();
}


void DeferredRenderStrategy::setupGeometryPass()
{
	auto dev = m_renderer->getDXDevice();

	// Load input layout
	std::shared_ptr<DXShader> vs = dev->createShader("DefaultVS.cso", DXShader::Type::VS);
	std::vector<D3D11_INPUT_ELEMENT_DESC> ilDesc;
	ilDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ilDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ilDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ComPtr<ID3D11InputLayout> il = dev->createInputLayout(ilDesc, vs->getShaderData());

	// Depth target
	DXTexture::Desc depthDesc;
	depthDesc.desc2D = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D24_UNORM_S8_UINT, dev->getClientWidth(), dev->getClientHeight(), 1, 1, D3D11_BIND_DEPTH_STENCIL);
	depthDesc.type = DXTexture::Type::TEX2D;
	std::shared_ptr<DXTexture> depthTexture = dev->createTexture(depthDesc, nullptr);

	// Create sampler
	D3D11_SAMPLER_DESC sDesc = { };
	sDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.MipLODBias = 0;
	sDesc.MaxAnisotropy = 1;	// not used
	sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sDesc.MinLOD = 0;
	sDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ComPtr<ID3D11SamplerState> minMagLinMipPointSamp = dev->createSamplerState(sDesc);

	// Create a matrix buffer
	m_gpMatrixBuffer = dev->createConstantBuffer(sizeof(DirectX::XMMATRIX) * 3, true, true);

	// Create GBuffers
	DXTexture::Desc gbDesc = { };
	gbDesc.type = DXTexture::Type::TEX2D;
	gbDesc.desc2D.Width = dev->getClientWidth();
	gbDesc.desc2D.Height = dev->getClientHeight();
	gbDesc.desc2D.MipLevels = 1;
	gbDesc.desc2D.ArraySize = 1;
	gbDesc.desc2D.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	gbDesc.desc2D.SampleDesc.Count = 1;
	gbDesc.desc2D.SampleDesc.Quality = 0;
	gbDesc.desc2D.Usage = D3D11_USAGE_DEFAULT;
	gbDesc.desc2D.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	gbDesc.desc2D.CPUAccessFlags = 0;
	gbDesc.desc2D.MiscFlags = 0;
	m_gbuffers.gbPosWS = dev->createTexture(gbDesc, nullptr);		// Position
	m_gbuffers.gbNorWS = dev->createTexture(gbDesc, nullptr);		// Normal
	m_gbuffers.gbUV = dev->createTexture(gbDesc, nullptr);			// UV
	m_gbuffers.gbDiffuse = dev->createTexture(gbDesc, nullptr);		// Diffuse

	// Create Rasterizer State for Wireframe
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.CullMode = D3D11_CULL_BACK;
	ComPtr<ID3D11RasterizerState> wireframeRS = dev->createRasterizerState(rsDesc);

	// Create Solid Render Pipeline
	std::shared_ptr<DXPipeline> solidRenderPipeline = std::make_shared<DXPipeline>();
	solidRenderPipeline->attachInputLayout(il);
	solidRenderPipeline->setInputTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create Wireframe Render Pipeline
	std::shared_ptr<DXPipeline> wireframeRenderPipeline = std::make_shared<DXPipeline>();
	wireframeRenderPipeline->attachInputLayout(il);
	wireframeRenderPipeline->attachRasterizerState(wireframeRS);

	// Create normal viewport for render to GBuffer
	D3D11_VIEWPORT gbVP = {};
	gbVP.TopLeftX = 0;
	gbVP.TopLeftY = 0;
	gbVP.Width = static_cast<float>(dev->getClientWidth());
	gbVP.Height = static_cast<float>(dev->getClientHeight());
	gbVP.MinDepth = 0.0;
	gbVP.MaxDepth = 1.0;

	// Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dssDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
	m_gDss = dev->createDepthStencilState(dssDesc);

	// Attach resources to RenderPass (Solid)
	m_geometryPassSolid = std::make_shared<DXRenderPass>();
	m_geometryPassSolid->attachPipeline(solidRenderPipeline);
	//m_geometryPassSolid->attachInputConstantBuffer(0, m_gpMatrixBuffer);
	m_geometryPassSolid->attachSampler(0, minMagLinMipPointSamp);
	m_geometryPassSolid->attachViewports({ gbVP });
	m_geometryPassSolid->attachOutputTargets({ m_gbuffers.gbPosWS, m_gbuffers.gbNorWS, m_gbuffers.gbUV, m_gbuffers.gbDiffuse });
	m_geometryPassSolid->attachDepthTarget(depthTexture);

	// Attach resources to RenderPass (Wireframe)
	m_geometryPassWireframe = std::make_shared<DXRenderPass>();
	m_geometryPassWireframe->attachPipeline(wireframeRenderPipeline);
	//m_geometryPassWireframe->attachInputConstantBuffer(0, m_gpMatrixBuffer);
	m_geometryPassWireframe->attachSampler(0, minMagLinMipPointSamp);
	m_geometryPassWireframe->attachViewports({ gbVP });
	m_geometryPassWireframe->attachOutputTargets({ m_gbuffers.gbPosWS, m_gbuffers.gbNorWS, m_gbuffers.gbUV, m_gbuffers.gbDiffuse });
	m_geometryPassWireframe->attachDepthTarget(depthTexture);
}

void DeferredRenderStrategy::setupLightPass()
{
	auto dev = m_renderer->getDXDevice();
	m_fsQuad = FullscreenQuad(dev);

	// Create shader
	auto vsShader = dev->createShader("LightPassVS.cso", DXShader::Type::VS);
	auto psShader = dev->createShader("LightPassPS.cso", DXShader::Type::PS);

	// Create Input Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> ilDesc;
	ilDesc.push_back({"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA , 0});
	ilDesc.push_back({"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0});
	ilDesc.push_back({"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0});
	auto inputLayout = dev->createInputLayout(ilDesc, vsShader->getShaderData());

	// Create the cel-shading discretization texture
	DXTexture::Desc discDesc = { };
	discDesc.type = DXTexture::Type::TEX1D;
	discDesc.desc1D.Width = 5;
	discDesc.desc1D.MipLevels = 1;
	discDesc.desc1D.ArraySize = 1;
	discDesc.desc1D.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	discDesc.desc1D.Usage = D3D11_USAGE_DEFAULT;
	discDesc.desc1D.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	discDesc.desc1D.CPUAccessFlags = 0;
	discDesc.desc1D.MiscFlags = 0;

	auto imageData = loadFileToTexture("Textures/CelShading/discretization.png");
	D3D11_SUBRESOURCE_DATA dat = { };
	dat.pSysMem = imageData.data;

	auto discTex = dev->createTexture(discDesc, &dat);
	//Clean up the image
	free(imageData.data);

	// Create a sampler for the GBuffers
	D3D11_SAMPLER_DESC sDesc = { };
	sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.MipLODBias = 0;
	sDesc.MaxAnisotropy = 0;
	sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sDesc.MinLOD = 0;
	sDesc.MaxLOD = D3D11_FLOAT32_MAX;
	auto samplerState = dev->createSamplerState(sDesc);

	// Sampler for shadow map
	sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.BorderColor[0] = 1.f;
	sDesc.BorderColor[1] = 1.f;
	sDesc.BorderColor[2] = 1.f;
	sDesc.BorderColor[3] = 1.f;
	sDesc.MipLODBias = 0;
	sDesc.MaxAnisotropy = 0;
	sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sDesc.MinLOD = 0;
	sDesc.MaxLOD = D3D11_FLOAT32_MAX;
	auto smSampler = dev->createSamplerState(sDesc);

	// Create a render pipeline for the deferred light pass
	auto lpPipeline = std::make_shared<DXPipeline>();
	lpPipeline->attachInputLayout(inputLayout);
	lpPipeline->setInputTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	lpPipeline->attachVS(vsShader);
	lpPipeline->attachPS(psShader);

	D3D11_VIEWPORT lpVP = {};
	lpVP.TopLeftX = 0;
	lpVP.TopLeftY = 0;
	lpVP.Width = static_cast<float>(dev->getClientWidth());
	lpVP.Height = static_cast<float>(dev->getClientHeight());
	lpVP.MinDepth = 0.0;
	lpVP.MaxDepth = 1.0;

	// Directional Light Buffer
	m_dirLightBuffer = dev->createConstantBuffer(sizeof(DirectionalLight::DirLight), true, true);

	// Camera constant buffer
	m_cameraBuffer = dev->createConstantBuffer(sizeof(CameraBuffer), true, true);

	// Setup light render pass
	m_lightPass = std::make_shared<DXRenderPass>();
	m_lightPass->attachPipeline(lpPipeline);
	m_lightPass->attachSampler(0, samplerState);
	m_lightPass->attachSampler(1, smSampler);
	m_lightPass->attachInputTexture(0, m_gbuffers.gbPosWS);
	m_lightPass->attachInputTexture(1, m_gbuffers.gbNorWS);
	m_lightPass->attachInputTexture(2, m_gbuffers.gbUV);
	m_lightPass->attachInputTexture(3, m_gbuffers.gbDiffuse);
	m_lightPass->attachViewports({lpVP});
	if (m_usePostProcessing)
		m_lightPass->attachOutputTargets({m_prePostTexture});
	else
		m_lightPass->attachOutputTargets({dev->getBackbuffer()});
	m_lightPass->attachInputConstantBuffer(0, m_dirLightBuffer);
	m_lightPass->attachInputConstantBuffer(1, m_cameraBuffer);
	m_lightPass->attachInputTexture(10, discTex);
}

void DeferredRenderStrategy::setupPostProcessPass()
{
	auto dev = m_renderer->getDXDevice();

	auto vsShader = dev->createShader("PostProcessVS.cso", DXShader::Type::VS);
	auto psShader = dev->createShader("PostProcessPS.cso", DXShader::Type::PS);

	// Define the input layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> ilDesc;
	ilDesc.push_back({"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA , 0});
	ilDesc.push_back({"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0});
	ilDesc.push_back({"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0});
	auto inputLayout = dev->createInputLayout(ilDesc, vsShader->getShaderData());

	// Create a texture sampler
	/******************************************************/
	/* --- Might not want to use point sampler here!! --- */
	/******************************************************/
	D3D11_SAMPLER_DESC sDesc = { };
	sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sDesc.MipLODBias = 0;
	sDesc.MaxAnisotropy = 0;
	sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sDesc.MinLOD = 0;
	sDesc.MaxLOD = D3D11_FLOAT32_MAX;
	auto samplerState = dev->createSamplerState(sDesc);

	// Setup pipeline state
	auto ppPipeline = std::make_shared<DXPipeline>();
	ppPipeline->attachInputLayout(inputLayout);
	ppPipeline->setInputTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ppPipeline->attachVS(vsShader);
	ppPipeline->attachPS(psShader);

	// Setup a viewport
	D3D11_VIEWPORT ppVP = {};
	ppVP.TopLeftX = 0;
	ppVP.TopLeftY = 0;
	ppVP.Width = static_cast<float>(dev->getClientWidth());
	ppVP.Height = static_cast<float>(dev->getClientHeight());
	ppVP.MinDepth = 0.0;
	ppVP.MaxDepth = 1.0;

	DXTexture::Desc pptDesc = { };
	pptDesc.type = DXTexture::Type::TEX2D;
	pptDesc.desc2D.Width = static_cast<float>(dev->getClientWidth());
	pptDesc.desc2D.Height = static_cast<float>(dev->getClientHeight());
	pptDesc.desc2D.MipLevels = 1;
	pptDesc.desc2D.ArraySize = 1;
	pptDesc.desc2D.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	pptDesc.desc2D.SampleDesc.Count = 1;
	pptDesc.desc2D.SampleDesc.Quality = 0;
	pptDesc.desc2D.Usage = D3D11_USAGE_DEFAULT;
	pptDesc.desc2D.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	pptDesc.desc2D.CPUAccessFlags = 0;
	pptDesc.desc2D.MiscFlags = 0;
	m_prePostTexture = dev->createTexture(pptDesc, nullptr);

	// Setup constant buffer
	m_postProcessVariableBuffer = dev->createConstantBuffer(sizeof(PostProcessVariables), true, true, nullptr);



	m_postProcessVariables.elapsedTime = 0.;
	m_postProcessVariables.deltaTime = 0.;
	m_postProcessVariables.clientHeight = dev->getClientHeight();
	m_postProcessVariables.clientWidth = dev->getClientWidth();

	// Previous view-proj
	m_prevMatrices = dev->createConstantBuffer(sizeof(DirectX::XMMATRIX) * 2, true, true);

	// Setup the render pass
	// Add textures here if they are needed for a post processing effect
	m_postProcessPass = std::make_shared<DXRenderPass>();
	m_postProcessPass->attachPipeline(ppPipeline);
	m_postProcessPass->attachViewports({ppVP});
	m_postProcessPass->attachSampler(0, samplerState);
	m_postProcessPass->attachInputTexture(0, m_prePostTexture);
	m_postProcessPass->attachInputTexture(1, m_gbuffers.gbPosWS);
	m_postProcessPass->attachInputConstantBuffer(0, m_gpMatrixBuffer);
	m_postProcessPass->attachInputConstantBuffer(1, m_prevMatrices);
	m_postProcessPass->attachInputConstantBuffer(2, m_postProcessVariableBuffer);
	m_postProcessPass->attachOutputTargets({dev->getBackbuffer()});
}

void DeferredRenderStrategy::setupUIRenderer()
{
	auto dev = m_renderer->getDXDevice();
	m_spriteRenderer = std::make_shared<SpriteRenderer>(dev);
}
