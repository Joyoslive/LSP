#pragma once
#include <vector>
#include "IRenderStrategy.h"
#include "GfxRenderer.h"

class ForwardRenderStrategy final : public IRenderStrategy
{
private:
	std::shared_ptr<DXShader> vs, ps;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> il;

	std::shared_ptr<DXBuffer> matrixBuffer;

	std::shared_ptr<DXPipeline> m_pipeline;
	std::shared_ptr<DXRenderPass> m_renderPass;

public:
	ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~ForwardRenderStrategy();
	void render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera) override;
};