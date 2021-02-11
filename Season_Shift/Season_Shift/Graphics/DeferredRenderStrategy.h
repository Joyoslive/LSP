#pragma once
#include "IRenderStrategy.h"

class DeferredRenderStrategy final : public IRenderStrategy
{
private:
	std::shared_ptr<DXRenderPass> m_geometryPassSolid;
	std::shared_ptr<DXRenderPass> m_geometryPassWireframe;

	std::shared_ptr<DXRenderPass> m_lightPass;

	void setupGeometryPass();
	void setupLightPass();
	void setupPostProcessPass();

public:
	DeferredRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~DeferredRenderStrategy() = default;

	void render(const std::vector<std::shared_ptr<Model>>& models, const std::shared_ptr<Camera>& mainCamera) override;


};

