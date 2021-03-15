#pragma once
#include "Sound.h"
#include "SimpleMath.h"

class Audio
{
public:
	Audio();
	~Audio();
	void start();
	void update(bool ground, bool hook, bool wall, DirectX::SimpleMath::Vector3 velocity, float delta);
	void playSound1(const std::string& soundName);
	void mute();
	void unmute();
private:
	int random();
	Sound m_sound;
	Sound m_sound2;
	Sound m_music;
	bool m_soundLoop;
	bool m_mute;
	float m_volume;
	float m_volume2;
	float m_pitch;
};