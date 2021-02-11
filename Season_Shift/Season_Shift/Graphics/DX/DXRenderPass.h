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

	std::vector<std::pair<unsigned int, std::shared_ptr<DXTexture>>> m_inputTextures;
	std::vector<std::pair<unsigned int, std::shared_ptr<DXBuffer>>> m_inputConstantBuffers;
	std::vector<std::pair<unsigned int, Microsoft::WRL::ComPtr<ID3D11SamplerState>>> m_samplers;


public:
	DXRenderPass();
	~DXRenderPass() = default;

	void attachInputTexture(unsigned int slot, std::shared_ptr<DXTexture> texture);			// SRVs only
	void attachInputConstantBuffer(unsigned int slot, std::shared_ptr<DXBuffer> buffer);
	void attachSampler(DXShader::Type shaderStage, unsigned int slot, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);

	void attachPipeline(std::shared_ptr<DXPipeline> pipeline);
	void attachOutputTargets(std::vector<std::shared_ptr<DXTexture>> outputTargets);
	void attachDepthTarget(std::shared_ptr<DXTexture> depthTarget);
	void attachViewports(std::vector<D3D11_VIEWPORT> viewports);

	void setScissorRects(const std::vector<D3D11_RECT>& rects);
	void attachUAOutputTargets(const std::vector<std::shared_ptr<DXTexture>>& unorderedAccessTargets);

	void clearAttachedDepthTarget(DXDevice* dev);

	void bind(DXDevice* dev);



};

