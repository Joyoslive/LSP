#include "Sound.h"


Sound::SoundEngine Sound::SoundEngine::s_soundEngine;

float explodeDelay = 30.0f;

void Sound::update()
{
	explodeDelay -= 0.01; //dt??
	if (explodeDelay < 0.f)
	{
		m_sound->Play();
		explodeDelay = 5.0;
	}
}

Sound::Sound()
{
	
	//m_sound = std::make_shared<DirectX::SoundEffect>(&SoundEngine::s_soundEngine.m_audioEngine, L"Sounds/Explo4.wav");
	m_componentType = ComponentEnum::SOUND;
}

Sound::~Sound()
{

}

DirectX::AudioEngine& Sound::SoundEngine::getAudioEngine()
{
	return s_soundEngine.m_audioEngine;
}

Sound::SoundEngine::SoundEngine()
{
	s_soundEngine.m_audioEngine = DirectX::AudioEngine();
	hej = 2;
}

Sound::SoundEngine::~SoundEngine()
{
	m_audioEngine.Suspend();
}
