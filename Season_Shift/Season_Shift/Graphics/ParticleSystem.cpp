#include "pch.h"
#include "ParticleSystem.h"

void ParticleSystem::emitt()
{
}

void ParticleSystem::simulate(float dt)
{
}

void ParticleSystem::draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
}

ParticleSystem::ParticleSystem(const std::shared_ptr<GfxRenderer>& renderer, const std::string& simShader, const std::string& emittShader)
{
	

	m_renderer = renderer;
	auto dev = m_renderer->getDXDevice();

	m_vertexShader = dev->createShader("ParticleVS.cso", DXShader::Type::VS);
	m_geometryShader = dev->createShader("ParticleQuadGS.cso", DXShader::Type::GS);
	m_pixelShader = dev->createShader("ParticlePS.cso", DXShader::Type::PS);

	if (simShader.empty())
		m_simulationShader = dev->createShader("ParticleDefaultSimCS.cso", DXShader::Type::CS);
	else
		m_simulationShader = dev->createShader(simShader, DXShader::Type::CS);

	if (emittShader.empty())
		m_emittShader = dev->createShader("ParticleDefaultEmittCS.cso", DXShader::Type::CS);
	else
		m_emittShader = dev->createShader(emittShader, DXShader::Type::CS);

}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::SimulateAndDraw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, float dt)
{

}
