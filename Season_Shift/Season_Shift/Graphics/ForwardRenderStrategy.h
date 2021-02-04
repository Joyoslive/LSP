#pragma once
#include <vector>
#include "../Transform.h"
#include "IRenderStrategy.h"
#include "GfxRenderer.h"

class ForwardRenderStrategy final : public IRenderStrategy
{
private:
	std::shared_ptr<DXShader> vs, ps;
	std::shared_ptr<DXBuffer> vb, ib;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> il;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;
	std::shared_ptr<DXBuffer> matrixBuffer;

	std::shared_ptr<DXBuffer> m_tmpBuf;

public:
	ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~ForwardRenderStrategy();
	void render(const std::vector<std::shared_ptr<Model>>& models, std::shared_ptr<Camera> mainCamera) override;
};

