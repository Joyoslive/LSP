#include "DXRenderPass.h"

DXRenderPass::DXRenderPass(std::shared_ptr<DXPipeline> pipeline, std::vector<std::shared_ptr<DXTexture>> outputTargets, std::shared_ptr<DXTexture> depthTarget, std::vector<D3D11_VIEWPORT> viewports) :
	m_pipelineState(pipeline),
	m_outputTargets(outputTargets),
	m_viewports(viewports),
	m_depthTarget(depthTarget)
{
}

void DXRenderPass::setScissorRects(const std::vector<D3D11_RECT>& rects)
{
	m_scissorRects = rects;
}

void DXRenderPass::setUAOutputTarget(const std::vector<std::shared_ptr<DXTexture>>& unorderedAccessTargets)
{
	m_outputTargetUnorderedAccess = unorderedAccessTargets;
}

void DXRenderPass::bind(DXDevice* dev)
{
	if (m_pipelineState)
	{
		m_pipelineState->bindPipeline(dev);
	}

	dev->bindViewports(m_viewports);
	dev->bindRenderTargets(m_outputTargets, m_depthTarget);
}

const std::shared_ptr<DXTexture> DXRenderPass::getDepthTarget() const
{
	return m_depthTarget;
}

std::shared_ptr<DXPipeline>& DXRenderPass::getPipeline()
{
	return m_pipelineState;
}
