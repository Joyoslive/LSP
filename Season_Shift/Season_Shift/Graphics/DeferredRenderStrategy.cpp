#include "DeferredRenderStrategy.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

DeferredRenderStrategy::DeferredRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	IRenderStrategy(renderer)
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
	ComPtr<ID3D11SamplerState> sampler;
	sampler = m_renderer->getDXDevice()->createSamplerState(sDesc);

	// Create a matrix buffer
	std::shared_ptr<DXBuffer> matrixBuffer = dev->createConstantBuffer(sizeof(DirectX::XMMATRIX) * 3, true, true);

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
	std::shared_ptr<DXTexture> gbPosWS = dev->createTexture(gbDesc, nullptr);		// Position
	std::shared_ptr<DXTexture> gbNorWS = dev->createTexture(gbDesc, nullptr);		// Normal
	std::shared_ptr<DXTexture> gbUV = dev->createTexture(gbDesc, nullptr);			// UV
	std::shared_ptr<DXTexture> gbDiffuse = dev->createTexture(gbDesc, nullptr);		// Diffuse

	// Create Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC dssDesc = {};










}

void DeferredRenderStrategy::setupLightPass()
{
}

void DeferredRenderStrategy::setupPostProcessPass()
{
}