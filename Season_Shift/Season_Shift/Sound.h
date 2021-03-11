#pragma once

#include "Component.h"
#include <Audio.h>

class CameraComponent;

class Sound : public Component
{
private:
	int m_loopingSoundId;
	Ref<CameraComponent> m_listenerCamera;
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	DirectX::AudioEmitter m_emitter;
	DirectX::AudioListener m_listener;

	std::unique_ptr<DirectX::SoundEffectInstance> m_effectInst;

	std::map<std::string, int> m_map;
	std::vector<DirectX::SoundEffect> m_sounds;
private:
	void update() override;
	void initialize() override;

public:
	Sound();
	Sound(const std::vector<std::string>& fileName);
	~Sound();

	size_t play(const std::string& soundname);
	void start(const std::vector<std::string>& fileName);
	void playLoop(const std::string& soundName);
	void stopLoop();

	void setVolume(float volume);
};

