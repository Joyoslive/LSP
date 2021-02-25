#pragma once

#include "Component.h"
#include <Audio.h>

class Sound : public Component
{
private:
	DirectX::AudioEngine m_audioEngine;
	std::map<std::string, int> m_map;
	std::vector<DirectX::SoundEffect> m_sounds;
private:
	void update() override;

public:
	Sound(std::vector<std::string>& fileName);
	~Sound();

	size_t play();
};

