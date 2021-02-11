#pragma once
#include <vector>
#include "DXPipeline.h"

class DXRenderPass
{
private:
	std::shared_ptr<DXPipeline> m_pipelineState;
	std::vector<D3D11_VIEWPORT> m_viewports;

	std::vector<D3D11_RECT> m_scissorRects;

	std::vector<std::shared_ptr<DXTexture>> m_outputTargets;
	std::shared_ptr<DXTexture> m_depthTarget;

	std::vector<std::shared_ptr<DXTexture>> m_outputTargetUnorderedAccess;

public:
	DXRenderPass(std::shared_ptr<DXPipeline> pipeline, std::vector<std::shared_ptr<DXTexture>> outputTargets, std::shared_ptr<DXTexture> depthTarget, std::vector<D3D11_VIEWPORT> viewports);
	~DXRenderPass() = default;

	void setScissorRects(const std::vector<D3D11_RECT>& rects);
	void setUAOutputTarget(const std::vector<std::shared_ptr<DXTexture>>& unorderedAccessTargets);

	void bind(DXDevice* dev);

	const std::shared_ptr<DXTexture> getDepthTarget() const;
	std::shared_ptr<DXPipeline>& getPipeline();


};

