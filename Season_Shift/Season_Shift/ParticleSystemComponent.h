#pragma once
#include "Component.h"
#include "Graphics\ParticleSystem.h"
class ParticleSystemComponent : public Component
{
private:
	std::shared_ptr<ParticleSystem> m_partSys;
	std::string m_simShaderFile;
	std::string m_emittShaderFile;
private:
	void initialize() override;
	void update() override;
public:
	ParticleSystemComponent();
	ParticleSystemComponent(const std::string& simShader, const std::string& emittShader);
	~ParticleSystemComponent();

};

