#pragma once

#include "Component.h"
#include <Audio.h>

class Transform;
class CameraComponent;

class Sound : public Component
{
public:
	Sound(const std::vector<std::string>& fileName, const Ref<CameraComponent>& listener = nullptr);
	~Sound();

private:
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
	

	size_t play(const std::string& soundname);

	void setVolume(float volume);
};

