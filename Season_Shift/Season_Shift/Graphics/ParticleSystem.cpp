#include "pch.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(std::shared_ptr<GfxRenderer> renderer, std::string simShader, std::string emittShader)
{
	m_renderer = renderer;
	auto dev = m_renderer->getDXDevice();
	std::shared_ptr<DXShader> m_simulationShader = dev->createShader(simShader, DXShader::Type::CS);
}

ParticleSystem::~ParticleSystem()
{

}
