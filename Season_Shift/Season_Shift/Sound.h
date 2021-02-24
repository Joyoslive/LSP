#pragma once

#include "Component.h"
#include <Audio.h>

class Sound : public Component
{
private:
	class SoundEngine
	{
	public:
		static DirectX::AudioEngine& getAudioEngine();
	private:
		SoundEngine();
		SoundEngine(const SoundEngine&) = delete;
		SoundEngine& operator =(const SoundEngine&) = delete;
		~SoundEngine();
		static SoundEngine s_soundEngine;
		DirectX::AudioEngine m_audioEngine;
		float hej;
	};
	
	Ref<DirectX::SoundEffect> m_sound;
private:
	void update() override;

public:
	Sound();
	~Sound();

};

