#include "pch.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"
#include "Graphics\Graphics.h"

void ParticleSystemComponent::initialize()
{
	m_partSys = getGameObject()->getScene()->getGraphics()->addParticleSystem(m_simShaderFile, m_emittShaderFile, m_maxParticles);
}

void ParticleSystemComponent::update()
{

}

ParticleSystemComponent::ParticleSystemComponent(unsigned int maxParticles)
{
	m_maxParticles = maxParticles;
	m_simShaderFile = "";
	m_emittShaderFile = "";
	m_componentType = ComponentEnum::PARTICLE_SYSTEM;
}

ParticleSystemComponent::ParticleSystemComponent(const std::string& simShader, const std::string& emittShader, unsigned int maxParticles)
{
	m_maxParticles = maxParticles;
	m_simShaderFile = simShader;
	m_emittShaderFile = emittShader;
	m_componentType = ComponentEnum::PARTICLE_SYSTEM;
}

ParticleSystemComponent::~ParticleSystemComponent()
{
	
}
