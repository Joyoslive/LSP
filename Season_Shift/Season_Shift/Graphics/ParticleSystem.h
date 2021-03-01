#pragma once
#include "GfxRenderer.h"

class ParticleSystem
{
private:
	std::shared_ptr<GfxRenderer> m_renderer;
	std::shared_ptr<DXShader> m_simulationShader;
	std::shared_ptr<DXShader> m_emittShader;
	std::shared_ptr<DXShader> m_vertexShader;
	std::shared_ptr<DXShader> m_geometryShader;
	std::shared_ptr<DXShader> m_pixelShader;

public:
	ParticleSystem(std::shared_ptr<GfxRenderer> renderer, std::string simShader, std::string emittShader);
	~ParticleSystem();
};

