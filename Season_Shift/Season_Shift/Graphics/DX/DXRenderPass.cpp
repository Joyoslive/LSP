#include "DXRenderPass.h"

DXRenderPass::DXRenderPass() :
	m_pipelineState(nullptr),
	m_outputTargets({}),
	m_viewports({}),
	m_depthTarget(nullptr)
{
}

void DXRenderPass::attachInputTexture(unsigned int slot, std::shared_ptr<DXTexture> texture)
{
	m_inputTextures.push_back({ slot, texture });
}

void DXRenderPass::attachInputConstantBuffer(unsigned int slot, std::shared_ptr<DXBuffer> buffer)
{
	m_inputConstantBuffers.push_back({ slot, buffer });
}

void DXRenderPass::attachSampler(unsigned int slot, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	m_samplers.push_back({ slot, sampler });
}

void DXRenderPass::attachPipeline(std::shared_ptr<DXPipeline> pipeline)
{
	m_pipelineState = pipeline;
}

void DXRenderPass::attachOutputTargets(std::vector<std::shared_ptr<DXTexture>> outputTargets)
{
	m_outputTargets = outputTargets;
}

void DXRenderPass::attachDepthTarget(std::shared_ptr<DXTexture> depthTarget)
{
	m_depthTarget = depthTarget;
}

void DXRenderPass::attachViewports(std::vector<D3D11_VIEWPORT> viewports)
{
	m_viewports = viewports;
}

void DXRenderPass::setScissorRects(const std::vector<D3D11_RECT>& rects)
{
	m_scissorRects = rects;
}

void DXRenderPass::attachUAOutputTargets(const std::vector<std::shared_ptr<DXTexture>>& unorderedAccessTargets)
{
	m_outputTargetUnorderedAccess = unorderedAccessTargets;
}

void DXRenderPass::clearAttachedDepthTarget(DXDevice* dev)
{
	dev->clearDepthTarget(m_depthTarget);
}

void DXRenderPass::bind(DXDevice* dev)
{
	if (m_pipelineState)
	{
		m_pipelineState->bindPipeline(dev);
	}

	for (auto& texture : m_inputTextures)
	{
		dev->bindShaderTexture(DXShader::Type::PS, texture.first, texture.second);
	}

	for (auto& buffer : m_inputConstantBuffers)
	{
		dev->bindShaderConstantBuffer(DXShader::Type::PS, buffer.first, buffer.second);
	}

	for (auto& sampler : m_samplers)
	{
		dev->bindShaderSampler(DXShader::Type::PS, sampler.first, sampler.second);
	}

	dev->bindViewports(m_viewports);
	dev->bindRenderTargets(m_outputTargets, m_depthTarget);
}

//const std::shared_ptr<DXTexture> DXRenderPass::getDepthTarget() const
//{
//	return m_depthTarget;
//}
//
//std::shared_ptr<DXPipeline>& DXRenderPass::getPipeline()
//{
//	return m_pipelineState;
//}