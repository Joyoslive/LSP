#include "ForwardRenderStrategy.h"
#include "../Camera.h"
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

ForwardRenderStrategy::ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer) :
	IRenderStrategy(renderer)
{
	OutputDebugStringW(L"Using forward render.\n");
	auto dev = renderer->getDXDevice();

	// Load shaders
	std::shared_ptr<DXShader> vs = dev->createShader("DefaultVS.cso", DXShader::Type::VS);

	std::vector<D3D11_INPUT_ELEMENT_DESC> ilDesc;
	ilDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ilDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ilDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 });
	ComPtr<ID3D11InputLayout> il = dev->createInputLayout(ilDesc, vs->getShaderData());

	// Create some sampler
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
	matrixBuffer = dev->createConstantBuffer(sizeof(DirectX::XMMATRIX) * 3, true, true);

	// Depth target
	DXTexture::Desc depthDesc;
	depthDesc.desc2D = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D24_UNORM_S8_UINT, dev->getClientWidth(), dev->getClientHeight(), 1, 1, D3D11_BIND_DEPTH_STENCIL);
	depthDesc.type = DXTexture::Type::TEX2D;
	std::shared_ptr<DXTexture> depthTexture = dev->createTexture(depthDesc, nullptr);

	// Setup Pipeline
	std::shared_ptr<DXPipeline> pipeline = std::make_shared<DXPipeline>();
	pipeline->attachSampler(DXShader::Type::PS, 0, sampler);
	pipeline->attachInputLayout(il);
	pipeline->setInputTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// Setup Render Pass
	std::vector<D3D11_VIEWPORT> vps = { *dev->getBackbufferViewport() };
	std::vector<std::shared_ptr<DXTexture>> targets = { dev->getBackbuffer() };

	m_renderPass = std::make_shared<DXRenderPass>();
	m_renderPass->attachPipeline(pipeline);
	m_renderPass->attachOutputTargets(targets);
	m_renderPass->attachViewports(vps);
	m_renderPass->attachDepthTarget(depthTexture);
}

ForwardRenderStrategy::~ForwardRenderStrategy()
{
}

void ForwardRenderStrategy::render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera, long double dt)
{
	auto dev = m_renderer->getDXDevice();
	dev->clearScreen();

	m_renderPass->bind(dev);
	m_renderPass->clearAttachedDepthTarget(dev);

	DirectX::XMMATRIX matrices[3] = { {}, mainCamera->getViewMatrix(), mainCamera->getProjectionMatrix() };
	for (auto& mod : models)
	{
		for (auto& mat : mod->getSubsetsMaterial())
		{
			mat.material->bindShader(dev);
			mat.material->bindTextures(dev);
			mat.material->bindBuffers(dev);

			matrices[0] = mod->getTransform()->getWorldMatrix();

			matrixBuffer->updateMapUnmap(matrices, sizeof(matrices), 0, D3D11_MAP_WRITE_DISCARD, 0);

			m_renderer->getDXDevice()->bindShaderConstantBuffer(DXShader::Type::VS, 0, matrixBuffer);

			dev->bindDrawIndexedBuffer(mod->getMesh()->getVB(), mod->getMesh()->getIB(), 0, 0);
			dev->drawIndexed(mat.indexCount, mat.indexStart, mat.vertexStart);
		}
	}

	dev->present();
}

void ForwardRenderStrategy::setUp()
{
	//allt har deletats efter en onResize så skapa om allt
	assert(false);
}
