#pragma once

#include "Component.h"
#include <Audio.h>

class Sound : public Component
{
private:
	DirectX::AudioEngine m_audioEngine;
	Ref<DirectX::SoundEffect> m_sound;
private:
	void update() override;

public:
	Sound();
	~Sound();

	size_t play();
};

