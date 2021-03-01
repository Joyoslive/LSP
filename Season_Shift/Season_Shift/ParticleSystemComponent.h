#pragma once
#include "Component.h"
class ParticleSystemComponent : public Component
{
private:
	void initialize() override;
	void update() override;
public:
	ParticleSystemComponent();
	ParticleSystemComponent(std::string simShader, std::string emittShader);
	~ParticleSystemComponent();

};

