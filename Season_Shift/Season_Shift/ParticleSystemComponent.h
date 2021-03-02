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
		EmitterMetaData(float lifeTime);
	};
	unsigned int m_maxParticles;
	std::shared_ptr<ParticleSystem> m_partSys;
	std::string m_simShaderFile;
	std::string m_emittShaderFile;

	std::vector<std::pair<ParticleSystem::EmittStructure, EmitterMetaData>> m_emittVec;

private:
	void initialize() override;
	void update() override;
public:
	ParticleSystemComponent(unsigned int maxParticles);
	ParticleSystemComponent(const std::string& simShader, const std::string& emittShader, unsigned int maxParticles);
	int addEmitter(unsigned int numParticles, float particleLifeTime, float emitterLifeTime, DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3(0, 0, 0),
		DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3(0, 0, 0));
	void stopEmitter(int id);

	void reviveEmitter(int id, float lifeTime);
	~ParticleSystemComponent();

};

