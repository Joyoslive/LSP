#include "pch.h"
#include "Audio.h"
#include <ctime>
Audio::Audio() 
{
	m_soundLoop = false;
	m_volume = 0.5;
	m_volume2 = 0;
	m_pitch = -0.3;
	m_mute = true;
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
	v1.push_back("Sounds/footstep1.wav");
	v1.push_back("Sounds/footstep1-2.wav");
	v1.push_back("Sounds/footstep1-3.wav");
	v1.push_back("Sounds/footstep2.wav");
	v1.push_back("Sounds/woosh.wav");
	m_sound.start(v1);
	m_sound.setVolume(0.8);
	std::vector<std::string> v;
	v.push_back("Sounds/heartbeat.wav");
	m_sound2.start(v);
	m_sound2.playLoop("Sounds/heartbeat.wav");
	std::vector<std::string> v4;
	v4.push_back("Sounds/footstep1.wav");
	v4.push_back("Sounds/footstep1Grass.wav");
	m_sound3.start(v4);
	m_sound3.setVolume(0.7);
	std::vector<std::string> v3;
	v3.push_back("Sounds/Spring2.wav");
	m_music.start(v3);
	m_music.playLoop("Sounds/Spring2.wav");
}

void Audio::update(bool ground, bool hook, bool wall, DirectX::SimpleMath::Vector3 velocity, float delta)
{
	if (m_mute) {
		if (velocity.Length() < 0.1 || (ground == false && wall == false && hook == false) && m_soundLoop == true)
		{
			m_soundLoop = false;
			m_sound.stopLoop();
			m_sound3.stopLoop();
		}
		else if (velocity.Length() != 0 && ground == true && m_soundLoop == false) {
			m_soundLoop = true;
			m_sound3.playLoop("Sounds/footstep1.wav");
		}
		else if (velocity.Length() != 0 && wall == true && m_soundLoop == false)
		{
			m_soundLoop = true;
			m_sound.playLoop("Sounds/wallrunBongo.wav");
		}
		else if (velocity.Length() != 0 && hook == true && ground == false && wall == false && m_soundLoop == false)
		{
			m_soundLoop = true;
			if (random() < 2)
				m_sound.playLoop("Sounds/swingT.wav");
		}

		if (50 < velocity.Length())
		{
			if (m_volume < 0.3)
				m_volume += 0.01 * delta;
			if (m_pitch <= 0)
				m_pitch += 0.11 * delta;
		}
		else if (85 > velocity.Length() && ground == true)
		{
			if (m_volume > 0.1)
				m_volume -= 0.02 * delta;
			if (m_pitch >= -0.7)
				m_pitch -= 0.1 * delta;
		}
		//HerthBeat
		if (velocity.Length() < 10)
		{
			m_volume2 += 0.1 * delta;
		}
		else if (ground == true)
		{
			if (m_volume2 > 0)
				m_volume2 -= 1 * delta;
		}
		m_sound2.setVolume(m_volume2);
		m_music.setVolume(m_volume);
		m_music.setPitch(m_pitch);
		m_sound3.setPitch(m_pitch);
	}
}

void Audio::playSound1(const std::string& soundName)
{
	m_sound.play(soundName);
}

void Audio::mute()
{
	m_mute = false;
	m_sound.setVolume(0);
	m_sound2.setVolume(0);
	m_music.setVolume(0);
}

void Audio::unmute()
{
	m_sound.setVolume(1);
	m_mute = true;
}

int Audio::random()
{
	std::srand(std::time(nullptr)); // use current time as seed for random generator
	int random_variable = std::rand();

	// roll 6-sided dice 20 times
	int x = 7;
	while (x > 6)
		x = 1 + std::rand() / ((RAND_MAX + 1u) / 6);  // Note: 1+rand()%6 is biased
	return x;
		

}