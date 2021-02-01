#pragma once
#include <vector>
#include "IRenderStrategy.h"
#include "GfxRenderer.h"

class ForwardRenderStrategy final : public IRenderStrategy
{
private:
	std::shared_ptr<DXShader> vs, ps;
	std::shared_ptr<DXBuffer> vb;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> il;

public:
	ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~ForwardRenderStrategy();
	void render() override;
};

