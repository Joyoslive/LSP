#pragma once
#include "IRenderStrategy.h"
#include "GfxRenderer.h"

class ForwardRenderStrategy final : public IRenderStrategy
{
private:


public:
	ForwardRenderStrategy(std::shared_ptr<GfxRenderer> renderer);
	~ForwardRenderStrategy();
	void render() override;
};

