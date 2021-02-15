#include "DeferredRenderStrategy.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

DeferredRenderStrategy::DeferredRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	IRenderStrategy(renderer),
	m_skybox(m_renderer)
{
	setupGeometryPass();
	setupLightPass();
	setupPostProcessPass();

}

void DeferredRenderStrategy::render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera)
{
	/*
	m_geometryPass->bind()
	render geom to multiple render targets

	m_lightPass->bind();
	gBuffer->bind();		// dif, normal, depth
	// bind other resources needed for light 
	drawQuad();	--> output to texture (will be used for postproc)


	m_postprocPass->bind();
	bindFinalLitTexture();
	bindPostProcBuffers();		// various postproc buffers (user postproc)

	drawQuad();		--> draw final texture AFTER screen space postprocess!
	*/

	auto dev = m_renderer->getDXDevice();
	dev->clearScreen();

	m_gbuffers.clear(dev);
	m_geometryPassSolid->bind(dev);
	m_geometryPassSolid->clearAttachedDepthTarget(dev);

	DirectX::XMMATRIX matrices[3] = { {}, mainCamera->getViewMatrix(), mainCamera->getProjectionMatrix() };
	for (auto& mod : models)
	{
		for (auto& mat : mod->getSubsetsMaterial())
		{
			mat.material->bindShader(dev);
			mat.material->bindTextures(dev);
			mat.material->bindBuffers(dev);

			matrices[0] = mod->getTransform()->getWorldMatrix();

			m_gpMatrixBuffer->updateMapUnmap(matrices, sizeof(matrices), 0, D3D11_MAP_WRITE_DISCARD, 0);

			m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::VS, 0, m_gpMatrixBuffer);

			dev->bindDrawIndexedBuffer(mod->getMesh()->getVB(), mod->getMesh()->getIB(), 0, 0);
			dev->drawIndexed(mat.indexCount, mat.indexStart, mat.vertexStart);
		}
	}
	m_skybox.draw(mainCamera);

	dev->present();
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
	ComPtr<ID3D11SamplerState> minMagLinMipPointSamp = m_renderer->getDXDevice()->createSamplerState(sDesc);

	// Create a matrix buffer
	m_gpMatrixBuffer = dev->createConstantBuffer(sizeof(DirectX::XMMATRIX) * 3, true, true);

	// Create GBuffers
	DXTexture::Desc gbDesc = { };
	gbDesc.type = DXTexture::Type::TEX2D;
	gbDesc.desc2D.Width = m_clientWidth;
	gbDesc.desc2D.Height = m_clientHeight;
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
	gbVP.Width = m_clientWidth;
	gbVP.Height = m_clientWidth;
	gbVP.MinDepth = 0.0;
	gbVP.MaxDepth = 0.0;

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
}

void DeferredRenderStrategy::setupPostProcessPass()
{
}