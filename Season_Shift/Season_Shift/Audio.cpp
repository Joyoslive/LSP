#include "pch.h"
#include "Audio.h"

Audio::Audio() 
{
	m_soundLoop = false;
	m_volume = 0;
}

Audio::~Audio()
{

}

void Audio::start()
{
	std::vector<std::string> v1;
	v1.push_back("Sounds/whoosh.wav");
	v1.push_back("Sounds/boing2.wav");
	v1.push_back("Sounds/hook.wav");
	v1.push_back("Sounds/walkingDrum.wav");
	v1.push_back("Sounds/landing.wav");
	v1.push_back("Sounds/landing2.wav");
	v1.push_back("Sounds/checkpoint.wav");
	v1.push_back("Sounds/dehook.wav");
	v1.push_back("Sounds/jump1a.wav");
	v1.push_back("Sounds/jump2a.wav");
	v1.push_back("Sounds/jump3a.wav");
	v1.push_back("Sounds/dashTest.wav");
	v1.push_back("Sounds/jumpTest.wav");
	v1.push_back("Sounds/wallrunBongo.wav");
	v1.push_back("Sounds/airBeat.wav");
	v1.push_back("Sounds/hookDrum.wav");
	v1.push_back("Sounds/hookDrumv2.wav");
	v1.push_back("Sounds/run2.wav");
	v1.push_back("Sounds/swing2.wav");
	v1.push_back("Sounds/swingUp.wav");
	v1.push_back("Sounds/swingDown.wav");
	v1.push_back("Sounds/swingFull.wav");
	v1.push_back("Sounds/swingT.wav");
	m_sound.start(v1);
	std::vector<std::string> v;
	v.push_back("Sounds/heartbeat.wav");
	m_sound2.start(v);
	std::vector<std::string> v3;
	v3.push_back("Sounds/Spring.wav");
	m_music.start(v3);
	m_music.setVolume(m_volume);
	m_music.playLoop("Sounds/Spring.wav");
}

void Audio::update(bool ground, bool hook, bool wall, DirectX::SimpleMath::Vector3 velocity)
{
	if (velocity.Length() < 0.1 || (ground == false && wall == false && hook == false) && m_soundLoop == true)
	{
		m_soundLoop = false;
		m_sound.stopLoop();
	}
	else if (velocity.Length() != 0 && ground == true && m_soundLoop == false) {
		m_soundLoop = true;
		m_sound.playLoop("Sounds/run2.wav");
	}
	else if (velocity.Length() != 0 && wall == true && m_soundLoop == false)
	{
		m_soundLoop = true;
		m_sound.playLoop("Sounds/wallrunBongo.wav");
	}
	else if (velocity.Length() != 0 && hook == true && ground == false && wall == false && m_soundLoop == false)
	{
		m_soundLoop = true;
		m_sound.playLoop("Sounds/swingT.wav");
	}

	if (m_volume < velocity.Length() / 150)
	{
		m_volume += 0.001;
	}
	else if (m_volume > velocity.Length() / 150)
	{
		m_volume -= 0.0011;
	}
	m_music.setVolume(m_volume);
}

void Audio::playSound1(const std::string& soundName)
{
	m_sound.play(soundName);
}