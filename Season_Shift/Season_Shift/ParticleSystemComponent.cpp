#include "pch.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"
#include "Graphics\Graphics.h"
#include <random>
#include "FrameTimer.h"

using namespace DirectX::SimpleMath;

void ParticleSystemComponent::initialize()
{
	m_partSys = getGameObject()->getScene()->getGraphics()->addParticleSystem(m_simShaderFile, m_emittShaderFile, m_maxParticles, m_particleLifeTime);
}
void ParticleSystemComponent::clearComponent()
{
	getGameObject()->getScene()->getGraphics()->removeParticleSystem(m_partSys);
}
float GenRandomFloat(const float& min, const float& max)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<> distr(min, max);
	return distr(eng);
}
void ParticleSystemComponent::update()
{
	float dt = FrameTimer::dt();

	for (auto& e : m_emittVec)
	{
		if (e.second.lifeTime > 0)
		{
			e.second.accumulatedTime += dt;
			if (1 / e.second.particlesPerSecond < e.second.accumulatedTime)
			{
				e.first.count = e.second.particlesPerSecond * e.second.accumulatedTime;
				e.first.randVec = { GenRandomFloat(-2.0f, 2.0f), GenRandomFloat(-2.0f, 2.0f), GenRandomFloat(-2.0f, 2.0f) };
				m_partSys->emitt(e.first);
				e.second.accumulatedTime -= (1 / e.second.particlesPerSecond);
			}
			
			e.second.lifeTime -= dt;
		}
	}
}

ParticleSystemComponent::ParticleSystemComponent(unsigned int maxParticles, unsigned int particleLifeTime)
{
	m_particleLifeTime = particleLifeTime;
	m_maxParticles = maxParticles;
	m_simShaderFile = "";
	m_emittShaderFile = "";
	m_componentType = ComponentEnum::PARTICLE_SYSTEM;
}

ParticleSystemComponent::ParticleSystemComponent(const std::string& simShader, const std::string& emittShader, unsigned int maxParticles, unsigned int particleLifeTime)
{
	m_particleLifeTime = particleLifeTime;
	m_maxParticles = maxParticles;
	m_simShaderFile = simShader;
	m_emittShaderFile = emittShader;
	m_componentType = ComponentEnum::PARTICLE_SYSTEM;
}

int ParticleSystemComponent::addEmitter(float particlesPerSecond, float startLifeTime,
	float emitterLifeTime, Vector3 color, Vector3 direction, Vector3 offset)
{
	Vector3 tempPos = getTransform()->getPosition() + offset; // add offset rotation from transform
	
	Vector3 randVec = Vector3(1, 1, 1); //fix random
	//tranform input direction with component transform
	m_emittVec.emplace_back(std::pair(ParticleSystem::EmittStructure(tempPos, startLifeTime, randVec, 0.0f, direction, 0, color), EmitterMetaData(emitterLifeTime, particlesPerSecond)));

	return m_emittVec.size()-1;
}

void ParticleSystemComponent::stopEmitter(int id)
{
	m_emittVec[id].second.lifeTime = 0;
}

void ParticleSystemComponent::reviveEmitter(int id, float lifeTime)
{
	m_emittVec[id].second.lifeTime = lifeTime;
}

ParticleSystemComponent::~ParticleSystemComponent()
{
}

ParticleSystemComponent::EmitterMetaData::EmitterMetaData(float lifeTime, float particlesPerSecond)
{
	this->lifeTime = lifeTime;
	this->particlesPerSecond = particlesPerSecond;
	this->accumulatedTime = 0;
}
