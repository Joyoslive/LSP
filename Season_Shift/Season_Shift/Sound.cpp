#include "Sound.h"

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
	m_audioEngine = DirectX::AudioEngine();
	m_sound = std::make_shared<DirectX::SoundEffect>(&m_audioEngine, L"Sounds/Explo4.wav");
	m_componentType = ComponentEnum::SOUND;
}

Sound::~Sound()
{
}
