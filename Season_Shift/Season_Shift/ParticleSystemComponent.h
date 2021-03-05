#pragma once
#include "Component.h"
#include "Graphics\ParticleSystem.h"

//class ParticleSystem;

class ParticleSystemComponent : public Component
{
	//friend ParticleSystem;
private:
	struct EmitterMetaData
	{
		float lifeTime;
		float particlesPerSecond;
		float accumulatedTime;
		DirectX::SimpleMath::Vector3 offset;
		EmitterMetaData(float lifeTime, float particlesPerSecond, DirectX::SimpleMath::Vector3 offset);
	};
	unsigned int m_maxParticles;
	unsigned int m_particleLifeTime;
	std::shared_ptr<ParticleSystem> m_partSys;
	std::string m_simShaderFile;
	std::string m_emittShaderFile;

	std::vector<std::pair<ParticleSystem::EmittStructure, EmitterMetaData>> m_emittVec;

private:
	void initialize() override;
	void clearComponent() override;
	void update() override;
public:
	ParticleSystemComponent(unsigned int maxParticles, unsigned int particleLifeTime);
	ParticleSystemComponent(const std::string& simShader, const std::string& emittShader, unsigned int maxParticles, unsigned int particleLifeTime);
	int addEmitter(float particlesPerSecond, float emitterLifeTime, float scale, DirectX::SimpleMath::Vector3 color, DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3::Zero,
		float particleStartLifeTime = 0, DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Zero);
	void stopEmitter(int id);

	void reviveEmitter(int id, float lifeTime);
	~ParticleSystemComponent();

};

